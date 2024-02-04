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
# include <sndfile.h>
# include "portaudio.h"
# include "FourierTransform.h"
# include "musicUtils.h"

using namespace std;

int main(int argc,char** argv,char** envp)
{
	int ch,index;
	string filterCommand,inputFile,outputFile,soundType;
	double timePerMeasure;
	while((ch=getopt(argc,argv,"f:i:s:t:o:"))!=EOF)
		{switch(ch)
			{case 'f':
				// Filter Command
				filterCommand=optarg;
				break;
			case 'i':
				// Song Input File(s)
				inputFile=optarg;
				break;
			case 's':
				// Sound Type
				soundType=optarg;
				break;
			case 't':
				// Time (seconds) Per Measure
				timePerMeasure=strtod(optarg,NULL);
				break;
			case 'o':
				// Output .wav file path
				outputFile=optarg;
				break;
			default:
				exit(EXIT_FAILURE);}}

	//int theSz;
	//string inputWaveFile="/home/user/Desktop/DrBeats_Music_Interface2/CSOUND/fox.wav";
	//string outputWaveFile="/home/user/Desktop/DrBeats_Music_Interface2/clipped.wav";
	//soundSignal inS=read_amplitudes_in_wave_file(inputWaveFile);
	//cout<<inS.N<<endl;
	//double *aTest=frequency_shift_signal(inputWaveFile,200,envp);

	//	for(int i=0;i<inS.N;i++){cout<<aTest[i]<<endl;}
	//clip_wave_file(inputWaveFile,0,44099,outputWaveFile);
	//exit(1);

/*	int N=100;
	double incrmnt=0.1;
	double *x=new double[N];
	for(int i=0;i<N;i++){x[i]=i*incrmnt;}
	double avg=5,std_dev=1;
	double* y=generate_normalized_Gaussian_distribution(avg,std_dev,x,N);
	string title="set title \"FFT(Signal) Magnitude Spectrum\"\n";
	string xLabel="set xlabel \"Frequency (Hertz (1/seconds))\"\n";
	string yLabel="set ylabel \"Amplitude\"\n";
	string xRange="set xrange [0:"+cnvrtNumToStrng(2*x[N-1],0)+"]\n";
	string yRange;//"set yrange [-2:2]";
	string plotOptions;//=title+xLabel+yLabel+xRange;//+yRange;
	string fftFile=get_current_dir_name(); fftFile+="/FFT(signal)";

	plotOptions=title+xLabel+yLabel+xRange;
	plot_signal(plotOptions,fftFile,x,y,N);

	exit(1);
*/

/*	string waveFile=get_current_dir_name(); waveFile+="/WAV/voice.wav";
	string voiceName="Brazil_Woman_1";
	double timeSpanOfWord=0.4;					// seconds
	double wordRate=1/timeSpanOfWord;				// words/sec
	string wordSpeed=cnvrtNumToStrng(wordRate*60,0);	// words/min
	string pitch="50";
	string amplitude="100";
	run_espeak("ow",amplitude,pitch,wordSpeed,voiceName,waveFile,envp);

	soundSignal inS=read_amplitudes_in_wave_file(waveFile);
	double *x=new double[inS.N];
	for(int i=0;i<inS.N;i++){x[i]=(double)inS.signal[i];}	
	double Max=determine_maximum_frequency_peak(x,inS.N,index);
	cout<<"Max Freq: "<<Max<<endl;


	double desiredFrequency=1046;
	// Estimate Out-Of-Tuneness
	double changeInFrequency=desiredFrequency-Max;
	// Define Output Wave File
	string outputWaveFile=extract_containing_folder(waveFile); outputWaveFile+="tuned.wav";
	// Tune Sound
	frequency_shift_signal_1_channel_input(waveFile,changeInFrequency,timeSpanOfWord,envp,outputWaveFile);

	exit(1);
*/

	string tmpFile,blockNameLst,blockDefLst;
	int numBlocks;
	soundSignal Output;
	bool COMP_DONE;
	if(soundType.compare("drums")==0)
		{//cout<<"Reading drums music...";
		// Checked for Blocked Music Input File
		numBlocks=check_for_blocked_music_input(inputFile,blockNameLst,blockDefLst);
		// Handle Blocked Music Input
		if(numBlocks>0)
			{//
			tmpFile=inputFile+"_expanded";
			write_expanded_music_input_file(tmpFile,blockNameLst,blockDefLst,numBlocks);
			//
			Output=read_drums_input_file(tmpFile,soundType,timePerMeasure,envp);
			}
		else
			{// Regular Music Input
			Output=read_drums_input_file(inputFile,soundType,timePerMeasure,envp);}			
		//cout<<"Done.\n";
		}
	else if(soundType.compare("sinusoid")==0)
		{//cout<<"Reading sinusoid music...";
		Output=read_sinusoid_input_file(inputFile,soundType,timePerMeasure);
		//cout<<"Done.\n";
		}
	else
		{// Check for Voice Reader
		COMP_DONE=false;
		for(int j=0;j<numVoiceSpeakers;j++)
			{if(soundType.compare(espeakVoiceSpeakers[j])==0)
				{Output=read_lyrics_input_file(inputFile,soundType,filterCommand,timePerMeasure,envp);
				COMP_DONE=true;
				break;}
			}
		if(!COMP_DONE){Output=read_music_input_file(inputFile,soundType,filterCommand,timePerMeasure,envp);}		
		}
	// Normalize Values Before Adding
	double maxValue=abs_maximum_of_array(Output.signal,Output.N,index);
	for(int j=0;j<Output.N;j++){Output.signal[j]/=(SAMPLE)maxValue;}
	double fs=SAMPLE_RATE;
/*	
	FILE *fOut;
	string aFile=get_current_dir_name(); aFile+="/signal";
	// Write Signal to File
	fOut=fopen(aFile.c_str(),"w");
	string header="# X Time (seconds) Y Amplitude";
	fprintf(fOut,"%s\n",header.c_str());
	//for(int i=0;i<Sz;i++){fprintf(fOut,"%0.11f\t%0.11f\n",(double)i/fs,x[i]);}
	for(int i=0;i<Output.N;i++){fprintf(fOut,"%0.12f\t%0.12f\n",(double)i/fs,Output.signal[i]);}
	//fOut.close();
	fclose(fOut);
*/
/*	double *time=new double[Output.N];
	for(int i=0;i<Output.N;i++){time[i]=(double)i/fs;}
	string mp3File="/home/user/Desktop/DrBeats_Music_Interface2/output.mp3";
	string outputMp4File="/home/user/Desktop/DrBeats_Music_Interface2/output.mp4";
	generate_animation(time,Output.signal,Output.N,mp3File,outputMp4File);
*/
	// Write WAV Output
	write_wave_file(outputFile,Output.signal,Output.N);
}


