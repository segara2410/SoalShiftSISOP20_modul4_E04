#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

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

void encrypt(char *dest, char *source)
{
	char str[100];
  strcpy(str, source);
  
  char* token = strtok(str, "/");
  if (strncmp(token, "encv1_", 6) == 0)
  {
    sprintf(dest, "%s", token);
    token = strtok(NULL, "/"); 
    while (token != NULL) 
    { 
      encryptString(token);
      strcat(dest, "/");
      strcat(dest, token);
      token = strtok(NULL, "/"); 
    }
  }
  else
    sprintf(dest, "%s", source);
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

void decrypt(char *dest, char *source)
{
	char str[100];
  strcpy(str, source);
  
  char* token = strtok(str, "/");
  if (strncmp(token, "encv1_", 6) != 0)
  {
    sprintf(dest, "%s", token);
    token = strtok(NULL, "/"); 
    while (token != NULL) 
    { 
      decryptString(token);
      strcat(dest, "/");
      strcat(dest, token);
      token = strtok(NULL, "/"); 
    }
  }
  else
    sprintf(dest, "%s", source);
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
  fputs(log, fp);

  fclose(fp);
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
  char fpath[1000];
  sprintf(fpath,"%s%s",dir_path,path);
  res = lstat(fpath, stbuf);

  if (res == -1)
    return -errno;

  return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

  int res = 0;

  DIR *dp;
  struct dirent *de;

  (void) offset;
  (void) fi;    

  dp = opendir(fpath);

  if (dp == NULL)
    return -errno;    

  while ((de = readdir(dp)) != NULL) 
  {
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;

    res = (filler(buf, de->d_name, &st, 0));
    if(res != 0) 
      break;
  }   

  closedir(dp);
  return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

	if (S_ISREG(mode)) 
  {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} 
  else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);

	if (res == -1)
		return -errno;

  char desc[100];
  sprintf(desc, "CREATE::%s", fpath);
  writeLog("INFO", desc);

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;

  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

  char desc[100];
  sprintf(desc, "MKDIR::%s", fpath);
  writeLog("INFO", desc);

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;

  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

	res = unlink(fpath);
	if (res == -1)
		return -errno;

  char desc[100];
  sprintf(desc, "REMOVE::%s", fpath);
  writeLog("WARNING", desc);
  
	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;

  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

  char desc[100];
  sprintf(desc, "RMDIR::%s", fpath);
  writeLog("WARNING", desc);

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;

  char ffrom[1000];
  char fto[1000];

  if(strcmp(from,"/") == 0)
  {
    from = dir_path;
    sprintf(ffrom, "%s", from);
  }
  else 
    sprintf(ffrom, "%s%s", dir_path, from);

  if(strcmp(from,"/") == 0)
  {
    to = dir_path;
    sprintf(fto, "%s", to);
  }
  else 
    sprintf(fto, "%s%s", dir_path, to);

	res = rename(ffrom, fto);
	if (res == -1)
		return -errno;

  char desc[100];
  sprintf(desc, "RENAME::%s::%s", ffrom, fto);
  writeLog("INFO", desc);

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;

  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;

  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;

    sprintf(fpath,"%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

  int res = 0;
  int fd = 0 ;

  (void) fi;
  fd = open(fpath, O_RDONLY);

  if (fd == -1)
    return -errno;

  res = pread(fd, buf, size, offset);

  if (res == -1)
    res = -errno;

  close(fd);

  return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];

  if(strcmp(path,"/") == 0)
  {
    path = dir_path;
    sprintf(fpath, "%s", path);
  }
  else 
    sprintf(fpath, "%s%s", dir_path, path);

	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

  char desc[100];
  sprintf(desc, "WRITE::%s", fpath);
  writeLog("INFO", desc);

	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.truncate	= xmp_truncate,
	.open		  = xmp_open,
	.read	  	= xmp_read,
	.write		= xmp_write,
};

int main(int argc, char *argv[])
{
  umask(0);
  return fuse_main(argc, argv, &xmp_oper, NULL);
}