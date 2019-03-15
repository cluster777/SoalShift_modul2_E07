#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main()
{
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    int fileout = open("/home/arino/Documents/daftar.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    pid_t pid1 = fork();
    if (pid1 == 0)
    {
        char *argv[] = {"unzip", "campur2.zip", NULL};
        execv("/usr/bin/unzip", argv);
    }
    else
    {
        int return_status1;
        waitpid(pid1, &return_status1, 0);
        pid_t pid2 = fork();
        if (pid2 == 0)
        {
            close(STDOUT_FILENO);
            dup2(fd1[1], 1);
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
            chdir("./campur2");
            char *argv[] = {"ls", NULL};
            execv("/bin/ls", argv);
        }
        else
        {
            int return_status2;
            waitpid(pid2, &return_status2, 0);
            pid_t pid3 = fork();
            if (pid3 == 0)
            {
                close(STDOUT_FILENO);
                close(STDIN_FILENO);
                dup2(fd1[0], 0);
                dup2(fd2[1], 1);
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);
                char *argv[] = {"grep", "txt$", NULL};
                execv("/bin/grep", argv);
            }
            else
            {
                close(STDIN_FILENO);
                close(STDOUT_FILENO);
                dup2(fd2[0], 0);
                dup2(fileout, 1);
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);
                char *argv[] = {"cat", NULL};
                execv("/bin/cat", argv);
            }
        }
    }
    return 0;
}
