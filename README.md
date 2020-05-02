# SoalShiftSISOP20_modul4_E04

Soal Shift Sistem Operasi 2020\
Modul 4\
Kelompok E 04

Source code file: [ssfs.c](https://github.com/segara2410/SoalShiftSISOP20_modul4_E04/blob/master/ssfs.c)

## 1. Enkripsi Versi 1
- Untuk menyelesaikan enkripsi versi 1, setiap folder berawalan `encv1_` akan berubah sesuai caesar cipher dengan key 10.
- Fungsi untuk melakukan enkripsi adalah `encryptV1()` yang melakukan traverse setelah ditemukan awalan `encv1_`. 
  ```c
  void encryptV1(char *src) 
  {
    int new_length = strlen(src);

    for (int i = strlen(src); i >= 0; i--) 
    {
      if(src[i] == '/')
        break;

      if(src[i] == '.')
      {
        new_length = i - 1;
        break;
      }
    }

    int begin = 0;

    for (int i = 1; i < new_length; i++)
      if (src[i] == '/')
        begin = i;

    for (int i = begin; i <= new_length; i++) 
    {
      if(src[i] == '/')
        continue;

      int caesar_index = 0;
      while(1)
      {
        if(src[i] == caesar[caesar_index])
        {
          src[i] = caesar[caesar_index + 10];
          break;
        }
        caesar_index++;
      }
    }
  }
  ```
  Setelah itu isinya akan diencrypt sesuai dengan array `char caesar[100]`. 
- Untuk mendecrypt fungsi `decryptV1()` akan dipanggil jika directory memiliki awalan `encv1_`, dimana fungsi ini akan melakukan dekripsi caesar cipher dengan key -10.
  ```c
  void decryptV1(char *src) 
  {
    int new_length = strlen(src); 
    int begin = 0;
      
    for (int i = 1; i < new_length; i++)
    {
      if(src[i] == '/' || src[i + 1] == '\0') 
      {
        begin = i + 1;
        break;
      }
    }

    for (int i = strlen(src); i >= 0; i--)
    {
      if (src[i] == '.') 
      {
        new_length = i - 1;
        break;
      }
    }

    for (int i = begin; i <= new_length; i++) 
    {
      if(src[i] == '/')
        continue;

      int caesar_index = strlen(caesar) - 1;
      while(1)
      {
        if(src[i] == caesar[caesar_index])
        {
          src[i] = caesar[caesar_index - 10];
          break;
        }
        caesar_index--;
      }
    }
  }
  ```
- Fungsi `encryptV1()` dan `decryptV1()` harus dipanggil di semua fungsi `FUSE` agar enkripsi dapat berjalan.

## 4. Membuat Log
- Untuk membuat `fs.log`, fungsi yang digunakan adalah `writeLog()`.
- Fungsi `writeLog()` dipanggil oleh fungsi fungsi `FUSE` yang berhubungan dengan modifikasi file untuk mencatat semua modifikasi yang telah terjadi.
- Fungsi `writeLog()` adalah fungsi yang menuliskan log dengan format `[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC]` di `/home/segara/fs.log`.
  ```c
  void writeLog(char *level, char *cmd_desc)
  {
    FILE * fp;
    fp = fopen (log_path, "a+");

    time_t rawtime = time(NULL);
    
    struct tm *info = localtime(&rawtime);
    
    char time[100];
    strftime(time, 100, "%y%m%d-%H:%M:%S", info);

    char log[100];
    sprintf(log, "%s::%s::%s\n", level, time, cmd_desc);
    fputs(log, fp);

    fclose(fp);
  }
  ```
