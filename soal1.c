#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0)
        exit (EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    pid_t sid = setsid();
    if (sid < 0)
        exit (EXIT_FAILURE);

    if ((chdir("/")) < 0)
        exit (EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        sleep(5);
    //chdir("/");
    //char cwd[256];
    //getcwd(cwd, sizeof cwd);
    //printf("%s\n", cwd);
    mkdir("home/arino/modul2", ACCESSPERMS);
    //if (mkdir("home/arino/modul2", ACCESSPERMS) == -1)
        //printf("%s\n", strerror(errno));
    mkdir("home/arino/modul2/gambar", ACCESSPERMS);
    //if (mkdir("home/arino/modul2/gambar", ACCESSPERMS) == -1)
        //printf("%s\n", strerror(errno));
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/arino/Pictures/Modul2");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *fname = dir->d_name;
            char *tmp = strdup(fname);
            char *dot = strrchr(tmp, '.');
            if (!dot)
                continue;
            char *ext = strdup(dot + 1);
            if (strcmp(ext, "png") == 0)
            {
                *dot = '\0';
                char newdir[256], origdir[256];
                strcpy(newdir, "/home/arino/modul2/gambar/");
                strcpy(origdir, "/home/arino/Pictures/Modul2/");
                strcat(origdir, fname);
                strcat(tmp, "_grey.");
                strcat(newdir, tmp);
                strcat(newdir, ext);
                //puts(origdir);
                //puts(newdir);
		rename(origdir, newdir);
                //if (rename(origdir, newdir) == -1)
                    //printf("%s\n", strerror(errno));
            }
        }
        closedir(d);
    }
    //return 0;
    sleep(5);
    }
    exit(EXIT_SUCCESS);
}
