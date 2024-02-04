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
	int ch;
	string inputFile,refFile,outputFile;
	double timePerMeasure;
	while((ch=getopt(argc,argv,"i:r:t:o:"))!=EOF)
		{switch(ch)
			{case 'i':
				// Sound Input Wave File (possibly a recording)
				inputFile=optarg;
				break;
			case 'r':
				// Sound Reference Defining Desired Frequencies
				refFile=optarg;
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


	// Total Song Array Size = numMeasures * beatsPerMeasure * timePerBeat * fs
	double fs=SAMPLE_RATE, f;	// Hertz, seconds, seconds	
	int numNotes=0, noteSize, N, index=0;
	string delimiter=GLOBAL_DELIMITER, innerDelimiter="`", letter, octave, noteLst="",durationLst="",ampLst="", tmp, tmpFile;
	string blockNameLst="",blockDefLst="";
	// Checked for Blocked Music Input File
	int numBlocks=check_for_blocked_music_input(refFile,blockNameLst,blockDefLst);
	// Handle Blocked Music Input
	if(numBlocks>0)
		{tmpFile=refFile+"_expanded";
		write_expanded_music_input_file(tmpFile,blockNameLst,blockDefLst,numBlocks);
		collect_measures_from_input_file(tmpFile,timePerMeasure,innerDelimiter,delimiter,noteLst,durationLst,ampLst,numNotes);}
	else
		{// Regular Music Input
		collect_measures_from_input_file(refFile,timePerMeasure,innerDelimiter,delimiter,noteLst,durationLst,ampLst,numNotes);}
	//cout<<durationLst<<endl;
	// Total Song Time (Seconds)
	double totalSongTime=determine_total_song_time(durationLst,numNotes,innerDelimiter);
	// Size of Array Containing Song
	int songArraySz=totalSongTime*fs;		cout<<songArraySz<<endl;
	if(songArraySz%2!=0){songArraySz--;}
	double *theOutput=new double[songArraySz];
	for(int i=0;i<songArraySz;i++){theOutput[i]=0;}
	// Note Durations (seconds)
	string *tmpArr2=fill_string_array(durationLst,numNotes,delimiter);
	// Notes
	string *tmpArr=fill_string_array(noteLst,numNotes,delimiter);
	// Amplitudes
	//string *amps=fill_string_array(ampLst,numNotes,innerDelimiter);
	string fileLst="",freqLst="",sIndexLst="",eIndexLst="",tempWaveFile,waveFldr=get_current_dir_name(); waveFldr+="/WAV/";
	int startIndex=0,endIndex,smallest;
	double *duration;
	string otherMarks,*notes;
	for(int i=0;i<numNotes;i++)
		{N=count_delimiter(tmpArr[i],innerDelimiter);
		notes=fill_string_array(tmpArr[i],N,innerDelimiter);
		duration=fill_double_array(tmpArr2[i],N,innerDelimiter);
		for(int j=0;j<1;j++)
			{letter=extract_letter_from_noteCode(notes[j]);
			otherMarks=extract_other_marks_from_noteCode(notes[j]);
			//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
			noteSize=duration[j]*fs;							// Array size of note			
			if(j==0){smallest=noteSize;}
			else if(noteSize<smallest){smallest=noteSize;}
			octave=extract_octave_from_noteCode(notes[j]);
			// Get Desired Frequency (This is the frequency the note should be)
			f=getFrequencyOfNote(letter,atoi(octave.c_str()));
			freqLst+=formatNumberString(cnvrtNumToStrng(f,SIG_FIGS))+delimiter;
			// Clip (Time Window) Input Wave File to Correct Out-Of-Tune Sounds
			startIndex=index; sIndexLst+=cnvrtNumToStrng(startIndex,0)+delimiter;
			endIndex=startIndex+noteSize-1;	eIndexLst+=cnvrtNumToStrng(endIndex,0)+delimiter;
			tempWaveFile=waveFldr+"clip_"+cnvrtNumToStrng(i+1,0)+".wav";
			//cout<<"|"<<i<<"|"<<startIndex<<"|"<<endIndex<<endl;
			//cout<<noteSize<<"|"<<duration[j]<<endl;
			clip_wave_file(inputFile,startIndex,endIndex,tempWaveFile);
			fileLst+=tempWaveFile+delimiter;
			}
		// Update time index
		index+=smallest;
		}

	// Determine Change in Frequency/Correction for Each Note
	string *theFiles=fill_string_array(fileLst,numNotes,delimiter);
	double *theFreqs=fill_double_array(freqLst,numNotes,delimiter);
	int *theStart=fill_int_array(sIndexLst,numNotes,delimiter);
	int *theEnd=fill_int_array(eIndexLst,numNotes,delimiter);
	double freqAtMax,*clippedValues,changeInFrequency,desiredFrequency,*tuned;
	int theIndex,Sz;
	freqLst=""; fileLst="";
	for(int i=0;i<numNotes;i++)
		{// Open Clipped WAve File for Fourier Frequency Analysis
		clippedValues=read_amplitudes_in_wave_file(theFiles[i],Sz);
		// Determine Maximum Peak from Fourier Magnitude Spectrum
		freqAtMax=determine_maximum_frequency_peak(clippedValues,Sz,theIndex);
		//run_csound(
		desiredFrequency=theFreqs[i];
		// Estimate Out-Of-Tuneness
		changeInFrequency=desiredFrequency-freqAtMax;
		// Define Output Wave File
		tmpFile=extract_containing_folder(theFiles[i]); tmpFile+="tuned_"+cnvrtNumToStrng(i+1,0)+".wav";
		//cout<<"|"<<i<<"|"<<changeInFrequency<<"|"<<desiredFrequency<<endl;
		fileLst+=tmpFile+delimiter;
		// Tune Sound
		frequency_shift_signal_2_channel_input(theFiles[i],changeInFrequency,envp,tmpFile);
		}

	// Construct Output
	delete [] theFiles;
	theFiles=fill_string_array(fileLst,numNotes,delimiter);
	double *tunedValues;
	for(int i=0;i<numNotes;i++)
		{// Open Tuned Files
		tunedValues=read_amplitudes_in_wave_file(theFiles[i],Sz);	//cout<<i<<"|"<<Sz<<endl;
		// Update Output
		for(int j=0;j<Sz;j++)
			{theIndex=theStart[i]+j;
			if(theIndex<songArraySz){theOutput[theIndex]=tunedValues[j];}
			}
		}

	// Write Output
	write_wave_file(outputFile,theOutput,songArraySz);

}
