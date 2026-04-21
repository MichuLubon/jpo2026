#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <windows.h>
#include <pdh.h>
#include <psapi.h>
#include <curl/curl.h>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QThread>
#include <QMessageBox>
#include <QDebug> // Dodane dla logowania diagnostycznego

/**
 * @file mainwindow.cpp
 * @brief Implementacja głównego okna aplikacji System Monitor.
 * @author Michał
 * @date 2026-04-20
 */

/**
 * @brief Konwertuje listę wartości liczbowych na ciąg znaków rozdzielony przecinkami.
 * @param list Lista wartości double (pobrane próbki).
 * @return QString sformatowany pod Pythonowe listy [x, y, z].
 */
QString listToString(QList<double> list) {
    QStringList sl;
    for(double d : list) sl << QString::number(d, 'f', 1);
    return sl.join(", ");
}

/**
 * @brief Pobiera aktualny procentowy stan wykorzystania pamięci RAM.
 * @return Wartość typu double (procenty).
 */
double getRAMUsage() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return (double)memInfo.dwMemoryLoad;
}

/**
 * @brief Pobiera obciążenie procesora (wszystkie rdzenie łącznie).
 * @return Obciążenie w % (double).
 */
double getCPUUsage() {
    static PDH_HQUERY cpuQuery;
    static PDH_HCOUNTER cpuTotal;
    static bool initialized = false;
    if (!initialized) {
        PdhOpenQuery(NULL, NULL, &cpuQuery);
        PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
        initialized = true;
        return 0.0;
    }
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}

/**
 * @brief Pobiera aktywność dysku fizycznego.
 * @return Procent czasu aktywności dysku (double).
 */
double getDiskUsage() {
    static PDH_HQUERY diskQuery;
    static PDH_HCOUNTER diskCounter;
    static bool initialized = false;
    if (!initialized) {
        PdhOpenQuery(NULL, NULL, &diskQuery);
        PdhAddEnglishCounter(diskQuery, L"\\PhysicalDisk(_Total)\\% Disk Time", NULL, &diskCounter);
        PdhCollectQueryData(diskQuery);
        initialized = true;
        return 0.0;
    }
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(diskQuery);
    PdhGetFormattedCounterValue(diskCounter, PDH_FMT_DOUBLE, NULL, &counterVal);
    double usage = counterVal.doubleValue;
    if (usage > 100.0) usage = 100.0;
    return usage;
}

/**
 * @brief Funkcja zwrotna (callback) dla biblioteki CURL do zapisu odpowiedzi HTTP.
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @brief Rozszerzone testy jednostkowe sprawdzające logikę i sensory.
 */
void runSimpleTests() {
    qDebug() << "--- URUCHAMIANIE TESTÓW JEDNOSTKOWYCH ---";

    // TEST 1: Standardowa konwersja
    QList<double> testData = {10.5, 20.2};
    if(listToString(testData) == "10.5, 20.2") {
        qDebug() << "[PASS] Test listToString (standard)";
    } else {
        qDebug() << "[FAIL] Test listToString (standard)";
    }

    // TEST 2: Pusta lista (czy program się nie sypie)
    QList<double> emptyData;
    if(listToString(emptyData) == "") {
        qDebug() << "[PASS] Test listToString (empty)";
    } else {
        qDebug() << "[FAIL] Test listToString (empty)";
    }

    // TEST 3: Logika sensora RAM (czy wartość mieści się w 0-100%)
    double ram = getRAMUsage();
    if(ram >= 0.0 && ram <= 100.0) {
        qDebug() << "[PASS] Test getRAMUsage (zakres):" << ram << "%";
    } else {
        qDebug() << "[FAIL] Test getRAMUsage (wartość poza zakresem!):" << ram;
    }

    // TEST 4: Logika sensora CPU
    double cpu = getCPUUsage();
    if(cpu >= 0.0 && cpu <= 100.0) {
        qDebug() << "[PASS] Test getCPUUsage (zakres):" << cpu << "%";
    } else {
        qDebug() << "[FAIL] Test getCPUUsage (wartość poza zakresem!)";
    }

    qDebug() << "--- ZAKOŃCZONO TESTY ---";
}

/**
 * @brief Konstruktor klasy MainWindow. Inicjalizuje GUI i ComboBox.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Wywołanie testów jednostkowych przy starcie
    runSimpleTests();

    if(ui->comboBoxCzas->count() == 0) {
        ui->comboBoxCzas->addItem("15");
        ui->comboBoxCzas->addItem("30");
        ui->comboBoxCzas->addItem("60");
    }
}

MainWindow::~MainWindow() { delete ui; }

/**
 * @brief Główna logika aplikacji wywoływana po kliknięciu przycisku.
 * @details Realizuje pętlę pomiarową, wysyła zapytanie do modelu Ollama i uruchamia skrypt Python.
 */
void MainWindow::on_pushButton_clicked() {
    int sekundy = ui->comboBoxCzas->currentText().toInt();

    ui->pushButton->setEnabled(false);
    ui->comboBoxCzas->setEnabled(false);
    ui->statusbar->showMessage(QString("Analiza systemu przez %1 sekund...").arg(sekundy));

    QList<double> ramHistory, cpuHistory, diskHistory;

    try {
        for(int i = 0; i < sekundy; i++) {
            double r = getRAMUsage();
            double c = getCPUUsage();
            double d = getDiskUsage();

            ramHistory.append(r);
            cpuHistory.append(c);
            diskHistory.append(d);

            ui->progressBar->setValue(c);
            ui->progressBar_2->setValue(r);
            ui->progressBar_3->setValue(d);

            ui->labelCPU->setText(QString("CPU: %1%").arg(c, 0, 'f', 1));
            ui->labelRAM->setText(QString("RAM: %1%").arg(r, 0, 'f', 0));
            ui->labelDisk->setText(QString("Dysk: %1%").arg(d, 0, 'f', 1));

            ui->label->setText(QString("Postęp: %1 / %2 s").arg(i+1).arg(sekundy));

            QCoreApplication::processEvents();
            QThread::sleep(1);
        }
    } catch (...) {
        QMessageBox::critical(this, "Błąd systemu", "Wystąpił nieoczekiwany błąd podczas odczytu danych.");
        ui->pushButton->setEnabled(true);
        ui->comboBoxCzas->setEnabled(true);
        return;
    }

    ui->statusbar->showMessage("Przesyłanie historii do AI...");
    qDebug() << "Rozpoczęto komunikację z Ollama...";

    CURL* curl = curl_easy_init();
    if(curl) {
        std::string response_string;
        QJsonObject jsonObj;
        jsonObj["model"] = "llama3";

        QString prompt = QString(
                             "ZADANIE: Napisz kod matplotlib używając plt.subplot(3, 1, x). "
                             "DANE: ram_data, cpu_data, disk_data mają po %1 elementów. "
                             "KOD MUSI WYGLĄDAĆ TAK:\n"
                             "plt.subplot(3,1,1); plt.plot(cpu_data, 'r'); plt.title('CPU'); plt.ylabel('%%'); plt.grid(True)\n"
                             "plt.subplot(3,1,2); plt.plot(ram_data, 'b'); plt.title('RAM'); plt.ylabel('%%'); plt.grid(True)\n"
                             "plt.subplot(3,1,3); plt.plot(disk_data, 'g'); plt.title('Dysk (Aktywność)'); plt.ylabel('%%'); plt.xlabel('Czas (s)'); plt.grid(True)\n"
                             "plt.tight_layout(); plt.show()\n"
                             "ZAKAZ: Żadnych komentarzy ani wstępów, tylko czysty kod Pythona."
                             ).arg(sekundy);

        jsonObj["prompt"] = prompt;
        jsonObj["stream"] = false;

        QJsonDocument doc(jsonObj);
        std::string jsonRequest = doc.toJson(QJsonDocument::Compact).toStdString();

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonRequest.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            QMessageBox::warning(this, "Błąd połączenia",
                                 QString("Nie można nawiązać połączenia z serwerem AI. Błąd: %1")
                                     .arg(curl_easy_strerror(res)));
        } else {
            QJsonDocument resDoc = QJsonDocument::fromJson(QByteArray::fromStdString(response_string));

            if(!resDoc.isNull() && resDoc.isObject()) {
                QString aiCode = resDoc.object().value("response").toString();

                aiCode.remove("```python");
                aiCode.remove("```");
                aiCode = aiCode.trimmed();

                int startIdx = aiCode.indexOf("plt");
                if (startIdx == -1) startIdx = aiCode.indexOf("import");
                if (startIdx > 0) aiCode = aiCode.mid(startIdx);

                ui->textEdit->setPlainText(aiCode);

                QFile file("wykres.py");
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << "import matplotlib.pyplot as plt\n";
                    out << "cpu_data = [" << listToString(cpuHistory) << "]\n";
                    out << "ram_data = [" << listToString(ramHistory) << "]\n";
                    out << "disk_data = [" << listToString(diskHistory) << "]\n\n";
                    out << aiCode;
                    file.close();

                    if(!QProcess::startDetached("py", QStringList() << "wykres.py")) {
                        QMessageBox::warning(this, "Błąd", "Nie udało się uruchomić skryptu Python.");
                    } else {
                        ui->statusbar->showMessage("Wykresy wygenerowane!");
                    }
                }
            }
        }
        curl_easy_cleanup(curl);
    }

    ui->pushButton->setEnabled(true);
    ui->comboBoxCzas->setEnabled(true);
}