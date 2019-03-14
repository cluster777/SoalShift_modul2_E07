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
string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%d:%m:%Y-%H:%M", &tstruct);

    return buf;
}


int main() {
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

  while(1) {
	pid_t child_id;
	int status;
	child_id = fork();

 	if (child_id == 0) {
		pid_t ch_id;
		int stat=0;
 		 // this is child
		ch_id = fork();
		if (ch_id == 0) {
        	 	// this is child
			string now=currentDateTime();
			ofstream outfile;
			outfile.open("dsccd.txt");
			now="/root/log/"+now;
			outfile<<now;
			outfile.close();
	                //buat folder
	        char *argv[4]={"mkdir","-p",(char*)(now.c_str()),NULL};
			execv("/bin/mkdir",argv );
	        }
	        else {
			while((wait(&stat))>0);
			string namafile;
			ifstream infile;
			infile.open("dsccd.txt");
			infile>>namafile;
			infile.close();
			string counter="0";
			while(1){
			
				//buat file
				
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
				char data;
				ifstream infile;
				ofstream outfile;
				infile.open("/var/log/syslog");
				outfile.open(namafile+"/log"+counter+".log");
				while((infile>>data)){
					//encrypt
					if(infile.eof())break;
					outfile<<data;
				}
				infile.close();
				outfile.close();
				//printf("woy");
				sleep(60);
				//outfile.open("test.txt");
					//outfile<<counter;
				
				//infile.close();
				//outfile.close();
			}
	    }

  	}
 	else {
		sleep(1800);

	}
}

  exit(EXIT_SUCCESS);
}

