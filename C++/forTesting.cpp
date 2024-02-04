# include <cstdlib>
# include <cstring>
# include <iostream>
# include <fstream>
# include <string>
# include "musicUtils.h"

using namespace std;

int main(int argc,char** argv,char** envp)
{
/*	ifstream fIn;
	// Read Note File
	int pos,Sz=150000;
	char Val[Sz];
	string noteFile="/home/user/Desktop/DrBeats_Music_Interface/SOUND/Chromatic Scale/notes",tmp,timeValue,timeLst="",noteValue,noteLst="",delimiter="?";
	fIn.open(noteFile.c_str());
	if(fIn.fail()){cerr<<"ERROR !\nCould not open note file.\n"<<noteFile<<endl; exit(EXIT_FAILURE);}			
	// Skip Header Line
	fIn.getline(Val,Sz);
	fIn.getline(Val,Sz);
	while(!fIn.eof())
		{tmp=Val;
		// Extract Time Value
		pos=tmp.find("\t",0);
		timeValue=tmp.substr(0,pos);
		timeLst+=timeValue+delimiter;
		// Extract Notes Value
		noteValue=tmp.substr(pos+1,tmp.length()-pos-1);
		noteLst+=noteValue+delimiter;
		cout<<"|"<<timeValue<<"|"<<noteValue<<"|\n";
		fIn.getline(Val,Sz);}
	fIn.close();
*/
	paTestData          data;
	string songInputFile="/home/user/Desktop/DrBeats_Music_Interface/SOUND/Chromatic Scale/Song_Input";
	syntheticSong theSong=read_song_input_file(songInputFile,envp);
	double fs=SAMPLE_RATE,maxValue;
	int index;
	data.maxFrameIndex=theSong.component[0].N; //S.N;	//data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
	data.frameIndex = 0;
	data.recordedSamples = (SAMPLE *) malloc( data.maxFrameIndex*sizeof(SAMPLE) );
	// Determine Greatest Amplitude (Must be between 0 and 1) Among Components
	double maxAmplitude=abs_maximum_of_array(theSong.component[0].amplitude,theSong.component[0].N,index);
	for(int i=1;i<theSong.numComponents;i++)
		{maxValue=abs_maximum_of_array(theSong.component[i].amplitude,theSong.component[i].N,index);
		if(maxValue>maxAmplitude){maxAmplitude=maxValue;}
		}
	//cout<<"|"<<maxAmplitude<<"|\n";
	// Add/Super-pose Normalized Signals
	for(int i=0;i<theSong.numComponents;i++)
		{for(long j=0;j<theSong.component[i].N;j++)
			{data.recordedSamples[j]+=theSong.component[i].amplitude[j]*theSong.component[i].signal[j];
			}
		}
	// Normalize Mixed Signal
	maxValue=abs_maximum_of_array(data.recordedSamples,data.maxFrameIndex,index);
	for(long i=0;i<data.maxFrameIndex;i++){data.recordedSamples[i]/=(SAMPLE)(maxValue/maxAmplitude);};

	int N=theSong.component[0].N;
	double *time=new double[N];
	for(int i=0;i<N;i++){time[i]=(double)i/fs;}

	// Write out Signal for Plotting
	FILE *fOut;
	string aFile="/home/user/Desktop/DrBeats_Music_Interface/SOUND/Chromatic Scale/signal";
	// Write Signal to File
	fOut=fopen(aFile.c_str(),"w");
	string header="# X Time (seconds) Y Amplitude";
	fprintf(fOut,"%s\n",header.c_str());
	//for(int i=0;i<Sz;i++){fprintf(fOut,"%0.11f\t%0.11f\n",(double)i/fs,x[i]);}
	for(int i=0;i<N;i++){fprintf(fOut,"%0.8f\t%0.8f\n",(double)i/fs,data.recordedSamples[i]);}
	fclose(fOut);

	//generate_animation(time,data.recordedSamples,data.maxFrameIndex,mp3File,outputMp4File);
	write_gnuplot_scripts_with_keys(aFile,theSong.noteFile,time[N-1],"Chromatic Scale",false);
}
