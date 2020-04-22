#include <stdio.h>
#include <string.h>
#include <time.h>

char *dir_path = "/home/segara/Documents";
char *log_path = "/home/segara/fs.log";
char caesar[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO9(ku@AW1[L";

void encryptString(char *source)
{
  char new[100];
  strcpy(new, source);
  for(int i = 0; i < strlen(new); i++)
  {
    int caesar_index = 0;
    while(1)
    {
      if(new[i] == caesar[caesar_index])
      {
        new[i] = caesar[caesar_index + 10];
        break;
      }
      caesar_index++;
    }
  }
  strcpy(source, new);
}

void encrypt(char *source)
{
	char str[100];
  strcpy(str, source);
  
  char* token = strtok(str, "/");
  if (strcmp(token, "encv1_rahasia") == 0)
  {
    sprintf(source, "%s", token);
    token = strtok(NULL, "/"); 
    while (token != NULL) 
    { 
      encryptString(token);
      strcat(source, "/");
      strcat(source, token);
      token = strtok(NULL, "/"); 
    }
  }
}

void decryptString(char *source)
{
  char new[100];
  strcpy(new, source);
  for(int i = 0; i < strlen(new); i++)
  {
    int caesar_index = strlen(caesar);
    while(1)
    {
      if(new[i] == caesar[caesar_index])
      {
        new[i] = caesar[caesar_index - 10];
        break;
      }
      caesar_index--;
    }
  }
  strcpy(source, new);
}

void decrypt(char *source)
{
	char str[100];
  strcpy(str, source);
  
  char* token = strtok(str, "/");
  if (strcmp(token, "encv1_rahasia") == 0)
  {
    sprintf(source, "%s", token);
    token = strtok(NULL, "/"); 
    while (token != NULL) 
    { 
      decryptString(token);
      strcat(source, "/");
      strcat(source, token);
      token = strtok(NULL, "/"); 
    }
  }
}

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
  // fputs(log, fp);
  puts(log);

  fclose(fp);
}

int main(int argc, char *argv[])
{
  writeLog("INFO", "MKDIR::/tes");
  char a[100] = "encv1_rahasia/FOTO_PENTING/kelincilucu.jpg";
  encrypt(a);
  puts(a);
  decrypt(a);
  puts(a);
}