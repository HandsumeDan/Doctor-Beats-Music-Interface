# include <cassert>
# include <cmath>
# include <cstdio>
# include <cstdlib>
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
{
    PaStreamParameters  inputParameters,outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;
    SAMPLE              max, val;

	int ch,N,index;
	string inFile_1,inFile_2,outFile;
	double A_1=1,A_2=1;
	while((ch=getopt(argc,argv,"a:b:i:j:o:"))!=EOF)
		{switch(ch)
			{case 'a':
				// First Song Amplitude
				A_1=strtod(optarg,NULL);
				break;
			case 'b':
				// Second Song Amplitude
				A_2=strtod(optarg,NULL);
				break;
			case 'i':
				// First Song Input File
				inFile_1=optarg;
				break;
			case 'j':
				// First Song Input File
				inFile_2=optarg;
				break;
			case 'o':
				// Output .wav file path
				outFile=optarg;
				break;
			default:
				exit(EXIT_FAILURE);}}

	int N_1,N_2,Sz;
	double* wave1=read_amplitudes_in_wave_file(inFile_1,N_1);
	// Normalize Values to hold unit variance (biggest magnitude = 1)
	double maxValue=abs_maximum_of_array(wave1,N_1,index);
	for(int j=0;j<N_1;j++){wave1[j]/=maxValue;}
	double* wave2=read_amplitudes_in_wave_file(inFile_2,N_2);
	maxValue=abs_maximum_of_array(wave2,N_2,index);
	for(int j=0;j<N_2;j++){wave2[j]/=maxValue;}

	// Check Size
	if(N_1!=N_2)
		{cerr<<"Error in merge_wave_files.exe\nWave file sizes do not match:\n"<<inFile_1<<"\n"<<N_1<<"\n"<<inFile_2<<"\n"<<N_2<<"\n";
		//exit(EXIT_FAILURE);
		if(N_1<N_2){Sz=N_1;}
		else{Sz=N_2;}
		}
	else
		{Sz=N_1;}
	double* output=new double[Sz];
	// Merge Files
	for(int i=0;i<Sz;i++){output[i]=A_1*wave1[i]+A_2*wave2[i];}
	maxValue=abs_maximum_of_array(output,Sz,index);
	for(int j=0;j<Sz;j++){output[j]/=maxValue;}

	// Write Output Wave file
	write_wave_file(outFile,output,Sz);
}

