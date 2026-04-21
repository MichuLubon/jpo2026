# AI System Monitor v1.0

Aplikacja desktopowa napisana w C++/Qt służąca do monitorowania wydajności komputera w czasie rzeczywistym, wykorzystująca lokalny model AI do generowania analiz graficznych.

## Główne Funkcjonalności
- Monitorowanie CPU, RAM i aktywności dysku (Windows API).
- Integracja z lokalnym modelem LLM (Llama3) przez REST API (Ollama).
- Automatyczne generowanie kodu wizualizacji danych w języku Python.
- Dokumentacja techniczna wygenerowana w systemie Doxygen.

## Wymagania Przed Uruchomieniem
1. **Ollama**: Zainstalowana i uruchomiona z pobranym modelem Llama3 (`ollama run llama3`).
2. **Python 3.x**: Zainstalowany w systemie i dodany do zmiennej PATH.
3. **Biblioteki Python**: Wymagane `matplotlib` (szczegóły w requirements.txt).

## Obsługa Błędów
Program posiada zaimplementowaną obsługę wyjątków dla:
- Braku łączności z serwerem AI.
- Błędów odczytu sensorów systemowych.
- Problemów z uruchamianiem procesów zewnętrznych.
