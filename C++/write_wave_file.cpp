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
	string tmp,tmp2,inputFile;
	double tmpVal,maxValue;

    PaStreamParameters  inputParameters,outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;
    SAMPLE              max, val;

	int ch,N,index;
	string waveFile;
	while((ch=getopt(argc,argv,"i:o:"))!=EOF)
		{switch(ch)
			{case 'i':
				// Song Input File(s)
				inputFile=optarg;
				break;
			case 'o':
				// Output .wav file path
				waveFile=optarg;
				break;
			default:
				exit(EXIT_FAILURE);}}

	// Read Input Song File, which should contain instructions for generating a song from one or more sound files	
	syntheticSong theSong=read_song_input_file(inputFile,envp);
	
	double fs=SAMPLE_RATE, value, incrmnt=(fs-0)/(theSong.component[0].N-1);
	//string waveFile=get_current_dir_name(); waveFile+="/output.wav";
	// Update Data
	//cout<<"|"<<theSong.component[0].N<<"|\n";
	data.maxFrameIndex=theSong.component[0].N; //S.N;	//data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
	data.frameIndex = 0;
	data.recordedSamples = (SAMPLE *) malloc( data.maxFrameIndex*sizeof(SAMPLE) );
	SAMPLE* rawMix = (SAMPLE *) malloc( data.maxFrameIndex*sizeof(SAMPLE) );
	for(int i=0;i<data.maxFrameIndex;i++){data.recordedSamples[i]=0;}
	
	// Normalize Values Before Adding
/*	for(int i=0;i<theSong.numComponents;i++)
		{maxValue=abs_maximum_of_array(theSong.component[i].signal,theSong.component[i].N,index);
		//cout<<theSong.component[i].N<<endl;
		for(int j=0;j<theSong.component[i].N;j++)
			{//cout<<j<<" | "<<theSong.component[i].signal[j]<<"| |"<<data.recordedSamples[j]<<"\n";			
			theSong.component[i].signal[j]/=maxValue;}
		}
*/	
	// Save Individual Components
	for(int i=0;i<theSong.numComponents;i++)
		{tmp=extract_containing_folder(waveFile);
		tmp+="Component_"+cnvrtNumToStrng(i+1,0)+".wav";
		if(VERBOSE){cout<<"Writing Component #"<<i+1<<"...";}
		write_wave_file(tmp,theSong.component[i].signal,theSong.component[i].N);
		//cout<<theSong.component[i].N<<endl;
		if(VERBOSE){cout<<"Done!\n";}
		}

	// Superpose (i.e. Add) Sounds Together
/*	for(int i=0;i<theSong.numComponents;i++)
		{// Merge Each Component One At a Time	
		//cout<<i<<"|";
		for(long j=0;j<theSong.component[i].N;j++)
			{//cout<<i<<"|"<<j<<".";
			if(j<data.maxFrameIndex)
				{//cout<<theSong.component[i].amplitude[j]<<"|"<<theSong.component[i].signal[j]<<"\n";
				if(i==0){data.recordedSamples[j]=theSong.component[i].amplitude[j]*theSong.component[i].signal[j];}
				else{data.recordedSamples[j]+=theSong.component[i].amplitude[j]*theSong.component[i].signal[j];}
				}
			}
		if(i!=0)
			{// Normalize to Unit Magnitude
			maxValue=abs_maximum_of_array(data.recordedSamples,data.maxFrameIndex,index);
			//cout<<maxValue<<endl;
			for(int j=0;j<theSong.component[i].N;j++){if(j<data.maxFrameIndex){data.recordedSamples[j]/=maxValue;}}
			}		
		}
*/
/*	string tmpFile=get_current_dir_name(); tmpFile+="/amplitudes";
	ofstream fOut;
	fOut.open(tmpFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR!\nInput music file could not be opened.\n"<<tmpFile<<endl; exit(EXIT_FAILURE);}
	string theLst="Comp Signal Amplitude\n";
	int aCounter=1;
	for(int i=0;i<theSong.numComponents;i++)
	//for(int i=0;i<1;i++)
		{for(int j=0;j<theSong.component[i].N;j++)
			{if(theSong.component[i].amplitude[j]>0.5)
				{if(aCounter%100==0){cout<<i<<" | "<<j<<" | "<<theSong.component[i].amplitude[j]<<endl;}
				theLst+=cnvrtNumToStrng(i+1,0)+" | "+cnvrtNumToStrng(j,0)+" | "+formatNumberString(cnvrtNumToStrng(theSong.component[i].amplitude[j],SIG_FIGS))+"\n";}
			//data.recordedSamples[j]+=theSong.component[i].amplitude[j]*theSong.component[i].signal[j];
			}
		}
	fOut<<theLst;
	fOut.close();
*/	
	// Determine Greatest Amplitude (Must be between 0 and 1) Among Components
	double maxAmplitude=abs_maximum_of_array(theSong.component[0].amplitude,theSong.component[0].N,index);
	for(int i=1;i<theSong.numComponents;i++)
		{maxValue=abs_maximum_of_array(theSong.component[i].amplitude,theSong.component[i].N,index);
		if(maxValue>maxAmplitude){maxAmplitude=maxValue;}
		}
	//cout<<"|"<<maxAmplitude<<"|\n";
	// Add/Super-pose Normalized Signals
	double aValue;
	for(int i=0;i<theSong.numComponents;i++)
		{for(long j=0;j<theSong.component[i].N;j++)
			{aValue=theSong.component[i].amplitude[j]*theSong.component[i].signal[j];
			//if(aValue>10){cout<<"|"<<theSong.component[i].amplitude[j]<<"|"<<theSong.component[i].signal[j]<<"|\n";}
			data.recordedSamples[j]+=aValue;
			}
		}
	// Normalize Mixed Signal
	maxValue=abs_maximum_of_array(data.recordedSamples,data.maxFrameIndex,index);
	//cout<<"|"<<maxValue<<"|\n";
	for(int i=0;i<data.maxFrameIndex;i++){data.recordedSamples[i]/=(SAMPLE)(maxValue/maxAmplitude);}//data.recordedSamples[i]/=(SAMPLE)maxValue/(SAMPLE)maxAmplitude;};

	// Write Output Wave file
	write_wave_file(waveFile,data.recordedSamples,data.maxFrameIndex);
}



