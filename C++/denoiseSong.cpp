# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <string>
# include "portaudio.h"
# include "FourierTransform.h"
# include "musicUtils.h"

int main(int argc,char** argv,char** envp)
{
	string outputFile,inputFile;
	int ch;
	while((ch=getopt(argc,argv,"i:o:"))!=EOF)
		{switch(ch)
			{case 'i':
				// Input Wave File to remove noise from
				inputFile=optarg;
				break;
			case 'o':
				// Song Input File(s)
				outputFile=optarg;
				break;
			default:
				exit(EXIT_FAILURE);}}

	// De-Noise Song
	dnoise_song(inputFile,outputFile,envp);
	int N=0,index;
	double *amps=read_amplitudes_in_wave_file(outputFile,N);
	double maxAmplitude=abs_maximum_of_array(amps,N,index);
	for(int i=0;i<N;i++){amps[i]/=maxAmplitude;}
	write_wave_file(outputFile,amps,N);
}
