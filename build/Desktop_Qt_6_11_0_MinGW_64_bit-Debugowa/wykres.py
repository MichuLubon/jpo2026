import matplotlib.pyplot as plt
cpu_data = [2.2, 2.8, 3.4, 1.5, 2.5, 2.8, 2.1, 2.2, 0.9, 1.2, 3.7, 5.3, 4.1, 1.6, 2.0]
ram_data = [31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0]
disk_data = [0.0, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.1, 0.0, 0.1, 0.0, 0.0, 0.1, 0.0, 0.1]

plt

plt.subplot(3,1,1); plt.plot(cpu_data, 'r'); plt.title('CPU'); plt.ylabel('%'); plt.grid(True)
plt.subplot(3,1,2); plt.plot(ram_data, 'b'); plt.title('RAM'); plt.ylabel('%'); plt.grid(True)
plt.subplot(3,1,3); plt.plot(disk_data, 'g'); plt.title('Dysk (Aktywność)'); plt.ylabel('%'); plt.xlabel('Czas (s)'); plt.grid(True)
plt.tight_layout(); plt.show()