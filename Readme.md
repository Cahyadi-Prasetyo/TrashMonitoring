# ♻️ Trash Monitoring – Sistem Monitoring Tempat Sampah Berbasis Web

**Trash Monitoring** adalah proyek akhir mata kuliah *Sistem Digital* yang bertujuan untuk memantau kondisi tempat sampah secara **real-time** menggunakan sensor dan menampilkan data melalui **dashboard web** berbasis **Firebase**.

---

## 📌 Deskripsi Singkat

Sistem ini menggunakan mikrokontroler dan beberapa sensor untuk mendeteksi:

- 📏 **Jarak objek** (sensor ultrasonik)
- 🌡️ **Kelembapan Sampah** (sensor DHT22)
- 🧪 **Gas berbahaya** (sensor MQ-series)

Semua data sensor dikirim ke **Firebase Realtime Database**, kemudian ditampilkan melalui dashboard web agar pengguna dapat memantau kondisi tempat sampah secara jarak jauh.

---

## 🛠️ Tools & Teknologi

| Platform      | Deskripsi                                               |
|---------------|---------------------------------------------------------|
| 🧪 **Wokwi**   | Simulator awal untuk pengujian sensor dan komunikasi    |
| 💻 **VS Code** | Pengembangan lanjutan dan integrasi Firebase            |
| 🔥 **Firebase**| Media penyimpanan data real-time                        |
| 🌐 **HTML + JS**| Dashboard monitoring berbasis web                      |
| 🔌 **ESP32**   | Mikrokontroler utama dalam simulasi (via Wokwi)         |

---

## 🔄 Alur Sistem

1. Sensor mendeteksi kondisi lingkungan (tinggi sampah, suhu, gas).
2. Data dikirim oleh ESP32 ke **Firebase Realtime Database**.
3. Dashboard web menarik data dari Firebase dan menampilkannya secara langsung.
4. Jika tempat sampah penuh atau mendeteksi gas berbahaya, sistem memberikan **indikator peringatan**.

---

## 📸 Tampilan Sistem

<div align="justify">
  <img src="https://github.com/user-attachments/assets/0f1cbb42-c583-49e5-8033-c07d803cb1a6" width="45%" alt="Simulasi Wokwi"/>
  &nbsp;
  <img src="https://github.com/user-attachments/assets/b2020647-bffd-416b-8680-0f01a72c483a" width="45%" alt="Dashboard Web"/>
</div>

---

## 🚀 Cara Menjalankan

### 🔧 Simulasi di Wokwi
- Buka file `.ino` di [Wokwi](https://wokwi.com/)
- Pastikan konfigurasi sensor dan koneksi Firebase sudah sesuai.
- Klik **Start Simulation**.

### 💻 Integrasi di VS Code
- Pastikan library seperti `Firebase ESP32`, `WiFi`, dan `DHT` sudah terinstal.
- Upload program ke ESP32 (jika dijalankan fisik).
- Jalankan `index.html` untuk melihat dashboard secara lokal.

---
