# include <cassert>
# include <cmath>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <fstream>
# include <iostream>
# include <sstream>
# include <string>
# include <utility>
# include <unistd.h>
# include <sys/wait.h>
#include <sndfile.h>
# include "portaudio.h"
# include "FourierTransform.h"
# include "musicUtils.h"

using namespace std;

int main(int argc,char** argv,char** envp)
{	string waveFileToPlay,outFile,recordSound_executable;
	int ch,N,index;
	while((ch=getopt(argc,argv,"i:e:o:"))!=EOF)
		{switch(ch)
			{case 'i':
				// Song Input File(s)
				waveFileToPlay=optarg;
				break;
			case 'e':
				// recordSound.exe File Path
				recordSound_executable=optarg;
				break;
			case 'o':
				// Output .wav file path
				outFile=optarg;
				break;
			default:
				exit(EXIT_FAILURE);}}

	double timeDurationToRecord=get_wave_file_time_duration(waveFileToPlay);	cout<<"|"<<timeDurationToRecord<<"|"<<endl;
	string timeDuration=formatNumberString(cnvrtNumToStrng(timeDurationToRecord,SIG_FIGS));

	string warningAudioFile=get_current_dir_name(); warningAudioFile+="/PRECEEDING_BEEPS.wav";

	string cmd="aplay \""+warningAudioFile+"\"";
	int result=system(cmd.c_str());
	int PID=fork(),status2;
	//string logFile=get_current_dir_name(); logFile+="/log_stderr";
	cmd="aplay \""+waveFileToPlay+"\"";
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		char* argv[]={recordSound_executable.c_str(),"-d",timeDuration.c_str(),"-o",outFile.c_str(),NULL};
		//freopen(logFile.c_str(),"w",stderr);
		result=syscall(SYS_execve,recordSound_executable.c_str(),argv,envp);						
		if(result<0)
			{cerr<<"ERROR in recordWithPlayback!\nCould not execute record_sound.exe\n";
			exit(EXIT_FAILURE);}
		}
	else if(PID>0)
		{//waitpid(PID,&status2,0);
		result=system(cmd.c_str());}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}

}
