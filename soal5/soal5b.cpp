#include<unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
	char *arg[3] {"pkill","soal5a",NULL};
	execv("/usr/bin/pkill",arg);
}
