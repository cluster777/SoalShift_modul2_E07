#include <sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string>
#include<time.h>
#include<fstream>
using namespace std;

string counter="0";
int main(){
	pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
	while(1){
		struct stat attr;
		stat("/root/Documents/ss2/makan_enak.txt", &attr);
		//dapet waktu
		//printf("Last modified time: %s", ctime(&attr.st_atime));
		
		//check jika telah dibuka 30 detik yg lalu
		time_t a=attr.st_atime;
		//Wed Mar 13 14:43:52 2019

		time_t b=time(0);
		if(b-30<=a){
		//akhir check time
			counter[counter.size()-1]++;
			for(int i=counter.size()-1;i>=0;i--)
			{
				if(counter[i]-'9'>0 && i>0){
					counter[i]='0';
					counter[i-1]++;
				}
				else if(counter[i]-'9'>0 && i==0){
					counter[i]='0';
					counter="1"+counter;
				}
			}
				ofstream outfile;
			outfile.open("/root/Documents/ss2/makansehat"+counter+".txt");
			outfile<<"diet woy";
			outfile.close();
		}
		sleep(5);
	}
}

