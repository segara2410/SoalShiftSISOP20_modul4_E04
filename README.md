# SoalShiftSISOP20_modul4_E04

Soal Shift Sistem Operasi 2020\
Modul 4\
Kelompok E 04

Source code file: [ssfs.c](https://github.com/segara2410/SoalShiftSISOP20_modul4_E04/blob/master/ssfs.c)

## 1. Enkripsi Versi 1
- Untuk menyelesaikan enkripsi versi 1, setiap folder berawalan `encv1_` akan berubah sesuai caesar cipher dengan key 10.
- Fungsi untuk melakukan enkripsi adalah `encryptV1()` yang melakukan traverse setelah ditemukan awalan `encv1_`. 
  Setelah itu isinya akan diencrypt sesuai dengan array `char caesar[100]`. 
- Untuk mendecrypt fungsi `decryptV1()` akan dipanggil jika directory memiliki awalan `encv1_`, dimana fungsi ini akan melakukan dekripsi caesar cipher dengan key -10.

## 4. Membuat Log
- Untuk membuat `fs.log`, fungsi yang digunakan adalah `writeLog()`.
- Fungsi `writeLog()` dipanggil oleh fungsi fungsi `FUSE` yang berhubungan dengan modifikasi file.
- Fungsi `writeLog()` adalah fungsi yang menuliskan log dengan format `[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC]` di `/home/segara/fs.log`.
