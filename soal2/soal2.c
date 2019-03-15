#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>
#include <string.h>

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
        chdir("/home/arino/hatiku");
        chmod("/home/arino/hatiku/elen.ku", 0777);
        struct stat st;
        if (stat("/home/arino/hatiku/elen.ku", &st) == -1)
            perror("Error Occured");
        struct passwd *pwd = getpwuid(st.st_uid);
        puts(pwd->pw_name);
        struct group *gr = getgrgid(st.st_gid);
        puts(gr->gr_name);
        if ((strcmp(gr->gr_name, "www-data") == 0) && (strcmp(pwd->pw_name, "www-data") == 0))
        {
            puts("Berhasil");
            remove("/home/arino/hatiku/elen.ku");
        }
        sleep(3);
    }
    exit(EXIT_SUCCESS);
}
