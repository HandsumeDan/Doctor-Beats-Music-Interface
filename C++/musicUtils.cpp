# include "musicUtils.h"

double abs_maximum_of_array(double* theArr,int N,int &index)
	{double Output;
	for(int i=0;i<N;i++)
		{if(i==0){Output=abs(theArr[i]); index=i;}
		if(abs(theArr[i])>Output){Output=abs(theArr[i]); index=i;}
		}
	return Output;}

double abs_maximum_of_array(SAMPLE* theArr,int N,int &index)
	{double Output=0;
	for(int i=0;i<N;i++)
		{if(i==0){Output=abs(theArr[i]); index=i;}
		if(abs(theArr[i])>Output){Output=abs(theArr[i]); index=i;}
		}
	return Output;}

double calc_average(double* X,int Sz)
	{double Output=0;
	int Counter=0;
	for(int i=0;i<Sz;i++)
		{if(!isnan(X[i]))
			{Output+=X[i];
			Counter++;}
		}
	if(Counter==0){return 0;}
	return Output/Counter;}

double calc_weighted_average(double* X,double* W,int Sz)
	{double Output=0;
	for(int i=0;i<Sz;i++)
		{if(!isnan(X[i]) && !isnan(W[i]))
			{Output+=X[i]*W[i];}
		}
	return Output;}

double calc_chi_squared(double *Y,double *expectedY,int N)
	{double Output=0;
	for(int i=0;i<N;i++){Output+=(Y[i]-expectedY[i])*(Y[i]-expectedY[i])/expectedY[i];}
	return Output;}

double calc_std_dev(double* X,int Sz,double Avg)
	{double Output=0;
	int Counter=0;
	for(int i=0;i<Sz;i++)
		{if(!isnan(X[i]))
			{Output+=(X[i]-Avg)*(X[i]-Avg);
			Counter++;}
		}
	if(Counter==1){return sqrt(Output/Counter);}
	else if(Counter==0){return 0;}
	else{return sqrt(Output/(Counter-1));}}

double calculate_guitar_pitch(double desiredFrequency)
	{string refFile=get_current_dir_name(); refFile+="/CSOUND/guitar_pitch_freq_relationship";
	ifstream fIn;
	fIn.open(refFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in calculate_guitar_pitch!\nCould not open reference file containing relationship between pitch and frequency.\n"<<refFile<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE,pos;
	char Val[Sz];
	string tmp,pitch,freq,allContent="",delimiter=";",pitchLst="",freqLst="";
	// Skip 1st Line
	fIn.getline(Val,Sz);
	int numLines=60;	
	for(int i=0;i<numLines;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		allContent+=tmp+delimiter;}
	fIn.close();
	string *theContent=fill_string_array(allContent,numLines,delimiter);
	for(int i=0;i<numLines;i++)
		{tmp=theContent[i];
		pos=tmp.find("\t",0);
		if(pos==string::npos){cerr<<"ERROR in calculate_guitar_pitch!\nExiting...\n"; exit(EXIT_FAILURE);}
		// Extract Pitch
		pitch=tmp.substr(0,pos);
		pitchLst+=pitch+delimiter;
		// Extract Frequency
		freq=tmp.substr(pos+1,tmp.length()-pos-1);
		freqLst+=freq+delimiter;
		}
	
	double *thePitchs=fill_double_array(pitchLst,numLines,delimiter),f1,f2,p1,p2;
	double *theFreqs=fill_double_array(freqLst,numLines,delimiter);
	if(desiredFrequency<theFreqs[0]){cerr<<"ERROR in calculate_guitar_pitch!\nDesired Frequency is too low and outside of the playable range\n"; exit(EXIT_FAILURE);}
	if(desiredFrequency>theFreqs[numLines-1]){cerr<<"ERROR in calculate_guitar_pitch!\nDesired Frequency is too high and outside of the playable range\n"; exit(EXIT_FAILURE);}
	for(int i=1;i<numLines;i++)
		{// Compare Frequencies with input
		f1=theFreqs[i-1];	p1=thePitchs[i-1];
		f2=theFreqs[i];	p2=thePitchs[i];
		if(desiredFrequency>=f1 && desiredFrequency<f2)
			{return interpolate(desiredFrequency,f1,f2,p1,p2);}
		}
	cerr<<"Critical Error in calculate_guitar_pitch!\nShould not be able to reach the end of this function\n"; exit(EXIT_FAILURE);}

double calculate_plucked_bass_pitch(double desiredFrequency)
	{string refFile=get_current_dir_name(); refFile+="/CSOUND/plucked_bass_pitch_freq_relationship";
	ifstream fIn;
	fIn.open(refFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in calculate_plucked_bass_pitch!\nCould not open reference file containing relationship between pitch and frequency.\n"<<refFile<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE,pos;
	char Val[Sz];
	string tmp,pitch,freq,allContent="",delimiter=";",pitchLst="",freqLst="";
	// Skip 1st Line
	fIn.getline(Val,Sz);
	int numLines=110;	
	for(int i=0;i<numLines;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		allContent+=tmp+delimiter;}
	fIn.close();
	string *theContent=fill_string_array(allContent,numLines,delimiter);
	for(int i=0;i<numLines;i++)
		{tmp=theContent[i];
		pos=tmp.find("\t",0);
		if(pos==string::npos){cerr<<"ERROR in calculate_plucked_bass_pitch!\nExiting...\n"; exit(EXIT_FAILURE);}
		// Extract Pitch
		pitch=tmp.substr(0,pos);
		pitchLst+=pitch+delimiter;
		// Extract Frequency
		freq=tmp.substr(pos+1,tmp.length()-pos-1);
		freqLst+=freq+delimiter;
		}
	
	double *thePitchs=fill_double_array(pitchLst,numLines,delimiter),f1,f2,p1,p2;
	double *theFreqs=fill_double_array(freqLst,numLines,delimiter);
	if(desiredFrequency<theFreqs[0])
		{cerr<<"ERROR in calculate_plucked_bass_pitch!\nDesired Frequency is too low and outside of the playable range\n"; exit(EXIT_FAILURE);
		f1=theFreqs[0];	p1=thePitchs[0];
		f2=theFreqs[1];	p2=thePitchs[1];	
		return extrapolate(desiredFrequency,f1,f2,p1,p2);}
	if(desiredFrequency>theFreqs[numLines-1])
		{cerr<<"ERROR in calculate_plucked_bass_pitch!\nDesired Frequency is too high and outside of the playable range\n"; exit(EXIT_FAILURE);
		f1=theFreqs[numLines-2];	p1=thePitchs[numLines-2];
		f2=theFreqs[numLines-1];	p2=thePitchs[numLines-1];
		return extrapolate(desiredFrequency,f1,f2,p1,p2);}
	for(int i=1;i<numLines;i++)
		{// Compare Frequencies with input
		f1=theFreqs[i-1];	p1=thePitchs[i-1];
		f2=theFreqs[i];	p2=thePitchs[i];
		if(desiredFrequency>=f1 && desiredFrequency<f2)
			{return interpolate(desiredFrequency,f1,f2,p1,p2);}
		}
	cerr<<"Error in calculate_plucked_bass_pitch!\nCould not determine pitch from frequency("<<desiredFrequency<<") ["<<theFreqs[0]<<","<<theFreqs[numLines-1]<<"]\n"; exit(EXIT_FAILURE);}

double calculate_plucked_string_pitch(double desiredFrequency)
	{string refFile=get_current_dir_name(); refFile+="/CSOUND/plucked_string_pitch_freq_relationship";
	ifstream fIn;
	fIn.open(refFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in calculate_plucked_string_pitch!\nCould not open reference file containing relationship between pitch and frequency.\n"<<refFile<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE,pos;
	char Val[Sz];
	string tmp,pitch,freq,allContent="",delimiter=";",pitchLst="",freqLst="";
	// Skip 1st Line
	fIn.getline(Val,Sz);
	int numLines=200;	
	for(int i=0;i<numLines;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		allContent+=tmp+delimiter;}
	fIn.close();
	string *theContent=fill_string_array(allContent,numLines,delimiter);
	for(int i=0;i<numLines;i++)
		{tmp=theContent[i];
		pos=tmp.find("\t",0);
		if(pos==string::npos){cerr<<"ERROR in calculate_plucked_string_pitch!\nNo tab delimitation\nLine:\n"<<i<<"|"<<tmp<<"|\n"; exit(EXIT_FAILURE);}
		// Extract Pitch
		pitch=tmp.substr(0,pos);
		pitchLst+=pitch+delimiter;
		// Extract Frequency
		freq=tmp.substr(pos+1,tmp.length()-pos-1);
		freqLst+=freq+delimiter;
		}
	
	double *thePitchs=fill_double_array(pitchLst,numLines,delimiter),f1,f2,p1,p2;
	double *theFreqs=fill_double_array(freqLst,numLines,delimiter);
	if(desiredFrequency<theFreqs[0]){cerr<<"ERROR in calculate_plucked_string_pitch!\nDesired Frequency is too low and outside of the playable range\n"; exit(EXIT_FAILURE);}
	if(desiredFrequency>theFreqs[numLines-1]){cerr<<"ERROR in calculate_plucked_string_pitch!\nDesired Frequency is too high and outside of the playable range\n"; exit(EXIT_FAILURE);}
	for(int i=1;i<numLines;i++)
		{// Compare Frequencies with input
		f1=theFreqs[i-1];	p1=thePitchs[i-1];
		f2=theFreqs[i];	p2=thePitchs[i];
		if(desiredFrequency>=f1 && desiredFrequency<f2)
			{return interpolate(desiredFrequency,f1,f2,p1,p2);}
		}
	cerr<<"Critical Error in calculate_plucked_string_pitch!\nShould not be able to reach the end of this function\n"; exit(EXIT_FAILURE);}

double calculate_soundFonts_pitch(string note,int octaveLevel)
	{// Check For Error: OctaveLevel contained in noteLetter
	int numNumbers=9,pos;
	string *theNumbers=new string[numNumbers];
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	for(int i=0;i<numNumbers;i++)
		{pos=note.find(theNumbers[i],0);
		if(pos!=string::npos)
			{// Octave Found, Error Avoided
			octaveLevel=atoi(theNumbers[i].c_str());
			note=note.substr(0,pos);
			break;}
		}
	// Check For Error: OtherMarks contained in octaveLevel
	string tmp=cnvrtNumToStrng(octaveLevel,0);
	if(tmp.length()>=2)
		{for(int i=0;i<numNumbers;i++)
			{pos=tmp.find(theNumbers[i],0);
			if(pos!=string::npos)
				{// Octave Found
				octaveLevel=atoi(theNumbers[i].c_str());
				break;}
			}
		}
	// Middle C (C4) has a MIDI note number of 60
	double pitch;
	if(note.compare("Bs")==0||note.compare("C")==0)
		{// B# Sharp / C Natural		
		pitch=24+12*(octaveLevel-1);}
	else if(note.compare("Cs")==0||note.compare("Df")==0)
		{// C# Sharp / Db Flat
		pitch=25+12*(octaveLevel-1);}
	else if(note.compare("D")==0)
		{// D Natural
		pitch=26+12*(octaveLevel-1);}
	else if(note.compare("Ds")==0||note.compare("Ef")==0)
		{// D# Sharp / Eb Flat
		pitch=27+12*(octaveLevel-1);}
	else if(note.compare("E")==0||note.compare("Ff")==0)
		{// E Natural / Fb Flat
		pitch=28+12*(octaveLevel-1);}
	else if(note.compare("Es")==0||note.compare("F")==0)
		{// F Natural / E# Sharp
		pitch=29+12*(octaveLevel-1);}
	else if(note.compare("Fs")==0||note.compare("Gf")==0)
		{// F# Sharp / Gb Flat
		pitch=30+12*(octaveLevel-1);}
	else if(note.compare("G")==0)
		{// G Natural
		pitch=31+12*(octaveLevel-1);}
	else if(note.compare("Gs")==0||note.compare("Af")==0)
		{// G# Sharp / Ab Flat
		pitch=32+12*(octaveLevel-1);}
	else if(note.compare("As")==0||note.compare("Bf")==0)
		{// A# Sharp / Bb Flat
		pitch=34+12*(octaveLevel-1);}
	else if(note.compare("A")==0)
		{// A Natural
		pitch=33+12*(octaveLevel-1);}
	else if(note.compare("B")==0||note.compare("Cf")==0)
		{// B Natural
		pitch=35+12*(octaveLevel-1);}
	else
		{cerr<<"Error in calculate_soundFonts_pitch()!\nUn-recognized note ("<<note<<") (octave: "<<octaveLevel<<")\n";
		exit(EXIT_FAILURE);}
	return pitch;}

int check_for_blocked_music_input(string inFile,string &blockNameLst,string &blockDefLst)
	{ifstream fIn;
	// Checked for Blocked Music Input File
	fIn.open(inFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in check_for_blocked_music_input!\nInput song file could not be opened.\n"<<inFile<<endl; exit(EXIT_FAILURE);}
	int Sz=LINE_SIZE,pos,pos2,pos3,numBlocks=0;
	char Val[Sz];
	string tmp,allContent="",delimiter=GLOBAL_DELIMITER,blockName,blockDef;
	fIn.getline(Val,Sz);
	while(!fIn.eof())
		{tmp=Val;
		allContent+=tmp+"\n";
		fIn.getline(Val,Sz);}
	fIn.close();
	// Count Blocks
	numBlocks=0;
	bool SEARCHING=true;
	pos=0;
	blockNameLst=""; blockDefLst="";
	while(SEARCHING)
		{pos=allContent.find("DEFINE ",pos);
		if(pos!=string::npos)
			{// Extract Block Variable Name
			pos2=allContent.find("\n",pos);
			blockName=allContent.substr(pos+7,pos2-pos-7);	//cout<<"|"<<blockName<<"|"<<endl;
			blockNameLst+=blockName+delimiter;
			// Extract Block Variable Definition
			pos3=allContent.find("END DEFINE",pos);
			if(pos3==string::npos)
				{pos3=allContent.find("ENDDEFINE",pos);
				if(pos3==string::npos){cerr<<"Error DEFINE statement missing END DEFINE"; exit(EXIT_FAILURE);}
				}
			blockDef=allContent.substr(pos2,pos3-pos2);		//cout<<"|"<<blockDef<<"|"<<endl;
			blockDefLst+=blockDef+delimiter;
			pos++; numBlocks++;}
		else{SEARCHING=false; break;}
		}
	return numBlocks;}

void clip_wave_file(string inputWaveFile,int startIndex,int endIndex,string outputWaveFile)
	{SNDFILE *SoundFile;
	SF_INFO SoundFileInfo;
	int bufferSize=256, filetype = SF_ENDIAN_LITTLE | SF_FORMAT_RAW | SF_FORMAT_PCM_16 , intVal, returnValue;
	short buffer[bufferSize];
	for(int k=0;k<bufferSize;k++){buffer[k]=k;}
	
	filetype=SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	SoundFile=sf_open(inputWaveFile.c_str(), SFM_READ, &SoundFileInfo);
	if(!SoundFile)
		{printf ("Line %d: sf_open_read failed attempting to open:\n%s\n", __LINE__,inputWaveFile.c_str());
		fflush(stdout);
		sf_perror(NULL);
		exit(EXIT_FAILURE);}

	returnValue=sf_seek(SoundFile,0,SEEK_SET);		// returns position offset in file cout<<returnValue<<endl;
	if(returnValue<0){cerr<<"Error seeking to beginning of file\n";}

	// Assess Wave File
	if(SoundFileInfo.format != filetype)
		{//printf ("Line %d: Returned format incorrect (0x%08X => 0x%08X).\n", __LINE__, filetype, SoundFileInfo.format);
		//exit(EXIT_FAILURE);
		}
	
	if(SoundFileInfo.frames < bufferSize)
		{printf ("Line %d: Incorrect number of frames in file (%s). (%d => %ld)\n", __LINE__, inputWaveFile.c_str(), bufferSize, (long)SoundFileInfo.frames);
		exit(EXIT_FAILURE);
		}
	
	// Read Wave File
	int Sz=SoundFileInfo.channels*SoundFileInfo.frames; //SoundFileInfo.channels * SoundFileInfo.frames;
	double *theInput=new double[Sz];
	intVal=sf_readf_double(SoundFile,theInput,Sz);
	if( intVal != Sz)
		{if(intVal>0)
			{//printf ("Line %d: short read (%d).\nNumber of Frames: %d\nNumber of Channels: %d\n%s\n", __LINE__, intVal, SoundFileInfo.frames, SoundFileInfo.channels,inputWaveFile.c_str()) ;
			//exit(EXIT_FAILURE);
			//N=intVal;
			}
		else if(intVal==0)
			{// End of File (EOF) Reach need to seek to top
			returnValue=sf_seek(SoundFile,0,SEEK_SET);		//cout<<returnValue<<endl;
			}
		else{cerr<<"So we got here\n";}
		}

	if(Sz<0){cerr<<"Error in clip_wave_file!\nNegative number of frames found|!\n"; exit(EXIT_FAILURE);}
	else if(Sz==0){cerr<<"Error in clip_wave_file!\nNo frames found|!\n";}
	returnValue=sf_close(SoundFile);
	if(returnValue!=0){cerr<<"Error in clip_wave_file!\nCould not close wave file:\n"<<inputWaveFile<<"\n"; exit(EXIT_FAILURE);}
	sf_close(SoundFile);

	// Clip Wave File Date using indices
	int N=(endIndex-startIndex)+1,Counter=0;
	//N=Sz;
	if(N%2!=0){N--;}
	double* theOutput=new double[N];
	for(int i=0;i<N;i++){theOutput[i]=theInput[startIndex+i];}

	// Write Output Wave File
	write_wave_file(outputWaveFile,theOutput,N);
	}

string cnvrtNumToStrng(double Num,int numberAfterDecimalpoint)
    {stringstream ss;       //create stringstream
     ss.setf(ios::fixed);
     if(numberAfterDecimalpoint>0)
        {ss.setf(ios::showpoint);}
     //ss.setf(ios::showpos);
     ss.precision(numberAfterDecimalpoint);
     ss<<Num;               //add number to stream
     return ss.str();}

string cnvrtNumToStrng(int Num,int numberAfterDecimalpoint)
    {stringstream ss;       //create stringstream
     ss.setf(ios::fixed);
     if(numberAfterDecimalpoint>0)
        {ss.setf(ios::showpoint);}
     //ss.setf(ios::showpos);
     ss.precision(numberAfterDecimalpoint);
     ss<<Num;               //add number to stream
     return ss.str();}      //return string with contents of stream

string cnvrtNumToStrng(SAMPLE Num,int numberAfterDecimalpoint)
    {stringstream ss;       //create stringstream
     ss.setf(ios::fixed);
     if(numberAfterDecimalpoint>0)
        {ss.setf(ios::showpoint);}
     //ss.setf(ios::showpos);
     ss.precision(numberAfterDecimalpoint);
     ss<<Num;               //add number to stream
     return ss.str();}

void collect_measures_from_drums_input_file(string inFile,double timePerMeasure,string* &theNotes,string* &theDurations,string* &theAmplitudes,int &numUsedDrums)
	{int N, numDrumTypes=23;
	string* theDrumTypes=new string[numDrumTypes];
	double fs=SAMPLE_RATE;
	// Drum Types
	theDrumTypes[0]=DRUM_01;								// "CRASH_CYMBAL_1";
	theDrumTypes[1]=DRUM_02;								// "CRASH_CYMBAL_2";
	theDrumTypes[2]=DRUM_03;								// "REVERSE_CYMBAL";
	theDrumTypes[3]=DRUM_04;								// "RIDE_CYMBAL";
	theDrumTypes[4]=DRUM_05;								// "808_CYMBAL";
	theDrumTypes[5]=DRUM_06;								// "OPEN_HIHAT_1";
	theDrumTypes[6]=DRUM_07;								// "OPEN_HIHAT_2";
	theDrumTypes[7]=DRUM_08;								// "808_OPEN_HIHAT";
	theDrumTypes[8]=DRUM_09;								// "CLOSED_HIHAT";
	theDrumTypes[9]=DRUM_10;								// "808_CLOSED_HIHAT";
	theDrumTypes[10]=DRUM_11;							// "TAMBOURINE";
	theDrumTypes[11]=DRUM_12;							// "BASS_DRUM";
	theDrumTypes[12]=DRUM_13;							// "808_BASS_DRUM";
	theDrumTypes[13]=DRUM_14;							// "HAND_CLAP";
	theDrumTypes[14]=DRUM_15;							// "808_COWBELL";
	theDrumTypes[15]=DRUM_16;							// "909_SNARE_1";
	theDrumTypes[16]=DRUM_17;							// "909_SNARE_2";
	theDrumTypes[17]=DRUM_18;							// "909_SNARE_3";
	theDrumTypes[18]=DRUM_19;							// "909_SNARE_4";
	theDrumTypes[19]=DRUM_20;							// "HIGH_TOM";
	theDrumTypes[20]=DRUM_21;							// "MID_TOM";
	theDrumTypes[21]=DRUM_22;							// "LOW_TOM";
	theDrumTypes[22]=DRUM_23;							// "RIM_SHOT";

	collect_measures_from_percussion_file(inFile,timePerMeasure,theDrumTypes,numDrumTypes,theNotes,theDurations,theAmplitudes,numUsedDrums);

	}

void collect_lyrics_from_input_file(string inFile,double timePerMeasure,string innerDelimiter,string outerDelimiter,string &theLyrics,string &theNotes,string &theDurations,string &theAmplitudes,int &numNotes)
	{double fs=SAMPLE_RATE, f;	// Hertz, seconds, seconds
	int numFrequencies=63, N;
	string* frequencies=new string[numFrequencies];		// 52 white keys, 88 keys total
	frequencies[0]="C0"; frequencies[1]="D0"; frequencies[2]="E0"; frequencies[3]="F0"; frequencies[4]="G0"; frequencies[5]="A0"; frequencies[6]="B0";
	frequencies[7]="C1"; frequencies[8]="D1"; frequencies[9]="E1"; frequencies[10]="F1"; frequencies[11]="G1"; frequencies[12]="A1"; frequencies[13]="B1"; 
	frequencies[14]="C2"; frequencies[15]="D2"; frequencies[16]="E2"; frequencies[17]="F2"; frequencies[18]="G2"; frequencies[19]="A2"; frequencies[20]="B2";
	frequencies[21]="C3"; frequencies[22]="D3"; frequencies[23]="E3"; frequencies[24]="F3"; frequencies[25]="G3"; frequencies[26]="A3"; frequencies[27]="B3";
	frequencies[28]="C4"; frequencies[29]="D4"; frequencies[30]="E4"; frequencies[31]="F4"; frequencies[32]="G4"; frequencies[33]="A4"; frequencies[34]="B4";
	frequencies[35]="C5"; frequencies[36]="D5"; frequencies[37]="E5"; frequencies[38]="F5"; frequencies[39]="G5"; frequencies[40]="A5"; frequencies[41]="B5";
	frequencies[42]="C6"; frequencies[43]="D6"; frequencies[44]="E6"; frequencies[45]="F6"; frequencies[46]="G6"; frequencies[47]="A6"; frequencies[48]="B6";
	frequencies[49]="C7"; frequencies[50]="D7"; frequencies[51]="E7"; frequencies[52]="F7"; frequencies[53]="G7"; frequencies[54]="A7"; frequencies[55]="B7";
	frequencies[56]="C8"; frequencies[57]="D8"; frequencies[58]="E8"; frequencies[59]="F8"; frequencies[60]="G8"; frequencies[61]="A8"; frequencies[62]="B8";

	ifstream fIn;
	fIn.open(inFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in collect_lyrics_from_input_file!\nInput music file could not be opened.\n"<<inFile<<endl; exit(EXIT_FAILURE);}
	int Sz=LINE_SIZE, pos, pos2, pos3, pos4, freqIndex, freqsInMeasure;
	// Define Possible Numbers
	int numNumbers=9,index,fSz;
	string *theNumbers=new string[numNumbers];
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	char Val[Sz];
	string lst, tmp,inputValue,topFreq, bottomFreq, nextFreq, freqLst="",delimiter=outerDelimiter;
	string* measure;
	// Read Header, Should Contain at Minimum: Sound Type & Tempo
	bool SOUND_TYPE_FOUND=false, COLLECTING_MEASURE, SEARCHING, READING_MEASURE, NOTE_SHARP, NOTE_FLAT, NOTE_DOUBLE_SHARP, NOTE_DOUBLE_FLAT;
	// Time per measure as been specified so this signal can already be defined discretely with respect to time knowing the sampling frequency, which is 44,100 Hz
	int firstNotePos, nextNotePos, notePos, measureCounter=0, measureSize=timePerMeasure*fs;		// Array size of Measure's Signal
	string* frequenciesInMeasure, letter, octave;
	double* timeOfMeasure=new double[measureSize];
	double noteDuration, fraction;
	string lyricLst="",noteLst="",durationLst="",ampLst="",tempLyric,tempNote,tempDuration,tempAmp;
	// Identify and Read In Measures
	fIn.getline(Val,Sz);
	//cout<<inFile<<endl;
	while(!fIn.eof())
		{tmp=Val;
		//cout<<"|"<<tmp<<"|\n";
		// Exclude comments
		pos=tmp.find("#",0);
		if(pos!=0 && pos!=1 && tmp.length()>1 )
			{// Find Top Frequency of Measure (will be labeled with appropriate frequency) A1-C8
			freqLst=""; freqsInMeasure=0; topFreq="";
			for(int i=0;i<numFrequencies;i++)
				{pos=tmp.find(frequencies[i],0);
				if(pos!=string::npos){topFreq=frequencies[i]; freqLst+=frequencies[i]+delimiter; freqIndex=i; break;}
				}
			if(topFreq.compare("")!=0)
				{// Top of Measure Found, so Record it
				inputValue="";
				inputValue+=tmp+"\n"; freqsInMeasure++; freqIndex--;
				//cout<<freqIndex<<endl;
				COLLECTING_MEASURE=true;
				fIn.getline(Val,Sz);
				while(COLLECTING_MEASURE)
					{tmp=Val;
					nextFreq=frequencies[freqIndex];
					pos=tmp.find(nextFreq,0);
					if(pos==string::npos)
						{// End of Measure
						COLLECTING_MEASURE=false;
						break;}
					else
						{inputValue+=tmp+"\n"; freqLst+=frequencies[freqIndex]+delimiter; freqsInMeasure++; freqIndex--;
						if(freqIndex<0){COLLECTING_MEASURE=false; break;}						
						}
					fIn.getline(Val,Sz);}
				// Continue Reading Frequency-Variant Instrument
				//cout<<freqsInMeasure<<"\n"<<inputValue<<endl;
				// Define Frequencies Input by User In Measure
				frequenciesInMeasure=fill_string_array(freqLst,freqsInMeasure,delimiter);	//for(int i=0;i<freqsInMeasure;i++){cout<<frequenciesInMeasure[i]<<endl;}
				// Define Measure as Input by User
				measure=fill_string_array(inputValue,freqsInMeasure,"\n");
				// Get Position  of First Note(s), used as reference for other notes positions
				firstNotePos=100;
				for(int i=0;i<freqsInMeasure;i++)
					{// Search for Lyrics
					for(int j=0;j<numNumbers;j++)
						{pos=measure[i].find(theNumbers[j],6);
						if(pos!=string::npos){if(pos<firstNotePos){firstNotePos=pos; index=j;}}
						}
					}
				//cout<<firstNotePos<<endl;
				// Determine Needed Frequencies for first note
				for(int i=0;i<freqsInMeasure;i++)
					{pos=measure[i].find(theNumbers[index],6);
					
					NOTE_SHARP=false; NOTE_FLAT=false; NOTE_DOUBLE_SHARP=false; NOTE_DOUBLE_FLAT=false;
					if(pos==firstNotePos)
						{// Extract Note and Duration
						tempNote=""; tempDuration=""; tempAmp="";
						extract_lyrics_from_measure(measure[i],inputValue,pos,timePerMeasure,frequenciesInMeasure[i],NOTE_SHARP,\
													NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempLyric,tempNote,tempDuration,tempAmp,fSz);
						// Append Note and Duration to List
						lyricLst+=tempLyric+innerDelimiter;
						noteLst+=tempNote+innerDelimiter;
						durationLst+=tempDuration+innerDelimiter;
						ampLst+=tempAmp+innerDelimiter;
						}
					}
				durationLst+=delimiter;
				lyricLst+=delimiter;
				noteLst+=delimiter;
				ampLst+=delimiter;

				READING_MEASURE=true;
				notePos=firstNotePos+fSz;			
				while(READING_MEASURE)
					{// Get Position of Next Note(s), if Exist
					nextNotePos=Sz;
					for(int i=0;i<freqsInMeasure;i++)
						{if(notePos<measure[i].length())
							{// Search lyrics
							for(int j=0;j<numNumbers;j++)
								{pos=measure[i].find(theNumbers[j],notePos);
								if(pos!=string::npos){if(pos<nextNotePos){nextNotePos=pos; index=j;}}
								}
							}
						}
					//cout<<nextNotePos<<endl;
					if(nextNotePos==Sz){READING_MEASURE=false; break;}
					// Determine Frequencies for Note
					for(int i=0;i<freqsInMeasure;i++)
						{pos=measure[i].find(theNumbers[index],notePos);
						NOTE_SHARP=false; NOTE_FLAT=false; NOTE_DOUBLE_SHARP=false; NOTE_DOUBLE_FLAT=false;
						if(pos==nextNotePos)
							{tempNote=""; tempDuration=""; tempAmp="";
							extract_lyrics_from_measure(measure[i],inputValue,pos,timePerMeasure,frequenciesInMeasure[i],NOTE_SHARP,\
														NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempLyric,tempNote,tempDuration,tempAmp,fSz);
							// Append Note and Duration to List
							lyricLst+=tempLyric+innerDelimiter;
							noteLst+=tempNote+innerDelimiter;
							durationLst+=tempDuration+innerDelimiter;
							ampLst+=tempAmp+innerDelimiter;
							}
						}
					durationLst+=delimiter;
					lyricLst+=delimiter;
					noteLst+=delimiter;
					ampLst+=delimiter;
					// Update to get next note
					notePos=nextNotePos+fSz;
					}
				measureCounter++;
				}
			} /* end of comment filter and line length filter */
		fIn.getline(Val,Sz);
		}
	fIn.close();
	
	// Remove Last Delimiter from Note and Duration Lists
	//noteLst=noteLst.substr(0,noteLst.length()-1);
	numNotes=count_delimiter(lyricLst,delimiter);
	//cout<<numNotes<<"\n"<<noteLst<<"\n"<<durationLst<<endl; //exit(1);
	if(count_delimiter(durationLst,delimiter)!=numNotes){cerr<<"Error in collect_lyrics_from_input_file!\nNotes and Durations do not match!\n"; exit(EXIT_FAILURE);}	
	// Duration of Each Note
	theDurations=durationLst;
	// Lyrics
	theLyrics=lyricLst;
	// Notes
	theNotes=noteLst;
	// Note Amplitudes
	theAmplitudes=ampLst;
	}

void collect_measures_from_input_file(string inFile,double timePerMeasure,string innerDelimiter,string outerDelimiter,string &theNotes,string &theDurations,string &theAmplitudes,int &numNotes)
	{double fs=SAMPLE_RATE, f, PI=2.0*acos(0.0);	// Hertz
	int numFrequencies=63, N;
	string* frequencies=new string[numFrequencies];		// 52 white keys, 88 keys total
	frequencies[0]="C0"; frequencies[1]="D0"; frequencies[2]="E0"; frequencies[3]="F0"; frequencies[4]="G0"; frequencies[5]="A0"; frequencies[6]="B0";
	frequencies[7]="C1"; frequencies[8]="D1"; frequencies[9]="E1"; frequencies[10]="F1"; frequencies[11]="G1"; frequencies[12]="A1"; frequencies[13]="B1"; 
	frequencies[14]="C2"; frequencies[15]="D2"; frequencies[16]="E2"; frequencies[17]="F2"; frequencies[18]="G2"; frequencies[19]="A2"; frequencies[20]="B2";
	frequencies[21]="C3"; frequencies[22]="D3"; frequencies[23]="E3"; frequencies[24]="F3"; frequencies[25]="G3"; frequencies[26]="A3"; frequencies[27]="B3";
	frequencies[28]="C4"; frequencies[29]="D4"; frequencies[30]="E4"; frequencies[31]="F4"; frequencies[32]="G4"; frequencies[33]="A4"; frequencies[34]="B4";
	frequencies[35]="C5"; frequencies[36]="D5"; frequencies[37]="E5"; frequencies[38]="F5"; frequencies[39]="G5"; frequencies[40]="A5"; frequencies[41]="B5";
	frequencies[42]="C6"; frequencies[43]="D6"; frequencies[44]="E6"; frequencies[45]="F6"; frequencies[46]="G6"; frequencies[47]="A6"; frequencies[48]="B6";
	frequencies[49]="C7"; frequencies[50]="D7"; frequencies[51]="E7"; frequencies[52]="F7"; frequencies[53]="G7"; frequencies[54]="A7"; frequencies[55]="B7";
	frequencies[56]="C8"; frequencies[57]="D8"; frequencies[58]="E8"; frequencies[59]="F8"; frequencies[60]="G8"; frequencies[61]="A8"; frequencies[62]="B8";

	ifstream fIn;
	fIn.open(inFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in collect_measures_from_input_file!\nInput music file could not be opened.\n"<<inFile<<endl; exit(EXIT_FAILURE);}
	int Sz=LINE_SIZE, pos, pos2, pos3, pos4, freqIndex, freqsInMeasure,tmpNumber;
	char Val[Sz];
	string lst, tmp,inputValue,topFreq, bottomFreq, nextFreq, freqLst="",delimiter=outerDelimiter, f1,f2,letter1,number1,letter2,number2,nmbr;
	string* measure;
	// Read Header, Should Contain at Minimum: Sound Type & Tempo
	bool SOUND_TYPE_FOUND=false, COLLECTING_MEASURE, SEARCHING, READING_MEASURE, NOTE_SHARP, NOTE_FLAT, NOTE_DOUBLE_SHARP, NOTE_DOUBLE_FLAT;
	// Time per measure as been specified so this signal can already be defined discretely with respect to time knowing the sampling frequency, which is 44,100 Hz
	int firstNotePos, nextNotePos, notePos, measureCounter=0, measureSize=timePerMeasure*fs;		// Array size of Measure's Signal
	string* frequenciesInMeasure, letter, octave;
	double* timeOfMeasure=new double[measureSize];
	double noteDuration, fraction;
	string noteLst="",durationLst="",ampLst="",tempNote,tempDuration,tempAmp;
	// Identify and Read In Measures
	fIn.getline(Val,Sz);
	//cout<<inFile<<endl;
	while(!fIn.eof())
		{tmp=Val;
		//cout<<"|"<<tmp<<"|\n";
		// Exclude comments
		pos=tmp.find("#",0);
		if(pos!=0 && pos!=1 && tmp.length()>1 )
			{// Find Top Frequency of Measure (will be labeled with appropriate frequency) A0-C8
			freqLst=""; freqsInMeasure=0; topFreq="";
			for(int i=0;i<numFrequencies;i++)
				{pos=tmp.find(frequencies[i],0);
				if(pos!=string::npos){topFreq=frequencies[i]; freqLst+=frequencies[i]+delimiter; freqIndex=i; break;}
				}
			//cout<<topFreq<<endl;
			if(topFreq.compare("")!=0)
				{// Top of Measure Found, so Record it
				inputValue="";
				inputValue+=tmp+"\n"; freqsInMeasure++; freqIndex--;
				//cout<<freqIndex<<endl;
				COLLECTING_MEASURE=true;
				fIn.getline(Val,Sz);
				while(COLLECTING_MEASURE)
					{tmp=Val;
					nextFreq=frequencies[freqIndex];
					pos=tmp.find(nextFreq,0);
					if(pos==string::npos)
						{// End of Measure						
						if(tmp.length()<6)
							{COLLECTING_MEASURE=false; break;}
						else
							{// Search for frequencies in in-correct order
							for(int i=0;i<numFrequencies;i++)
								{pos=tmp.find(frequencies[i],3);
								if(pos!=string::npos)
									{cout<<"Error in collect_measures_from_input_file!\nFrequencies are not in the correct order.\nTop Freq: "<<topFreq<<"\nFound: "<<frequencies[i]<<"\nLooking for: "<<nextFreq<<"\n"; exit(EXIT_FAILURE);}
								}
							// All in correct order, output input to user
							cerr<<"Error in collect_measures_from_input_file!\nUnknown input found:\n"<<tmp<<endl; exit(EXIT_FAILURE);
							}
						}
					else
						{inputValue+=tmp+"\n"; freqLst+=frequencies[freqIndex]+delimiter; freqsInMeasure++; freqIndex--;
						if(freqIndex<0){COLLECTING_MEASURE=false; break;}						
						}
					fIn.getline(Val,Sz);}
				//cout<<freqsInMeasure<<"\n"<<inputValue<<endl;	// Continue Reading Frequency-Variant Instrument
				// Define Frequencies Input by User In Measure
				frequenciesInMeasure=fill_string_array(freqLst,freqsInMeasure,delimiter);	//for(int i=0;i<freqsInMeasure;i++){cout<<frequenciesInMeasure[i]<<endl;}				
				// Define Measure as Input by User
				measure=fill_string_array(inputValue,freqsInMeasure,"\n");
				// Get Position  of First Note(s), used as reference for other notes positions
				firstNotePos=100;
				for(int i=0;i<freqsInMeasure;i++)
					{// Search for Notes
					pos=measure[i].find("O",0);									// Normal/Regular Notes
					if(pos!=string::npos){if(pos<firstNotePos){firstNotePos=pos;}}
					pos2=measure[i].find("S",3);								// Sustained Notes
					if(pos2!=string::npos){if(pos2<firstNotePos){firstNotePos=pos2;}}
					pos3=measure[i].find("X",0);							// Staccato Notes
					if(pos3!=string::npos){if(pos3<firstNotePos){firstNotePos=pos3;}}
					// Search for Rests
					pos=measure[i].find("R",3);
					if(pos!=string::npos){if(pos<firstNotePos){firstNotePos=pos;}}
					}
				//cout<<firstNotePos<<endl;
				// Determine Needed Frequencies for first note
				for(int i=0;i<freqsInMeasure;i++)
					{pos=measure[i].find("O",0); 		// Normal / Regular Notes
					pos2=measure[i].find("R",3);		// Rest
					pos3=measure[i].find("S",3);		// Sustained Notes
					pos4=measure[i].find("X",0);		// Staccato Notes
					NOTE_SHARP=false; NOTE_FLAT=false; NOTE_DOUBLE_SHARP=false; NOTE_DOUBLE_FLAT=false;
					if(pos==firstNotePos)
						{// Extract Note and Duration
						tempNote=""; tempDuration=""; tempAmp="";
						extract_regular_notes_from_measure(measure[i],inputValue,pos,timePerMeasure,frequenciesInMeasure[i],NOTE_SHARP,\
													NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempNote,tempDuration,tempAmp);
						// Append Note and Duration to List
						noteLst+=tempNote+innerDelimiter;
						durationLst+=tempDuration+innerDelimiter;
						ampLst+=tempAmp+innerDelimiter;
						// Show the Note
						if(VERBOSE)
							{//f=getFrequencyOfNote(letter,atoi(octave.c_str()));
							f=getFrequencyOfNote(extract_letter_from_noteCode(tempNote),atoi(extract_octave_from_noteCode(tempNote).c_str()));
							if(!NOTE_FLAT && !NOTE_DOUBLE_FLAT && !NOTE_SHARP && !NOTE_DOUBLE_SHARP){cout<<"Note ("<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
							else if(NOTE_FLAT){cout<<"Note (b"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
							else if(NOTE_DOUBLE_FLAT){cout<<"Note (bb"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
							else if(NOTE_SHARP){cout<<"Note (#"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
							else if(NOTE_DOUBLE_SHARP){cout<<"Note (##"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
							}

						}
					else if(pos2==firstNotePos)
						{// Rest
						tempNote=""; tempDuration="";
						extract_rests_from_measure(measure[i],inputValue,pos2,timePerMeasure,tempNote,tempDuration);
						// Append Note and Duration to List
						noteLst+=tempNote+innerDelimiter;
						durationLst+=tempDuration+innerDelimiter;
						ampLst+="0"+innerDelimiter;
						}
					else if(pos3==firstNotePos)
						{// Sustained Note (Mark Note with 'S')
						tempNote=""; tempDuration=""; tempAmp="";
						extract_sustained_notes_from_measure(measure[i],inputValue,pos3,timePerMeasure,frequenciesInMeasure[i],\
													NOTE_SHARP,NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempNote,tempDuration,tempAmp);
						// Append Note and Duration to List
						noteLst+=tempNote+innerDelimiter;
						durationLst+=tempDuration+innerDelimiter;
						ampLst+=tempAmp+innerDelimiter;
						}
					else if(pos4==firstNotePos)
						{// Staccato Note (Mark Note with 'X')
						tempNote=""; tempDuration=""; tempAmp="";
						extract_staccato_notes_from_measure(measure[i],inputValue,pos4,timePerMeasure,frequenciesInMeasure[i],\
													NOTE_SHARP,NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempNote,tempDuration,tempAmp);
						// Append Note and Duration to List
						noteLst+=tempNote+innerDelimiter;
						durationLst+=tempDuration+innerDelimiter;
						ampLst+=tempAmp+innerDelimiter;
						}
					}
				if(tempNote.length()>0)
					{durationLst+=delimiter;
					noteLst+=delimiter;
					ampLst+=delimiter;}

				READING_MEASURE=true;
				notePos=firstNotePos+1;			
				while(READING_MEASURE)
					{// Get Position of Next Note(s), if Exist
					nextNotePos=Sz;
					for(int i=0;i<freqsInMeasure;i++)
						{if(notePos<measure[i].length())
							{// Search notes
							pos=measure[i].find("O",notePos);
							if(pos!=string::npos){if(pos<nextNotePos){nextNotePos=pos;}}
							pos2=measure[i].find("S",notePos);								// Sustained Notes
							if(pos2!=string::npos){if(pos2<nextNotePos){nextNotePos=pos2;}}
							pos3=measure[i].find("X",notePos);							// Staccato Notes
							if(pos3!=string::npos){if(pos3<nextNotePos){nextNotePos=pos3;}}
							// Search rests
							pos=measure[i].find("R",notePos);
							if(pos!=string::npos){if(pos<nextNotePos){nextNotePos=pos;}}
							}
						}
					//cout<<nextNotePos<<endl;
					if(nextNotePos==Sz){READING_MEASURE=false; break;}
					// Determine Frequencies for Note
					for(int i=0;i<freqsInMeasure;i++)
						{pos=measure[i].find("O",notePos);		// Regulare Note
						pos2=measure[i].find("R",notePos);		// Rest
						pos3=measure[i].find("S",notePos);		// Sustained Note
						pos4=measure[i].find("X",notePos);		// Staccato Note
						NOTE_SHARP=false; NOTE_FLAT=false; NOTE_DOUBLE_SHARP=false; NOTE_DOUBLE_FLAT=false;
						if(pos==nextNotePos && pos2!=nextNotePos && pos3!=nextNotePos && pos4!=nextNotePos)
							{tempNote=""; tempDuration=""; tempAmp="";
							extract_regular_notes_from_measure(measure[i],inputValue,pos,timePerMeasure,frequenciesInMeasure[i],NOTE_SHARP,\
														NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempNote,tempDuration,tempAmp);
							// Append Note and Duration to List
							noteLst+=tempNote+innerDelimiter;
							durationLst+=tempDuration+innerDelimiter;
							ampLst+=tempAmp+innerDelimiter;
							// Show the Note
							if(VERBOSE)
								{f=getFrequencyOfNote(extract_letter_from_noteCode(tempNote),atoi(extract_octave_from_noteCode(tempNote).c_str()));
								if(!NOTE_FLAT && !NOTE_DOUBLE_FLAT && !NOTE_SHARP && !NOTE_DOUBLE_SHARP){cout<<"Note ("<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
								else if(NOTE_FLAT){cout<<"Note (b"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
								else if(NOTE_DOUBLE_FLAT){cout<<"Note (bb"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
								else if(NOTE_SHARP){cout<<"Note (#"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
								else if(NOTE_DOUBLE_SHARP){cout<<"Note (##"<<frequenciesInMeasure[i]<<"="<<f<<") Duration: "<<tempDuration<<" s\n";}
								}

							}
						else if(pos!=nextNotePos && pos2==nextNotePos && pos3!=nextNotePos && pos4!=nextNotePos)
							{// Rest
							tempNote=""; tempDuration="";
							extract_rests_from_measure(measure[i],inputValue,pos2,timePerMeasure,tempNote,tempDuration);
							// Append Note and Duration to List
							noteLst+=tempNote+innerDelimiter;
							durationLst+=tempDuration+innerDelimiter;
							ampLst+="0"+innerDelimiter;
							}
						else if(pos!=nextNotePos && pos2!=nextNotePos && pos3==nextNotePos && pos4!=nextNotePos)
							{// Sustained Note
							tempNote=""; tempDuration=""; tempAmp="";
							extract_sustained_notes_from_measure(measure[i],inputValue,pos3,timePerMeasure,frequenciesInMeasure[i],\
														NOTE_SHARP,NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempNote,tempDuration,tempAmp);
							// Append Note and Duration to List
							noteLst+=tempNote+innerDelimiter;
							durationLst+=tempDuration+innerDelimiter;
							ampLst+=tempAmp+innerDelimiter;
							}
						else if(pos!=nextNotePos && pos2!=nextNotePos && pos3!=nextNotePos && pos4==nextNotePos)
							{// Staccato Note
							tempNote=""; tempDuration=""; tempAmp="";
							extract_staccato_notes_from_measure(measure[i],inputValue,pos4,timePerMeasure,frequenciesInMeasure[i],\
														NOTE_SHARP,NOTE_DOUBLE_SHARP,NOTE_FLAT,NOTE_DOUBLE_FLAT,tempNote,tempDuration,tempAmp);
							// Append Note and Duration to List
							noteLst+=tempNote+innerDelimiter;
							durationLst+=tempDuration+innerDelimiter;
							ampLst+=tempAmp+innerDelimiter;
							}
						}
					if(tempNote.length()>0)
						{durationLst+=delimiter;
						noteLst+=delimiter;
						ampLst+=delimiter;}
					// Update to get next note
					notePos=nextNotePos+1;
					tempNote="";
					}
				measureCounter++;
				}
			} /* end of comment filter and line length filter */
		fIn.getline(Val,Sz);
		}
	fIn.close();
	
	// Remove Last Delimiter from Note and Duration Lists
	//noteLst=noteLst.substr(0,noteLst.length()-1);

	numNotes=count_delimiter(noteLst,delimiter);
	//cout<<numNotes<<"\n"<<noteLst<<"\n"<<durationLst<<endl; //exit(1);
	if(count_delimiter(durationLst,delimiter)!=numNotes){cerr<<"Error in collect_measures_from_input_file!\nNotes and Durations do not match!\n"; exit(EXIT_FAILURE);}	
	// Duration of Each Note
	theDurations=durationLst;
	// Notes
	theNotes=noteLst;
	// Note Amplitudes
	theAmplitudes=ampLst;
	}

void collect_measures_from_misc_percussion_input_file(string inFile,double timePerMeasure,string* &theNotes,string* &theDurations,string* &theAmplitudes,int &numUsedDrums)
	{int N, numDrumTypes=41;
	string* theDrumTypes=new string[numDrumTypes];
	double fs=SAMPLE_RATE;
	// Drum Types
	theDrumTypes[0]=MISCELLANEOUS_PERCUSSION_01;					// "ROLL_HI_TRIANGLE";
	theDrumTypes[1]=MISCELLANEOUS_PERCUSSION_02;					// "MUTED_HI_TRIANGLE";
	theDrumTypes[2]=MISCELLANEOUS_PERCUSSION_03;					// "OPEN_HI_TRIANGLE";
	theDrumTypes[3]=MISCELLANEOUS_PERCUSSION_04;					// "GONG";
	theDrumTypes[4]=MISCELLANEOUS_PERCUSSION_05;					// "TAMTAM_A";
	theDrumTypes[5]=MISCELLANEOUS_PERCUSSION_06;					// "TAMTAM_B";
	theDrumTypes[6]=MISCELLANEOUS_PERCUSSION_07;					// "TAMTAM_C";
	theDrumTypes[7]=MISCELLANEOUS_PERCUSSION_08;					// "HI_FINGER_CYMBALS";
	theDrumTypes[8]=MISCELLANEOUS_PERCUSSION_09;					// "LO_FINGER_CYMBALS";
	theDrumTypes[9]=MISCELLANEOUS_PERCUSSION_10;					// "SHAKE_TAMBOURINE";
	theDrumTypes[10]=MISCELLANEOUS_PERCUSSION_11;				// "HIT_TAMBOURINE";
	theDrumTypes[11]=MISCELLANEOUS_PERCUSSION_12;				// "ROLL_TAMBOURINE";
	theDrumTypes[12]=MISCELLANEOUS_PERCUSSION_13;				// "COWBELL";
	theDrumTypes[13]=MISCELLANEOUS_PERCUSSION_14;				// "VIBRA_SLAP";
	theDrumTypes[14]=MISCELLANEOUS_PERCUSSION_15;				// "HIT_HIGH_CONGA";
	theDrumTypes[15]=MISCELLANEOUS_PERCUSSION_16;				// "TAP_HIGH_CONGA";
	theDrumTypes[16]=MISCELLANEOUS_PERCUSSION_17;				// "HIT_MID_CONGA";
	theDrumTypes[17]=MISCELLANEOUS_PERCUSSION_18;				// "TAP_MID_CONGA";
	theDrumTypes[18]=MISCELLANEOUS_PERCUSSION_19;				// "HIT_LOW_CONGA";
	theDrumTypes[19]=MISCELLANEOUS_PERCUSSION_20;				// "TAP_LOW_CONGA";
	theDrumTypes[20]=MISCELLANEOUS_PERCUSSION_21;				// "CABASA";
	theDrumTypes[21]=MISCELLANEOUS_PERCUSSION_22;				// "SHAKER";
	theDrumTypes[22]=MISCELLANEOUS_PERCUSSION_23;				// "CRANK_RATCHET";
	theDrumTypes[23]=MISCELLANEOUS_PERCUSSION_24;				// "FAST_RATCHET";
	theDrumTypes[24]=MISCELLANEOUS_PERCUSSION_25;				// "SLOW_RATCHET";
	theDrumTypes[25]=MISCELLANEOUS_PERCUSSION_26;				// "CASTANETS";
	theDrumTypes[26]=MISCELLANEOUS_PERCUSSION_27;				// "CLAVES";
	theDrumTypes[27]=MISCELLANEOUS_PERCUSSION_28;				// "HI_WOOD_BLOCK";
	theDrumTypes[28]=MISCELLANEOUS_PERCUSSION_29;				// "LO_WOOD_BLOCK";
	theDrumTypes[29]=MISCELLANEOUS_PERCUSSION_30;				// "ROLL_LO_TRIANGLE";
	theDrumTypes[30]=MISCELLANEOUS_PERCUSSION_31;				// "MUTED_LO_TRIANGLE";
	theDrumTypes[31]=MISCELLANEOUS_PERCUSSION_32;				// "OPEN_LO_TRIANGLE";
	theDrumTypes[32]=MISCELLANEOUS_PERCUSSION_33;				// "SLEIGH_BELLS";
	theDrumTypes[33]=MISCELLANEOUS_PERCUSSION_34;				// "BELL_TREE_1";
	theDrumTypes[34]=MISCELLANEOUS_PERCUSSION_35;				// "BELL_TREE_2";
	theDrumTypes[35]=MISCELLANEOUS_PERCUSSION_36;				// "BELL_TREE_3";
	theDrumTypes[36]=MISCELLANEOUS_PERCUSSION_37;				// "BELL_TREE_4";
	theDrumTypes[37]=MISCELLANEOUS_PERCUSSION_38;				// "BELL_TREE_5";
	theDrumTypes[38]=MISCELLANEOUS_PERCUSSION_39;				// "BAR_CHIMES_A";
	theDrumTypes[39]=MISCELLANEOUS_PERCUSSION_40;				// "BAR_CHIMES_B";
	theDrumTypes[40]=MISCELLANEOUS_PERCUSSION_41;				// "BAR_CHIMES_C";

	collect_measures_from_percussion_file(inFile,timePerMeasure,theDrumTypes,numDrumTypes,theNotes,theDurations,theAmplitudes,numUsedDrums);

	}

void collect_measures_from_percussion_file(string inFile,double timePerMeasure,string* theDrumTypes,int numDrumTypes,string* &theNotes,string* &theDurations,string* &theAmplitudes,int &numUsedDrums)
	{int N; 
	double fs=SAMPLE_RATE;
	ifstream fIn;
	fIn.open(inFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in collect_measures_from_drums_input_file!\nInput music file could not be opened.\n"<<inFile<<endl; exit(EXIT_FAILURE);}
	int Sz=LINE_SIZE, pos, pos2, freqIndex, freqsInMeasure, drumIndex, drumsInMeasure;
	char Val[Sz];
	string lst, tmp,tmp2,tmp3,inputValue,topFreq, topDrum, bottomFreq, nextFreq, nextDrum, freqLst="",drumLst="",delimiter=GLOBAL_DELIMITER;
	string* measure;
	// Read Header, Should Contain at Minimum: Sound Type & Tempo
	bool DRUMS=false, TEMPO_FOUND=false, SOUND_TYPE_FOUND=false, COLLECTING_MEASURE, SEARCHING, READING_MEASURE, NOTE_SHARP, NOTE_FLAT, NOTE_DOUBLE_SHARP, NOTE_DOUBLE_FLAT;
	// Time per measure as been specified so this signal can already be defined discretely with respect to time knowing the sampling frequency, which is 44,100 Hz
	int firstNotePos, nextNotePos, notePos, measureSize=timePerMeasure*fs, *usedDrumIndex, aPos;		// Array size of Measure's Signal
	string* frequenciesInMeasure, *theDrumsInMeasure, letter, octave;
	double* timeOfMeasure=new double[measureSize];
	double noteDurationTime, fraction, val1, val2;
	string *noteLst, *durationLst, *ampLst, drumTypeValue;
	bool FIRST_MEASURE=true;
	// Identify and Read In Measures	
	fIn.getline(Val,Sz);
	while(!fIn.eof())
		{tmp=Val;		
		// Exclude comments
		pos=tmp.find("#",0);
		if(pos!=0 && pos!=1 && tmp.length()>1 )
			{// Find Top Drum Type of Measure (will be labeled with appropriately)
			drumLst=""; topDrum=""; drumsInMeasure=0;
			for(int i=0;i<numDrumTypes;i++)
				{pos=tmp.find(theDrumTypes[i],0);
				if(pos!=string::npos){topDrum=theDrumTypes[i]; drumLst+=theDrumTypes[i]+delimiter; drumIndex=i; break;}
				}
			if(topDrum.compare("")!=0)
				{// Top of Measure Found, so Record it
				inputValue="";
				inputValue+=tmp+"\n"; drumsInMeasure++; drumIndex++;
				COLLECTING_MEASURE=true;
				fIn.getline(Val,Sz);
				while(COLLECTING_MEASURE)
					{tmp=Val;
					nextDrum=theDrumTypes[drumIndex];
					pos=tmp.find(nextDrum,0);
					if(pos==string::npos)
						{// End of Measure
						COLLECTING_MEASURE=false;
						break;}
					else
						{inputValue+=tmp+"\n"; drumLst+=theDrumTypes[drumIndex]+delimiter; drumsInMeasure++; drumIndex++;
						if(drumIndex>numDrumTypes-1){COLLECTING_MEASURE=false; break;}						
						}
					fIn.getline(Val,Sz);}
				}		//cout<<topDrum<<" | "<<drumsInMeasure<<"\n"<<inputValue<<endl;
			// Define Drums Input by User In Measure
			theDrumsInMeasure=fill_string_array(drumLst,drumsInMeasure,delimiter);	//for(int i=0;i<drumsInMeasure;i++){cout<<theDrumsInMeasure[i]<<endl;}
			// Define Measure as Input by User (A Measure is an entire row of the input sound file)
			measure=fill_string_array(inputValue,drumsInMeasure,"\n");

			if(FIRST_MEASURE)
				{// Determine Which Drums are Used
				lst="";
				for(int i=0;i<drumsInMeasure;i++)
					{// Search for Notes
					pos=measure[i].find("O",20);
					if(pos!=string::npos)
						{// Note found, so record drum as used
						lst+=cnvrtNumToStrng(i,0)+delimiter;
						}
					else
						{// Search for Rests
						pos=measure[i].find("R",20);
						if(pos!=string::npos)
							{// Rest found, so record drum as used
							lst+=cnvrtNumToStrng(i,0)+delimiter;
							}
						}
					}
				// Fill array
				numUsedDrums=count_delimiter(lst,delimiter);
				usedDrumIndex=fill_int_array(lst,numUsedDrums,delimiter);	//for(int i=0;i<numUsedDrums;i++){cout<<theDrumsInMeasure[usedDrumIndex[i]]<<endl;}

				// Declare Appropriate Sized Array for Collection
				noteLst=new string[numUsedDrums];
				durationLst=new string[numUsedDrums];
				ampLst=new string[numUsedDrums];
				for(int i=0;i<numUsedDrums;i++){durationLst[i]=""; noteLst[i]=""; ampLst[i]="";}
				FIRST_MEASURE=false;}			
			
			for(int i=0;i<numUsedDrums;i++)
				{// Get Position  of First Note(s) or Rest(s), used as reference for other notes positions
				firstNotePos=Sz+21;
				// Search for Notes
				pos=measure[usedDrumIndex[i]].find("O",20);
				if(pos!=string::npos)
					{if(pos<firstNotePos){firstNotePos=pos;}}
				// Search for Rests
				pos=measure[usedDrumIndex[i]].find("R",20);
				if(pos!=string::npos)
					{if(pos<firstNotePos){firstNotePos=pos;}}
				// Begin Recording
				pos=measure[usedDrumIndex[i]].find("O",20); pos2=measure[usedDrumIndex[i]].find("R",20);
				if(pos==firstNotePos)
					{// Note
					SEARCHING=true; lst="";
					while(SEARCHING)
						{if(pos+1<measure[usedDrumIndex[i]].length()){inputValue=measure[usedDrumIndex[i]].substr(pos+1,1);}
						else{SEARCHING=false; break;}
						if(inputValue.compare("O")==0 || inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
							{SEARCHING=false; break;}
						else
							{lst+=inputValue; pos++;}
						}
					noteDurationTime=define_note_duration_from_fraction(lst,timePerMeasure);
					// Get Drum Type 
					drumTypeValue=theDrumsInMeasure[usedDrumIndex[i]];
					// Generate Drum Type and Note Duration List
					noteLst[i]+=drumTypeValue+delimiter;
					durationLst[i]+=cnvrtNumToStrng(noteDurationTime,SIG_FIGS)+delimiter;
					// Amplitude
					ampLst[i]+=define_amplitude_from_note(lst)+delimiter;
					}
				else if(pos2==firstNotePos)
					{// Rest Get Note Duration
					SEARCHING=true; lst="";
					while(SEARCHING)
						{if(pos2+1<measure[usedDrumIndex[i]].length()){inputValue=measure[usedDrumIndex[i]].substr(pos2+1,1);}
						else{SEARCHING=false; break;}
						if(inputValue.compare("O")==0 || inputValue.compare("R")==0 || inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
							{SEARCHING=false; break;}
						else
							{lst+=inputValue; pos2++;}
						}
					noteDurationTime=define_note_duration_from_fraction(lst,timePerMeasure);
					// Generate Silence
					noteLst[i]+="R"+delimiter;
					durationLst[i]+=cnvrtNumToStrng(noteDurationTime,SIG_FIGS)+delimiter;
					ampLst[i]+="0"+delimiter;
					}

				// Read Another Note
				READING_MEASURE=true;
				notePos=firstNotePos+1;			
				while(READING_MEASURE)
					{// Get Position of Next Note(s), if Exist
					nextNotePos=Sz;
					if(notePos<measure[usedDrumIndex[i]].length())
						{// Search notes
						pos=measure[usedDrumIndex[i]].find("O",notePos);
						if(pos!=string::npos)
							{if(pos<nextNotePos){nextNotePos=pos;}}
						// Search rests
						pos=measure[usedDrumIndex[i]].find("R",notePos);
						if(pos!=string::npos)
							{if(pos<nextNotePos){nextNotePos=pos;}}
						}

					if(nextNotePos==Sz){READING_MEASURE=false; break;}
					// Determine Drum Types
					pos=measure[usedDrumIndex[i]].find("O",notePos);		// finds notes
					pos2=measure[usedDrumIndex[i]].find("R",notePos);		// finds rests
					if(pos==nextNotePos && pos2!=nextNotePos)
						{// Get Note Duration
						SEARCHING=true; lst="";
						while(SEARCHING)
							{if(pos+1<measure[usedDrumIndex[i]].length()){inputValue=measure[usedDrumIndex[i]].substr(pos+1,1);}
							else{SEARCHING=false; break;}
							if(inputValue.compare("O")==0 || inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
								{SEARCHING=false; break;}
							else
								{lst+=inputValue; pos++;}
							}
						noteDurationTime=define_note_duration_from_fraction(lst,timePerMeasure);
						// Get Drum Type
						drumTypeValue=theDrumsInMeasure[usedDrumIndex[i]];
						// Store sound and duration
						noteLst[i]+=drumTypeValue+delimiter;
						durationLst[i]+=cnvrtNumToStrng(noteDurationTime,SIG_FIGS)+delimiter;
						ampLst[i]+=define_amplitude_from_note(lst)+delimiter;
						}
					else if(pos!=nextNotePos && pos2==nextNotePos)
						{// Rest Get Note Duration
						SEARCHING=true; lst="";
						while(SEARCHING)
							{if(pos2+1<measure[usedDrumIndex[i]].length()){inputValue=measure[usedDrumIndex[i]].substr(pos2+1,1);}
							else{SEARCHING=false; break;}
							if(inputValue.compare("O")==0 || inputValue.compare("R")==0 || inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
								{SEARCHING=false; break;}
							else
								{lst+=inputValue; pos2++;}
							}
						noteDurationTime=define_note_duration_from_fraction(lst,timePerMeasure);
						// Generate Silence
						noteLst[i]+="R"+delimiter;
						durationLst[i]+=cnvrtNumToStrng(noteDurationTime,SIG_FIGS)+delimiter;
						ampLst[i]+="0"+delimiter;
						}
					// Update to get next note
					notePos=nextNotePos+1; 
					} /* While Loop */
				//cout<<"Notes|"<<noteLst[i]<<"|\n";
				//cout<<"Times|"<<durationLst[i]<<"|\n";
				}	/* Measure Recording For Loop */
			} /* end of comment filter and line length filter */
		fIn.getline(Val,Sz);
		}
	fIn.close();

	theDurations=new string[numUsedDrums];
	theNotes=new string[numUsedDrums];
	theAmplitudes=new string[numUsedDrums];
	for(int i=0;i<numUsedDrums;i++)
		{// Duration of Each Note
		theDurations[i]=durationLst[i];
		// Notes
		theNotes[i]=noteLst[i];
		// Note Amplitudes
		theAmplitudes[i]=ampLst[i];
		}
	}

void collect_measures_from_simple_drums_input_file(string inFile,double timePerMeasure,string* &theNotes,string* &theDurations,string* &theAmplitudes,int &numUsedDrums)
	{int numDrumTypes=20;
	string* theDrumTypes=new string[numDrumTypes];
	double fs=SAMPLE_RATE;
	// Drum Types
	theDrumTypes[0]=SIMPLE_DRUM_01;			// "BASS_DRUM_1";
	theDrumTypes[1]=SIMPLE_DRUM_02;			// "BASS_DRUM_2";
	theDrumTypes[2]=SIMPLE_DRUM_03;			// "LEFT_HIT_SNARE_1";
	theDrumTypes[3]=SIMPLE_DRUM_04;			// "ROUND_HIT_SNARE_1";
	theDrumTypes[4]=SIMPLE_DRUM_05;			// "RIGHT_HIT_SNARE_1";
	theDrumTypes[5]=SIMPLE_DRUM_06;			// "ROLL_SNARE_1";
	theDrumTypes[6]=SIMPLE_DRUM_07;			// "LEFT_HIT_SNARE_2";
	theDrumTypes[7]=SIMPLE_DRUM_08;			// "ROUND_HIT_SNARE_2";
	theDrumTypes[8]=SIMPLE_DRUM_09;			// "RIGHT_HIT_SNARE_2";
	theDrumTypes[9]=SIMPLE_DRUM_10;			// "TAP_SNARE_2";
	theDrumTypes[10]=SIMPLE_DRUM_11;			// "ROLL_SNARE_2";
	theDrumTypes[11]=SIMPLE_DRUM_12;			// "ROLL_A_CYMBALS_1";
	theDrumTypes[12]=SIMPLE_DRUM_13;			// "LO_CRASH_CYMBALS_1";
	theDrumTypes[13]=SIMPLE_DRUM_14;			// "ROLL_B_CYMBALS_1";
	theDrumTypes[14]=SIMPLE_DRUM_15;			// "HI_CRASH_CYMBALS_1";
	theDrumTypes[15]=SIMPLE_DRUM_16;			// "SHORT_ROLL_CYMBALS_2";
	theDrumTypes[16]=SIMPLE_DRUM_17;			// "LO_CRASH_CYMBALS_2";
	theDrumTypes[17]=SIMPLE_DRUM_18;			// "MID_ROLL_CYMBALS_2";
	theDrumTypes[18]=SIMPLE_DRUM_19;			// "LONG_ROLL_CYMBALS_2";
	theDrumTypes[19]=SIMPLE_DRUM_20;			// "HI_CRASH_CYMBALS_2";

	collect_measures_from_percussion_file(inFile,timePerMeasure,theDrumTypes,numDrumTypes,theNotes,theDurations,theAmplitudes,numUsedDrums);

	}

bool copyFile(string srcFile,string destFile)
	{ifstream theSource(srcFile.c_str(),ifstream::in|ios::binary);
	ofstream theDestination(destFile.c_str(),ofstream::out|ofstream::trunc|ios::binary);
	theDestination<<theSource.rdbuf();
	return theSource && theDestination;}

int count_delimiter(string Data,string delimiter)
	{int Counter=0,Sz=delimiter.length();
	string tmp="";
	for(int i=0;i<Data.length()-Sz;i++)
		{tmp="";
		for(int j=0;j<Sz;j++){tmp+=Data[i+j];}
		if(tmp.compare(delimiter)==0)
			{Counter++;}}
	tmp=Data[Data.length()-1];
	if(tmp.compare(delimiter)==0){Counter++;}
	return Counter;}

string define_amplitude_from_note(string inputFrac)
	{int aPos=inputFrac.find("A",0);
	string tmp;
	if(aPos==string::npos){return "-1";}
	else
		{tmp=inputFrac.substr(aPos+1,inputFrac.length()-aPos-1);
		//cout<<tmp<<"|"<<formatNumberString(tmp)<<"\n";
		return formatNumberString(tmp);}
	}

int* determine_peak_indices(double* X,double* freq,int Sz,int numNeighbors,double cutOffFrac,int &numPeaks)
	{string indexLst="",tmpLst="",delimiter=GLOBAL_DELIMITER;
	int theIndex;
	numPeaks=0;
	double maxValue=abs_maximum_of_array(X,Sz,theIndex),xVal;
	double cutOffValue=cutOffFrac*log10(maxValue);
	double *tmpArr;
	for(int i=0;i<Sz;i++)
		{tmpLst="";
		for(int j=-numNeighbors;j<numNeighbors;j++)
			{theIndex=i+j;
			if(theIndex<0)
				{xVal=0;}
			else if(theIndex>=Sz)
				{xVal=0;}
			else
				{xVal=X[theIndex];}
			tmpLst+=formatNumberString(cnvrtNumToStrng(xVal,SIG_FIGS))+delimiter;
			}
		tmpArr=fill_double_array(tmpLst,2*numNeighbors+1,delimiter);
		maxValue=abs_maximum_of_array(tmpArr,2*numNeighbors+1,theIndex);
		if(theIndex==numNeighbors)
			{// Check CutOff
			maxValue=log10(maxValue);
			if(maxValue>cutOffValue)
				{// Maximum Found
				indexLst+=cnvrtNumToStrng(i+theIndex,0)+delimiter;
				numPeaks++;}
			}
		delete [] tmpArr;
		}
	return fill_int_array(indexLst,numPeaks,delimiter);}

double* generate_normalized_Gaussian_distribution(double avg,double std_dev,double* x,int Sz)
	{double PI=2.0*acos(0.0);
	double* Output=new double[Sz];
	double A=1.0/(std_dev*sqrt(2.0*PI)),B;
	for(int i=0;i<Sz;i++)
		{B=(x[i]-avg)/std_dev;
		Output[i]=A*exp(-B*B/2.0);}
	// Normalize
	int theIndex;
	double maxValue=abs_maximum_of_array(Output,Sz,theIndex);
	for(int j=0;j<Sz;j++){Output[j]/=maxValue;}
	return Output;}

double determine_average_maximum_frequency_peak(double* theInput,int N,int &index)
	{// Define Frequencies
	double fs=SAMPLE_RATE;
	double *freq=new double[N];
	double incrmnt=(fs-0)/(double)N;//incrmnt=(fs-0)/(N-1);
	// Generate Frequencies
	for(int i=0;i<N;i++){freq[i]=i*incrmnt;}
	// FFT(signal)
	double** X=fft(theInput,N);
	double *Mag=new double[N];
	double *tmpMag=new double[N];
	//for(int i=0;i<N;i++){Mag[i]=sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1]);}
	for(int i=0;i<N;i++){Mag[i]=abs(X[i][0]*X[i][0] + X[i][1]*X[i][1]); tmpMag[i]=Mag[i];}
	//for(int i=0;i<N;i++){Mag[i]=sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1])*sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1]);}
	// Determine Frequency of Peak
	int numPeaks=1,theIndex;
	int numNeighboringElements=100;
	double maxValue=abs_maximum_of_array(Mag,N/2,theIndex);
	double cutOffFrac=2.0/3.0;
	string lst="",delimiter=GLOBAL_DELIMITER;
	//cout<<maxValue<<"("<<Mag[theIndex]<<")@"<<freq[theIndex]<<endl;
	// update tmpMag
	tmpMag[theIndex]=0;
	// Update Array Index List
	lst+=cnvrtNumToStrng(theIndex,0)+delimiter;
	double cutOffValue=cutOffFrac*log10(maxValue);
	int *tmpIntArr;
	bool SEARCHING=true,CHECK;
	while(SEARCHING)
		{maxValue=abs_maximum_of_array(tmpMag,N/2,theIndex);
		if(maxValue>=cutOffValue)
			{// Check for Neighbors
			CHECK=true;
			tmpIntArr=fill_int_array(lst,numPeaks,delimiter);
			for(int i=0;i<numPeaks;i++){if(theIndex<tmpIntArr[i]+numNeighboringElements && theIndex>tmpIntArr[i]-numNeighboringElements){CHECK=false; break;}}
			delete [] tmpIntArr;
			if(CHECK)
				{// Update Array Index List
				lst+=cnvrtNumToStrng(theIndex,0)+delimiter;
				numPeaks++;}
			// Update tmpMag
			tmpMag[theIndex]=0;
			}
		else
			{SEARCHING=false; break;}
		}
	int *indices=fill_int_array(lst,numPeaks,delimiter);
	//int* indices=determine_peak_indices(Mag,freq,N/2,numNeighboringElements,2.0/3.0,numPeaks);
	//cout<<"Maxima at:\n";
	lst="";
	string lst2="";
	for(int i=0;i<numPeaks;i++)
		{//cout<<Mag[indices[i]]<<"@"<<freq[indices[i]]<<"+/-"<<incrmnt<<" Hz ("<<indices[i]<<")\n";
		// Freq List
		lst+=formatNumberString(cnvrtNumToStrng(freq[indices[i]],SIG_FIGS))+delimiter;
		// Array Index List
		lst2+=cnvrtNumToStrng(indices[i],0)+delimiter;
		if(i==0){maxValue=Mag[indices[i]]; theIndex=i;}
		if(Mag[indices[i]]>maxValue){maxValue=Mag[indices[i]]; theIndex=i;}
		}
	if(numPeaks==0){cerr<<"Error!\nNo peaks found.\n"; exit(EXIT_FAILURE);}
	//cout<<"------------------\n";
	// Convert Discrete FFT into Continuous function assuming peaks are Gaussian Distributions
	double* freqValues=fill_double_array(lst,numPeaks,delimiter);
	tmpIntArr=fill_int_array(lst2,numPeaks,delimiter);
	bool SORTING=true,SORTED;
	int tmpIntValue;
	while(SORTING)
		{SORTED=true;
		for(int i=1;i<numPeaks;i++)
			{if(tmpIntArr[i-1]>tmpIntArr[i])
				{tmpIntValue=tmpIntArr[i];
				tmpIntArr[i]=tmpIntArr[i-1];
				tmpIntArr[i-1]=tmpIntValue;
				SORTED=false;}
			}
		if(SORTED)
			{SORTING=false; break;}
		}
	//for(int i=0;i<numPeaks;i++){cout<<Mag[tmpIntArr[i]]<<"@"<<freq[tmpIntArr[i]]<<"+/-"<<incrmnt<<" Hz ("<<tmpIntArr[i]<<")\n";}
	// Determine Local Maxima
	double df,lastDf=1;
	lst="";
	int numLocalMaxs=0;
	for(int i=1;i<numPeaks;i++)
		{df=Mag[tmpIntArr[i]]-Mag[tmpIntArr[i-1]];
		if(lastDf>0 && df<0)
			{// Maximum Found
			lst+=cnvrtNumToStrng(tmpIntArr[i-1],0)+delimiter;
			numLocalMaxs++;}
		lastDf=df;}
	int *formantIndices=fill_int_array(lst,numLocalMaxs,delimiter);
	//cout<<"Formant(s)\n";
	//for(int i=0;i<numLocalMaxs;i++){cout<<Mag[formantIndices[i]]<<"@ "<<freq[formantIndices[i]]<<" +/- "<<incrmnt<<" Hz ("<<formantIndices[i]<<")\n";}
	//
	index=formantIndices[0];
	double Output=0,Sum=0;
	for(int i=0;i<numLocalMaxs;i++)
		{//if(Mag[formantIndices[i]]>=cutOffValue)
		//	{index=formantIndices[i];
		//	Output+=freq[formantIndices[i]];
		//	Counter++;
		//	}
		Output+=Mag[formantIndices[i]]*freq[formantIndices[i]];
		Sum+=Mag[formantIndices[i]];
		}
	Output/=numLocalMaxs;
	Output/=Sum;
	return Output;}

double determine_last_maximum_frequency_peak(double* theInput,int N,int &index)
	{// Define Frequencies
	double fs=SAMPLE_RATE;
	double *freq=new double[N];
	double incrmnt=(fs-0)/(double)N;//incrmnt=(fs-0)/(N-1);
	// Generate Frequencies
	for(int i=0;i<N;i++){freq[i]=i*incrmnt;}
	// FFT(signal)
	double** X=fft(theInput,N);
	double *Mag=new double[N];
	double *tmpMag=new double[N];
	//for(int i=0;i<N;i++){Mag[i]=sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1]);}
	for(int i=0;i<N;i++){Mag[i]=abs(X[i][0]*X[i][0] + X[i][1]*X[i][1]); tmpMag[i]=Mag[i];}
	//for(int i=0;i<N;i++){Mag[i]=sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1])*sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1]);}
	// Determine Frequency of Peak
	int numPeaks=1,theIndex;
	int numNeighboringElements=100;
	double maxValue=abs_maximum_of_array(Mag,N/2,theIndex);
	double cutOffFrac=2.0/3.0;
	string lst="",delimiter=GLOBAL_DELIMITER;
	//cout<<maxValue<<"("<<Mag[theIndex]<<")@"<<freq[theIndex]<<endl;
	// update tmpMag
	tmpMag[theIndex]=0;
	// Update Array Index List
	lst+=cnvrtNumToStrng(theIndex,0)+delimiter;
	double cutOffValue=cutOffFrac*log10(maxValue);
	int *tmpIntArr;
	bool SEARCHING=true,CHECK;
	while(SEARCHING)
		{maxValue=abs_maximum_of_array(tmpMag,N/2,theIndex);
		if(maxValue>=cutOffValue)
			{// Check for Neighbors
			CHECK=true;
			tmpIntArr=fill_int_array(lst,numPeaks,delimiter);
			for(int i=0;i<numPeaks;i++){if(theIndex<tmpIntArr[i]+numNeighboringElements && theIndex>tmpIntArr[i]-numNeighboringElements){CHECK=false; break;}}
			delete [] tmpIntArr;
			if(CHECK)
				{// Update Array Index List
				lst+=cnvrtNumToStrng(theIndex,0)+delimiter;
				numPeaks++;}
			// Update tmpMag
			tmpMag[theIndex]=0;
			}
		else
			{SEARCHING=false; break;}
		}
	int *indices=fill_int_array(lst,numPeaks,delimiter);
	//int* indices=determine_peak_indices(Mag,freq,N/2,numNeighboringElements,2.0/3.0,numPeaks);
	//cout<<"Maxima at:\n";
	lst="";
	string lst2="";
	for(int i=0;i<numPeaks;i++)
		{//cout<<Mag[indices[i]]<<"@"<<freq[indices[i]]<<"+/-"<<incrmnt<<" Hz ("<<indices[i]<<")\n";
		// Freq List
		lst+=formatNumberString(cnvrtNumToStrng(freq[indices[i]],SIG_FIGS))+delimiter;
		// Array Index List
		lst2+=cnvrtNumToStrng(indices[i],0)+delimiter;
		if(i==0){maxValue=Mag[indices[i]]; theIndex=i;}
		if(Mag[indices[i]]>maxValue){maxValue=Mag[indices[i]]; theIndex=i;}
		}
	if(numPeaks==0){cerr<<"Error!\nNo peaks found.\n"; exit(EXIT_FAILURE);}
	//cout<<"------------------\n";
	// Convert Discrete FFT into Continuous function assuming peaks are Gaussian Distributions
	double* freqValues=fill_double_array(lst,numPeaks,delimiter);
	tmpIntArr=fill_int_array(lst2,numPeaks,delimiter);
	bool SORTING=true,SORTED;
	int tmpIntValue;
	while(SORTING)
		{SORTED=true;
		for(int i=1;i<numPeaks;i++)
			{if(tmpIntArr[i-1]>tmpIntArr[i])
				{tmpIntValue=tmpIntArr[i];
				tmpIntArr[i]=tmpIntArr[i-1];
				tmpIntArr[i-1]=tmpIntValue;
				SORTED=false;}
			}
		if(SORTED)
			{SORTING=false; break;}
		}
	//for(int i=0;i<numPeaks;i++){cout<<Mag[tmpIntArr[i]]<<"@"<<freq[tmpIntArr[i]]<<"+/-"<<incrmnt<<" Hz ("<<tmpIntArr[i]<<")\n";}
	// Determine Local Maxima
	double df,lastDf=1;
	lst="";
	int numLocalMaxs=0;
	for(int i=1;i<numPeaks;i++)
		{df=Mag[tmpIntArr[i]]-Mag[tmpIntArr[i-1]];
		if(lastDf>0 && df<0)
			{// Maximum Found
			lst+=cnvrtNumToStrng(tmpIntArr[i-1],0)+delimiter;
			numLocalMaxs++;}
		lastDf=df;}
	int *formantIndices=fill_int_array(lst,numLocalMaxs,delimiter);
	cout<<"Formant(s)\n";
	for(int i=0;i<numLocalMaxs;i++){cout<<Mag[formantIndices[i]]<<"@ "<<freq[formantIndices[i]]<<" +/- "<<incrmnt<<" Hz ("<<formantIndices[i]<<")\n";}
	//
	index=formantIndices[0];
	double Output=freq[formantIndices[0]];
	for(int i=1;i<numLocalMaxs;i++)
		{if(Output>freq[formantIndices[i]] && Mag[formantIndices[i]]>=cutOffValue)
			{index=formantIndices[i];
			Output=freq[formantIndices[i]];
			}
		}
	return Output;}

double determine_maximum_frequency_peak(double* theInput,int N,int &index)
	{// Define Frequencies
	double fs=SAMPLE_RATE;
	double *freq=new double[N];
	double incrmnt=(fs-0)/(double)N;//incrmnt=(fs-0)/(N-1);
	// Generate Frequencies
	for(int i=0;i<N;i++){freq[i]=i*incrmnt;}
	// FFT(signal)
	double** X=fft(theInput,N);
	double *Mag=new double[N];
	double *tmpMag=new double[N];
	//for(int i=0;i<N;i++){Mag[i]=sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1]);}
	for(int i=0;i<N;i++){Mag[i]=abs(X[i][0]*X[i][0] + X[i][1]*X[i][1]); tmpMag[i]=Mag[i];}
	//for(int i=0;i<N;i++){Mag[i]=sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1])*sqrt(X[i][0]*X[i][0] + X[i][1]*X[i][1]);}
	// Determine Frequency of Peak
	int numPeaks=1,theIndex;
	int numNeighboringElements=100;
	double maxValue=abs_maximum_of_array(Mag,N/2,theIndex);
	double cutOffFrac=2.0/3.0;
	string lst="",delimiter=GLOBAL_DELIMITER;
	//cout<<maxValue<<"("<<Mag[theIndex]<<")@"<<freq[theIndex]<<endl;
	// update tmpMag
	tmpMag[theIndex]=0;
	// Update Array Index List
	lst+=cnvrtNumToStrng(theIndex,0)+delimiter;
	double cutOffValue=cutOffFrac*log10(maxValue);
	int *tmpIntArr;
	bool SEARCHING=true,CHECK;
	while(SEARCHING)
		{maxValue=abs_maximum_of_array(tmpMag,N/2,theIndex);
		if(maxValue>=cutOffValue)
			{// Check for Neighbors
			CHECK=true;
			tmpIntArr=fill_int_array(lst,numPeaks,delimiter);
			for(int i=0;i<numPeaks;i++){if(theIndex<tmpIntArr[i]+numNeighboringElements && theIndex>tmpIntArr[i]-numNeighboringElements){CHECK=false; break;}}
			delete [] tmpIntArr;
			if(CHECK)
				{// Update Array Index List
				lst+=cnvrtNumToStrng(theIndex,0)+delimiter;
				numPeaks++;}
			// Update tmpMag
			tmpMag[theIndex]=0;
			}
		else
			{SEARCHING=false; break;}
		}
	int *indices=fill_int_array(lst,numPeaks,delimiter);
	//int* indices=determine_peak_indices(Mag,freq,N/2,numNeighboringElements,2.0/3.0,numPeaks);
	//cout<<"Maxima at:\n";
	lst="";
	string lst2="";
	for(int i=0;i<numPeaks;i++)
		{//cout<<Mag[indices[i]]<<"@"<<freq[indices[i]]<<"+/-"<<incrmnt<<" Hz ("<<indices[i]<<")\n";
		// Freq List
		lst+=formatNumberString(cnvrtNumToStrng(freq[indices[i]],SIG_FIGS))+delimiter;
		// Array Index List
		lst2+=cnvrtNumToStrng(indices[i],0)+delimiter;
		if(i==0){maxValue=Mag[indices[i]]; theIndex=i;}
		if(Mag[indices[i]]>maxValue){maxValue=Mag[indices[i]]; theIndex=i;}
		}
	if(numPeaks==0){cerr<<"Error!\nNo peaks found.\n"; exit(EXIT_FAILURE);}
	//cout<<"------------------\n";
	// Convert Discrete FFT into Continuous function assuming peaks are Gaussian Distributions
	double* freqValues=fill_double_array(lst,numPeaks,delimiter);
	tmpIntArr=fill_int_array(lst2,numPeaks,delimiter);
	bool SORTING=true,SORTED;
	int tmpIntValue;
	while(SORTING)
		{SORTED=true;
		for(int i=1;i<numPeaks;i++)
			{if(tmpIntArr[i-1]>tmpIntArr[i])
				{tmpIntValue=tmpIntArr[i];
				tmpIntArr[i]=tmpIntArr[i-1];
				tmpIntArr[i-1]=tmpIntValue;
				SORTED=false;}
			}
		if(SORTED)
			{SORTING=false; break;}
		}
	//for(int i=0;i<numPeaks;i++){cout<<Mag[tmpIntArr[i]]<<"@"<<freq[tmpIntArr[i]]<<"+/-"<<incrmnt<<" Hz ("<<tmpIntArr[i]<<")\n";}
	// Determine Local Maxima
	double df,lastDf=1;
	lst="";
	int numLocalMaxs=0;
	for(int i=1;i<numPeaks;i++)
		{df=Mag[tmpIntArr[i]]-Mag[tmpIntArr[i-1]];
		if(lastDf>0 && df<0)
			{// Maximum Found
			lst+=cnvrtNumToStrng(tmpIntArr[i-1],0)+delimiter;
			numLocalMaxs++;}
		lastDf=df;}
	int *formantIndices=fill_int_array(lst,numLocalMaxs,delimiter);
	//cout<<"Formant(s)\n";
	//for(int i=0;i<numLocalMaxs;i++){cout<<Mag[formantIndices[i]]<<"@ "<<freq[formantIndices[i]]<<" +/- "<<incrmnt<<" Hz ("<<formantIndices[i]<<")\n";}

	string title="set title \"FFT(Signal) Magnitude Spectrum\"\n";
	string xLabel="set xlabel \"Frequency (Hertz (1/seconds))\"\n";
	string yLabel="set ylabel \"Amplitude\"\n";
	string xRange="set xrange [0:"+cnvrtNumToStrng(freq[N/2],3)+"]\n";//xRange="set xrange [0:"+cnvrtNumToStrng(3*freqValues[numPeaks-1]/4,3)+"]\n";
	string yRange;//"set yrange [-2:2]";
	string plotOptions;//=title+xLabel+yLabel+xRange;//+yRange;
	string fftFile=get_current_dir_name(); fftFile+="/FFT(signal)";
/*	// Fit Gaussian Distribution to each peak to determine frequency with better resolution
	string magLst="",freqLst="",delimiter=GLOBAL_DELIMITER;
	double *magValues,*freqValues,*distribution;
	double avg,std_dev,variance,avgFreq;
	for(int i=0;i<numPeaks;i++)
		{magLst="",freqLst="";
		for(int j=indices[i]-numNeighboringElements;j<indices[i]+numNeighboringElements;j++)
			{magLst+=formatNumberString(cnvrtNumToStrng(Mag[j],SIG_FIGS))+delimiter;
			freqLst+=formatNumberString(cnvrtNumToStrng(freq[j],SIG_FIGS))+delimiter;}
		// Analyze Data List
		magValues=fill_double_array(magLst,2*numNeighboringElements+1,delimiter);		
		freqValues=fill_double_array(freqLst,2*numNeighboringElements+1,delimiter);
		avgFreq=calc_weighted_average(freqValues,magValues,2*numNeighboringElements+1);
		maxValue=abs_maximum_of_array(magValues,2*numNeighboringElements+1,theIndex);
		for(int j=0;j<2*numNeighboringElements+1;j++){magValues[j]/=maxValue;}
		avg=calc_average(magValues,2*numNeighboringElements+1);
		std_dev=calc_std_dev(magValues,2*numNeighboringElements+1,avg);
		variance=std_dev*std_dev;

		distribution=generate_Gaussian_distribution(avgFreq,std_dev,freqValues,2*numNeighboringElements+1);
		xRange="set xrange ["+formatNumberString(cnvrtNumToStrng(freqValues[0],SIG_FIGS))+":"+formatNumberString(cnvrtNumToStrng(freqValues[2*numNeighboringElements],SIG_FIGS))+"]\n";
		yRange="set yrange [0:1]\n";
		plotOptions=title+xLabel+yLabel+xRange;//+yRange;
		//plot_signal(plotOptions,fftFile,freqValues,distribution,2*numNeighboringElements+1);
		//plot_signal(plotOptions,fftFile,freqValues,magValues,2*numNeighboringElements+1);
		cout<<"|"<<i<<"|"<<avgFreq<<endl;
		for(int k=0;k<2*numNeighboringElements+1;k++){cout<<freqValues[k]<<"|"<<distribution[k]<<endl;}
		}
	
	//cout<<"Max Peak at "<<freq[index]<<" ("<<freq[previousIndex]<<" & "<<freq[previousIndex2]<<") Hz\n";
	plot_signal(plotOptions,fftFile,freq,Mag,N/2);
*/
	//plotOptions=title+xLabel+yLabel+xRange;
	//plot_signal(plotOptions,fftFile,freq,Mag,N/2);

	//return calc_average(freqValues,numPeaks);
	//double* freqWeights=new double[numPeaks];
	//double Sum=0;
	//for(int i=0;i<numPeaks;i++){Sum+=Mag[indices[i]];}
	//for(int i=0;i<numPeaks;i++){freqWeights[i]=Mag[indices[i]]/Sum;}
	//double std_dev=determine_standard_deviation_of_peak(freq,Mag,theIndex,3,N);
	//cout<<std_dev<<endl;
	//return calc_weighted_average(freqValues,freqWeights,numPeaks);
	return freqValues[theIndex];}

double define_note_duration_from_fraction(string inputFrac,double timePerMeasure)
	{// Check for Amplitude value
	int aPos=inputFrac.find("A",0);
	if(aPos!=string::npos){inputFrac=inputFrac.substr(0,aPos);}
	// Handle Fraction
	string tmp;
	aPos=inputFrac.find("/",0);
	double val1,val2,fraction,noteDuration;
	if(aPos!=string::npos)
		{// Extract values and divide
		tmp=inputFrac.substr(0,aPos);		// top
		val1=strtod(tmp.c_str(),NULL);
		tmp=inputFrac.substr(aPos+1,inputFrac.length()-aPos-1);		// bottom
		val2=strtod(tmp.c_str(),NULL);
		fraction=val1/val2;
		//cout<<val1<<"/"<<val2<<"="<<fraction<<endl;
		}
	else
		{// No fraction detected, proceed
		fraction=strtod(inputFrac.c_str(),NULL);}
	noteDuration=timePerMeasure/fraction;
	return noteDuration;}

double determine_standard_deviation_of_peak(double *inputX,double *inputY,int index,int numNeighbors,int Sz)
	{int N=2*numNeighbors+1;
	double avg=inputX[index];
	double *freq=new double[N];
	double *Mag=new double[N];
	int Counter=0,theIndex;
	for(int i=index-numNeighbors;i<index+numNeighbors;i++)
		{if(i>=0 && i<Sz)
			{freq[Counter]=inputX[i];
			Mag[Counter]=inputY[i];
			Counter++;}
		}
	double maxValue=abs_maximum_of_array(Mag,Counter,theIndex);
	for(int i=0;i<N;i++){Mag[i]/=maxValue;}
	// 1st Guess
	double std_dev1=1;
	double* y1=generate_normalized_Gaussian_distribution(avg,std_dev1,freq,Counter);
	double Err1=calc_chi_squared(Mag,y1,Counter);
	cout<<"Error1: "<<Err1<<endl;
	// 2nd Guess
	double std_dev2=2;
	double* y2=generate_normalized_Gaussian_distribution(avg,std_dev2,freq,Counter);
	double Err2=calc_chi_squared(Mag,y2,Counter);
	cout<<"Error2: "<<Err2<<endl;
	//
	double std_dev,*y,Err;
	bool RUNNING=true;
	while(RUNNING)
		{// Define New Coefficient Based on Error (C2=next to last,C1=last)
		std_dev=(std_dev2-std_dev1)*(1e-8-Err1)/(Err2-Err1)+std_dev1;
		cout<<"Next Guess:\tC = "<<std_dev<<"...";
		y=generate_normalized_Gaussian_distribution(avg,std_dev,freq,Counter);
		Err=calc_chi_squared(Mag,y,Counter);
		cout<<"Done!\tError %: "<<Err<<"|"<<ACCEPTED_ERROR_PERCENT<<endl;
		
		if(abs(Err)>ACCEPTED_ERROR_PERCENT)
			{// Update Coefficient Before Last
			std_dev1=std_dev2; Err1=Err2;
			// Update Last Coefficient
			std_dev2=std_dev;Err2=Err;			
			}
		else{RUNNING=false;break;}
		}
	return std_dev;}

double determine_total_song_time(string durationLst,int numNotes,string innerDelimiter)
	{string delimiter=GLOBAL_DELIMITER;
	// Note Durations (seconds)
	string *tmpArr2=fill_string_array(durationLst,numNotes,delimiter);
	int N;
	double* tmpArr3, smallest=0, totalSongTime=0;
	for(int i=0;i<numNotes;i++)
		{//cout<<i<<" | "<<tmpArr2[i]<<"\n";
		if(tmpArr2[i].length()>0)
			{N=count_delimiter(tmpArr2[i],innerDelimiter);
			tmpArr3=fill_double_array(tmpArr2[i],N,innerDelimiter);
			// Find Smallest Duration
			smallest=tmpArr3[0];
			for(int j=0;j<N;j++)
				{if(tmpArr3[j]<smallest){smallest=tmpArr3[j];}
				}
			// Total Song Time (seconds)
			totalSongTime+=smallest;
			}
		else
			{cerr<<"Error in determine_total_song_time()\nEnsure all scales are in proper order (as a C Major instrument)\nC1 D1 E1 F1 G1 A1 B1 C2 D2 E2 F2 G2 A2 B2 C3 ...\n|"<<tmpArr2[i]<<"|\n";}
		}
	return totalSongTime;}

bool directory_exist(string fPath)
	{struct stat sb;
	if(stat(fPath.c_str(),&sb)==0 && S_ISDIR(sb.st_mode)){return true;}
	else{return false;}}

string extract_containing_folder(string fPath)
	{int pos=fPath.rfind("/");
	if(pos!=string::npos)
		{// Full File Path
		return fPath.substr(0,pos+1);
		}
	else
		{// No Folder, only File Name
		return "";}
	}

string extract_file_name(string fPath)
	{int pos=fPath.rfind("/");
	if(pos!=string::npos)
		{// Full File Path
		return fPath.substr(pos+1,fPath.length()-pos-1);
		}
	else
		{// Already File Name
		return fPath;}
	}

string extract_letter_from_noteCode(string noteCode)
	{// noteCode = letter+octave+otherMarks
	int numNumbers=9,pos;
	string *theNumbers=new string[numNumbers],octave,letter,otherMarks;
	if(noteCode.compare("R")==0){letter="R"; return letter;}
	// Extract letter by finding octave, being a number 1-8	
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	// Search Note Code for Number
	for(int i=0;i<numNumbers;i++)
		{pos=noteCode.find(theNumbers[i],0);
		if(pos!=string::npos)
			{// Octave Found, so extract
			octave=theNumbers[i];
			// Extract letter using position
			letter=noteCode.substr(0,pos);
			break;}
		}
	return letter;}

string extract_amplitude_from_noteCode(string noteCode)
	{string Output="-1";
	int pos=noteCode.find("A",0);
	int numNumbers=9;
	string *theNumbers=new string[numNumbers+1],tmp;
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	theNumbers[numNumbers]=".";
	bool SEARCHING=true;
	while(SEARCHING)
		{for(int i=0;i<numNumbers+1;i++)
			{tmp=noteCode[pos+1];
			if(tmp.compare(theNumbers[i])==0)
				{// Amplitude Sequence found
				if(pos!=string::npos){Output=noteCode.substr(pos+1,noteCode.length()-pos-1); SEARCHING=false; break;}
				}
			}
		if(!SEARCHING || pos==string::npos){break;}
		else{pos=noteCode.find("A",pos+1);}
		}
	return formatNumberString(Output);}

string extract_lyric_from_noteCode(string noteCode)
	{// noteCode = lyric+otherMarks ooh16A.5
	int numNumbers=9,pos,*tmpIntArr,N=0,tmpIntValue;
	string *theNumbers=new string[numNumbers],lyric,lst="",lst2="",otherMarks,delimiter=GLOBAL_DELIMITER;
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	// Search Note Code for Number
	for(int i=0;i<numNumbers;i++)
		{pos=noteCode.find(theNumbers[i],0);
		if(pos!=string::npos)
			{// Update Number Pos list
			lst+=cnvrtNumToStrng(pos,0)+delimiter;
			N++;}
		}
	// Search for Fraction /
	pos=noteCode.find("/",0);
	if(pos!=string::npos)
		{lst+=cnvrtNumToStrng(pos,0)+delimiter;
		N++;}
	tmpIntArr=fill_int_array(lst,N,delimiter);
	bool SORTING=true,SORTED;
	while(SORTING)
		{SORTED=true;
		for(int i=0;i<N-1;i++)
			{if(tmpIntArr[i]>tmpIntArr[i+1])
				{tmpIntValue=tmpIntArr[i];
				tmpIntArr[i]=tmpIntArr[i+1];
				tmpIntArr[i+1]=tmpIntValue;
				SORTED=false;}
			}
		if(SORTED){SORTING=false; break;}
		}
	lst="";
	for(int i=0;i<N;i++){lst+=noteCode[tmpIntArr[i]];}
	//cout<<"|"<<lst<<"|"<<endl;
	//
	pos=tmpIntArr[0];
	int theStart=0;
	int theEnd=pos-1;
	// Lyric
	lyric=noteCode.substr(theStart,theEnd-theStart+1);
	return lyric;}

string extract_octave_from_noteCode(string noteCode)
	{// noteCode = letter+octave+otherMarks
	int numNumbers=9,pos;
	string *theNumbers=new string[numNumbers],octave,letter,otherMarks;
	if(noteCode.compare("R")==0){letter="R"; octave=""; return octave;}
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	// Search Note Code for Number
	for(int i=0;i<numNumbers;i++)
		{pos=noteCode.find(theNumbers[i],0);
		if(pos!=string::npos)
			{// Octave Found, so extract
			octave=theNumbers[i];
			// Extract letter using position
			letter=noteCode.substr(0,pos);
			break;}
		}
	return octave;}

string extract_other_marks_from_noteCode(string noteCode)
	{// noteCode = letter+octave+otherMarks
	int numNumbers=9,pos,df;
	string *theNumbers=new string[numNumbers],octave,letter,otherMarks,tmp;
	if(noteCode.compare("R")==0){letter="R"; otherMarks=""; return otherMarks;}
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	// Search Note Code for Number
	for(int i=0;i<numNumbers;i++)
		{pos=noteCode.find(theNumbers[i],0);
		if(pos!=string::npos)
			{// Octave Found, so extract
			octave=theNumbers[i];
			// Extract letter using position
			letter=noteCode.substr(0,pos);
			break;}
		}
	// Combine letter and octave to subtract otherMarks
	tmp=letter+octave;
	if(tmp.length()==noteCode.length())
		{// No Other Marks
		return "";}
	else if(noteCode.length()>tmp.length())
		{df=noteCode.length()-tmp.length();
		// Extract Other Marks
		otherMarks=noteCode.substr(tmp.length(),df);
		return otherMarks;}
	else{cerr<<"Error in extract_other_marks_from_noteCode!\nNever should get here...\n"; exit(EXIT_FAILURE);}
	}

string extract_time_from_noteCode(string noteCode)
	{// noteCode = lyric+otherMarks ooh16A.5
	int numNumbers=9,pos,*tmpIntArr,N=0,tmpIntValue,aPos;
	string *theNumbers=new string[numNumbers+1],Output,lst="",lst2="",delimiter=GLOBAL_DELIMITER;
	string remaining=noteCode,tmp;
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	theNumbers[numNumbers]=".";
	// Check for Amplitude
	bool SEARCHING=true;
	pos=noteCode.find("A",0);
	while(SEARCHING)
		{if(pos!=string::npos)
			{tmp=noteCode[pos+1];
			for(int i=0;i<numNumbers+1;i++)
				{if(tmp.compare(theNumbers[i])==0){remaining=noteCode.substr(0,pos); SEARCHING=false; break;}
				}
			}
		if(!SEARCHING || pos==string::npos){break;}
		else{pos=noteCode.find("A",pos+1);}
		}
	
	// Search Note Code for Number
	for(int i=0;i<numNumbers;i++)
		{pos=remaining.find(theNumbers[i],0);
		if(pos!=string::npos)
			{// Update Number Pos list
			lst+=cnvrtNumToStrng(pos,0)+delimiter;
			N++;}
		}
	// Search for Fraction /
	pos=remaining.find("/",0);
	if(pos!=string::npos)
		{lst+=cnvrtNumToStrng(pos,0)+delimiter;
		N++;}	
	tmpIntArr=fill_int_array(lst,N,delimiter);
	bool SORTING=true,SORTED;
	while(SORTING)
		{SORTED=true;
		for(int i=0;i<N-1;i++)
			{if(tmpIntArr[i]>tmpIntArr[i+1])
				{tmpIntValue=tmpIntArr[i];
				tmpIntArr[i]=tmpIntArr[i+1];
				tmpIntArr[i+1]=tmpIntValue;
				SORTED=false;}
			}
		if(SORTED){SORTING=false; break;}
		}
	lst="";
	for(int i=0;i<N;i++){lst+=noteCode[tmpIntArr[i]];}
	Output=lst;
	return Output;}

void extract_lyrics_from_measure(string measure,string inputValue,int &pos,double timePerMeasure,string frequenciesInMeasure,\
								bool &NOTE_SHARP,bool &NOTE_DOUBLE_SHARP,bool &NOTE_FLAT,bool &NOTE_DOUBLE_FLAT,string &theLyric,string &theNote,string &theLyricDuration,string &theAmplitude,int &fracSz)
	{bool SEARCHING=true,FOUND_NUMBER;
	string lst="",tmp;
	// pos is position of first number character
	int numNumbers=9,tmpPos,theStart,theEnd;
	string *theNumbers=new string[numNumbers];
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	// Extract Note Code
	tmpPos=measure.rfind("-",pos);
	if(tmpPos!=string::npos && tmpPos>6)
		{// Found line
		theStart=tmpPos+1;}
	else
		{tmpPos=measure.rfind(" ",pos);
		if(tmpPos!=string::npos && tmpPos>6)
			{// Found Space
			theStart=tmpPos+1;}
		else
			{tmpPos=measure.rfind("|",pos);
			if(tmpPos!=string::npos && tmpPos>6)
				{// Found |
				theStart=tmpPos+1;}
			else
				{cerr<<"Error in extract_lyrics_from_measure!\nCould not determine starting point for:\n"<<measure<<endl; exit(EXIT_FAILURE);}
			}
		}
	// Find End
	tmpPos=measure.find("-",pos);
	if(tmpPos!=string::npos && tmpPos>6)
		{// Found line
		theEnd=tmpPos-1;}
	else
		{tmpPos=measure.find(" ",pos);
		if(tmpPos!=string::npos && tmpPos>6)
			{// Found Space
			theEnd=tmpPos-1;}
		else
			{tmpPos=measure.find("|",pos);
			if(tmpPos!=string::npos && tmpPos>6)
				{// Found |
				theEnd=tmpPos-1;}
			else
				{tmpPos=measure.find("\n",pos);
				if(tmpPos!=string::npos && tmpPos>6)
					{// Found linebreak
					theEnd=tmpPos-1;}
				else
					{if(pos>=measure.length()-1){theEnd=pos;}
					else{cerr<<"Error in extract_lyrics_from_measure!\nCould not determine end point for:\n"<<measure<<endl; exit(EXIT_FAILURE);}					
					}
				}
			}
		}
	//
	string noteInput=measure.substr(theStart,theEnd-theStart+1);
	//cout<<"|"<<noteInput<<"|"<<endl;
	//cout<<"|"<<extract_amplitude_from_noteCode(noteInput)<<"|"<<endl;	
	//cout<<"|"<<extract_lyric_from_noteCode(noteInput)<<"|"<<endl;
	//cout<<"|"<<extract_time_from_noteCode(noteInput)<<"|"<<endl;
	tmp=extract_time_from_noteCode(noteInput);
	fracSz=tmp.length();
	// Handle Time Fraction
	double lyricDuration=define_note_duration_from_fraction(tmp,timePerMeasure);
	theLyricDuration=formatNumberString(cnvrtNumToStrng(lyricDuration,SIG_FIGS));
	// Handle Amplitude
	theAmplitude=extract_amplitude_from_noteCode(noteInput);
	// The Lyric/Phoneme is generate
	theLyric=extract_lyric_from_noteCode(noteInput);
	tmpPos=theLyric.rfind("##");
	if(tmpPos!=string::npos){NOTE_DOUBLE_SHARP=true; theLyric.replace(tmpPos,2,"");}
	else
		{tmpPos=theLyric.rfind("#");
		if(tmpPos!=string::npos){NOTE_SHARP=true;  theLyric.replace(tmpPos,1,"");}
		else
			{tmpPos=theLyric.rfind("_bb");
			if(tmpPos!=string::npos && tmpPos==theLyric.length()-3){NOTE_FLAT=true; theLyric.replace(tmpPos,1,""); tmpPos=theLyric.rfind("b"); theLyric.replace(tmpPos,1,"");}
			else
				{tmpPos=theLyric.rfind("bb");
				if(tmpPos!=string::npos && tmpPos==theLyric.length()-2){NOTE_DOUBLE_FLAT=true; theLyric.replace(tmpPos,2,"");}
				else
					{tmpPos=theLyric.rfind("_b");
					if(tmpPos!=string::npos){theLyric.replace(tmpPos,1,"");}
					else
						{tmpPos=theLyric.rfind("b");
						if(tmpPos!=string::npos && tmpPos==theLyric.length()-1 && theLyric.length()>1){NOTE_FLAT=true; theLyric.replace(tmpPos,1,"");}}				
					}
				}			
			}
		}
	SEARCHING=true;
	tmpPos=0;
	while(SEARCHING)
		{tmpPos=theLyric.find("_b",tmpPos);
		if(tmpPos!=string::npos){theLyric.replace(tmpPos,1,"");}
		else{SEARCHING=false; break;}
		}
	// Handle Notes
	// Get Frequency
	string letter=frequenciesInMeasure.substr(0,1);
	string octave=frequenciesInMeasure.substr(1,1);
	if(NOTE_FLAT){letter+="f";}
	if(NOTE_SHARP){letter+="s";}
	if(theLyric.compare("R")==0){theLyric=""; theNote="R";}
	else{theNote=letter+octave;}	
	}

void extract_regular_notes_from_measure(string measure,string inputValue,int &pos,double timePerMeasure,string frequenciesInMeasure,\
								bool &NOTE_SHARP,bool &NOTE_DOUBLE_SHARP,bool &NOTE_FLAT,bool &NOTE_DOUBLE_FLAT,string &theNote,string &theNoteDuration,string &theAmplitude)
	{bool SEARCHING=true;
	string lst="",tmp;
	//cout<<inputValue<<endl;
	while(SEARCHING)
		{if(pos+1<measure.length()){inputValue=measure.substr(pos+1,1);}
		else{SEARCHING=false; break;}
		if(inputValue.compare("O")==0 || inputValue.compare("X")==0 || inputValue.compare("S")==0 || inputValue.compare("R")==0 || \
			inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
			{SEARCHING=false; break;}
		else if(inputValue.compare("#")==0 && !NOTE_SHARP)
			{// Sharp Note
			NOTE_SHARP=true; pos++;}
		else if(inputValue.compare("#")==0 && NOTE_SHARP)
			{// Double Sharp Note
			NOTE_DOUBLE_SHARP=true; NOTE_SHARP=false; pos++;}
		else if(inputValue.compare("b")==0 && !NOTE_FLAT)
			{// Flat Note
			NOTE_FLAT=true; pos++;}
		else if(inputValue.compare("b")==0 && NOTE_FLAT)
			{// Double Flat Note
			NOTE_DOUBLE_FLAT=true; NOTE_FLAT=false; pos++;}
		else
			{lst+=inputValue; pos++;}
		}
	// Handle Fraction
	double noteDuration=define_note_duration_from_fraction(lst,timePerMeasure);
	//cout<<lst<<"|"<<noteDuration<<endl;
	// Handle Amplitude
	theAmplitude=define_amplitude_from_note(lst);
	//cout<<lst<<"|"<<theAmplitude<<endl;
	// Get Frequency
	string letter=frequenciesInMeasure.substr(0,1);
	string octave=frequenciesInMeasure.substr(1,1);
	if(NOTE_FLAT){letter+="f";}
	if(NOTE_SHARP){letter+="s";}
	// Generate Note and Note Duration List
	//noteLst+=letter+octave+innerDelimiter;
	theNote=letter+octave;
	//durationLst+=formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS))+innerDelimiter;
	theNoteDuration=formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS));
	}

void extract_rests_from_measure(string measure,string inputValue,int &pos2,double timePerMeasure,string &theNote,string &theNoteDuration)
	{bool SEARCHING=true;
	string lst="",tmp;
	while(SEARCHING)
		{if(pos2+1<measure.length()){inputValue=measure.substr(pos2+1,1);}
		else{SEARCHING=false; break;}
		if(inputValue.compare("O")==0 || inputValue.compare("X")==0 || inputValue.compare("S")==0 || inputValue.compare("R")==0 || \
			inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
			{SEARCHING=false; break;}
		else
			{lst+=inputValue; pos2++;}
		}
	// Handle Fraction
	double noteDuration=define_note_duration_from_fraction(lst,timePerMeasure);
	// Generate Silence
	theNote="R";
	theNoteDuration=formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS));
	}

void extract_staccato_notes_from_measure(string measure,string inputValue,int &pos,double timePerMeasure,string frequenciesInMeasure,\
								bool &NOTE_SHARP,bool &NOTE_DOUBLE_SHARP,bool &NOTE_FLAT,bool &NOTE_DOUBLE_FLAT,string &theNote,string &theNoteDuration,string &theAmplitude)
	{bool SEARCHING=true;
	string lst="",tmp;
	//cout<<inputValue<<endl;
	while(SEARCHING)
		{if(pos+1<measure.length()){inputValue=measure.substr(pos+1,1);}
		else{SEARCHING=false; break;}
		if(inputValue.compare("O")==0 || inputValue.compare("X")==0 || inputValue.compare("S")==0 || inputValue.compare("R")==0 || \
			inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
			{SEARCHING=false; break;}
		else if(inputValue.compare("#")==0 && !NOTE_SHARP)
			{// Sharp Note
			NOTE_SHARP=true; pos++;}
		else if(inputValue.compare("#")==0 && NOTE_SHARP)
			{// Double Sharp Note
			NOTE_DOUBLE_SHARP=true; NOTE_SHARP=false; pos++;}
		else if(inputValue.compare("b")==0 && !NOTE_FLAT)
			{// Flat Note
			NOTE_FLAT=true; pos++;}
		else if(inputValue.compare("b")==0 && NOTE_FLAT)
			{// Double Flat Note
			NOTE_DOUBLE_FLAT=true; NOTE_FLAT=false; pos++;}
		else
			{lst+=inputValue; pos++;}
		}
	// Handle Fraction
	double noteDuration=define_note_duration_from_fraction(lst,timePerMeasure);
	// Handle Amplitude
	theAmplitude=define_amplitude_from_note(lst);
	// Get Frequency
	string letter=frequenciesInMeasure.substr(0,1);
	string octave=frequenciesInMeasure.substr(1,1);
	if(NOTE_FLAT){letter+="f";}
	if(NOTE_SHARP){letter+="s";}
	// Generate Note and Note Duration List
	theNote=letter+octave+"X";
	theNoteDuration=formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS));
	}

void extract_sustained_notes_from_measure(string measure,string inputValue,int &pos,double timePerMeasure,string frequenciesInMeasure,\
								bool &NOTE_SHARP,bool &NOTE_DOUBLE_SHARP,bool &NOTE_FLAT,bool &NOTE_DOUBLE_FLAT,string &theNote,string &theNoteDuration,string &theAmplitude)
	{bool SEARCHING=true;
	string lst="",tmp;
	//cout<<inputValue<<endl;
	while(SEARCHING)
		{if(pos+1<measure.length()){inputValue=measure.substr(pos+1,1);}
		else{SEARCHING=false; break;}
		if(inputValue.compare("O")==0 || inputValue.compare("X")==0 || inputValue.compare("S")==0 || inputValue.compare("R")==0 || \
			inputValue.compare("|")==0 || inputValue.compare("-")==0 || inputValue.compare(" ")==0 || inputValue.compare("\n")==0)
			{SEARCHING=false; break;}
		else if(inputValue.compare("#")==0 && !NOTE_SHARP)
			{// Sharp Note
			NOTE_SHARP=true; pos++;}
		else if(inputValue.compare("#")==0 && NOTE_SHARP)
			{// Double Sharp Note
			NOTE_DOUBLE_SHARP=true; NOTE_SHARP=false; pos++;}
		else if(inputValue.compare("b")==0 && !NOTE_FLAT)
			{// Flat Note
			NOTE_FLAT=true; pos++;}
		else if(inputValue.compare("b")==0 && NOTE_FLAT)
			{// Double Flat Note
			NOTE_DOUBLE_FLAT=true; NOTE_FLAT=false; pos++;}
		else
			{lst+=inputValue; pos++;}
		}
	// Handle Fraction
	double noteDuration=define_note_duration_from_fraction(lst,timePerMeasure);
	// Handle Amplitude
	theAmplitude=define_amplitude_from_note(lst);
	// Get Frequency
	string letter=frequenciesInMeasure.substr(0,1);
	string octave=frequenciesInMeasure.substr(1,1);
	if(NOTE_FLAT){letter+="f";}
	if(NOTE_SHARP){letter+="s";}
	// Generate Note and Note Duration List
	theNote=letter+octave+"S";
	theNoteDuration=formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS));
	}

bool file_exist(string fPath)
	{struct stat sb;
	if(stat(fPath.c_str(),&sb)==0 && S_ISREG(sb.st_mode)){return true;}
	else{return false;}}

int* fill_int_array(string Data,int numPnts,string delimiter)
	{int* Output=new int[numPnts];
	string bld="",tmp="";
	int Counter=0;
	for(int i=0;i<Data.length();i++)
		{tmp=Data[i];
		if(tmp.compare(delimiter)==0 && Counter<numPnts)
			{Output[Counter]=stoi(bld);
			Counter++;bld="";}
		else{bld+=Data[i];}}
	return Output;}

double* fill_double_array(string Data,int numPnts,string delimiter)
	{double* Output=new double[numPnts];
	string bld="",tmp="";
	int Counter=0;
	for(int i=0;i<Data.length();i++)
		{tmp=Data[i];
		if(tmp.compare(delimiter)==0 && Counter<numPnts)
			{Output[Counter]=strtod(bld.c_str(),NULL);
			Counter++;bld="";}
		else{bld+=Data[i];}}
	return Output;}

SAMPLE* fill_sample_array(string Data,int numPnts,string delimiter)
	{SAMPLE* Output=new SAMPLE[numPnts];
	string bld="",tmp="";
	int Counter=0;
	for(int i=0;i<Data.length();i++)
		{tmp=Data[i];
		if(tmp.compare(delimiter)==0 && Counter<numPnts)
			{Output[Counter]=(SAMPLE)strtod(bld.c_str(),NULL);
			Counter++;bld="";}
		else{bld+=Data[i];}}
	return Output;}

string* fill_string_array(string Data,int numPnts,string delimiter)
	{string* Output=new string[numPnts];
	string bld="",tmp="";
	int Counter=0;
	for(int i=0;i<Data.length();i++)
		{tmp=Data[i];
		if(tmp.compare(delimiter)==0 && Counter<numPnts)
			{Output[Counter]=bld;Counter++;bld="";}
		else{bld+=Data[i];}}
	return Output;}

string formatNumberString(string Num)
	{string Output,tmp,tmp2;
	/* Check if contains decimal*/
	int i,pos=Num.find(".",0);
	if(pos==string::npos)
		{/* Number Value is an integer with no decimal*/
		for(i=0;i<Num.length();i++)
			{tmp=Num[i];
			/* Find 1st Non-Zero Number*/
			if(tmp.compare("0")!=0){break;}}
		Output=Num.substr(i,Num.length()-i);}
	else if(pos==0)
		{/* Precede decimal with zero*/
		Output="0";
		for(i=Num.length();i>0;i--)
			{tmp=Num[i-1];
			/* Search in reverse for 1st Non-Zero Number*/
			if(tmp.compare("0")!=0){break;}}
		Output+=Num.substr(0,i);}
	else{/* Number holds form XXX.XXX*/
		for(i=0;i<pos;i++)
			{tmp=Num[i];
			/* Find 1st Non-Zero Number*/
			if(tmp.compare("0")!=0){break;}}
		/* Define value preceding decimal*/
		if(i==pos){/* Only Zeros found up to the decimal*/Output="0";}
		else{Output=Num.substr(i,pos-i);}
		for(i=Num.length();i>pos;i--)
			{tmp=Num[i-1];
			/* Search in reverse for 1st Non-Zero Number*/
			if(tmp.compare("0")!=0){break;}}
		tmp2=Num.substr(pos,i-pos);
		/* will contain .XXX*/
		if(tmp2.length()>1){Output+=tmp2;}}
	return Output;}

void frequency_shift_signal_1_channel_input(string inputWaveFile,double changeInFrequency,double noteDuration,char **envp,string outputWaveFile)
	{// Save input to wave file for inputting into csound
	//string tmpFile=get_current_dir_name(); tmpFile+="/untuned_input.wav";
	//write_wave_file(tmpFile,inS.signal,inS.N);
	int N;
	//double *theInput=read_amplitudes_in_wave_file(inputWaveFile,N);
	//if(N%2!=0){N--;}
	//cout<<N<<endl;
	double fs=SAMPLE_RATE;
	//double noteDuration=get_wave_file_time_duration(inputWaveFile);
	//cout<<noteDuration<<endl;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/freqShift.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/freqShift.orc";
	//
	//string waveFile=get_current_dir_name(); waveFile+="/WAV/tuned_output.wav";
	if(file_exist(outputWaveFile)){remove(outputWaveFile.c_str());}
	//string waveFile=inputWaveFile+"_tuned";
	double *theSound;
	int tmpN;
	int status,rChk;
	ifstream fIn;
	ofstream fOut;
	// Write Score File
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in frequency_shift_signal\nOutput score file file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_score_for_frequency_shift(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)),formatNumberString(cnvrtNumToStrng(changeInFrequency,SIG_FIGS)));
	fOut.close();
	// Write Orchestra File
	fOut.open(orchestraFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in frequency_shift_signal\nOutput orchestra file could not be opened.\n"<<orchestraFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_orchestra_for_frequency_shift_1_channel(inputWaveFile);
	fOut.close();	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,outputWaveFile,envp);
	
	//double *Output=read_amplitudes_in_wave_file(outputWaveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	//return Output;
	}

void frequency_shift_signal_2_channel_input(string inputWaveFile,double changeInFrequency,char **envp,string outputWaveFile)
	{// Save input to wave file for inputting into csound
	//string tmpFile=get_current_dir_name(); tmpFile+="/untuned_input.wav";
	//write_wave_file(tmpFile,inS.signal,inS.N);
	int N;
	//double *theInput=read_amplitudes_in_wave_file(inputWaveFile,N);
	//if(N%2!=0){N--;}
	//cout<<N<<endl;
	double fs=SAMPLE_RATE;
	double noteDuration=get_wave_file_time_duration(inputWaveFile);
	//cout<<noteDuration<<endl;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/freqShift.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/freqShift.orc";
	//
	//string waveFile=get_current_dir_name(); waveFile+="/WAV/tuned_output.wav";
	if(file_exist(outputWaveFile)){remove(outputWaveFile.c_str());}
	//string waveFile=inputWaveFile+"_tuned";
	double *theSound;
	int tmpN;
	int status,rChk;
	ifstream fIn;
	ofstream fOut;
	// Write Score File
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in frequency_shift_signal\nOutput score file file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_score_for_frequency_shift(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)),formatNumberString(cnvrtNumToStrng(changeInFrequency,SIG_FIGS)));
	fOut.close();
	// Write Orchestra File
	fOut.open(orchestraFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in frequency_shift_signal\nOutput orchestra file could not be opened.\n"<<orchestraFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_orchestra_for_frequency_shift_2_channels(inputWaveFile);
	fOut.close();	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,outputWaveFile,envp);
	
	//double *Output=read_amplitudes_in_wave_file(outputWaveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	//return Output;
	}

int get_pitch_for_misc_percussion_type(string drumType)
	{// Middle C (C4) has a MIDI note number of 60
	int pitch;
	if(drumType.compare("ROLL_HI_TRIANGLE")==0){pitch=43;}
	else if(drumType.compare("MUTED_HI_TRIANGLE")==0){pitch=44;}
	else if(drumType.compare("OPEN_HI_TRIANGLE")==0){pitch=45;}
	else if(drumType.compare("GONG")==0){pitch=46;}
	else if(drumType.compare("TAMTAM_A")==0){pitch=47;}
	else if(drumType.compare("TAMTAM_B")==0){pitch=48;}
	else if(drumType.compare("TAMTAM_C")==0){pitch=49;}
	else if(drumType.compare("HI_FINGER_CYMBALS")==0){pitch=51;}
	else if(drumType.compare("LO_FINGER_CYMBALS")==0){pitch=52;}
	else if(drumType.compare("SHAKE_TAMBOURINE")==0){pitch=53;}
	else if(drumType.compare("HIT_TAMBOURINE")==0){pitch=54;}
	else if(drumType.compare("ROLL_TAMBOURINE")==0){pitch=55;}
	else if(drumType.compare("COWBELL")==0){pitch=56;}
	else if(drumType.compare("VIBRA_SLAP")==0){pitch=58;}
	else if(drumType.compare("HIT_HIGH_CONGA")==0){pitch=60;}
	else if(drumType.compare("TAP_HIGH_CONGA")==0){pitch=61;}
	else if(drumType.compare("HIT_MID_CONGA")==0){pitch=62;}
	else if(drumType.compare("TAP_MID_CONGA")==0){pitch=63;}
	else if(drumType.compare("HIT_LOW_CONGA")==0){pitch=64;}
	else if(drumType.compare("TAP_LOW_CONGA")==0){pitch=65;}
	else if(drumType.compare("CABASA")==0){pitch=69;}
	else if(drumType.compare("SHAKER")==0){pitch=70;}
	else if(drumType.compare("CRANK_RATCHET")==0){pitch=71;}
	else if(drumType.compare("FAST_RATCHET")==0){pitch=72;}
	else if(drumType.compare("SLOW_RATCHET")==0){pitch=73;}
	else if(drumType.compare("CASTANETS")==0){pitch=74;}
	else if(drumType.compare("CLAVES")==0){pitch=75;}
	else if(drumType.compare("HI_WOOD_BLOCK")==0){pitch=76;}
	else if(drumType.compare("LO_WOOD_BLOCK")==0){pitch=77;}
	else if(drumType.compare("ROLL_LO_TRIANGLE")==0){pitch=79;}
	else if(drumType.compare("MUTED_LO_TRIANGLE")==0){pitch=80;}
	else if(drumType.compare("OPEN_LO_TRIANGLE")==0){pitch=81;}
	else if(drumType.compare("SLEIGH_BELLS")==0){pitch=82;}
	else if(drumType.compare("BELL_TREE_1")==0){pitch=83;}
	else if(drumType.compare("BELL_TREE_2")==0){pitch=84;}
	else if(drumType.compare("BELL_TREE_3")==0){pitch=86;}
	else if(drumType.compare("BELL_TREE_4")==0){pitch=87;}
	else if(drumType.compare("BELL_TREE_5")==0){pitch=88;}
	else if(drumType.compare("BAR_CHIMES_A")==0){pitch=89;}
	else if(drumType.compare("BAR_CHIMES_B")==0){pitch=90;}
	else if(drumType.compare("BAR_CHIMES_C")==0){pitch=91;}
	else{cerr<<"Error in get_pitch_for_misc_percussion_type()!\nUn-recognized drum type ("<<drumType<<")\n"; exit(EXIT_FAILURE);}
	return pitch;}

int get_pitch_for_simple_drums_type(string drumType)
	{// Middle C (C4) has a MIDI note number of 60
	int pitch;
	if(drumType.compare("BASS_DRUM_1")==0){pitch=36;}
	else if(drumType.compare("BASS_DRUM_2")==0){pitch=38;}
	else if(drumType.compare("LEFT_HIT_SNARE_1")==0){pitch=48;}
	else if(drumType.compare("ROUND_HIT_SNARE_1")==0){pitch=49;}
	else if(drumType.compare("RIGHT_HIT_SNARE_1")==0){pitch=50;}
	else if(drumType.compare("ROLL_SNARE_1")==0){pitch=52;}
	else if(drumType.compare("LEFT_HIT_SNARE_2")==0){pitch=53;}
	else if(drumType.compare("ROUND_HIT_SNARE_2")==0){pitch=54;}
	else if(drumType.compare("RIGHT_HIT_SNARE_2")==0){pitch=55;}
	else if(drumType.compare("TAP_SNARE_2")==0){pitch=56;}
	else if(drumType.compare("ROLL_SNARE_2")==0){pitch=57;}
	else if(drumType.compare("ROLL_A_CYMBALS_1")==0){pitch=60;}
	else if(drumType.compare("LO_CRASH_CYMBALS_1")==0){pitch=61;}
	else if(drumType.compare("ROLL_B_CYMBALS_1")==0){pitch=62;}
	else if(drumType.compare("HI_CRASH_CYMBALS_1")==0){pitch=63;}
	else if(drumType.compare("SHORT_ROLL_CYMBALS_2")==0){pitch=65;}
	else if(drumType.compare("LO_CRASH_CYMBALS_2")==0){pitch=66;}
	else if(drumType.compare("MID_ROLL_CYMBALS_2")==0){pitch=67;}
	else if(drumType.compare("LONG_ROLL_CYMBALS_2")==0){pitch=69;}
	else if(drumType.compare("HI_CRASH_CYMBALS_2")==0){pitch=70;}
	else{cerr<<"Error in get_pitch_for_simple_drums_type()!\nUn-recognized drum type ("<<drumType<<")\n"; exit(EXIT_FAILURE);}
	return pitch;}

double get_wave_file_time_duration(string waveFile)
	{double fs=SAMPLE_RATE;
	SNDFILE *SoundFile;
	SF_INFO SoundFileInfo;
	int bufferSize=256, filetype = SF_ENDIAN_LITTLE | SF_FORMAT_RAW | SF_FORMAT_PCM_16 , intVal, returnValue;
	short buffer[bufferSize];
	for(int k=0;k<bufferSize;k++){buffer[k]=k;}
	
	filetype=SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	SoundFile=sf_open(waveFile.c_str(), SFM_READ, &SoundFileInfo);
	if(!SoundFile)
		{printf ("Line %d: sf_open_write failed in get_wave_file_time_duration with error : ", __LINE__); fflush(stdout); sf_perror(NULL);
		exit(EXIT_FAILURE);}

	returnValue=sf_seek(SoundFile,0,SEEK_SET);		// returns position offset in file cout<<returnValue<<endl;
	if(returnValue<0){cerr<<"Error seeking to beginning of file\n";}
	
	if(SoundFileInfo.frames < bufferSize)
		{printf ("Line %d: Incorrect number of.frames in file(%s). (%d => %ld)\n", __LINE__,waveFile.c_str(), bufferSize, (long)SoundFileInfo.frames);
		exit(EXIT_FAILURE);}

	//SoundFile=sf_open(inFile.c_str(), SFM_READ, &SoundFileInfo);	//open a file and put it's info into a struct "SoundFileInfo"
	int Sz=SoundFileInfo.channels*SoundFileInfo.frames; //SoundFileInfo.channels * SoundFileInfo.frames;
	//int Sz=SoundFileInfo.frames;
	double *Output= new double[Sz];	//allocate an array to hold the samples
	intVal=sf_readf_double(SoundFile, Output,Sz);
	if( intVal != Sz)
		{
		if(intVal>0)
			{//printf ("Line %d: short read (%d).\nNumber of Frames: %d\nNumber of Channels: %d\n%s", __LINE__, intVal, SoundFileInfo.frames, SoundFileInfo.channels,waveFile.c_str()) ;
			//exit(EXIT_FAILURE);
			//Sz=intVal;
			}
		else if(intVal==0)
			{// End of File (EOF) Reach need to seek to top
			returnValue=sf_seek(SoundFile,0,SEEK_SET);		//cout<<returnValue<<endl;
			}
		else{cerr<<"So we got here\n";}
		}

	//for(int i=0;i<Sz;i++){cout<<i<<" | "<<Output[i]<<endl;}
	if(Sz<0){cerr<<"Error in dnoise_song!\nNegative number of frames found|!\n"; exit(EXIT_FAILURE);}
	else if(Sz==0){cerr<<"Error in dnoise_song!\nNo frames found|!\n";}
	
	returnValue=sf_close(SoundFile);
	if(returnValue!=0){cerr<<"Error in dnoise_song!\nCould not close wave file:\n"<<waveFile<<"\n"; exit(EXIT_FAILURE);}
	sf_close(SoundFile);

	// Calculate Time Duration
	double timeDuration=Sz/fs;
	timeDuration/=(double)SoundFileInfo.channels;
	return timeDuration;}

void dnoise_song(string inputFile,string outputFile,char** envp)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/dnoise.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/dnoise.orc";

	// Remove Previous Temporary Files, if present in current working directory (input.wav, output.wav and noise.wav)
	int status,N,rChk;
	ifstream fIn;
	string tmpFile=get_current_dir_name(); tmpFile+="/input.wav";
	fIn.open(tmpFile.c_str(),ifstream::in);
	if(!fIn.fail()){fIn.close(); rChk=remove(tmpFile.c_str());}
	tmpFile=get_current_dir_name(); tmpFile+="/output.wav";
	fIn.open(tmpFile.c_str(),ifstream::in);
	if(!fIn.fail()){fIn.close(); rChk=remove(tmpFile.c_str());}

	// Copy Input File into current working directory as input.wav
	string destFile=get_current_dir_name(); destFile+="/input.wav";
	if(!copyFile(inputFile,destFile)){cerr<<"Error in copyFile!\n"; exit(EXIT_FAILURE);}

	// Determine Time Duration (seconds) of input wave file
	double timeDuration=get_wave_file_time_duration(inputFile);
	// Write Score File
	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in dnoise_song\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_dnoise_score(formatNumberString(cnvrtNumToStrng(timeDuration,SIG_FIGS)));
	fOut.close();
	// Write Orchestra File
	fOut.open(orchestraFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in dnoise_song\nInput music file could not be opened.\n"<<orchestraFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_dnoise_orchestra();
	fOut.close();
	// De-Noise Song with cSound
	run_dnoise(orchestraFile,scoreFile,envp);

	// Copy Output File into current working directory as input.wav
	string srcFile=get_current_dir_name(); srcFile+="/output.wav";
	if(!copyFile(srcFile,outputFile)){cerr<<"Error in copyFile!\nCould not copy output"; exit(EXIT_FAILURE);}
	
	}

//void generate_animation(double *X,SAMPLE *Y,int Sz,string title,string mp3File,string noteFile,string outputMp4File)
void generate_animation(string dataFile,double endTime,int Sz,double xRange,string title,string mp3File,string noteFile,string outputMp4File)
	{// Write the Animation Files for running gnuplot
	string animationFldr=extract_containing_folder(dataFile);
	//write_gnuplot_scripts(dataFile,X[Sz-1]);
	write_gnuplot_scripts_with_keys(dataFile,noteFile,endTime,xRange,title,false);
	// Run Gnuplot Animation Simulation ANIMATION/
	string cmd="cd "+animationFldr+"; rm -f raw.h264; rm -f new.mp4; rm -f syncedOutput.mp4; rm -f outOfSyncOutput.mp4; gnuplot animation.plt | ffmpeg -f png_pipe -s:v 1920x1080 -i pipe: outOfSyncOutput.mp4";
	int ret=system(cmd.c_str());
	if(ret<0){cerr<<"Error!\n";}
	// Copy Raw Bitstream format of video
	//cmd="ffmpeg -i ANIMATION/outOfSyncOutput.mp4 -map 0:v -c:v copy -bsf:v h264_mp4toannexb ANIMATION/raw.h264";
	cmd="cd "+animationFldr+"; ffmpeg -i outOfSyncOutput.mp4 -map 0:v -c:v copy -bsf:v h264_mp4toannexb raw.h264";
	ret=system(cmd.c_str());
	if(ret<0){cerr<<"Error!\n";}
	// Change to New Frame Rate 4*initial rate of 25 frames per second
	int newFrameRate=4*25;
	//cmd="ffmpeg -fflags +genpts -r "+cnvrtNumToStrng(newFrameRate,0)+" -i ANIMATION/raw.h264 -c:v copy ANIMATION/syncedOutput.mp4";
	cmd="cd "+animationFldr+"; ffmpeg -fflags +genpts -r "+cnvrtNumToStrng(newFrameRate,0)+" -i raw.h264 -c:v copy syncedOutput.mp4";
	ret=system(cmd.c_str());
	if(ret<0){cerr<<"Error!\n";}
	//
	//cmd="ffmpeg -i ANIMATION/syncedOutput.mp4 -i \""+mp3File+"\" -c copy -map 0:v:0 -map 1:a:0 \""+outputMp4File+"\"";
	cmd="cd "+animationFldr+"; ffmpeg -i syncedOutput.mp4 -i input.mp3 -c copy -map 0:v:0 -map 1:a:0 new.mp4";
	ret=system(cmd.c_str());
	if(ret<0){cerr<<"Error!\n";}
	}

double* generate_drum_sound(string drumType,double strikeVelocity,double noteVolume,char** envp,double noteDuration,int& outputArrSz)
	{// Input: noteDuration is time in seconds the note is supposed to last
	// Input: Time Per Measure (seconds)
	double fs=SAMPLE_RATE;
	string csound_executable=CSOUND_EXE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/drums.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/drums.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+=drumType+formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS))+"s.wav";
	string noteStartTime="0";

	// Run CSound
	string cmd;
	int status,N,tempoInput;
	ifstream fIn;
	ofstream fOut;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();		
		
		// Exit Function
		double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
		outputArrSz=N;
		return Output;
		}
	else
		{// Write Drum Beats Music Score File
		fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut.fail()){cerr<<"ERROR in \nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
		if(drumType.compare("TAMBOURINE")==0)
			{double* Output=generate_tambourine_sound(noteDuration,envp,outputArrSz);
			return Output;
			}
		else
			{fOut<<write_drums_score(drumType,noteStartTime,noteDuration,\
					formatNumberString(cnvrtNumToStrng(strikeVelocity,SIG_FIGS)),\
					formatNumberString(cnvrtNumToStrng(noteVolume,SIG_FIGS)));}
		fOut.close();
		
		// Generate wave file with csound
		run_csound(orchestraFile,scoreFile,waveFile,envp);
		}
	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_guitar_sound(double noteDuration,double pitch,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/guitar.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/guitar.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="GUITAR_"+formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS))+"_"+formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS))+"s.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_guitar_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_score_for_soundFonts(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)),formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)),"100");
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_miramba_sound(double freq,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/miramba.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/miramba.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="MIRAMBA.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_miramba_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_miramba_score(formatNumberString(cnvrtNumToStrng(freq,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_piano_sound(double noteDuration,double freq,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/piano.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/piano.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="PIANO.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_piano_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_piano_score(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(freq,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_pluck_sound(double noteVolume,double pitch,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/pluck.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/pluck.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="PLUCK.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_pluck_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_pluck_score(formatNumberString(cnvrtNumToStrng(interpolate(noteVolume,0,200,0,2),SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_pluck2_sound(double noteVolume,double pitch,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/pluck2.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/pluck.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="PLUCK.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_pluck2_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_pluck2_score(formatNumberString(cnvrtNumToStrng(interpolate(noteVolume,0,200,0,2),SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_plucked_bass_sound(double noteVolume,double pitch,double pluckDuration,double noteDuration,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/plucked_bass.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/physmodl.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="PLUCKED_BASS"+formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS))+"s.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_plucked_bass_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_plucked_bass_score(formatNumberString(cnvrtNumToStrng(interpolate(noteVolume,0,200,0,50000),SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(pluckDuration,SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_plucked_string_sound(double noteVolume,double pitch,double noteDuration,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/plucked_string.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/multifx.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="PLUCKED_STRING"+formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS))+"s.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_plucked_string_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_plucked_string_score(formatNumberString(cnvrtNumToStrng(interpolate(noteVolume,0,200,0,16000),SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

string get_soundFont_File_from_soundType(string soundType,string otherMarks)
	{string soundFontsFile;
	if(soundType.compare("accordion_hit")==0){soundFontsFile=ACCORDION_HIT_SOUNDFONT;}
	else if(soundType.compare("acoustic_guitar")==0){soundFontsFile=ACOUSTIC_GUITAR_SOUNDFONT;}
	else if(soundType.compare("aeon_flux_guitar")==0){soundFontsFile=AEON_FLUX_GUITAR_SOUNDFONT;}
	else if(soundType.compare("aggressive_organ")==0){soundFontsFile=AGGRESSIVE_ORGAN_SOUNDFONT;}
	else if(soundType.compare("alien_hit")==0){soundFontsFile=ALIEN_HIT_SOUNDFONT;}
	else if(soundType.compare("barharp")==0){soundFontsFile=BARHARP_SOUNDFONT;}
	else if(soundType.compare("berlin_trumpet")==0){soundFontsFile=BERLIN_TRUMPET_SOUNDFONT;}
	else if(soundType.compare("bjorkestra_hit")==0){soundFontsFile=BJORKESTRA_HIT_SOUNDFONT;}
	else if(soundType.compare("boston_brass")==0){soundFontsFile=BOSTON_BRASS_SOUNDFONT;}
	else if(soundType.compare("bottle_blow")==0){soundFontsFile=BOTTLE_BLOW_SOUNDFONT;}
	else if(soundType.compare("bounce_bow")==0){soundFontsFile=BOUNCE_BOW_SOUNDFONT;}
	else if(soundType.compare("boy_choir")==0){soundFontsFile=BOY_CHOIR_SOUNDFONT;}
	else if(soundType.compare("brass_squirt")==0){soundFontsFile=BRASS_SQUIRT_SOUNDFONT;}
	else if(soundType.compare("brass")==0){soundFontsFile=BRASS_SOUNDFONT;}
	else if(soundType.compare("clarinet")==0){soundFontsFile=CLARINET_SOUNDFONT;}
	else if(soundType.compare("classic_hit")==0){soundFontsFile=CLASSIC_HIT_SOUNDFONT;}
	else if(soundType.compare("crazy_organ")==0){soundFontsFile=CRAZY_ORGAN_SOUNDFONT;}
	else if(soundType.compare("crypt_organ")==0){soundFontsFile=CRYPT_ORGAN_SOUNDFONT;}
	else if(soundType.compare("crystal_rhodes_piano")==0){soundFontsFile=CRYSTAL_RHODES_PIANO_SOUNDFONT;}
	else if(soundType.compare("deep_string")==0){soundFontsFile=DEEP_STRING_SOUNDFONT;}
	else if(soundType.compare("deport_hit")==0){soundFontsFile=DEPORT_HIT_SOUNDFONT;}
	else if(soundType.compare("didgeridoo")==0){soundFontsFile=DIDGERIDOO_SOUNDFONT;}
	else if(soundType.compare("dirty_electric_organ")==0){soundFontsFile=DIRTY_ELECTRIC_ORGAN_SOUNDFONT;}
	else if(soundType.compare("dirty_strings")==0){soundFontsFile=DIRTY_STRINGS_SOUNDFONT;}
	else if(soundType.compare("echo_metal_guitar")==0){soundFontsFile=ECHO_METAL_GUITAR_SOUNDFONT;}
	else if(soundType.compare("endorian_bass")==0){soundFontsFile=ENDORIAN_BASS_SOUNDFONT;}
	else if(soundType.compare("ensemble_violin")==0){soundFontsFile=ENSEMBLE_VIOLIN_SOUNDFONT;}
	else if(soundType.compare("fantastic_strings")==0){soundFontsFile=FANTASTIC_STRINGS_SOUNDFONT;}
	else if(soundType.compare("flugelhorns")==0){soundFontsFile=FLUGELHORNS_SOUNDFONT;}
	else if(soundType.compare("flugelradio")==0){soundFontsFile=FLUGELRADIO_SOUNDFONT;}
	else if(soundType.compare("flute")==0){soundFontsFile=FLUTE_SOUNDFONT;}
	else if(soundType.compare("fret_guitar")==0){soundFontsFile=FRET_GUITAR_SOUNDFONT;}
	else if(soundType.compare("funky_guitar")==0){soundFontsFile=FUNKY_GUITAR_SOUNDFONT;}
	else if(soundType.compare("german8_harpsichord")==0){soundFontsFile=GERMAN8_HARPSICHORD_SOUNDFONT;}
	else if(soundType.compare("glockenspiel")==0){soundFontsFile=GLOCKENSPIEL_SOUNDFONT;}
	else if(soundType.compare("gothic_organ")==0){soundFontsFile=GOTHIC_ORGAN_SOUNDFONT;}
	else if(soundType.compare("grand_piano")==0){soundFontsFile=GRAND_PIANO_SOUNDFONT;}
	//else if(soundType.compare("guitar")==0)
	//	{pitch=calculate_guitar_pitch(freq);
	//	theSound=generate_guitar_sound(noteDuration,pitch,envp,tmpN);}
	else if(soundType.compare("harpsichord")==0){soundFontsFile=HARPSICHORD_SOUNDFONT;}
	else if(soundType.compare("harp")==0){soundFontsFile=HARP_SOUNDFONT;}
	else if(soundType.compare("justified_hit")==0){soundFontsFile=JUSTIFIED_HIT_SOUNDFONT;}
	else if(soundType.compare("jv_1080_bass")==0){soundFontsFile=JV_1080_BASS_SOUNDFONT;}
	else if(soundType.compare("kick_arse_bass")==0){soundFontsFile=KICK_ARSE_BASS_SOUNDFONT;}
	else if(soundType.compare("laid_bass")==0){soundFontsFile=LAID_BASS_SOUNDFONT;}
	else if(soundType.compare("lead_guitars")==0){soundFontsFile=LEAD_GUITARS_SOUNDFONT;}
	else if(soundType.compare("magnificent_organ")==0){soundFontsFile=MAGNIFICENT_ORGAN_SOUNDFONT;}
	else if(soundType.compare("mean_saxophone")==0){soundFontsFile=MEAN_SAXOPHONE_SOUNDFONT;}
	else if(soundType.compare("metallic_bass")==0){soundFontsFile=METALLIC_BASS_SOUNDFONT;}
	else if(soundType.compare("misc_percussion")==0){soundFontsFile=MISCELLANEOUS_PERCUSSION_SOUNDFONT;}
	else if(soundType.compare("monkey_thump")==0){soundFontsFile=MONKEY_THUMP_SOUNDFONT;}
	else if(soundType.compare("mourn_saxophone")==0){soundFontsFile=MOURN_SAXOPHONE_SOUNDFONT;}
	//else if(soundType.compare("miramba")==0){theSound=generate_miramba_sound(freq,envp,tmpN);}
	else if(soundType.compare("multi_guitar")==0){soundFontsFile=MULTI_GUITAR_SOUNDFONT;}
	else if(soundType.compare("neo_organ")==0){soundFontsFile=NEO_ORGAN_SOUNDFONT;}
	else if(soundType.compare("NIN_guitar")==0){soundFontsFile=NIN_GUITAR_SOUNDFONT;}
	else if(soundType.compare("numb_hit")==0){soundFontsFile=NUMB_HIT_SOUNDFONT;}
	else if(soundType.compare("orchestra_all_brass")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_ALL_BRASS_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_ALL_BRASS_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_ALL_BRASS_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_all_strings")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_ALL_STRINGS_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_ALL_STRINGS_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_ALL_STRINGS_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_all_woodwinds")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_ALL_WOODWINDS_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_ALL_WOODWINDS_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_ALL_WOODWINDS_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_alto_flute")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_ALTO_FLUTE_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_ALTO_FLUTE_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_ALTO_FLUTE_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_bass_clarinet")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_BASS_CLARINET_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_BASS_CLARINET_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_BASS_CLARINET_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_bass_trombone")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_BASS_TROMBONE_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_BASS_TROMBONE_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_BASS_TROMBONE_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_bassoon")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_BASSOON_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_BASSOON_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_BASSOON_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_bass")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_BASS_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_BASS_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_BASS_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_cello")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_CELLO_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_CELLO_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_CELLO_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_clarinet")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_CLARINET_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_CLARINET_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_CLARINET_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_contrabassoon")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_CONTRABASSOON_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_CONTRABASSOON_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_CONTRABASSOON_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_english_horn")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_ENGLISH_HORN_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_ENGLISH_HORN_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_ENGLISH_HORN_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_first_violin")==0||soundType.compare("orchestra_1st_violin")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_1ST_VIOLIN_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_1ST_VIOLIN_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_1ST_VIOLIN_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_flute")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_FLUTE_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_FLUTE_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_FLUTE_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_french_horn")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_FRENCH_HORN_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_FRENCH_HORN_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_FRENCH_HORN_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_oboe")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_OBOE_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_OBOE_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_OBOE_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_piccolo")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_PICCOLO_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_PICCOLO_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_PICCOLO_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_second_violin")==0||soundType.compare("orchestra_2nd_violin")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_2ND_VIOLIN_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_2ND_VIOLIN_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_2ND_VIOLIN_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_trombone")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_TROMBONE_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_TROMBONE_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_TROMBONE_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_trumpet")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_TRUMPET_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_TRUMPET_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_TRUMPET_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_tuba")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_TUBA_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_TUBA_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_TUBA_SOUNDFONT;}
		}
	else if(soundType.compare("orchestra_viola")==0)
		{if(otherMarks.length()==0){soundFontsFile=REGULAR_VIOLA_SOUNDFONT;}
		else if(otherMarks.compare("S")==0){soundFontsFile=SUSTAINED_VIOLA_SOUNDFONT;}
		else if(otherMarks.compare("X")==0){soundFontsFile=STACCATO_VIOLA_SOUNDFONT;}
		}
	else if(soundType.compare("pagan_whistle")==0){soundFontsFile=PAGAN_WHISTLE_SOUNDFONT;}
	else if(soundType.compare("phantom_hit")==0){soundFontsFile=PHANTOM_HIT_SOUNDFONT;}
	else if(soundType.compare("phantom_organ")==0){soundFontsFile=PHANTOM_ORGAN_SOUNDFONT;}
	else if(soundType.compare("piano_set")==0){soundFontsFile=PIANO_SET_SOUNDFONT;}
	//else if(soundType.compare("piano")==0){theSound=generate_piano_sound(noteDuration,freq,envp,tmpN);}
	//else if(soundType.compare("pluck")==0||soundType.compare("pluck1")==0){noteVolume=1.0; theSound=generate_pluck_sound(noteVolume,freq,envp,tmpN);}
	//else if(soundType.compare("pluck2")==0){noteVolume=1.0; theSound=generate_pluck2_sound(noteVolume,freq,envp,tmpN);}
	//else if(soundType.compare("plucked_bass")==0){noteVolume=200; pluckDuration=0.5; pitch=calculate_plucked_bass_pitch(freq); theSound=generate_plucked_bass_sound(noteVolume,pitch,pluckDuration,noteDuration,envp,tmpN);}
	//else if(soundType.compare("plucked_string")==0){noteVolume=200; pluckDuration=0.5; pitch=calculate_plucked_string_pitch(freq); theSound=generate_plucked_string_sound(noteVolume,pitch,noteDuration,envp,tmpN);}
	else if(soundType.compare("quick_hit")==0){soundFontsFile=QUICK_HIT_SOUNDFONT;}
	else if(soundType.compare("robotech_hit")==0){soundFontsFile=ROBOTECH_HIT_SOUNDFONT;}
	else if(soundType.compare("rock_guitar")==0){soundFontsFile=ROCK_GUITAR_SOUNDFONT;}
	else if(soundType.compare("rock_hit")==0){soundFontsFile=ROCK_HIT_SOUNDFONT;}
	else if(soundType.compare("rock_organ")==0){soundFontsFile=ROCK_ORGAN_SOUNDFONT;}
	else if(soundType.compare("roland_bass2")==0){soundFontsFile=ROLAND_BASS2_SOUNDFONT;}
	else if(soundType.compare("roland_bass1")==0||soundType.compare("roland_bass")==0){soundFontsFile=ROLAND_BASS1_SOUNDFONT;}
	else if(soundType.compare("roland_dulcimer")==0){soundFontsFile=ROLAND_DULCIMER_SOUNDFONT;}
	else if(soundType.compare("roland_piano")==0){soundFontsFile=ROLAND_PIANO_SOUNDFONT;}
	else if(soundType.compare("roland_strings")==0){soundFontsFile=ROLAND_STRINGS_SOUNDFONT;}
	else if(soundType.compare("roland_violin")==0){soundFontsFile=ROLAND_VIOLIN_SOUNDFONT;}
	else if(soundType.compare("seagull_acoustic_guitar")==0){soundFontsFile=SEAGULL_ACOUSTIC_GUITAR_SOUNDFONT;}
	else if(soundType.compare("seinfeld_bass")==0){soundFontsFile=SEINFELD_BASS_SOUNDFONT;}
	else if(soundType.compare("session_guitar")==0){soundFontsFile=SESSION_GUITAR_SOUNDFONT;}
	else if(soundType.compare("simple_drums")==0){soundFontsFile=SIMPLE_DRUMS_SOUNDFONT;}
	else if(soundType.compare("sitar")==0){soundFontsFile=SITAR_SOUNDFONT;}
	else if(soundType.compare("sleeptalk_hit")==0){soundFontsFile=SLEEPTALK_HIT_SOUNDFONT;}
	else if(soundType.compare("soft_horn")==0){soundFontsFile=SOFT_HORN_SOUNDFONT;}
	else if(soundType.compare("solid_bass")==0){soundFontsFile=SOLID_BASS_SOUNDFONT;}
	else if(soundType.compare("stage_guitar")==0){soundFontsFile=STAGE_GUITAR_SOUNDFONT;}
	else if(soundType.compare("star_wars_hit")==0){soundFontsFile=STAR_WARS_HIT_SOUNDFONT;}
	else if(soundType.compare("steel_guitar")==0){soundFontsFile=STEEL_GUITAR_SOUNDFONT;}
	else if(soundType.compare("steinway_grand_piano")==0){soundFontsFile=STEINWAY_GRAND_PIANO_SOUNDFONT;}
	else if(soundType.compare("straight_strings")==0){soundFontsFile=STRAIGHT_STRINGS_SOUNDFONT;}
	else if(soundType.compare("strat_marshall")==0){soundFontsFile=STRAT_MARSHALL_SOUNDFONT;}
	//else if(soundType.compare("strings")==0||soundType.compare("strings1")==0){noteVolume=1.0; theSound=generate_strings_sound(noteVolume,freq,envp,tmpN);}
	//else if(soundType.compare("strings2")==0){noteVolume=1.0; theSound=generate_strings2_sound(noteVolume,freq,envp,tmpN);}
	else if(soundType.compare("strings_with_piano")==0){soundFontsFile=STRINGS_WITH_PIANO_SOUNDFONT;}
	else if(soundType.compare("stud_bass")==0){soundFontsFile=STUD_BASS_SOUNDFONT;}
	else if(soundType.compare("subpiano")==0){soundFontsFile=SUBPIANO_SOUNDFONT;}
	else if(soundType.compare("t_hit")==0){soundFontsFile=T_HIT_SOUNDFONT;}
	else if(soundType.compare("timpani")==0){soundFontsFile=TIMPANI_SOUNDFONT;}
	else if(soundType.compare("tired_accordion")==0){soundFontsFile=TIRED_ACCORDION_SOUNDFONT;}
	//else if(soundType.compare("trumpet")==0){theSound=generate_trumpet_sound(noteDuration,pitch,envp,tmpN);}
	else if(soundType.compare("trumpets")==0){soundFontsFile=TRUMPETS_SOUNDFONT;}
	else if(soundType.compare("tubular_bells")==0){soundFontsFile=TUBULAR_BELLS_SOUNDFONT;}
	else if(soundType.compare("twilight_zone_hit")==0){soundFontsFile=TWILIGHT_ZONE_HIT_SOUNDFONT;}
	else if(soundType.compare("U20_piano")==0){soundFontsFile=U20_PIANO_SOUNDFONT;}
	else if(soundType.compare("velocity_grand_piano")==0){soundFontsFile=VELOCITY_GRAND_PIANO_SOUNDFONT;}
	else if(soundType.compare("vibraphone")==0){soundFontsFile=VIBRAPHONE_SOUNDFONT;}
	else if(soundType.compare("violin_pizzicato")==0){soundFontsFile=VIOLIN_PIZZICATO_SOUNDFONT;}
	else if(soundType.compare("violin")==0){soundFontsFile=VIOLIN_SOUNDFONT;}
	else if(soundType.compare("whistle")==0){soundFontsFile=WHISTLE_SOUNDFONT;}
	else if(soundType.compare("worship_choir")==0){soundFontsFile=WORSHIP_CHOIR_SOUNDFONT;}
	else if(soundType.compare("xylophone")==0){soundFontsFile=XYLOPHONE_SOUNDFONT;}
	return soundFontsFile;}

double* generate_espeak_sound(string soundType,double desiredFrequency,double noteDuration,string lyric,string pitch,string amplitude,char** envp,int& outputArrSz)
	{double wordRate=1.0/noteDuration;				// words/sec
	string wordSpeed=formatNumberString(cnvrtNumToStrng(wordRate*60,SIG_FIGS));	// words/min
	//string pitch="50";
	//string amplitude="100";
	string tmpWaveFile=get_current_dir_name(); tmpWaveFile+="/WAV/"; tmpWaveFile+=soundType+"_"+lyric+"_p"+pitch+"_a"+amplitude+"_s"+wordSpeed+".wav";
	// Define Output Wave File
	string outputWaveFile=extract_containing_folder(tmpWaveFile); outputWaveFile+=soundType+"_"+lyric+"_p"+pitch+"_a"+amplitude+"_s"+wordSpeed+"_f"+formatNumberString(cnvrtNumToStrng(desiredFrequency,SIG_FIGS))+".wav";
	double Max,*x,changeInFrequency;
	soundSignal inS;
	int index,N;
	if(!file_exist(outputWaveFile))
		{// Run espeak to create it
		run_espeak(lyric,amplitude,pitch,wordSpeed,soundType,tmpWaveFile,envp);
		inS=read_amplitudes_in_wave_file(tmpWaveFile);
		x=new double[inS.N];
		for(int i=0;i<inS.N;i++){x[i]=(double)inS.signal[i];}	
		//Max=determine_maximum_frequency_peak(x,inS.N,index);	//cout<<"Max Freq: "<<Max<<endl;
		//Max=determine_last_maximum_frequency_peak(x,inS.N,index);	//cout<<"Max Freq: "<<Max<<endl;
		Max=determine_average_maximum_frequency_peak(x,inS.N,index);	//cout<<"Max Freq: "<<Max<<endl;
		// Estimate Out-Of-Tuneness
		changeInFrequency=desiredFrequency-Max;
		// Tune Sound
		frequency_shift_signal_1_channel_input(tmpWaveFile,changeInFrequency,noteDuration,envp,outputWaveFile);
		}

	double *Output=read_amplitudes_in_wave_file(outputWaveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_soundFont_sound(string soundType,double noteDuration,double pitch,string otherMarks,char** envp,int& outputArrSz)
	{string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/"+soundType+".sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/"+soundType+".orc";
	//
	string waveFile=get_current_dir_name(); waveFile+="/WAV/"+soundType+"_";
	waveFile+=formatNumberString(cnvrtNumToStrng(pitch,0))+"_";
	waveFile+=formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS))+".wav";	
	string soundFontsFile=get_soundFont_File_from_soundType(soundType,otherMarks);
	//cout<<soundFontsFile<<"\n";
	double *theSound;
	int tmpN;
	int status,N,rChk;
	ifstream fIn;
	ofstream fOut;
	if(!file_exist(waveFile))
		{// Write .wav file with csound
		fIn.open(waveFile.c_str(),ifstream::in);
		if(!fIn.fail()){fIn.close(); rChk=remove(waveFile.c_str());}
		// Write Score File
		fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut.fail()){cerr<<"ERROR in generate_soundFont_sound\nOutput score file file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
		fOut<<write_score_for_soundFonts(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)),formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)),"100");
		fOut.close();
		//cout<<scoreFile<<"\n";
		// Write Orchestra File
		fOut.open(orchestraFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut.fail()){cerr<<"ERROR in generate_soundFont_sound\nOutput orchestra file could not be opened.\n"<<orchestraFile<<endl; exit(EXIT_FAILURE);}
		fOut<<write_orchestra_for_soundFonts(soundFontsFile);
		fOut.close();
		//cout<<orchestraFile<<endl;
		// Generate wave file with csound
		run_csound(orchestraFile,scoreFile,waveFile,envp);
		}
	
	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_soundFont_sound(string soundType,string filterCommand,double noteDuration,double pitch,string otherMarks,char** envp,int& outputArrSz)
	{// Handle No Filter Scenario
	if(filterCommand.length()==0){return generate_soundFont_sound(soundType,noteDuration,pitch,otherMarks,envp,outputArrSz);}
	
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/"+soundType+".sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/"+soundType+".orc";
	//
	string waveFile=get_current_dir_name(); waveFile+="/WAV/"+soundType+"_";
	waveFile+=formatNumberString(cnvrtNumToStrng(pitch,0))+"_";
	waveFile+=formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS))+"_";
	waveFile+="filtered.wav";	
	string soundFontsFile=get_soundFont_File_from_soundType(soundType,otherMarks);
	double *theSound;
	int tmpN;
	int status,N,rChk;
	ifstream fIn;
	ofstream fOut;
	if(!file_exist(waveFile))
		{// Write .wav file with csound
		fIn.open(waveFile.c_str(),ifstream::in);
		if(!fIn.fail()){fIn.close(); rChk=remove(waveFile.c_str());}		
		// Write Score File
		fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut.fail()){cerr<<"ERROR in generate_soundFont_sound\nOutput score file file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
		fOut<<write_score_for_soundFonts(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)),formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)),"100");
		fOut.close();
		// Write Orchestra File
		fOut.open(orchestraFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut.fail()){cerr<<"ERROR in generate_soundFont_sound\nOutput orchestra file could not be opened.\n"<<orchestraFile<<endl; exit(EXIT_FAILURE);}
		fOut<<write_orchestra_for_soundFonts(soundFontsFile,filterCommand);
		fOut.close();	
		// Generate wave file with csound
		run_csound(orchestraFile,scoreFile,waveFile,envp);
		}
	
	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_sound(string soundType,string filterCommand,double noteDuration,double freq,double timePerMeasure,string note,int octaveLevel,string otherMarks,char **envp,int &tmpN)
	{double pitch,noteVolume,staccatoDuration,pluckDuration;
	double* theSound;
	pitch=calculate_soundFonts_pitch(note,octaveLevel);
	if(soundType.compare("accordion_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("acoustic_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("aeon_flux_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("aggressive_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("alien_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("barharp")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("berlin_trumpet")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("bjorkestra_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("boston_brass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("bottle_blow")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("bounce_bow")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("boy_choir")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("brass_squirt")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("brass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("clarinet")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("classic_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("crazy_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("crypt_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("crystal_rhodes_piano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("deep_string")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("deport_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("didgeridoo")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("dirty_electric_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("dirty_strings")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("echo_metal_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("endorian_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("ensemble_violin")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("fantastic_strings")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("flugelhorns")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("flugelradio")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("flute")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("fret_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("funky_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("german8_harpsichord")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("glockenspiel")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("gothic_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("grand_piano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("guitar")==0)
		{pitch=calculate_guitar_pitch(freq);
		theSound=generate_guitar_sound(noteDuration,pitch,envp,tmpN);}
	else if(soundType.compare("harpsichord")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("harp")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("justified_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("jv_1080_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("kick_arse_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("laid_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("lead_guitars")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("magnificent_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("mean_saxophone")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("metallic_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("monkey_thump")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("mourn_saxophone")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("miramba")==0)
		{theSound=generate_miramba_sound(freq,envp,tmpN);}
	else if(soundType.compare("multi_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("neo_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("NIN_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("numb_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_all_brass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_all_strings")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_all_woodwinds")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_alto_flute")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_bass_clarinet")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_bass_trombone")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_bassoon")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_cello")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_clarinet")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_contrabassoon")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_english_horn")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_first_violin")==0||\
			soundType.compare("orchestra_1st_violin")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_flute")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_french_horn")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_oboe")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_piccolo")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_second_violin")==0||\
			soundType.compare("orchestra_2nd_violin")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_trombone")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_trumpet")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_tuba")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("orchestra_viola")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("pagan_whistle")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("phantom_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("phantom_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("piano_set")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("piano")==0)
		{theSound=generate_piano_sound(noteDuration,freq,envp,tmpN);}
	else if(soundType.compare("pluck")==0||soundType.compare("pluck1")==0)
		{noteVolume=1.0;
		theSound=generate_pluck_sound(noteVolume,freq,envp,tmpN);}
	else if(soundType.compare("pluck2")==0)
		{noteVolume=1.0;
		theSound=generate_pluck2_sound(noteVolume,freq,envp,tmpN);}
	else if(soundType.compare("plucked_bass")==0)
		{noteVolume=200;
		pluckDuration=0.5;
		pitch=calculate_plucked_bass_pitch(freq);
		theSound=generate_plucked_bass_sound(noteVolume,pitch,pluckDuration,noteDuration,envp,tmpN);}
	else if(soundType.compare("plucked_string")==0)
		{noteVolume=200;
		pluckDuration=0.5;
		pitch=calculate_plucked_string_pitch(freq);
		theSound=generate_plucked_string_sound(noteVolume,pitch,noteDuration,envp,tmpN);}
	else if(soundType.compare("quick_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("robotech_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("rock_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("rock_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("rock_organ")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("roland_bass2")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("roland_bass1")==0||\
			soundType.compare("roland_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("roland_dulcimer")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("roland_piano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("roland_strings")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("roland_violin")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("seagull_acoustic_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("seinfeld_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("session_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("sitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("sleeptalk_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("soft_horn")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("solid_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("stage_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("star_wars_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("steel_guitar")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("steinway_grand_piano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("straight_strings")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("strat_marshall")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("strings")==0||soundType.compare("strings1")==0)
		{noteVolume=1.0;
		theSound=generate_strings_sound(noteVolume,freq,envp,tmpN);}
	else if(soundType.compare("strings2")==0)
		{noteVolume=1.0;
		theSound=generate_strings2_sound(noteVolume,freq,envp,tmpN);}
	else if(soundType.compare("strings_with_piano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("stud_bass")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("subpiano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("t_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("timpani")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("tired_accordion")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("trumpets")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("trumpet")==0){theSound=generate_trumpet_sound(noteDuration,pitch,envp,tmpN);}
	else if(soundType.compare("tubular_bells")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("twilight_zone_hit")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("U20_piano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("velocity_grand_piano")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("vibraphone")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("violin_pizzicato")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("violin")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("whistle")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("worship_choir")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else if(soundType.compare("xylophone")==0){theSound=generate_soundFont_sound(soundType,filterCommand,noteDuration,pitch,otherMarks,envp,tmpN);}
	else{cerr<<"Error in generate_sound!\nUnrecognized Sound Type ("<<soundType<<")\n"; exit(EXIT_FAILURE);}
	return theSound;}

double* generate_strings_sound(double noteVolume,double pitch,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/strings.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/strings.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="STRINGS.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_strings_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_strings_score(formatNumberString(cnvrtNumToStrng(interpolate(noteVolume,0,200,0,2),SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_strings2_sound(double noteVolume,double pitch,char** envp,int& outputArrSz)
	{// Input: noteVolume is from 0 to 200
	// Input: pitch related to frequency
	// Input: pluckDuration (seconds) 0.25
	// Input: noteDuration
	double fs=SAMPLE_RATE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/strings.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/strings.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="STRINGS.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_strings2_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_strings2_score(formatNumberString(cnvrtNumToStrng(interpolate(noteVolume,0,200,0,2),SIG_FIGS)),\
		formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	run_csound(orchestraFile,scoreFile,waveFile,envp);
	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_tambourine_sound(double noteDuration,char** envp,int& outputArrSz)
	{// Input: noteDuration is in seconds
	double fs=SAMPLE_RATE;
	string csound_executable=CSOUND_EXE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/tambourine.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/tambourine.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="STRINGS.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_tambourine_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_tambourine_score(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)));
	fOut.close();
	
	// Generate wave file with csound
	int PID=fork(),status2,result;
	string logFile2=get_current_dir_name(); logFile2+="/log_stderr";
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		char* argv[]={csound_executable.c_str(),"-f","--noheader","--nchnls=1","-W","-T","-o",waveFile.c_str(),orchestraFile.c_str(),scoreFile.c_str(),NULL};
		freopen(logFile2.c_str(),"w",stderr);
		result=syscall(SYS_execve,csound_executable.c_str(),argv,envp);						
		if(result<0)
			{cerr<<"ERROR!\n";
			exit(EXIT_FAILURE);}
		}
	else if(PID>0){waitpid(PID,&status2,0);}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double* generate_trumpet_sound(double noteDuration,double pitch,char** envp,int& outputArrSz)
	{// Input: noteDuration is in seconds
	double fs=SAMPLE_RATE;
	string csound_executable=CSOUND_EXE;
	string scoreFile=get_current_dir_name(); scoreFile+="/CSOUND/trumpet.sco";
	string orchestraFile=get_current_dir_name(); orchestraFile+="/CSOUND/trumpet.orc";
	string waveFile=get_current_dir_name(); waveFile+="/WAV/";
	waveFile+="TRUMPET.wav";

	int status,N,rChk;
	ifstream fIn;
	fIn.open(waveFile.c_str(),ifstream::in);
	if(!fIn.fail())
		{//cout<<"Wave File to generate already exists, so skipping csound\n"<<waveFile<<endl;
		fIn.close();
		rChk=remove(waveFile.c_str());
		}

	ofstream fOut;
	fOut.open(scoreFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in generate_trumpet_sound\nInput music file could not be opened.\n"<<scoreFile<<endl; exit(EXIT_FAILURE);}
	fOut<<write_score_for_soundFonts(formatNumberString(cnvrtNumToStrng(noteDuration,SIG_FIGS)),formatNumberString(cnvrtNumToStrng(pitch,SIG_FIGS)),"100");
	fOut.close();
	
	// Generate wave file with csound
	int PID=fork(),status2,result;
	string logFile2=get_current_dir_name(); logFile2+="/log_stderr";
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		char* argv[]={csound_executable.c_str(),"-f","--noheader","--nchnls=1","-W","-T","-o",waveFile.c_str(),orchestraFile.c_str(),scoreFile.c_str(),NULL};
		freopen(logFile2.c_str(),"w",stderr);
		result=syscall(SYS_execve,csound_executable.c_str(),argv,envp);						
		if(result<0)
			{cerr<<"ERROR!\n";
			exit(EXIT_FAILURE);}
		}
	else if(PID>0){waitpid(PID,&status2,0);}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}

	double *Output=read_amplitudes_in_wave_file(waveFile,N);		//for(int i=0;i<N;i++){cout<<i<<" | "<<Output[i]<<endl;}
	outputArrSz=N;
	return Output;}

double get_A_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=A0_FREQ; break;
		case 1: Output=A1_FREQ; break;
		case 2: Output=A2_FREQ; break;
		case 3: Output=A3_FREQ; break;
		case 4: Output=A4_FREQ; break;
		case 5: Output=A5_FREQ; break;
		case 6: Output=A6_FREQ; break;
		case 7: Output=A7_FREQ; break;
		case 8: Output=A8_FREQ; break;
		default: cerr<<"Error in get_A_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_As_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=A0s_FREQ; break;
		case 1: Output=A1s_FREQ; break;
		case 2: Output=A2s_FREQ; break;
		case 3: Output=A3s_FREQ; break;
		case 4: Output=A4s_FREQ; break;
		case 5: Output=A5s_FREQ; break;
		case 6: Output=A6s_FREQ; break;
		case 7: Output=A7s_FREQ; break;
		case 8: Output=A8s_FREQ; break;
		default: cerr<<"Error in get_As_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_B_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=B0_FREQ; break;
		case 1: Output=B1_FREQ; break;
		case 2: Output=B2_FREQ; break;
		case 3: Output=B3_FREQ; break;
		case 4: Output=B4_FREQ; break;
		case 5: Output=B5_FREQ; break;
		case 6: Output=B6_FREQ; break;
		case 7: Output=B7_FREQ; break;
		case 8: Output=B8_FREQ; break;
		default: cerr<<"Error in get_B_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_C_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=C0_FREQ; break;
		case 1: Output=C1_FREQ; break;
		case 2: Output=C2_FREQ; break;
		case 3: Output=C3_FREQ; break;
		case 4: Output=C4_FREQ; break;
		case 5: Output=C5_FREQ; break;
		case 6: Output=C6_FREQ; break;
		case 7: Output=C7_FREQ; break;
		case 8: Output=C8_FREQ; break;
		default: cerr<<"Error in get_C_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_Cs_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=C0s_FREQ; break;
		case 1: Output=C1s_FREQ; break;
		case 2: Output=C2s_FREQ; break;
		case 3: Output=C3s_FREQ; break;
		case 4: Output=C4s_FREQ; break;
		case 5: Output=C5s_FREQ; break;
		case 6: Output=C6s_FREQ; break;
		case 7: Output=C7s_FREQ; break;
		case 8: Output=C8s_FREQ; break;
		default: cerr<<"Error in get_Cs_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_D_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=D0_FREQ; break;
		case 1: Output=D1_FREQ; break;
		case 2: Output=D2_FREQ; break;
		case 3: Output=D3_FREQ; break;
		case 4: Output=D4_FREQ; break;
		case 5: Output=D5_FREQ; break;
		case 6: Output=D6_FREQ; break;
		case 7: Output=D7_FREQ; break;
		case 8: Output=D8_FREQ; break;
		default: cerr<<"Error in get_D_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_Ds_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=D0s_FREQ; break;
		case 1: Output=D1s_FREQ; break;
		case 2: Output=D2s_FREQ; break;
		case 3: Output=D3s_FREQ; break;
		case 4: Output=D4s_FREQ; break;
		case 5: Output=D5s_FREQ; break;
		case 6: Output=D6s_FREQ; break;
		case 7: Output=D7s_FREQ; break;
		case 8: Output=D8s_FREQ; break;
		default: cerr<<"Error in get_Ds_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_E_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=E0_FREQ; break;
		case 1: Output=E1_FREQ; break;
		case 2: Output=E2_FREQ; break;
		case 3: Output=E3_FREQ; break;
		case 4: Output=E4_FREQ; break;
		case 5: Output=E5_FREQ; break;
		case 6: Output=E6_FREQ; break;
		case 7: Output=E7_FREQ; break;
		case 8: Output=E8_FREQ; break;
		default: cerr<<"Error in get_E_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_F_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=F0_FREQ; break;
		case 1: Output=F1_FREQ; break;
		case 2: Output=F2_FREQ; break;
		case 3: Output=F3_FREQ; break;
		case 4: Output=F4_FREQ; break;
		case 5: Output=F5_FREQ; break;
		case 6: Output=F6_FREQ; break;
		case 7: Output=F7_FREQ; break;
		case 8: Output=F8_FREQ; break;
		default: cerr<<"Error in get_F_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_Fs_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=F0s_FREQ; break;
		case 1: Output=F1s_FREQ; break;
		case 2: Output=F2s_FREQ; break;
		case 3: Output=F3s_FREQ; break;
		case 4: Output=F4s_FREQ; break;
		case 5: Output=F5s_FREQ; break;
		case 6: Output=F6s_FREQ; break;
		case 7: Output=F7s_FREQ; break;
		case 8: Output=F8s_FREQ; break;
		default: cerr<<"Error in get_Fs_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_G_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=G0_FREQ; break;
		case 1: Output=G1_FREQ; break;
		case 2: Output=G2_FREQ; break;
		case 3: Output=G3_FREQ; break;
		case 4: Output=G4_FREQ; break;
		case 5: Output=G5_FREQ; break;
		case 6: Output=G6_FREQ; break;
		case 7: Output=G7_FREQ; break;
		case 8: Output=G8_FREQ; break;
		default: cerr<<"Error in get_G_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

double get_Gs_frequency_from_octave(int octaveLevel)
	{double Output;
	switch(octaveLevel)
		{case 0: Output=G0s_FREQ; break;
		case 1: Output=G1s_FREQ; break;
		case 2: Output=G2s_FREQ; break;
		case 3: Output=G3s_FREQ; break;
		case 4: Output=G4s_FREQ; break;
		case 5: Output=G5s_FREQ; break;
		case 6: Output=G6s_FREQ; break;
		case 7: Output=G7s_FREQ; break;
		case 8: Output=G8s_FREQ; break;
		default: cerr<<"Error in get_Gs_frequency_from_octave!\nOctave Level ("<<octaveLevel<<") is outside working range.\n"; exit(EXIT_FAILURE); break;
		}
	return Output;}

// see musicConstants.h
//
double getFrequencyOfNote(string note,int octaveLevel)
	{// Input:
	// note is a capital letter follwed by s (for sharp), f (for flat) or nothin (for natural)
	// octaveLevel specifies how many octaves above the lowest level (lowest = 1), the note is
	//
	// Output:
	// frequency (Hz) of desired note

	// Check For Error: OctaveLevel contained in noteLetter	
	int numNumbers=9,pos;
	string *theNumbers=new string[numNumbers];
	for(int i=0;i<numNumbers;i++){theNumbers[i]=cnvrtNumToStrng(i,0);}
	for(int i=0;i<numNumbers;i++)
		{pos=note.find(theNumbers[i],0);
		if(pos!=string::npos)
			{// Octave Found, Error Avoided
			octaveLevel=atoi(theNumbers[i].c_str());
			note=note.substr(0,pos);
			break;}
		}
	// Check For Error: OtherMarks contained in octaveLevel
	string tmp=cnvrtNumToStrng(octaveLevel,0);
	if(tmp.length()>=2)
		{for(int i=0;i<numNumbers;i++)
			{pos=tmp.find(theNumbers[i],0);
			if(pos!=string::npos)
				{// Octave Found
				octaveLevel=atoi(theNumbers[i].c_str());
				break;}
			}
		}

	double Output;
	if(note.compare("As")==0|| note.compare("Bf")==0)
		{// A# Sharp / Bb Flat
		Output=get_As_frequency_from_octave(octaveLevel);}
	else if(note.compare("A")==0)
		{// A Natural
		Output=get_A_frequency_from_octave(octaveLevel);}	
	else if(note.compare("B")==0||note.compare("Cf")==0)
		{// B Natural / C Flat
		Output=get_B_frequency_from_octave(octaveLevel);}
	else if(note.compare("Bs")==0||note.compare("C")==0)
		{// B# Sharp / C Natural
		Output=get_C_frequency_from_octave(octaveLevel);}
	else if(note.compare("Cs")==0||note.compare("Df")==0)
		{// C# Sharp / Db Flat
		Output=get_Cs_frequency_from_octave(octaveLevel);}
	else if(note.compare("D")==0)
		{// D Natural
		Output=get_D_frequency_from_octave(octaveLevel);}
	else if(note.compare("Ds")==0||note.compare("Ef")==0)
		{// D# Sharp / Eb Flat
		Output=get_Ds_frequency_from_octave(octaveLevel);}
	else if(note.compare("E")==0||note.compare("Ff")==0)
		{// E Natural / Fb Flat
		Output=get_E_frequency_from_octave(octaveLevel);}
	else if(note.compare("F")==0||note.compare("Es")==0)
		{// F Natural / E# Sharp
		Output=get_F_frequency_from_octave(octaveLevel);}
	else if(note.compare("Fs")==0||note.compare("Gf")==0)
		{// F# Sharp / Gb Flat
		Output=get_Fs_frequency_from_octave(octaveLevel);}
	else if(note.compare("G")==0)
		{// G Natural
		Output=get_G_frequency_from_octave(octaveLevel);}
	else if(note.compare("Gs")==0||note.compare("Af")==0)
		{// G# Sharp / Ab Flat
		Output=get_Gs_frequency_from_octave(octaveLevel);}
	else{cerr<<"Error in getFrequencyOfNote!!!\nUn-recognized note ("<<note<<") (octave: "<<octaveLevel<<")\n"; exit(EXIT_FAILURE);}

	//double exponent=octaveLevel-1.0;
	//Output=rootFrequency*pow(2.0,exponent);
	return Output;}

FILE * spawn_gnuplot()
{
  int pipefd[2];
  int r = pipe(pipefd);
  assert(!r);
  int cpid = fork();
  if (cpid == 0){ // child
    close(pipefd[1]); //write end
    dup2(pipefd[0], 0);
    execlp("gnuplot", "gnuplot", "-persist", NULL);
	//execlp("gnuplot", "gnuplot", NULL);
	//system("gnuplot -persist &");
    //execlp("cat", "cat", NULL);
    assert(false);
  }
  close(pipefd[0]); //read end
  FILE * f = fdopen(pipefd[1], "w");
  return f;
}

void plot_signal(string plotOptions,string plotFile,double* x,double* y,int Sz)
	{string header="# ";
	int pos=plotOptions.find("set xlabel",0);
	if(pos!=string::npos)
		{pos=plotOptions.find("\"",pos);
		header+="X "+plotOptions.substr(pos+1,plotOptions.find("\"",pos+1)-pos-1)+" ";
		}
	pos=plotOptions.find("set ylabel",0);
	if(pos!=string::npos)
		{pos=plotOptions.find("\"",pos);
		header+="Y "+plotOptions.substr(pos+1,plotOptions.find("\"",pos+1)-pos-1);
		}
	
	FILE *fOut;
	// Write Signal to File
	fOut=fopen(plotFile.c_str(),"w");
	fprintf(fOut,"%s\n",header.c_str());
	//for(int i=0;i<Sz;i++){fprintf(fOut,"%0.11f\t%0.11f\n",(double)i/fs,x[i]);}
	for(int i=0;i<Sz;i++){fprintf(fOut,"%0.12f\t%0.12f\n",x[i],y[i]);}
	//fOut.close();
	fclose(fOut);
	// Write FFT of Signal to File
	//fOut=fopen(fftFile.c_str(),"w");
	//header="# X Frequency\tY Signal Amplitude";
	//fprintf(fOut,"%s\n",header.c_str());
	//double incrmnt=(fs-0)/(N-1);
	//for(int i=0;i<N/2;i++){fprintf(fOut,"%0.11f\t%0.11f\n",(double)i*incrmnt,X[i][0]);}
	//fclose(fOut);

	// Plotting
	/*string title="set title \"Original Signal\"";
	string xLabel="set xlabel \"Time (seconds)\"";
	string yLabel="set ylabel \"Signal Amplitude\"";
	string xRange="set xrange [0:5.5]";
	string yRange="set yrange [-2:2]";*/
	string plotCommand="plot \'"+plotFile+"\' using 1:2";
	// Plot Signal
	FILE *f=spawn_gnuplot();
	//fprintf(f,"%s\n","set style data points");
	fprintf(f,"%s\n","set style data lines");
	fprintf(f,"%s\n",plotOptions.c_str());
	fprintf(f,"%s\n",plotCommand.c_str());
	fclose(f);
	}

void plot_signal(string plotOptions,string plotFile,double* x,SAMPLE* y,int Sz)
	{string header="# ";
	int pos=plotOptions.find("set xlabel",0);
	if(pos!=string::npos)
		{pos=plotOptions.find("\"",pos);
		header+="X "+plotOptions.substr(pos+1,plotOptions.find("\"",pos+1)-pos-1)+" ";
		}
	pos=plotOptions.find("set ylabel",0);
	if(pos!=string::npos)
		{pos=plotOptions.find("\"",pos);
		header+="Y "+plotOptions.substr(pos+1,plotOptions.find("\"",pos+1)-pos-1);
		}
	
	FILE *fOut;
	// Write Signal to File
	fOut=fopen(plotFile.c_str(),"w");
	fprintf(fOut,"%s\n",header.c_str());
	for(int i=0;i<Sz;i++){fprintf(fOut,"%0.12f\t%0.12f\n",x[i],y[i]);}
	fclose(fOut);

	string plotCommand="plot \'"+plotFile+"\' using 1:2";
	// Plot Signal
	FILE *f=spawn_gnuplot();
	//fprintf(f,"%s\n","set style data points");
	fprintf(f,"%s\n","set style data lines");
	fprintf(f,"%s\n",plotOptions.c_str());
	fprintf(f,"%s\n",plotCommand.c_str());
	fclose(f);
	}

soundSignal read_drums_input_file(string inFile,string soundType,double timePerMeasure,char **envp)
	{soundSignal S;
	S.soundType=soundType;
	// Song Time Length = numMeasures * beatsPerMeasure * timePerBeat
	// Total Song Array Size = numMeasures * beatsPerMeasure * timePerBeat * fs
	// Note File
	//string noteFile=extract_containing_folder(inFile)+"notes";
	// Label File
	string labelFile=extract_containing_folder(inFile)+"labels";
	double fs=SAMPLE_RATE, f;	// Hertz, seconds, seconds
	int N;//, numDrumTypes=23;

	string *noteLst, *durationLst, *ampLst;
	int numUsedDrums;
	int Sz=LINE_SIZE, pos, pos2, freqIndex, freqsInMeasure, drumIndex, drumsInMeasure;
	char Val[Sz];
	string lst, tmp,inputValue,topFreq, topDrum, bottomFreq, nextFreq, nextDrum, freqLst="",drumLst="",delimiter=GLOBAL_DELIMITER;
	string* measure;
	// Read Header, Should Contain at Minimum: Sound Type & Tempo
	bool DRUMS=false, TEMPO_FOUND=false, SOUND_TYPE_FOUND=false, COLLECTING_MEASURE, SEARCHING, READING_MEASURE, NOTE_SHARP, NOTE_FLAT, NOTE_DOUBLE_SHARP, NOTE_DOUBLE_FLAT;
	// Time per measure as been specified so this signal can already be defined discretely with respect to time knowing the sampling frequency, which is 44,100 Hz
	int firstNotePos, nextNotePos, notePos, measureSize=timePerMeasure*fs, *usedDrumIndex, aPos;		// Array size of Measure's Signal
	string* frequenciesInMeasure, *theDrumsInMeasure, letter, octave;
	double* timeOfMeasure=new double[measureSize];
	double noteDurationTime, fraction, val1, val2;
	string drumTypeValue;
	bool FIRST_MEASURE=true;

	collect_measures_from_drums_input_file(inFile,timePerMeasure,noteLst,durationLst,ampLst,numUsedDrums);

	int numNotes, index=0, Counter=0, tmpN, songArraySize, noteSize, theIndex, tmpPos;
	string *noteDuration, *drumNote, *noteAmplitude, tmpLabel, *labelsOverTime;
	//int *drumSize=new int[numUsedDrums];		
	//string **allLabelsOverTime=new string*[numUsedDrums];
	string *notes=new string[1];
	double *duration=new double[1];
	double theValue, smallest, totalSongTime=0, value, *tmpDrumArr, maxValue, ampVal;
	FIRST_MEASURE=true;
	for(int i=0;i<numUsedDrums;i++)
		{numNotes=count_delimiter(noteLst[i],delimiter);
		//cout<<numNotes<<"\n"<<noteLst<<"\n"<<durationLst<<endl; exit(1);
		if(count_delimiter(durationLst[i],delimiter)!=numNotes){cerr<<"Error in read_drums_input_file!\nNotes and Durations do not match!\n"; exit(EXIT_FAILURE);}
		noteAmplitude=fill_string_array(ampLst[i],numNotes,delimiter);
		noteDuration=fill_string_array(durationLst[i],numNotes,delimiter);
		for(int j=0;j<numNotes;j++)
			{// Total Song Time (seconds)
			totalSongTime+=strtod(noteDuration[j].c_str(),NULL);
			}
		if(VERBOSE){cout<<numNotes<<" | "<<totalSongTime/((double)i+1)<<endl;}
		songArraySize=totalSongTime*fs;
		//drumSize[i]=songArraySize;
		//allLabelsOverTime[i]=new string[songArraySize];
		//for(int a=0;a<songArraySize;a++){allLabelsOverTime[i][a]="";}
		if(FIRST_MEASURE)
			{// Initialize Output
			S.N=songArraySize;
			S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
			S.time=new double[S.N];
			S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
			labelsOverTime=new string[S.N];
			for(int a=0;a<S.N;a++){S.signal[a]=0; S.time[a]=a/fs; S.amplitude[a]=-1; labelsOverTime[a]="";}
			FIRST_MEASURE=false;}
		// Notes
		drumNote=fill_string_array(noteLst[i],numNotes,delimiter);
		index=0;
		for(int a=0;a<numNotes;a++)
			{//cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";
			//cout<<"Note #: "<<a+1<<" / "<<numNotes<<endl;
			//N=count_delimiter(drumNote[a],delimiter);
			notes[0]=drumNote[a];//fill_string_array(drumNote[a],N,delimiter);
			N=1;//count_delimiter(noteDuration[a],delimiter);
			duration[0]=strtod(noteDuration[a].c_str(),NULL);//fill_double_array(noteDuration[a],N,delimiter);
			//for(int j=0;j<N;j++){duration[j]=timePerMeasure/duration[j];}
			ampVal=strtod(noteAmplitude[a].c_str(),NULL);
			value=0;
			//cout<<a<<"|2|"<<notes[0]<<endl;
			for(int j=0;j<N;j++)
				{if(notes[j].substr(0,1).compare("R")==0){letter="R";}
				else{letter=notes[j].substr(0,notes[j].length()-1);}
				//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
				if(notes[j].length()>=7 || notes[j].length()==2 || notes[j].length()==3)
					{// Play note	
					noteSize=duration[j]*fs;
					if(j==0){smallest=noteSize;}
					else if(noteSize<smallest){smallest=noteSize;}
					tmpN=0;
					//cout<<"HERE\n";
					tmpDrumArr=generate_drum_sound(notes[j],200,1.0,envp,duration[j],tmpN);
					//cout<<"HERE2\n";
					maxValue=abs_maximum_of_array(tmpDrumArr,tmpN,theIndex);
					tmpLabel=notes[j];
					SEARCHING=true;
					while(SEARCHING)
						{tmpPos=tmpLabel.find("_",0);
						if(tmpPos!=string::npos)
							{// Underscore found, so replace with space
							tmpLabel.replace(tmpPos,1," ");}
						else{SEARCHING=false; break;}
						}
					//cout<<i<<"|"<<tmpLabel<<endl;
					//cout<<"Gen. Drum Sound Size: "<<tmpN<<" | "<<S.N<<" | "<<index<<endl;
					for(int k=0;k<tmpN;k++){tmpDrumArr[k]/=maxValue;}
					for(int k=0;k<tmpN;k++)
						{if(index+k<S.N)
							{S.signal[index+k]+=(SAMPLE)tmpDrumArr[k];
							S.amplitude[index+k]=(SAMPLE)ampVal;
							//
							//allLabelsOverTime[i][index+k]+=tmpLabel+delimiter;
							if(k<noteSize){labelsOverTime[index+k]+=tmpLabel+delimiter;}							
							}
						}
					delete [] tmpDrumArr;		
					}
				else if(letter.compare("R")==0 && notes[j].length()<7)
					{// Play rest
					noteSize=fs*duration[j];							// Array size of note
					if(j==0){smallest=noteSize;}
					else if(noteSize<smallest){smallest=noteSize;}
					// Generate Silience
					for(int k=0;k<noteSize;k++)
						{if(index+k<S.N)
							{S.amplitude[index+k]+=0;}}
					}
				}
			// Update time index
			index+=smallest;
			}
		}
	// Normalize for Multiple Drum Parts
	//if(numUsedDrums>1){for(int i=0;i<S.N;i++){S.signal[i]/=numUsedDrums;}}

	// Handle Drum Labels
/*	maxValue=drumSize[0];
	for(int i=0;i<numUsedDrums;i++){if(drumSize[i]>maxValue){maxValue=drumSize[i];}}
	songArraySize=maxValue;
	string *labelsOverTime=new string[songArraySize];
	for(int i=0;i<songArraySize;i++){labelsOverTime[i]="";}
	//
	for(int i=0;i<numUsedDrums;i++)
		{for(int j=0;j<drumSize[i];j++)
			{labelsOverTime[j]+=allLabelsOverTime[i][j];}
		}
*/
	// Update Note File
/*	FILE *fOut;
	ifstream fIn;
	ofstream fOut2;
	int tmpInt=SIG_FIGS;
	string frmt="%0."+cnvrtNumToStrng(tmpInt,0)+"f\t%s\n",header,allContent="";
	// Update Label File
	if(!file_exist(labelFile))
		{// Write New File
		fOut=fopen(labelFile.c_str(),"w");
		header="# Time (seconds) Labels";
		fprintf(fOut,"%s\n",header.c_str());
		for(int i=0;i<S.N;i++){fprintf(fOut,frmt.c_str(),S.time[i],labelsOverTime[i].c_str());}
		fclose(fOut);
		}
	else
		{// Update Existent File
		fIn.open(labelFile.c_str());
		if(fIn.fail()){cerr<<"ERROR in read_drums_input_file!\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		Counter=0;
		fIn.getline(Val,Sz);
		allContent="";
		while(!fIn.eof())
			{tmp=Val;
			if(labelsOverTime[Counter].compare("R"+delimiter)!=0)
				{// Append Additional labels
				allContent+=tmp+labelsOverTime[Counter]+"\n";
				}
			else
				{allContent+=tmp+"\n";}
			Counter++;
			fIn.getline(Val,Sz);}
		fIn.close();
		// Re-Write Label File
		fOut2.open(labelFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut2.fail()){cerr<<"ERROR in read_drums_input_file!\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}
		fOut2<<allContent;
		fOut2.close();
		}
*/
	return S;}

soundSignal read_misc_percussion_input_file(string inFile,string soundType,double timePerMeasure,char **envp)
	{soundSignal S;
	S.soundType=soundType;
	// Song Time Length = numMeasures * beatsPerMeasure * timePerBeat
	// Total Song Array Size = numMeasures * beatsPerMeasure * timePerBeat * fs
	// Label File
	string labelFile=extract_containing_folder(inFile)+"labels";
	double fs=SAMPLE_RATE, f;	// Hertz, seconds, seconds
	int N;//, numDrumTypes=23;

	string *noteLst, *durationLst, *ampLst;
	int numUsedDrums;
	int Sz=LINE_SIZE, pos, pos2, freqIndex, freqsInMeasure, drumIndex, drumsInMeasure;
	char Val[Sz];
	string lst, tmp,inputValue,topFreq, topDrum, bottomFreq, nextFreq, nextDrum, freqLst="",drumLst="",delimiter=GLOBAL_DELIMITER;
	string* measure;
	// Read Header, Should Contain at Minimum: Sound Type & Tempo
	bool DRUMS=false, TEMPO_FOUND=false, SOUND_TYPE_FOUND=false, COLLECTING_MEASURE, SEARCHING, READING_MEASURE, NOTE_SHARP, NOTE_FLAT, NOTE_DOUBLE_SHARP, NOTE_DOUBLE_FLAT;
	// Time per measure as been specified so this signal can already be defined discretely with respect to time knowing the sampling frequency, which is 44,100 Hz
	int firstNotePos, nextNotePos, notePos, measureSize=timePerMeasure*fs, *usedDrumIndex, aPos;		// Array size of Measure's Signal
	string* frequenciesInMeasure, *theDrumsInMeasure, letter, octave;
	double* timeOfMeasure=new double[measureSize];
	double noteDurationTime, fraction, val1, val2;
	string drumTypeValue;
	bool FIRST_MEASURE=true;

	collect_measures_from_misc_percussion_input_file(inFile,timePerMeasure,noteLst,durationLst,ampLst,numUsedDrums);	//cout<<"|"<<numUsedDrums<<"|"<<noteLst[0]<<"|"<<endl;

	int numNotes, index=0, Counter=0, tmpN, songArraySize, noteSize, theIndex, pitchValue,tmpPos;
	string *noteDuration, *drumNote, *noteAmplitude, *labelsOverTime, tmpLabel;
	string *notes=new string[1];
	double *duration=new double[1];
	double theValue, smallest, totalSongTime=0, value, *tmpDrumArr, maxValue, ampVal;
	FIRST_MEASURE=true;
	for(int i=0;i<numUsedDrums;i++)
		{numNotes=count_delimiter(noteLst[i],delimiter);
		//cout<<numNotes<<"\n"<<noteLst<<"\n"<<durationLst<<endl; exit(1);
		if(count_delimiter(durationLst[i],delimiter)!=numNotes){cerr<<"Error in read_misc_percussion_input_file!\nNotes and Durations do not match!\n"; exit(EXIT_FAILURE);}
		noteAmplitude=fill_string_array(ampLst[i],numNotes,delimiter);
		noteDuration=fill_string_array(durationLst[i],numNotes,delimiter);
		for(int j=0;j<numNotes;j++)
			{// Total Song Time (seconds)
			totalSongTime+=strtod(noteDuration[j].c_str(),NULL);
			}
		if(VERBOSE){cout<<numNotes<<" | "<<totalSongTime/((double)i+1)<<endl;}
		songArraySize=totalSongTime*fs;
		if(FIRST_MEASURE)
			{// Initialize Output
			S.N=songArraySize;
			S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
			S.time=new double[S.N];
			S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
			labelsOverTime=new string[S.N];
			for(int a=0;a<S.N;a++){S.signal[a]=0; S.time[a]=a/fs; S.amplitude[a]=-1; labelsOverTime[a]="";}
			FIRST_MEASURE=false;}
		// Notes
		drumNote=fill_string_array(noteLst[i],numNotes,delimiter);
		index=0;
		for(int a=0;a<numNotes;a++)
			{notes[0]=drumNote[a];//fill_string_array(drumNote[a],N,delimiter);
			N=1;//count_delimiter(noteDuration[a],delimiter);
			duration[0]=strtod(noteDuration[a].c_str(),NULL);//fill_double_array(noteDuration[a],N,delimiter);
			//for(int j=0;j<N;j++){duration[j]=timePerMeasure/duration[j];}
			ampVal=strtod(noteAmplitude[a].c_str(),NULL);
			value=0;
			for(int j=0;j<N;j++)
				{if(notes[j].substr(0,1).compare("R")==0){letter="R";}
				else{letter=notes[j].substr(0,notes[j].length()-1);}
				//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
				if(notes[j].length()>=4 || notes[j].length()==2 || notes[j].length()==3)
					{// Play note
					noteSize=duration[j]*fs;
					if(j==0){smallest=noteSize;}
					else if(noteSize<smallest){smallest=noteSize;}
					pitchValue=get_pitch_for_misc_percussion_type(notes[j]);
					//cout<<"|"<<notes[j]<<"|"<<duration[j]<<"|"<<pitchValue<<"|"<<ampVal<<"|\n";
					tmpN=0;
					tmpDrumArr=generate_soundFont_sound("misc_percussion",duration[j],pitchValue,"",envp,tmpN);
					maxValue=abs_maximum_of_array(tmpDrumArr,tmpN,theIndex);
					//cout<<"|"<<notes[j]<<"|"<<duration[j]<<"|"<<pitchValue<<"|"<<ampVal<<"|"<<tmpN<<"|"<<maxValue<<"|\n";
					tmpLabel=notes[j];
					SEARCHING=true;
					while(SEARCHING)
						{tmpPos=tmpLabel.find("_",0);
						if(tmpPos!=string::npos)
							{// Underscore found, so replace with space
							tmpLabel.replace(tmpPos,1," ");}
						else{SEARCHING=false; break;}
						}
					for(int k=0;k<tmpN;k++){tmpDrumArr[k]/=maxValue;}
					for(int k=0;k<tmpN;k++)
						{if(index+k<S.N)
							{S.signal[index+k]+=(SAMPLE)tmpDrumArr[k];
							S.amplitude[index+k]=(SAMPLE)ampVal;
							//
							if(k<noteSize){labelsOverTime[index+k]+=tmpLabel+delimiter;}
							}
						}
					delete [] tmpDrumArr;		
					}
				else if(letter.compare("R")==0 && notes[j].length()<7)
					{// Play rest
					noteSize=fs*duration[j];							// Array size of note
					if(j==0){smallest=noteSize;}
					else if(noteSize<smallest){smallest=noteSize;}
					// Generate Silience
					for(int k=0;k<noteSize;k++)
						{if(index+k<S.N)
							{S.amplitude[index+k]+=0;}}
					}
				}
			// Update time index
			index+=smallest;
			}
		}

/*	FILE *fOut;
	ifstream fIn;
	ofstream fOut2;
	int tmpInt=SIG_FIGS;
	string frmt="%0."+cnvrtNumToStrng(tmpInt,0)+"f\t%s\n",header,allContent="";
	// Update Label File
	if(!file_exist(labelFile))
		{// Write New File
		fOut=fopen(labelFile.c_str(),"w");
		header="# Time (seconds) Labels";
		fprintf(fOut,"%s\n",header.c_str());
		for(int i=0;i<S.N;i++){fprintf(fOut,frmt.c_str(),S.time[i],labelsOverTime[i].c_str());}
		fclose(fOut);
		}
	else
		{// Update Existent File
		fIn.open(labelFile.c_str());
		if(fIn.fail()){cerr<<"ERROR in read_misc_percussion_input_file!\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		Counter=0;
		fIn.getline(Val,Sz);
		allContent="";
		while(!fIn.eof())
			{tmp=Val;
			if(labelsOverTime[Counter].compare("R"+delimiter)!=0)
				{// Append Additional labels
				allContent+=tmp+labelsOverTime[Counter]+"\n";
				}
			else
				{allContent+=tmp+"\n";}
			Counter++;
			fIn.getline(Val,Sz);}
		fIn.close();
		// Re-Write Label File
		fOut2.open(labelFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut2.fail()){cerr<<"ERROR in read_misc_percussion_input_file!\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}
		fOut2<<allContent;
		fOut2.close();
		}
*/
	return S;}

soundSignal read_lyrics_input_file(string inFile,string soundType,string filterCommand,double timePerMeasure,char** envp)
	{soundSignal S;
	S.soundType=soundType;
	// Song Time Length = numMeasures * beatsPerMeasure * timePerBeat
	// Total Song Array Size = numMeasures * beatsPerMeasure * timePerBeat * fs
	// Note File
	string noteFile=extract_containing_folder(inFile)+"notes";
	double fs=SAMPLE_RATE, f;	// Hertz, seconds, seconds	
	int numNotes=0, noteSize, N, tmpN;
	string delimiter=GLOBAL_DELIMITER, innerDelimiter=GLOBAL_DELIMITER2, letter, octave, lyricLst="",noteLst="",durationLst="", ampLst="",tmp, tmpFile, theLst;
	string blockNameLst="",blockDefLst="";
	// Checked for Blocked Music Input File
	int numBlocks=check_for_blocked_music_input(inFile,blockNameLst,blockDefLst);
	// Handle Blocked Music Input
	if(VERBOSE){cout<<"Collecting lyrics...";}
	if(numBlocks>0)
		{tmpFile=inFile+"_expanded";
		write_expanded_music_input_file(tmpFile,blockNameLst,blockDefLst,numBlocks);
		collect_lyrics_from_input_file(tmpFile,timePerMeasure,innerDelimiter,delimiter,lyricLst,noteLst,durationLst,ampLst,numNotes);}
	else
		{// Regular Music Input
		collect_lyrics_from_input_file(inFile,timePerMeasure,innerDelimiter,delimiter,lyricLst,noteLst,durationLst,ampLst,numNotes);}
	//cout<<"|"<<lyricLst<<"|\n|"<<noteLst<<"|\n|"<<durationLst<<"|\n|"<<ampLst<<"|\n";
	if(VERBOSE){cout<<"Done!\n";}
	// Note Durations (seconds)
	string *tmpArr2=fill_string_array(durationLst,numNotes,delimiter);
	// Notes
	string *tmpArr=fill_string_array(noteLst,numNotes,delimiter);
	// Amplitudes
	//string *amps=fill_string_array(ampLst,numNotes,innerDelimiter);
	string *amps=fill_string_array(ampLst,numNotes,delimiter);
	// Lyrics
	string *tmpArr4=fill_string_array(lyricLst,numNotes,delimiter);
	//for(int i=0;i<numNotes;i++){cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";}
	//
	double* tmpArr3, theValue,startTime,pitch;
	//if(VERBOSE){cout<<durationLst<<endl;}
	double totalSongTime=determine_total_song_time(durationLst,numNotes,innerDelimiter),smallest;
	if(VERBOSE){cout<<"# Notes: "<<numNotes<<" | "<<formatNumberString(cnvrtNumToStrng(totalSongTime,SIG_FIGS))<<"s\n";}
	int songArraySize=totalSongTime*fs;
	string *notesOverTime=new string[songArraySize];
	// Define Output
	S.N=songArraySize;
	S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
	S.time=new double[S.N];
	S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
	for(int i=0;i<S.N;i++){S.signal[i]=0; S.time[i]=i/fs; S.amplitude[i]=-1; notesOverTime[i]="";}

	string *notes, otherMarks, *lyrics, thePitch, theAmplitude;
	double *duration, value, *theSound, timeElapsed, staccatoDuration,amplitude,*ampValues;
	double maxValue, noteVolume;
	int index=0, Counter=0, noteCutOff, theIndex;
	int Sz=LINE_SIZE;
	char Val[Sz];
	string allContent="";
	for(int i=0;i<numNotes;i++)
		{//cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";
		//if(i%50==0){cout<<"Note #: "<<i+1<<" / "<<numNotes<<endl;}		
		N=count_delimiter(tmpArr[i],innerDelimiter);
		notes=fill_string_array(tmpArr[i],N,innerDelimiter);
		lyrics=fill_string_array(tmpArr4[i],N,innerDelimiter);
		duration=fill_double_array(tmpArr2[i],N,innerDelimiter);
		//cout<<"Note #: "<<i+1<<" / "<<numNotes<<endl;
		//cout<<"|"<<notes<<"|"<<duration<<"|"<<amps[i]<<"|\n";
		ampValues=fill_double_array(amps[i],count_delimiter(amps[i],innerDelimiter),innerDelimiter);
		amplitude=ampValues[0];
		delete [] ampValues;
		value=0;		
		for(int j=0;j<N;j++)
			{letter=extract_letter_from_noteCode(notes[j]);
			otherMarks=extract_other_marks_from_noteCode(notes[j]);
			//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
			noteSize=duration[j]*fs;							// Array size of note
			// Determine Note CutOff
			if(duration[j]>=timePerMeasure/4)
				{// Quarter Note or Greater, so use default cutoff
				noteCutOff=noteSize-timePerMeasure*fs/64;}
			else{noteCutOff=noteSize-noteSize/16;}
			if(j==0){smallest=noteSize;}
			else if(noteSize<smallest){smallest=noteSize;}
			octave=extract_octave_from_noteCode(notes[j]);
			//cout<<"N="<<tmpN<<" | "<<noteSize<<" | "<<S.N<<endl;
			//if(notes[j].length()>=2 || notes[j].length()==2 || notes[j].length()==3 && otherMarks.length()==0)
			if(notes[j].length()>=2)
				{// Play Regular Note (play sound 15/16th of desired beat, resting for 1/16th)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				tmpN=0;				
				thePitch=cnvrtNumToStrng(calculate_soundFonts_pitch(letter,atoi(octave.c_str())),0);		//"50";
				if(amplitude<0){theAmplitude=formatNumberString(cnvrtNumToStrng(interpolate(1,0,1,0,200),SIG_FIGS));}
				else{theAmplitude=formatNumberString(cnvrtNumToStrng(interpolate(amplitude,0,1,0,200),SIG_FIGS));}
				//cout<<i<<"|"<<lyrics[j]<<endl;
				theSound=generate_espeak_sound(soundType,f,duration[j],lyrics[j],thePitch,theAmplitude,envp,tmpN);
				// Normalize Sound
				maxValue=abs_maximum_of_array(theSound,tmpN,theIndex);
				for(int k=0;k<tmpN;k++){theSound[k]/=maxValue;}
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N && k<tmpN)
						{//if(k < noteCutOff)
						//	{S.signal[index+k]+=(SAMPLE)theSound[k];
						//	S.amplitude[index+k]=(SAMPLE)amplitude;}
						//else{S.amplitude[index+k]=0;}
						S.signal[index+k]+=(SAMPLE)theSound[k];
						S.amplitude[index+k]=(SAMPLE)amplitude;
						notesOverTime[index+k]+=notes[j]+delimiter;
						}
					else if(index+k<S.N && k>=tmpN)
						{S.signal[index+k]+=0;
						S.amplitude[index+k]=0;
						notesOverTime[index+k]+=notes[j]+delimiter;
						}
					}
				delete [] theSound;
				}
			else if(letter.compare("R")==0)
				{// Play rest
				for(int k=0;k<noteSize;k++){if(index+k<S.N){S.amplitude[index+k]=0; notesOverTime[index+k]+=notes[j]+delimiter;}}
				}
			}
		// Normalize Chords Amplitude
		if(N>1)
			{//tmpArr3=new double[noteSize];
			//for(int k=0;k<noteSize;k++){if(index+k<S.N){tmpArr3[k]=S.signal[index+k];}}
			//maxValue=abs_maximum_of_array(tmpArr3,noteSize,theIndex);
			//delete [] tmpArr3;
			theLst="";
			//tmpN=0;
			for(int k=0;k<noteSize;k++)
				{if(index+k<S.N)
					{S.signal[index+k]/=(double)N;
					theLst+=formatNumberString(cnvrtNumToStrng(S.signal[index+k],SIG_FIGS))+delimiter;
					//tmpN++;
					}
				}
			tmpN=count_delimiter(theLst,delimiter);
			tmpArr3=fill_double_array(theLst,tmpN,delimiter);
			maxValue=abs_maximum_of_array(tmpArr3,tmpN,theIndex);
			for(int k=0;k<tmpN;k++)
				{if(index+k<S.N)
					{S.signal[index+k]=tmpArr3[k]/maxValue;}
				}
			delete [] tmpArr3;
			}
		// Update time index
		index+=smallest;
		}
	// Update Note File
	FILE *fOut;
	ifstream fIn;
	ofstream fOut2;
	int tmpInt=SIG_FIGS;
	string frmt="%0."+cnvrtNumToStrng(tmpInt,0)+"f\t%s\n";
	if(!file_exist(noteFile))
		{// Write New File
		fOut=fopen(noteFile.c_str(),"w");
		string header="# X Time (seconds) Notes";
		fprintf(fOut,"%s\n",header.c_str());
		//for(int i=0;i<Sz;i++){fprintf(fOut,"%0.11f\t%0.11f\n",(double)i/fs,x[i]);}
		for(int i=0;i<S.N;i++){fprintf(fOut,frmt.c_str(),S.time[i],notesOverTime[i].c_str());}
		fclose(fOut);
		}
	else
		{// Update Existent File
		fIn.open(noteFile.c_str());
		if(fIn.fail()){cerr<<"ERROR in read_music_input_file!\nInput note file could not be opened.\n"<<noteFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		Counter=0;
		fIn.getline(Val,Sz);
		while(!fIn.eof())
			{tmp=Val;
			if(notesOverTime[Counter].compare("R"+delimiter)!=0)
				{// Append Additional notes
				allContent+=tmp+notesOverTime[Counter]+"\n";
				}
			else
				{allContent+=tmp+"\n";}
			Counter++;
			fIn.getline(Val,Sz);}
		fIn.close();
		// Re-Write Note File
		fOut2.open(noteFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut2.fail()){cerr<<"ERROR in read_music_input_file!\nInput note file could not be opened.\n"<<noteFile<<endl; exit(EXIT_FAILURE);}
		fOut2<<allContent;
		fOut2.close();
		}
	
	return S;}


soundSignal read_music_input_file(string inFile,string soundType,string filterCommand,double timePerMeasure,char** envp)
	{soundSignal S;
	S.soundType=soundType;
	// Song Time Length = numMeasures * beatsPerMeasure * timePerBeat
	// Total Song Array Size = numMeasures * beatsPerMeasure * timePerBeat * fs
	// Note File
	string noteFile=extract_containing_folder(inFile)+"notes";
	double fs=SAMPLE_RATE, f;	// Hertz, seconds, seconds	
	int numNotes=0, noteSize, N, tmpN;
	string delimiter=GLOBAL_DELIMITER, innerDelimiter=GLOBAL_DELIMITER2, letter, octave, noteLst="",durationLst="", ampLst="",tmp, tmpFile, theLst;
	string blockNameLst="",blockDefLst="";
	// Checked for Blocked Music Input File
	int numBlocks=check_for_blocked_music_input(inFile,blockNameLst,blockDefLst);
	// Handle Blocked Music Input
	if(VERBOSE){cout<<"Collecting measures...";}
	if(numBlocks>0)
		{tmpFile=inFile+"_expanded";
		write_expanded_music_input_file(tmpFile,blockNameLst,blockDefLst,numBlocks);
		collect_measures_from_input_file(tmpFile,timePerMeasure,innerDelimiter,delimiter,noteLst,durationLst,ampLst,numNotes);}
	else
		{// Regular Music Input
		collect_measures_from_input_file(inFile,timePerMeasure,innerDelimiter,delimiter,noteLst,durationLst,ampLst,numNotes);}
	//cout<<"|"<<noteLst<<"|\n|"<<durationLst<<"|\n|"<<ampLst<<"|\n";
	if(VERBOSE){cout<<"Done!\n";}
	// Note Durations (seconds)
	string *tmpArr2=fill_string_array(durationLst,numNotes,delimiter);
	// Notes
	string *tmpArr=fill_string_array(noteLst,numNotes,delimiter);
	// Amplitudes
	//string *amps=fill_string_array(ampLst,numNotes,innerDelimiter);
	string *amps=fill_string_array(ampLst,numNotes,delimiter);
	//for(int i=0;i<numNotes;i++){cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";}
	//
	double* tmpArr3, theValue,startTime,pitch;
	//cout<<durationLst<<endl;//if(VERBOSE){cout<<durationLst<<endl;}
	double totalSongTime=determine_total_song_time(durationLst,numNotes,innerDelimiter),smallest;
	if(VERBOSE){cout<<"# Notes: "<<numNotes<<" | "<<formatNumberString(cnvrtNumToStrng(totalSongTime,SIG_FIGS))<<"s\n";}
	int songArraySize=totalSongTime*fs;
	string *notesOverTime=new string[songArraySize];
	// Define Output
	S.N=songArraySize;
	S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
	S.time=new double[S.N];
	S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
	for(int i=0;i<S.N;i++){S.signal[i]=0; S.time[i]=i/fs; S.amplitude[i]=-1; notesOverTime[i]="";}

	string *notes, otherMarks;
	double *duration, value, *theSound, timeElapsed, staccatoDuration,amplitude,*ampValues;
	double maxValue, noteVolume;
	int index=0, Counter=0, noteCutOff, theIndex;
	int Sz=LINE_SIZE;
	char Val[Sz];
	string allContent="";
	for(int i=0;i<numNotes;i++)
		{//cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";
		//if(i%50==0){cout<<"Note #: "<<i+1<<" / "<<numNotes<<endl;}		
		N=count_delimiter(tmpArr[i],innerDelimiter);
		notes=fill_string_array(tmpArr[i],N,innerDelimiter);
		duration=fill_double_array(tmpArr2[i],N,innerDelimiter);
		//cout<<"Note #: "<<i+1<<" / "<<numNotes<<endl;
		//cout<<"|"<<notes<<"|"<<duration<<"|"<<amps[i]<<"|\n";
		ampValues=fill_double_array(amps[i],count_delimiter(amps[i],innerDelimiter),innerDelimiter);
		amplitude=ampValues[0];
		delete [] ampValues;
		value=0;		
		for(int j=0;j<N;j++)
			{letter=extract_letter_from_noteCode(notes[j]);
			otherMarks=extract_other_marks_from_noteCode(notes[j]);
			//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
			noteSize=duration[j]*fs;							// Array size of note
			// Determine Note CutOff
			if(duration[j]>=timePerMeasure/4)
				{// Quarter Note or Greater, so use default cutoff
				noteCutOff=noteSize-timePerMeasure*fs/64;}
			else{noteCutOff=noteSize-noteSize/16;}
			if(j==0){smallest=noteSize;}
			else if(noteSize<smallest){smallest=noteSize;}
			octave=extract_octave_from_noteCode(notes[j]);
			//cout<<"N="<<tmpN<<" | "<<noteSize<<" | "<<S.N<<endl;
			if(notes[j].length()>=7 || notes[j].length()==2 || notes[j].length()==3 && otherMarks.length()==0)
				{// Play Regular Note (play sound 15/16th of desired beat, resting for 1/16th)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				tmpN=0;
				theSound=generate_sound(soundType,filterCommand,duration[j],f,timePerMeasure,letter,atoi(octave.c_str()),otherMarks,envp,tmpN);
				// Normalize Sound
				maxValue=abs_maximum_of_array(theSound,tmpN,theIndex);
				for(int k=0;k<tmpN;k++){theSound[k]/=maxValue;}
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N && k<tmpN)
						{//if(k < noteCutOff)
						//	{S.signal[index+k]+=(SAMPLE)theSound[k];
						//	S.amplitude[index+k]=(SAMPLE)amplitude;}
						//else{S.amplitude[index+k]=0;}
						S.signal[index+k]+=(SAMPLE)theSound[k];
						S.amplitude[index+k]=(SAMPLE)amplitude;
						notesOverTime[index+k]+=notes[j]+delimiter;
						}
					}
				delete [] theSound;
				}
			else if(letter.compare("R")==0 && notes[j].length()<7)
				{// Play rest
				for(int k=0;k<noteSize;k++){if(index+k<S.N){S.amplitude[index+k]=0; notesOverTime[index+k]+=notes[j]+delimiter;}}
				}
			else if(otherMarks.compare("S")==0)
				{// Play Sustained Notes (play sound entire measure)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				tmpN=0;
				theSound=generate_sound(soundType,filterCommand,duration[j],f,timePerMeasure,letter,atoi(octave.c_str()),otherMarks,envp,tmpN);
				// Normalize Sound
				maxValue=abs_maximum_of_array(theSound,tmpN,theIndex);
				for(int k=0;k<tmpN;k++){theSound[k]/=maxValue;}
				// Write Out Notes for Visualization
				for(int k=0;k<noteSize;k++){if(index+k<S.N && k<tmpN){notesOverTime[index+k]+=notes[j]+delimiter;}}

				if(tmpN>=noteSize)
					{//Generated Sound Longer than Needed
					timeElapsed=0;
					for(int k=0;k<tmpN;k++)
						{if(index+k<S.N && timeElapsed<=2*timePerMeasure)
							{S.signal[index+k]+=(SAMPLE)theSound[k];
							S.amplitude[index+k]=(SAMPLE)amplitude;
							//notesOverTime[index+k]+=notes[j]+delimiter;
							}
						// Update time
						timeElapsed+=(1/fs);
						}
					}
				else{// Generated Sound Not Long Enough
					for(int k=0;k<noteSize;k++)
						{if(index+k<S.N && k<tmpN)
							{S.signal[index+k]+=(SAMPLE)theSound[k];
							S.amplitude[index+k]=(SAMPLE)amplitude;
							//notesOverTime[index+k]+=notes[j]+delimiter;
							}
						}
					}
				delete [] theSound;
				}
			else if(otherMarks.compare("X")==0)
				{// Play Staccato Notes (1/10th the duration of regular notes and twice as loud)
				// Determine Staccato Time Duration
				staccatoDuration=timePerMeasure/10;
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				tmpN=0;
				theSound=generate_sound(soundType,filterCommand,duration[j],f,timePerMeasure,letter,atoi(octave.c_str()),otherMarks,envp,tmpN);
				// Normalize Sound
				maxValue=abs_maximum_of_array(theSound,tmpN,theIndex);
				for(int k=0;k<tmpN;k++){theSound[k]/=maxValue;}
				timeElapsed=0;
				//for(int k=0;k<noteSize;k++)
				for(int k=0;k<(noteSize/2+1);k++)
					{if(index+k<S.N && k<tmpN)
						{// time, staccatoDuration, timeElapsed=0
						if(timeElapsed <= staccatoDuration)
							{S.signal[index+k]+=(SAMPLE)theSound[k];
							S.amplitude[index+k]=(SAMPLE)amplitude;
							notesOverTime[index+k]+=notes[j]+delimiter;}
						}
					// Update Time
					timeElapsed+=(1/fs);
					}
				delete [] theSound;
				}
			}
		// Normalize Chords Amplitude
		if(N>1)
			{//tmpArr3=new double[noteSize];
			//for(int k=0;k<noteSize;k++){if(index+k<S.N){tmpArr3[k]=S.signal[index+k];}}
			//maxValue=abs_maximum_of_array(tmpArr3,noteSize,theIndex);
			//delete [] tmpArr3;
			theLst="";
			//tmpN=0;
			for(int k=0;k<noteSize;k++)
				{if(index+k<S.N)
					{S.signal[index+k]/=(double)N;
					theLst+=formatNumberString(cnvrtNumToStrng(S.signal[index+k],SIG_FIGS))+delimiter;
					//tmpN++;
					}
				}
			tmpN=count_delimiter(theLst,delimiter);
			tmpArr3=fill_double_array(theLst,tmpN,delimiter);
			maxValue=abs_maximum_of_array(tmpArr3,tmpN,theIndex);
			for(int k=0;k<tmpN;k++)
				{if(index+k<S.N)
					{S.signal[index+k]=tmpArr3[k]/maxValue;}
				}
			delete [] tmpArr3;
			}
		// Update time index
		index+=smallest;
		}
	// Update Note File
	FILE *fOut;
	ifstream fIn;
	ofstream fOut2;
	int tmpInt=SIG_FIGS;
	string frmt="%0."+cnvrtNumToStrng(tmpInt,0)+"f\t%s\n";
	if(!file_exist(noteFile))
		{// Write New File
		fOut=fopen(noteFile.c_str(),"w");
		string header="# X Time (seconds) Notes";
		fprintf(fOut,"%s\n",header.c_str());
		//for(int i=0;i<Sz;i++){fprintf(fOut,"%0.11f\t%0.11f\n",(double)i/fs,x[i]);}
		for(int i=0;i<S.N;i++){fprintf(fOut,frmt.c_str(),S.time[i],notesOverTime[i].c_str());}
		fclose(fOut);
		}
	else
		{// Update Existent File
		fIn.open(noteFile.c_str());
		if(fIn.fail()){cerr<<"ERROR in read_music_input_file!\nInput note file could not be opened.\n"<<noteFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		Counter=0;
		fIn.getline(Val,Sz);
		while(!fIn.eof())
			{tmp=Val;
			if(notesOverTime[Counter].compare("R"+delimiter)!=0)
				{// Append Additional notes
				allContent+=tmp+notesOverTime[Counter]+"\n";
				}
			else
				{allContent+=tmp+"\n";}
			Counter++;
			fIn.getline(Val,Sz);}
		fIn.close();
		// Re-Write Note File
		fOut2.open(noteFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut2.fail()){cerr<<"ERROR in read_music_input_file!\nInput note file could not be opened.\n"<<noteFile<<endl; exit(EXIT_FAILURE);}
		fOut2<<allContent;
		fOut2.close();
		}
	
	return S;}

soundSignal read_simple_drums_input_file(string inFile,string soundType,double timePerMeasure,char **envp)
	{soundSignal S;
	S.soundType=soundType;
	// Song Time Length = numMeasures * beatsPerMeasure * timePerBeat
	// Total Song Array Size = numMeasures * beatsPerMeasure * timePerBeat * fs
	// Label File
	string labelFile=extract_containing_folder(inFile)+"labels";
	double fs=SAMPLE_RATE, f;	// Hertz, seconds, seconds
	int N;//, numDrumTypes=23;

	string *noteLst, *durationLst, *ampLst;
	int numUsedDrums;
	int Sz=LINE_SIZE, pos, pos2, freqIndex, freqsInMeasure, drumIndex, drumsInMeasure;
	char Val[Sz];
	string lst, tmp,inputValue,topFreq, topDrum, bottomFreq, nextFreq, nextDrum, freqLst="",drumLst="",delimiter=GLOBAL_DELIMITER;
	string* measure;
	// Read Header, Should Contain at Minimum: Sound Type & Tempo
	bool DRUMS=false, TEMPO_FOUND=false, SOUND_TYPE_FOUND=false, COLLECTING_MEASURE, SEARCHING, READING_MEASURE, NOTE_SHARP, NOTE_FLAT, NOTE_DOUBLE_SHARP, NOTE_DOUBLE_FLAT;
	// Time per measure as been specified so this signal can already be defined discretely with respect to time knowing the sampling frequency, which is 44,100 Hz
	int firstNotePos, nextNotePos, notePos, measureSize=timePerMeasure*fs, *usedDrumIndex, aPos;		// Array size of Measure's Signal
	string* frequenciesInMeasure, *theDrumsInMeasure, letter, octave;
	double* timeOfMeasure=new double[measureSize];
	double noteDurationTime, fraction, val1, val2;
	string drumTypeValue;
	bool FIRST_MEASURE=true;

	collect_measures_from_simple_drums_input_file(inFile,timePerMeasure,noteLst,durationLst,ampLst,numUsedDrums);	//cout<<"|"<<numUsedDrums<<"|"<<noteLst[0]<<"|"<<endl;

	int numNotes, index=0, Counter=0, tmpN, songArraySize, noteSize, theIndex, pitchValue, tmpPos;
	string *noteDuration, *drumNote, *noteAmplitude, *labelsOverTime, tmpLabel;
	string *notes=new string[1];
	double *duration=new double[1];
	double theValue, smallest, totalSongTime=0, value, *tmpDrumArr, maxValue, ampVal;
	FIRST_MEASURE=true;
	for(int i=0;i<numUsedDrums;i++)
		{numNotes=count_delimiter(noteLst[i],delimiter);
		//cout<<numNotes<<"\n"<<noteLst<<"\n"<<durationLst<<endl; exit(1);
		if(count_delimiter(durationLst[i],delimiter)!=numNotes){cerr<<"Error in read_simple_drums_input_file!\nNotes and Durations do not match!\n"; exit(EXIT_FAILURE);}
		noteAmplitude=fill_string_array(ampLst[i],numNotes,delimiter);
		noteDuration=fill_string_array(durationLst[i],numNotes,delimiter);
		for(int j=0;j<numNotes;j++)
			{// Total Song Time (seconds)
			totalSongTime+=strtod(noteDuration[j].c_str(),NULL);
			}
		if(VERBOSE){cout<<numNotes<<" | "<<totalSongTime/((double)i+1)<<endl;}
		songArraySize=totalSongTime*fs;
		if(FIRST_MEASURE)
			{// Initialize Output
			S.N=songArraySize;
			S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
			S.time=new double[S.N];
			S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
			labelsOverTime=new string[S.N];
			for(int a=0;a<S.N;a++){S.signal[a]=0; S.time[a]=a/fs; S.amplitude[a]=-1; labelsOverTime[a]="";}
			FIRST_MEASURE=false;}
		// Notes
		drumNote=fill_string_array(noteLst[i],numNotes,delimiter);
		index=0;
		for(int a=0;a<numNotes;a++)
			{notes[0]=drumNote[a];//fill_string_array(drumNote[a],N,delimiter);
			N=1;//count_delimiter(noteDuration[a],delimiter);
			duration[0]=strtod(noteDuration[a].c_str(),NULL);//fill_double_array(noteDuration[a],N,delimiter);
			//for(int j=0;j<N;j++){duration[j]=timePerMeasure/duration[j];}
			ampVal=strtod(noteAmplitude[a].c_str(),NULL);
			value=0;
			for(int j=0;j<N;j++)
				{if(notes[j].substr(0,1).compare("R")==0){letter="R";}
				else{letter=notes[j].substr(0,notes[j].length()-1);}
				//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
				if(notes[j].length()>=7 || notes[j].length()==2 || notes[j].length()==3)
					{// Play note
					noteSize=duration[j]*fs;
					if(j==0){smallest=noteSize;}
					else if(noteSize<smallest){smallest=noteSize;}
					pitchValue=get_pitch_for_simple_drums_type(notes[j]);
					//cout<<"|"<<notes[j]<<"|"<<duration[j]<<"|"<<pitchValue<<"|"<<ampVal<<"|\n";
					tmpN=0;
					tmpDrumArr=generate_soundFont_sound("simple_drums",duration[j],pitchValue,"",envp,tmpN);
					maxValue=abs_maximum_of_array(tmpDrumArr,tmpN,theIndex);
					//cout<<"|"<<notes[j]<<"|"<<duration[j]<<"|"<<pitchValue<<"|"<<ampVal<<"|"<<tmpN<<"|"<<maxValue<<"|\n";
					tmpLabel=notes[j];
					SEARCHING=true;
					while(SEARCHING)
						{tmpPos=tmpLabel.find("_",0);
						if(tmpPos!=string::npos)
							{// Underscore found, so replace with space
							tmpLabel.replace(tmpPos,1," ");}
						else{SEARCHING=false; break;}
						}
					for(int k=0;k<tmpN;k++)
						{if(index+k<S.N)
							{S.signal[index+k]+=(SAMPLE)tmpDrumArr[k]/(SAMPLE)maxValue;
							S.amplitude[index+k]=(SAMPLE)ampVal;
							//
							if(k<noteSize){labelsOverTime[index+k]+=tmpLabel+delimiter;}
							}
						}
					delete [] tmpDrumArr;		
					}
				else if(letter.compare("R")==0 && notes[j].length()<7)
					{// Play rest
					noteSize=fs*duration[j];							// Array size of note
					if(j==0){smallest=noteSize;}
					else if(noteSize<smallest){smallest=noteSize;}
					// Generate Silience
					for(int k=0;k<noteSize;k++)
						{if(index+k<S.N)
							{S.amplitude[index+k]+=0;}}
					}
				}
			// Update time index
			index+=smallest;
			}
		}
	
	FILE *fOut;
	ifstream fIn;
	ofstream fOut2;
	int tmpInt=SIG_FIGS;
	string frmt="%0."+cnvrtNumToStrng(tmpInt,0)+"f\t%s\n",header,allContent="";
	// Update Label File
	if(!file_exist(labelFile))
		{// Write New File
		fOut=fopen(labelFile.c_str(),"w");
		header="# Time (seconds) Labels";
		fprintf(fOut,"%s\n",header.c_str());
		for(int i=0;i<S.N;i++){fprintf(fOut,frmt.c_str(),S.time[i],labelsOverTime[i].c_str());}
		fclose(fOut);
		}
	else
		{// Update Existent File
		fIn.open(labelFile.c_str());
		if(fIn.fail()){cerr<<"ERROR in read_simple_drums_input_file!\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		Counter=0;
		fIn.getline(Val,Sz);
		allContent="";
		while(!fIn.eof())
			{tmp=Val;
			if(labelsOverTime[Counter].compare("R"+delimiter)!=0)
				{// Append Additional labels
				allContent+=tmp+labelsOverTime[Counter]+"\n";
				}
			else
				{allContent+=tmp+"\n";}
			Counter++;
			fIn.getline(Val,Sz);}
		fIn.close();
		// Re-Write Label File
		fOut2.open(labelFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut2.fail()){cerr<<"ERROR in read_simple_drums_input_file!\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}
		fOut2<<allContent;
		fOut2.close();
		}

	return S;}

soundSignal read_sawtooth_input_file(string inFile,string soundType,double timePerMeasure)
	{soundSignal S;
	S.soundType=soundType;
	double fs=SAMPLE_RATE, f, A, PI=2.0*acos(0.0);	// Hertz, seconds, seconds	
	int numNotes=0, noteSize, N;
	string delimiter=GLOBAL_DELIMITER, innerDelimiter=GLOBAL_DELIMITER2, letter, octave, noteLst="",durationLst="",ampLst="";
	collect_measures_from_input_file(inFile,timePerMeasure,innerDelimiter,delimiter,noteLst,durationLst,ampLst,numNotes);
	// Note Durations (seconds)
	string *tmpArr2=fill_string_array(durationLst,numNotes,delimiter);
	// Notes
	string *tmpArr=fill_string_array(noteLst,numNotes,delimiter);
	// Amplitudes
	string *amps=fill_string_array(ampLst,numNotes,innerDelimiter);
	//for(int i=0;i<numNotes;i++){cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";}
	//
	double* tmpArr3, theValue,startTime,pitch;
	double totalSongTime=determine_total_song_time(durationLst,numNotes,innerDelimiter);
	//cout<<numNotes<<" | "<<totalSongTime<<endl;
	int songArraySize=totalSongTime*fs;
	// Define Output
	S.N=songArraySize;
	S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
	S.time=new double[S.N];
	S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
	for(int i=0;i<S.N;i++){S.signal[i]=0.0; S.time[i]=i/fs; S.amplitude[i]=-1;}

	string *notes, otherMarks;
	double *duration, value, timeElapsed, staccatoDuration,smallest,amplitude;
	double maxValue, period;
	int index=0, Counter=0, tmpN,noteCutOff,theIndex;
	for(int i=0;i<numNotes;i++)
		{//cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";
		//cout<<"Note #: "<<i+1<<" / "<<numNotes<<endl;
		N=count_delimiter(tmpArr[i],innerDelimiter);
		notes=fill_string_array(tmpArr[i],N,innerDelimiter);
		duration=fill_double_array(tmpArr2[i],N,innerDelimiter);
		amplitude=strtod(amps[i].c_str(),NULL);
		value=0;
		for(int j=0;j<N;j++)
			{letter=extract_letter_from_noteCode(notes[j]);
			otherMarks=extract_other_marks_from_noteCode(notes[j]);
			//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
			noteSize=duration[j]*fs;							// Array size of note
			// Determine Note CutOff
			if(duration[j]>=timePerMeasure/4)
				{// Quarter Note or Greater, so use default cutoff
				noteCutOff=noteSize-timePerMeasure*fs/64;}
			else{noteCutOff=noteSize-noteSize/16;}
			if(j==0){smallest=noteSize;}
			else if(noteSize<smallest){smallest=noteSize;}
			octave=extract_octave_from_noteCode(notes[j]);
			//cout<<"N="<<tmpN<<" | "<<noteSize<<" | "<<S.N<<endl;
			if(notes[j].length()>=7 || notes[j].length()==2 || notes[j].length()==3 && otherMarks.length()==0)
				{// Play Regular Note (play sound 15/16th of desired beat, resting for 1/16th)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				period=fs/f;		
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{if(k < noteCutOff)
							{//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
							value=(index+k)/period;
							S.signal[index+k]+=2*(value-(0.5+value));
							S.amplitude[index+k]=(SAMPLE)amplitude;}
						}
					}
				}
			else if(letter.compare("R")==0 && notes[j].length()<7)
				{// Play rest
				}
			else if(otherMarks.compare("S")==0)
				{// Play Sustained Notes (play sound entire measure)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				period=fs/f;
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
						value=(index+k)/period;
						S.signal[index+k]+=2*(value-(0.5+value));
						S.amplitude[index+k]=(SAMPLE)amplitude;}
					}
				}
			else if(otherMarks.compare("X")==0)
				{// Play Staccato Notes (1/10th the duration of regular notes and twice as loud)
				// Determine Staccato Time Duration
				staccatoDuration=timePerMeasure/40;
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				period=fs/f;
				timeElapsed=0;
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{// time, staccatoDuration, timeElapsed=0
						if(timeElapsed <= staccatoDuration)
							{//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
							value=(index+k)/period;
							S.signal[index+k]+=2*(value-(0.5+value));
							S.amplitude[index+k]=(SAMPLE)amplitude;}
						}
					// Update Time
					timeElapsed+=(1/fs);
					}
				}
			}
		// Normalize Chords Amplitude
		if(N>1)
			{//tmpArr3=new double[noteSize];
			//for(int k=0;k<noteSize;k++){if(index+k<S.N){tmpArr3[k]=S.signal[index+k];}}
			//maxValue=abs_maximum_of_array(tmpArr3,noteSize,theIndex);
			for(int k=0;k<noteSize;k++){if(index+k<S.N){S.signal[index+k]/=N;}}
			}
		// Update time index
		index+=smallest;
		}
	return S;}

soundSignal read_sinusoid_input_file(string inFile,string soundType,double timePerMeasure)
	{soundSignal S;
	S.soundType=soundType;
	// Note File
	string noteFile=extract_containing_folder(inFile)+"notes";
	// Label File
	string labelFile=extract_containing_folder(inFile)+"labels";
	double fs=SAMPLE_RATE, f, A, PI=2.0*acos(0.0);	// Hertz, seconds, seconds	
	int numNotes=0, noteSize, N;
	string delimiter=GLOBAL_DELIMITER, innerDelimiter=GLOBAL_DELIMITER2, letter, octave, noteLst="",durationLst="",ampLst="",theLst="";
	// Collect the Measures
	collect_measures_from_input_file(inFile,timePerMeasure,innerDelimiter,delimiter,noteLst,durationLst,ampLst,numNotes);
	// Note Durations (seconds)
	string *tmpArr2=fill_string_array(durationLst,numNotes,delimiter);
	// Notes
	string *tmpArr=fill_string_array(noteLst,numNotes,delimiter);
	// Amplitudes
	string *amps=fill_string_array(ampLst,numNotes,delimiter);
	//for(int i=0;i<numNotes;i++){cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";}
	//
	double* tmpArr3, theValue,startTime,pitch;
	double totalSongTime=determine_total_song_time(durationLst,numNotes,innerDelimiter);
	//cout<<numNotes<<" | "<<totalSongTime<<endl;
	int songArraySize=totalSongTime*fs;
	string *notesOverTime=new string[songArraySize];
	string *labelsOverTime=new string[songArraySize];
	// Define Output
	S.N=songArraySize;
	S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
	S.time=new double[S.N];
	S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
	for(int i=0;i<S.N;i++){S.signal[i]=0; S.time[i]=i/fs; S.amplitude[i]=-1; notesOverTime[i]=""; labelsOverTime[i]="";}

	string *notes, otherMarks, tmpLabel;
	double *duration, value, timeElapsed, staccatoDuration,smallest,amplitude;
	double maxValue;
	int index=0, Counter=0, tmpN,noteCutOff,theIndex;
	for(int i=0;i<numNotes;i++)
		{//cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";
		//cout<<"Note #: "<<i+1<<" / "<<numNotes<<endl;
		N=count_delimiter(tmpArr[i],innerDelimiter);
		notes=fill_string_array(tmpArr[i],N,innerDelimiter);
		duration=fill_double_array(tmpArr2[i],N,innerDelimiter);
		amplitude=strtod(amps[i].c_str(),NULL);
		value=0;
		for(int j=0;j<N;j++)
			{letter=extract_letter_from_noteCode(notes[j]);
			otherMarks=extract_other_marks_from_noteCode(notes[j]);
			//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
			noteSize=duration[j]*fs;							// Array size of note
			// Determine Note CutOff
			if(duration[j]>=timePerMeasure/4)
				{// Quarter Note or Greater, so use default cutoff
				noteCutOff=noteSize-timePerMeasure*fs/64;}
			else{noteCutOff=noteSize-noteSize/16;}
			if(j==0){smallest=noteSize;}
			else if(noteSize<smallest){smallest=noteSize;}
			octave=extract_octave_from_noteCode(notes[j]);
			//cout<<"N="<<tmpN<<" | "<<noteSize<<" | "<<S.N<<endl;
			if(notes[j].length()>=7 || notes[j].length()==2 || notes[j].length()==3 && otherMarks.length()==0)
				{// Play Regular Note (play sound 15/16th of desired beat, resting for 1/16th)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				tmpLabel=cnvrtNumToStrng(f,3)+" Hz";
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{if(k < noteCutOff)
							{//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
							S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/(2*fs));
							S.amplitude[index+k]=(SAMPLE)amplitude;
							}
						//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
						//S.amplitude[index+k]=(SAMPLE)amplitude;
						notesOverTime[index+k]+=notes[j]+delimiter;
						labelsOverTime[index+k]+=tmpLabel+delimiter;
						}
					}
				}
			else if(letter.compare("R")==0 && notes[j].length()<7)
				{// Play rest
				tmpLabel="R";
				for(int k=0;k<noteSize;k++){if(index+k<S.N){notesOverTime[index+k]+=notes[j]+delimiter; labelsOverTime[index+k]+=tmpLabel+delimiter;}}
				}
			else if(otherMarks.compare("S")==0)
				{// Play Sustained Notes (play sound entire measure)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				tmpLabel=cnvrtNumToStrng(f,3)+" Hz";
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
						S.amplitude[index+k]=(SAMPLE)amplitude;
						notesOverTime[index+k]+=notes[j]+delimiter;
						labelsOverTime[index+k]+=tmpLabel+delimiter;}
					}
				}
			else if(otherMarks.compare("X")==0)
				{// Play Staccato Notes (1/10th the duration of regular notes and twice as loud)
				// Determine Staccato Time Duration
				staccatoDuration=timePerMeasure/40;
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				tmpLabel=cnvrtNumToStrng(f,3)+" Hz";
				timeElapsed=0;
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{// time, staccatoDuration, timeElapsed=0
						if(timeElapsed <= staccatoDuration)
							{S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
							S.amplitude[index+k]=(SAMPLE)amplitude;
							notesOverTime[index+k]+=notes[j]+delimiter;
							labelsOverTime[index+k]+=tmpLabel+delimiter;}
						}
					// Update Time
					timeElapsed+=(1/fs);
					}
				}
			}
		// Normalize Chords Amplitude
		if(N>1)
			{theLst="";
			//tmpN=0;
			for(int k=0;k<noteSize;k++)
				{if(index+k<S.N)
					{S.signal[index+k]/=(double)N;
					theLst+=formatNumberString(cnvrtNumToStrng(S.signal[index+k],SIG_FIGS))+delimiter;
					//tmpN++;
					}
				}
			tmpN=count_delimiter(theLst,delimiter);
			tmpArr3=fill_double_array(theLst,tmpN,delimiter);
			maxValue=abs_maximum_of_array(tmpArr3,tmpN,theIndex);
			for(int k=0;k<tmpN;k++)
				{if(index+k<S.N)
					{S.signal[index+k]=tmpArr3[k]/maxValue;}
				}
			delete [] tmpArr3;
			}
		// Update time index
		index+=smallest;
		}
	// Update Note File
	FILE *fOut;
	ifstream fIn;
	ofstream fOut2;
	int tmpInt=SIG_FIGS;
	int Sz=LINE_SIZE;
	char Val[Sz];
	string frmt="%0."+cnvrtNumToStrng(tmpInt,0)+"f\t%s\n",header,allContent="",tmp;
	if(!file_exist(noteFile))
		{// Write New File
		fOut=fopen(noteFile.c_str(),"w");
		header="# Time (seconds) Notes";
		fprintf(fOut,"%s\n",header.c_str());
		//for(int i=0;i<Sz;i++){fprintf(fOut,"%0.11f\t%0.11f\n",(double)i/fs,x[i]);}
		for(int i=0;i<S.N;i++){fprintf(fOut,frmt.c_str(),S.time[i],notesOverTime[i].c_str());}
		fclose(fOut);
		}
	else
		{// Update Existent File
		fIn.open(noteFile.c_str());
		if(fIn.fail()){cerr<<"ERROR in read_sinusoid_input_file!\nInput note file could not be opened.\n"<<noteFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		Counter=0;
		fIn.getline(Val,Sz);
		while(!fIn.eof())
			{tmp=Val;
			if(notesOverTime[Counter].compare("R"+delimiter)!=0)
				{// Append Additional notes
				allContent+=tmp+notesOverTime[Counter]+"\n";
				}
			else
				{allContent+=tmp+"\n";}
			Counter++;
			fIn.getline(Val,Sz);}
		fIn.close();
		// Re-Write Note File
		fOut2.open(noteFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut2.fail()){cerr<<"ERROR in read_sinusoid_input_file\nInput note file could not be opened.\n"<<noteFile<<endl; exit(EXIT_FAILURE);}
		fOut2<<allContent;
		fOut2.close();
		}
	// Update Label File
	if(!file_exist(labelFile))
		{// Write New File
		fOut=fopen(labelFile.c_str(),"w");
		header="# Time (seconds) Labels";
		fprintf(fOut,"%s\n",header.c_str());
		for(int i=0;i<S.N;i++){fprintf(fOut,frmt.c_str(),S.time[i],labelsOverTime[i].c_str());}
		fclose(fOut);
		}
	else
		{// Update Existent File
		fIn.open(labelFile.c_str());
		if(fIn.fail()){cerr<<"ERROR in read_sinusoid_input_file!\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		Counter=0;
		fIn.getline(Val,Sz);
		allContent="";
		while(!fIn.eof())
			{tmp=Val;
			if(labelsOverTime[Counter].compare("R"+delimiter)!=0)
				{// Append Additional labels
				allContent+=tmp+labelsOverTime[Counter]+"\n";
				}
			else
				{allContent+=tmp+"\n";}
			Counter++;
			fIn.getline(Val,Sz);}
		fIn.close();
		// Re-Write Label File
		fOut2.open(labelFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut2.fail()){cerr<<"ERROR\nInput label file could not be opened.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}
		fOut2<<allContent;
		fOut2.close();
		}

	return S;}

syntheticSong read_song_input_file(string inFile,char** envp)
	{syntheticSong S;
	ifstream fIn;
	fIn.open(inFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in read_song_input_file!\nInput song file could not be opened.\n"<<inFile<<endl; exit(EXIT_FAILURE);}
	// Delete Files to be Written
	// Note File
	string noteFile=extract_containing_folder(inFile)+"notes";
	// Label File
	string labelFile=extract_containing_folder(inFile)+"labels";
	//if(file_exist(labelFile)){}	
	bool FILE_FOUND=false, AMP_FOUND=false, SEARCHING;
	int Sz=LINE_SIZE, pos, pos2, pos3, N, Counter=0;
	char Val[Sz];
	string tmp, tmp2, soundFile, soundAmp, soundType, filter, fileLst="", ampLst="", delimiter=GLOBAL_DELIMITER,content, inputValue, lst="",stLst="", filterLst="",innerDelimiter=GLOBAL_DELIMITER2;
	bool TEMPO_FOUND=false;
	double timePerMeasure;
	fIn.getline(Val,Sz);
	while(!fIn.eof())
		{tmp=Val;
		// Exclude comments
		pos=tmp.find("#",0);
		if(pos!=0 && pos!=1)
			{// Not comment
			pos=tmp.find("TIME_PER_MEASURE",0);
			if(pos!=string::npos)
				{// Tempo
				if(tmp.find("TIME_PER_MEASURE = ",0)!=string::npos){inputValue=tmp.substr(19,tmp.length()-19);}
				else if(tmp.find("TIME_PER_MEASURE =",0)!=string::npos){inputValue=tmp.substr(18,tmp.length()-18);}
				else if(tmp.find("TIME_PER_MEASURE= ",0)!=string::npos){inputValue=tmp.substr(18,tmp.length()-18);}
				else if(tmp.find("TIME_PER_MEASURE=",0)!=string::npos){inputValue=tmp.substr(17,tmp.length()-17);}
				else if(tmp.find("TIME_PER_MEASURE ",0)!=string::npos){inputValue=tmp.substr(17,tmp.length()-17);}
				else{cerr<<"Tempo Not Defined!\nPlease specify a Tempo for your input at the top of your file.\n\nFor example:\n# Tempo (seconds)\nTIME_PER_MEASURE = 4.8\n\n"; exit(EXIT_FAILURE);}
				timePerMeasure=strtod(inputValue.c_str(),NULL);
				TEMPO_FOUND=true;}
			}
		if(TEMPO_FOUND){break;}
		else{fIn.getline(Val,Sz);}
		}

	fIn.getline(Val,Sz);
	Counter=0;
	ampLst="";
	while(!fIn.eof())
		{tmp=Val;
		// Check for Comments
		pos=tmp.find("#",0);
		if(pos!=0)
			{pos=tmp.find("FILE_",0);
			pos2=tmp.find("AMPLITUDE_",0);		
			if(pos!=string::npos && pos2==string::npos)
				{FILE_FOUND=true;
				// Extract File Path of Sound 
				if(tmp.find(" = ",0)!=string::npos){soundFile=tmp.substr(tmp.find(" = ",0)+3,tmp.length()-tmp.find(" = ",0)-3);}
				else if(tmp.find("= ",0)!=string::npos){soundFile=tmp.substr(tmp.find("= ",0)+2,tmp.length()-tmp.find("= ",0)-2);}
				else if(tmp.find(" =",0)!=string::npos){soundFile=tmp.substr(tmp.find(" =",0)+2,tmp.length()-tmp.find(" =",0)-2);}
				else if(tmp.find("=",0)!=string::npos){soundFile=tmp.substr(tmp.find("=",0)+1,tmp.length()-tmp.find("=",0)-1);}
				else if(tmp.find("=",0)==string::npos){cerr<<"Error in read_song_input_file\nBadly formatted file path for component ("<<Counter+1<<")\n"<<soundFile<<"\n"; exit(EXIT_FAILURE);}
				else{cerr<<"Error in read_song_input_file\nUn-predicted error handling sound file:\n"<<soundFile<<"\n"; exit(EXIT_FAILURE);}
				//cout<<soundFile<<endl;
				}
			else if(pos==string::npos && pos2!=string::npos)
				{AMP_FOUND=true;
				// Extract Amplitude(s)
				SEARCHING=true; lst="";
				while(SEARCHING)
					{// Extract Amplitude
					if(tmp.find(" = ",0)!=string::npos){soundAmp=tmp.substr(tmp.find(" = ",0)+3,tmp.length()-tmp.find(" = ",0)-3);}
					else if(tmp.find("= ",0)!=string::npos){soundAmp=tmp.substr(tmp.find("= ",0)+2,tmp.length()-tmp.find("= ",0)-2);}
					else if(tmp.find(" =",0)!=string::npos){soundAmp=tmp.substr(tmp.find(" =",0)+2,tmp.length()-tmp.find(" =",0)-2);}
					else if(tmp.find("=",0)!=string::npos){soundAmp=tmp.substr(tmp.find("=",0)+1,tmp.length()-tmp.find("=",0)-1);}
					else if(tmp.find("=",0)==string::npos){SEARCHING=false; break;}
					else{cerr<<"Error in read_song_input_file\nUn-predicted error handling amplitude:\n"<<soundAmp<<"\n"; exit(EXIT_FAILURE);}
					// Store in Convenient Music Input
					lst+=tmp+innerDelimiter;
					fIn.getline(Val,Sz);
					tmp=Val;
					pos=tmp.find("AMPLITUDE_",0);
					if(pos==string::npos)
						{// Amplitude data not found
						SEARCHING=false; break;}
					}
				}
			else
				{// Something Else
				}
			// Handle Possible Case of Sound Type
			pos=tmp.find("SOUND_",0);
			if(pos!=string::npos)
				{// Extract Sound Type Information
				if(tmp.find(" = ",0)!=string::npos){soundType=tmp.substr(tmp.find(" = ",0)+3,tmp.length()-tmp.find(" = ",0)-3);}
				else if(tmp.find("= ",0)!=string::npos){soundType=tmp.substr(tmp.find("= ",0)+2,tmp.length()-tmp.find("= ",0)-2);}
				else if(tmp.find(" =",0)!=string::npos){soundType=tmp.substr(tmp.find(" =",0)+2,tmp.length()-tmp.find(" =",0)-2);}
				else if(tmp.find("=",0)!=string::npos){soundType=tmp.substr(tmp.find("=",0)+1,tmp.length()-tmp.find("=",0)-1);}
				else if(tmp.find("=",0)==string::npos){cerr<<"Error in read_song_input_file\nBadly formatted sound type for component ("<<Counter+1<<")\n"<<soundType<<"\n"; exit(EXIT_FAILURE);}
				else{cerr<<"Error in read_song_input_file\nUn-predicted error handling amplitude:\n"<<soundType<<"\n"; exit(EXIT_FAILURE);}
				// Record Sound Type For Later
				stLst+=soundType+delimiter;
				}
			// Handle Possible Filter Input
			// Format: /* out bqrez in, freq, resonance, 1 */ /* hpOut,lpOut,bpOut,brOut statevar in, freq, Q */
			pos=tmp.find("FILTER_",0);
			if(pos!=string::npos)
				{// Extract Filter Command
				if(tmp.find(" = ",0)!=string::npos){filter=tmp.substr(tmp.find(" = ",0)+3,tmp.length()-tmp.find(" = ",0)-3);}
				else if(tmp.find("= ",0)!=string::npos){filter=tmp.substr(tmp.find("= ",0)+2,tmp.length()-tmp.find("= ",0)-2);}
				else if(tmp.find(" =",0)!=string::npos){filter=tmp.substr(tmp.find(" =",0)+2,tmp.length()-tmp.find(" =",0)-2);}
				else if(tmp.find("=",0)!=string::npos){filter=tmp.substr(tmp.find("=",0)+1,tmp.length()-tmp.find("=",0)-1);}
				else if(tmp.find("=",0)==string::npos){cerr<<"Error in read_song_input_file\nBadly formatted filter command for component ("<<Counter+1<<")\n"<<filter<<"\n"; exit(EXIT_FAILURE);}
				else{cerr<<"Error in read_song_input_file\nUn-predicted error handling filter command:\n"<<filter<<"\n"; exit(EXIT_FAILURE);}
				// Record Sound Type For Later
				filterLst+=filter+delimiter;
				}
			}

		if(AMP_FOUND && FILE_FOUND)
			{AMP_FOUND=false; FILE_FOUND=false;
			//cout<<"Amp: |"<<soundAmp<<"|\nFile:|"<<soundFile<<"|\n";
			ampLst+=lst+delimiter; //ampLst+=soundAmp+delimiter;
			fileLst+=soundFile+delimiter;
			Counter++;
			}
		fIn.getline(Val,Sz);
		}
	fIn.close();

	// Error Check
	if(Counter!=count_delimiter(ampLst,delimiter)){cerr<<"Error in read_song_input_file\nNumber of amplitude arrays ("<<ampLst<<"("<<count_delimiter(ampLst,delimiter)<<") does not match the number of components detected ("<<Counter<<")\n"; exit(EXIT_FAILURE);}

	S.numComponents=Counter;
	S.timePerMeasure=timePerMeasure;
	S.component=new soundSignal[S.numComponents];
	S.mInput=new musicInput[S.numComponents];
	string *tmpArr=fill_string_array(ampLst,S.numComponents,delimiter),*tmpArr2;
	for(int i=0;i<S.numComponents;i++)
		{S.mInput[i].numAmpInputs=count_delimiter(tmpArr[i],innerDelimiter);
		//cout<<i<<"|"<<tmpArr[i]<<endl;
		if(S.mInput[i].numAmpInputs>0){S.mInput[i].amplitudeInput=fill_string_array(tmpArr[i],S.mInput[i].numAmpInputs,innerDelimiter);}
		else if(S.mInput[i].numAmpInputs==0)
			{// No Amplitude data input, so use default value, 1.0
			S.mInput[i].numAmpInputs=1.0;
			S.mInput[i].amplitudeInput=new string[S.mInput[i].numAmpInputs];
			S.mInput[i].amplitudeInput[0]="AMPLITUDE_"+cnvrtNumToStrng(i+1,0)+" = 1.0";}
		else{cerr<<"Error in read_song_input_file\nUn-predicted error during post-handling\n"; exit(EXIT_FAILURE);}
		//
		}
	S.soundType=fill_string_array(stLst,S.numComponents,delimiter);
	// Filter
	S.filter=fill_string_array(filterLst,S.numComponents,delimiter);
	// Need to define amplitude array later
	string* songFiles=fill_string_array(fileLst,count_delimiter(fileLst,delimiter),delimiter);
	// Notes File
	S.noteFile=extract_containing_folder(songFiles[0])+"notes";
	if(file_exist(S.noteFile)){remove(S.noteFile.c_str());}
	if(file_exist(noteFile)){remove(noteFile.c_str());}
	// Labels File
	if(file_exist(labelFile)){remove(labelFile.c_str());}
	ofstream fOut;
	string allContent="",blockName,blockNameLst="",blockDef,blockDefLst="",Output,currentBlock,currentBlockValue,tmpFile;
	string *blocks, *blockValues;
	int numBlocks,numLines;
	bool COMP_DONE;
	for(int i=0;i<S.numComponents;i++)
		{if(VERBOSE){cout<<"Reading Component #"<<i+1<<"...\n";}
		// Check for Voice Reader
		COMP_DONE=false;
		for(int j=0;j<numVoiceSpeakers;j++)
			{if(S.soundType[i].compare(espeakVoiceSpeakers[j])==0)
				{S.component[i]=read_lyrics_input_file(songFiles[i],S.soundType[i],S.filter[i],timePerMeasure,envp);
				COMP_DONE=true;
				break;}
			}
		if(COMP_DONE){continue;}
		// Check for Usual Instruments/Sounds
		if(S.soundType[i].compare("drums")==0)
			{//cout<<"Reading drums music...";
			// Checked for Blocked Music Input File
			numBlocks=check_for_blocked_music_input(songFiles[i],blockNameLst,blockDefLst);
			// Handle Blocked Music Input
			if(numBlocks>0)
				{//
				tmpFile=songFiles[i]+"_expanded";
				write_expanded_music_input_file(tmpFile,blockNameLst,blockDefLst,numBlocks);
				//
				S.component[i]=read_drums_input_file(tmpFile,S.soundType[i],timePerMeasure,envp);
				}
			else
				{// Regular Music Input
				S.component[i]=read_drums_input_file(songFiles[i],S.soundType[i],timePerMeasure,envp);}
			//cout<<"Done.\n";read_songread_song
			}
		else if(S.soundType[i].compare("misc_percussion")==0)
			{//cout<<"Reading drums music...";
			// Checked for Blocked Music Input File
			numBlocks=check_for_blocked_music_input(songFiles[i],blockNameLst,blockDefLst);
			// Handle Blocked Music Input
			if(numBlocks>0)
				{//
				tmpFile=songFiles[i]+"_expanded";
				write_expanded_music_input_file(tmpFile,blockNameLst,blockDefLst,numBlocks);
				//
				S.component[i]=read_misc_percussion_input_file(tmpFile,S.soundType[i],timePerMeasure,envp);
				}
			else
				{// Regular Music Input
				S.component[i]=read_misc_percussion_input_file(songFiles[i],S.soundType[i],timePerMeasure,envp);}
			//cout<<"Done.\n";read_songread_song
			}
		else if(S.soundType[i].compare("simple_drums")==0)
			{//cout<<"Reading drums music...";
			// Checked for Blocked Music Input File
			numBlocks=check_for_blocked_music_input(songFiles[i],blockNameLst,blockDefLst);
			// Handle Blocked Music Input
			if(numBlocks>0)
				{//
				tmpFile=songFiles[i]+"_expanded";
				write_expanded_music_input_file(tmpFile,blockNameLst,blockDefLst,numBlocks);
				//
				S.component[i]=read_simple_drums_input_file(tmpFile,S.soundType[i],timePerMeasure,envp);
				}
			else
				{// Regular Music Input
				S.component[i]=read_simple_drums_input_file(songFiles[i],S.soundType[i],timePerMeasure,envp);}
			//cout<<"Done.\n";read_songread_song
			}
		else if(S.soundType[i].compare("sawtooth")==0)
			{//cout<<"Reading sawtooth waveform music...";
			S.component[i]=read_sawtooth_input_file(songFiles[i],S.soundType[i],timePerMeasure);
			//cout<<"Done.\n";
			}
		else if(S.soundType[i].compare("sinusoid")==0)
			{//cout<<"Reading sinusoid music...";
			S.component[i]=read_sinusoid_input_file(songFiles[i],S.soundType[i],timePerMeasure);
			//cout<<"Done.\n";
			}
		else if(S.soundType[i].compare("triangle_wave")==0)
			{//cout<<"Reading triangle waveform music...";
			S.component[i]=read_triangle_wave_input_file(songFiles[i],S.soundType[i],timePerMeasure);
			//cout<<"Done.\n";
			}
		else
			{//if(S.filter[i].length()==0){S.component[i]=read_music_input_file(songFiles[i],S.soundType[i],timePerMeasure,envp);}
			//else
			//	{S.component[i]=read_music_input_file(songFiles[i],S.soundType[i],S.filter[i],timePerMeasure,envp);
			//	}
			S.component[i]=read_music_input_file(songFiles[i],S.soundType[i],S.filter[i],timePerMeasure,envp);
			}
		if(VERBOSE){cout<<"Component #"<<i+1<<" Finished!\n";}
		}
	//for(int i=0;i<S.component[0].N;i++){if(S.component[0].signal[i]!=0){cout<<i<<" | "<<S.component[0].signal[i]<<"\n";}}
	// Check that signals are same length
	N=S.component[0].N;
	long minimum=2*N, maxDF=-1, df, acceptedDF=3,maxLength=0;
	// Get Minimum Length (if they vary)
	for(int i=0;i<S.numComponents;i++){if(S.component[i].N<minimum){minimum=S.component[i].N;}}
	// Check for difference between values
	for(int i=0;i<S.numComponents;i++)
		{df=S.component[i].N-minimum;
		if(maxDF<df){maxDF=df;}}
	// Error Check
	if(maxDF>acceptedDF)
		{cerr<<"Error in read_song_input_file\nComponents are not all the same length!\nNumber of Components: "<<S.numComponents<<"\n";
		// Determine Smallest Component Size
		for(int i=0;i<S.numComponents;i++)
			{cout<<"Comp #"<<i+1<<": "<<S.component[i].N<<"\n";
			if(i==0){minimum=S.component[i].N;}
			//
			if(S.component[i].N<minimum){minimum=S.component[i].N;}
			}
		if(minimum%2!=0){minimum--;}
		// Re-Size Components to have Same Length
		for(int i=0;i<S.numComponents;i++){S.component[i].N=minimum;}

/*		// Determine Max Component Size
		for(int i=0;i<S.numComponents;i++)
			{cout<<"Comp #"<<i+1<<": "<<S.component[i].N<<"\n";
			if(S.component[i].N>maxLength){maxLength=S.component[i].N;}
			}
		if(maxLength%2!=0){maxLength++;}
		// Re-Size Components to have Same Length
		for(int i=0;i<S.numComponents;i++)
			{df=maxLength-S.component[i].N;
			lst="";
			if(df>0)
				{for(int j=0;j<S.component[i].N;j++)
					{lst+=formatNumberString(cnvrtNumToStrng(S.component[i].signal[j],SIG_FIGS))+delimiter;
					}
				// Zero-Padd Remainder of Signal
				for(int j=0;j<df;j++){lst+="0"+delimiter;}
				// Update Values
				S.component[i].N=maxLength;
				delete [] S.component[i].signal;
				S.component[i].signal=fill_sample_array(lst,maxLength,delimiter);
				}
			}
*/
		//exit(EXIT_FAILURE);		
		}
	else
		{// Ensure all components have same length
		if(minimum%2!=0){minimum--;}
		for(int i=0;i<S.numComponents;i++){S.component[i].N=minimum;}
		}

	// Define Amplitude Array
	//S.amplitude=new double*[S.numComponents];
	//for(int i=0;i<S.numComponents;i++){S.amplitude[i]=new double[minimum];}
	// Interpret Music Input
	double val; long index, index2;
	for(int i=0;i<S.numComponents;i++)
		{for(int j=0;j<S.mInput[i].numAmpInputs;j++)
			{// Extract Amplitude Range and Input Value
			tmp=S.mInput[i].amplitudeInput[j];
			//cout<<i<<"|"<<tmp<<endl;
			// Extract Input Value
			if(tmp.find(" = ",0)!=string::npos){soundAmp=tmp.substr(tmp.find(" = ",0)+3,tmp.length()-tmp.find(" = ",0)-3);}
			else if(tmp.find("= ",0)!=string::npos){soundAmp=tmp.substr(tmp.find("= ",0)+2,tmp.length()-tmp.find("= ",0)-2);}
			else if(tmp.find(" =",0)!=string::npos){soundAmp=tmp.substr(tmp.find(" =",0)+2,tmp.length()-tmp.find(" =",0)-2);}
			else if(tmp.find("=",0)!=string::npos){soundAmp=tmp.substr(tmp.find("=",0)+1,tmp.length()-tmp.find("=",0)-1);}
			else if(tmp.find("=",0)==string::npos){cerr<<"Error in read_song_input_file\nBadly formatted amplitude in post-handling\n"; exit(EXIT_FAILURE);}
			else{cerr<<"Error in read_song_input_file\nUn-predicted and highly un-probable error during post-handling\n"<<soundAmp<<"\n"; exit(EXIT_FAILURE);}
			soundAmp=remove_excess_white_space(soundAmp);	//cout<<"|"<<soundAmp<<"|\n";
			val=strtod(soundAmp.c_str(),NULL);	

			//for(int k=0;k<minimum;k++){if(S.component[i].amplitude[k]!=-1){cout<<k<<"|"<<S.component[i].amplitude[k]<<endl;}}
			for(int k=0;k<minimum;k++){if(S.component[i].amplitude[k]<0){S.component[i].amplitude[k]=(SAMPLE)val;}}
			}
		}

	return S;}

soundSignal read_triangle_wave_input_file(string inFile,string soundType,double timePerMeasure)
	{soundSignal S;
	S.soundType=soundType;
	double fs=SAMPLE_RATE, f, A, PI=2.0*acos(0.0);	// Hertz, seconds, seconds	
	int numNotes=0, noteSize, N;
	string delimiter=GLOBAL_DELIMITER, innerDelimiter=GLOBAL_DELIMITER2, letter, octave, noteLst="",durationLst="",ampLst="";
	collect_measures_from_input_file(inFile,timePerMeasure,innerDelimiter,delimiter,noteLst,durationLst,ampLst,numNotes);
	// Note Durations (seconds)
	string *tmpArr2=fill_string_array(durationLst,numNotes,delimiter);
	// Notes
	string *tmpArr=fill_string_array(noteLst,numNotes,delimiter);
	// Amplitudes
	string *amps=fill_string_array(ampLst,numNotes,innerDelimiter);
	//for(int i=0;i<numNotes;i++){cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";}
	//
	double* tmpArr3, theValue,startTime,pitch;
	double totalSongTime=determine_total_song_time(durationLst,numNotes,innerDelimiter);
	//cout<<numNotes<<" | "<<totalSongTime<<endl;
	int songArraySize=totalSongTime*fs;
	// Define Output
	S.N=songArraySize;
	S.signal=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) ); //new double[S.N];
	S.time=new double[S.N];
	S.amplitude=(SAMPLE *) malloc( S.N*sizeof(SAMPLE) );//new double[S.N];
	for(int i=0;i<S.N;i++){S.signal[i]=0.0; S.time[i]=i/fs; S.amplitude[i]=-1;}

	string *notes, otherMarks;
	double *duration, value, timeElapsed, staccatoDuration,smallest,amplitude;
	double maxValue, period;
	int index=0, Counter=0, tmpN,noteCutOff;
	for(int i=0;i<numNotes;i++)
		{//cout<<i<<" | "<<tmpArr[i]<<"("<<tmpArr2[i]<<")\n";
		//cout<<"Note #: "<<i+1<<" / "<<numNotes<<endl;
		N=count_delimiter(tmpArr[i],innerDelimiter);
		notes=fill_string_array(tmpArr[i],N,innerDelimiter);
		duration=fill_double_array(tmpArr2[i],N,innerDelimiter);
		amplitude=strtod(amps[i].c_str(),NULL);
		value=0;
		for(int j=0;j<N;j++)
			{letter=extract_letter_from_noteCode(notes[j]);
			otherMarks=extract_other_marks_from_noteCode(notes[j]);
			//cout<<j+1<<"/"<<N<<"|"<<notes[j]<<":";
			noteSize=duration[j]*fs;							// Array size of note
			// Determine Note CutOff
			if(duration[j]>=timePerMeasure/4)
				{// Quarter Note or Greater, so use default cutoff
				noteCutOff=noteSize-timePerMeasure*fs/64;}
			else{noteCutOff=noteSize-noteSize/16;}
			if(j==0){smallest=noteSize;}
			else if(noteSize<smallest){smallest=noteSize;}
			octave=extract_octave_from_noteCode(notes[j]);
			//cout<<"N="<<tmpN<<" | "<<noteSize<<" | "<<S.N<<endl;
			if(notes[j].length()>=7 || notes[j].length()==2 || notes[j].length()==3 && otherMarks.length()==0)
				{// Play Regular Note (play sound 15/16th of desired beat, resting for 1/16th)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				period=fs/f;		
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{if(k < noteCutOff)
							{//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
							value=(index+k)/period;
							S.signal[index+k]+=2*(2*(value-(0.5+value)))-1;
							S.amplitude[index+k]=(SAMPLE)amplitude;}
						}
					}
				}
			else if(letter.compare("R")==0 && notes[j].length()<7)
				{// Play rest
				}
			else if(otherMarks.compare("S")==0)
				{// Play Sustained Notes (play sound entire measure)
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				period=fs/f;
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
						value=(index+k)/period;
						S.signal[index+k]+=2*(2*(value-(0.5+value)))-1;
						S.amplitude[index+k]=(SAMPLE)amplitude;}
					}
				}
			else if(otherMarks.compare("X")==0)
				{// Play Staccato Notes (1/10th the duration of regular notes and twice as loud)
				// Determine Staccato Time Duration
				staccatoDuration=timePerMeasure/40;
				f=getFrequencyOfNote(letter,atoi(octave.c_str()));	// Note Frequency
				period=fs/f;
				timeElapsed=0;
				for(int k=0;k<noteSize;k++)
					{if(index+k<S.N)
						{// time, staccatoDuration, timeElapsed=0
						if(timeElapsed <= staccatoDuration)
							{//S.signal[index+k]+=(SAMPLE)sin(2.0*PI* f * (index+k)/fs);
							value=(index+k)/period;
							S.signal[index+k]+=2*(2*(value-(0.5+value)))-1;
							S.amplitude[index+k]=(SAMPLE)amplitude;}
						}
					// Update Time
					timeElapsed+=(1/fs);
					}
				}
			}
		// Update time index
		index+=smallest;
		}
	return S;}

string remove_excess_white_space(string inStr)
	{int startIndex,endIndex;
	string tmp;
	// Check from front
	for(int i=0;i<inStr.length();i++){tmp=inStr[i]; if(tmp.compare(" ")!=0){startIndex=i; break;}}
	// Check from back
	for(int i=inStr.length()-1;i>=0;i--){tmp=inStr[i]; if(tmp.compare(" ")!=0){endIndex=i+1; break;}}
	string Output=inStr.substr(startIndex,endIndex-startIndex);
	return Output;}

double* read_amplitudes_in_wave_file(string inFile,int& N)
	{double *Output;
	SNDFILE *SoundFile;
	SF_INFO SoundFileInfo;
	int bufferSize=256, filetype = SF_ENDIAN_LITTLE | SF_FORMAT_RAW | SF_FORMAT_PCM_16 , intVal, returnValue;
	short buffer[bufferSize];
	for(int k=0;k<bufferSize;k++){buffer[k]=k;}
	
	filetype=SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	//if (! (file = sf_open (filename, SFM_WRITE, &sfinfo)))
	SoundFile=sf_open(inFile.c_str(), SFM_READ, &SoundFileInfo);
	if(!SoundFile)
		{printf ("Line %d: sf_open_read failed attempting to open:\n%s\n", __LINE__,inFile.c_str());
		fflush(stdout);
		sf_perror(NULL);
		exit(EXIT_FAILURE);
		}

	returnValue=sf_seek(SoundFile,0,SEEK_SET);		// returns position offset in file cout<<returnValue<<endl;
	if(returnValue<0){cerr<<"Error seeking to beginning of file\n";}

	// Assess Wave File
	if(SoundFileInfo.format != filetype)
		{//printf ("Line %d: Returned format incorrect (0x%08X => 0x%08X).\n", __LINE__, filetype, SoundFileInfo.format);
		//exit(EXIT_FAILURE);
		}
	
	if(SoundFileInfo.frames < bufferSize)
		{printf ("Line %d: Incorrect number of.frames in file (%s). (%d => %ld)\n", __LINE__, inFile.c_str(), bufferSize, (long)SoundFileInfo.frames);
		exit(EXIT_FAILURE);
		}
	
	//if (SoundFileInfo.channels != 1)
	//	{printf ("Line %d: Incorrect number of channels in file.\n", __LINE__) ;
	//	exit (1) ;
	//	}
	
	//SoundFile=sf_open(inFile.c_str(), SFM_READ, &SoundFileInfo);	//open a file and put it's info into a struct "SoundFileInfo"
	int Sz=SoundFileInfo.channels*SoundFileInfo.frames; //SoundFileInfo.channels * SoundFileInfo.frames;
	Output= new double[Sz];	//allocate an array to hold the samples
	//cout<<Sz<<endl;
	//sf_readf_double(SoundFile, Output, SoundFileInfo.frames);	//fill the array with sample values, a frame equals on sample per channel 
	intVal=sf_readf_double(SoundFile, Output,Sz);
	//intVal=sf_readf_short(SoundFile, Output, SoundFileInfo.frames);
	//cout<<intVal<<endl;
	if( intVal != Sz)
		{
		if(intVal>0)
			{//printf ("Line %d: short read (%d).\nNumber of Frames: %d\nNumber of Channels: %d\n%s\n", __LINE__, intVal, SoundFileInfo.frames, SoundFileInfo.channels,inFile.c_str()) ;
			//exit(EXIT_FAILURE);
			//Sz=intVal;
			}
		else if(intVal==0)
			{// End of File (EOF) Reach need to seek to top
			returnValue=sf_seek(SoundFile,0,SEEK_SET);		//cout<<returnValue<<endl;
			}
		else{cerr<<"So we got here\n";}
		}

	//for(int i=0;i<Sz;i++){cout<<i<<" | "<<Output[i]<<endl;}


	if(Sz<0)
		{cerr<<"Error in read_amplitudes_in_wave_file!\nNegative number of frames found|!\n"; exit(EXIT_FAILURE);}
	else if(Sz==0)
		{cerr<<"Error in read_amplitudes_in_wave_file!\nNo frames found|!\n";}
	
	N=Sz;
	returnValue=sf_close(SoundFile);
	if(returnValue!=0){cerr<<"Error in read_amplitudes_in_wave_file!\nCould not close wave file:\n"<<inFile<<"\n"; exit(EXIT_FAILURE);}

	sf_close(SoundFile);
	return Output;}

soundSignal read_amplitudes_in_wave_file(string inFile)
	{soundSignal Output;
	SNDFILE *SoundFile;
	SF_INFO SoundFileInfo;
	int bufferSize=256, filetype = SF_ENDIAN_LITTLE | SF_FORMAT_RAW | SF_FORMAT_PCM_16 , intVal, returnValue;
	short buffer[bufferSize];
	for(int k=0;k<bufferSize;k++){buffer[k]=k;}
	
	filetype=SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	//if (! (file = sf_open (filename, SFM_WRITE, &sfinfo)))
	SoundFile=sf_open(inFile.c_str(), SFM_READ, &SoundFileInfo);
	if(!SoundFile)
		{printf ("Line %d: sf_open_read failed attempting to open:\n%s\n", __LINE__,inFile.c_str());
		fflush(stdout);
		sf_perror(NULL);
		exit(EXIT_FAILURE);
		}

	returnValue=sf_seek(SoundFile,0,SEEK_SET);		// returns position offset in file cout<<returnValue<<endl;
	if(returnValue<0){cerr<<"Error seeking to beginning of file\n";}

	// Assess Wave File
	if(SoundFileInfo.format != filetype)
		{//printf ("Line %d: Returned format incorrect (0x%08X => 0x%08X).\n", __LINE__, filetype, SoundFileInfo.format);
		//exit(EXIT_FAILURE);
		}
	
	if(SoundFileInfo.frames < bufferSize)
		{printf ("Line %d: Incorrect number of.frames in file (%s). (%d => %ld)\n", __LINE__, inFile.c_str(), bufferSize, (long)SoundFileInfo.frames);
		exit(EXIT_FAILURE);
		}
	
	int Sz=SoundFileInfo.channels*SoundFileInfo.frames; //SoundFileInfo.channels * SoundFileInfo.frames;
	double *tmpArr=new double[Sz];
	Output.signal=new SAMPLE[Sz];
	Output.time=new double[Sz];
	Output.amplitude=new SAMPLE[Sz];	
	double fs=SAMPLE_RATE;
	//cout<<Sz<<endl;
	//sf_readf_double(SoundFile, Output, SoundFileInfo.frames);	//fill the array with sample values, a frame equals on sample per channel 
	intVal=sf_readf_double(SoundFile,tmpArr,Sz);
	//intVal=sf_readf_short(SoundFile, Output, SoundFileInfo.frames);
	//cout<<intVal<<endl;
	if( intVal != Sz)
		{
		if(intVal>0)
			{//printf ("Line %d: short read (%d).\nNumber of Frames: %d\nNumber of Channels: %d\n%s\n", __LINE__, intVal, SoundFileInfo.frames, SoundFileInfo.channels,inFile.c_str()) ;
			//exit(EXIT_FAILURE);
			//Sz=intVal;
			}
		else if(intVal==0)
			{// End of File (EOF) Reach need to seek to top
			returnValue=sf_seek(SoundFile,0,SEEK_SET);		//cout<<returnValue<<endl;
			}
		else{cerr<<"So we got here\n";}
		}

	Output.N=Sz;
	if(Sz<0){cerr<<"Error in read_amplitudes_in_wave_file!\nNegative number of frames found|!\n"; exit(EXIT_FAILURE);}
	else if(Sz==0){cerr<<"Error in read_amplitudes_in_wave_file!\nNo frames found|!\n";}

	for(int i=0;i<Sz;i++)
		{Output.signal[i]=(SAMPLE)tmpArr[i];
		Output.time[i]=i/fs;
		Output.amplitude[i]=1;}
	
	returnValue=sf_close(SoundFile);
	if(returnValue!=0){cerr<<"Error in read_amplitudes_in_wave_file!\nCould not close wave file:\n"<<inFile<<"\n"; exit(EXIT_FAILURE);}

	sf_close(SoundFile);
	return Output;}

string repeat_string(string Input,int Num){string Output="";for(int i=0;i<Num;i++){Output+=Input;}return Output;}

void run_csound(string orchestraFile,string scoreFile,string waveFile,char** envp)
	{// Generate wave file with csound
	string csound_executable=CSOUND_EXE;
	int PID=fork(),status2,result;
	string logFile=get_current_dir_name(); logFile+="/log_stderr";
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		char* argv[]={csound_executable.c_str(),"-f","--noheader","--nchnls=2","-W","-T","-o",waveFile.c_str(),orchestraFile.c_str(),scoreFile.c_str(),NULL};
		freopen(logFile.c_str(),"w",stderr);
		result=syscall(SYS_execve,csound_executable.c_str(),argv,envp);						
		if(result<0)
			{cerr<<"ERROR!\n";
			exit(EXIT_FAILURE);}
		}
	else if(PID>0){waitpid(PID,&status2,0);}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	}

void run_dnoise(string orchestraFile,string scoreFile,char** envp)
	{// Generate wave file with csound
	string csound_executable=CSOUND_EXE;
	int PID=fork(),status2,result;
	string logFile=get_current_dir_name(); logFile+="/log_stderr";
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		char* argv[]={csound_executable.c_str(),orchestraFile.c_str(),scoreFile.c_str(),NULL};
		freopen(logFile.c_str(),"w",stderr);
		result=syscall(SYS_execve,csound_executable.c_str(),argv,envp);						
		if(result<0)
			{cerr<<"ERROR!\n";
			exit(EXIT_FAILURE);}
		}
	else if(PID>0){waitpid(PID,&status2,0);}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	}

void run_espeak(string theWord,string amplitude,string pitch,string wordSpeed,string voiceName,string waveFile,char** envp)
	{// Generate wave file with csound
	string espeak_executable=ESPEAK_EXE;
	string espeak_voiceName=get_espeak_voiceName_from_voice_selection(voiceName);
	int PID=fork(),status2,result;
	string logFile=get_current_dir_name(); logFile+="/log_stderr_espeak";
	string logFile2=get_current_dir_name(); logFile2+="/log_stdout_espeak";
	if(file_exist(logFile)){remove(logFile.c_str());}
	if(file_exist(logFile2)){remove(logFile2.c_str());}
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		// -a amplitude (0 - 200, default: 100)
		// -p pitch (0 - 100, default: 50)
		// -s speed (words/min, default: 160)
		// -v voice name/type		espeak --voices=mb		
		// -w output wave file
		// -x write phonemes to stdout
		// -z no final sentence silence
		//char* argv[]={espeak_executable.c_str(),"-v",espeak_voiceName.c_str(),"-a",amplitude.c_str(),"-p",pitch.c_str(),"-s",wordSpeed.c_str(),"-w",waveFile.c_str(),"-x","-z",theWord.c_str(),NULL};
		char* argv[]={espeak_executable.c_str(),"-v",espeak_voiceName.c_str(),"-a",amplitude.c_str(),"-p",pitch.c_str(),"-s",wordSpeed.c_str(),"-w",waveFile.c_str(),"-x",theWord.c_str(),NULL};
		freopen(logFile.c_str(),"w",stderr);
		freopen(logFile2.c_str(),"w",stdout);
		result=syscall(SYS_execve,espeak_executable.c_str(),argv,envp);						
		if(result<0)
			{cerr<<"ERROR!\n";
			exit(EXIT_FAILURE);}
		}
	else if(PID>0){waitpid(PID,&status2,0);}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	}

string get_espeak_voiceName_from_voice_selection(string voice)
	{string Output;
	if(voice.compare("Afrikaans_Man_1")==0){Output="afrikaans-mbrola-1";}
	else if(voice.compare("Afrikaans_Man_2")==0){Output="en-afrikaans";}
	else if(voice.compare("Brazil_Man_1")==0){Output="brazil-mbrola-1";}
	else if(voice.compare("Brazil_Woman_1")==0){Output="brazil-mbrola-2";}
	else if(voice.compare("Brazil_Man_2")==0){Output="brazil-mbrola-3";}
	else if(voice.compare("Brazil_Woman_2")==0){Output="brazil-mbrola-4";}
	else if(voice.compare("British_Man")==0){Output="english-mb-en1";}
	else if(voice.compare("Croatian_Man")==0){Output="croatian-mbrola-1";}
	else if(voice.compare("Czech_Man")==0){Output="czech-mbrola-2";}
	else if(voice.compare("Dutch_Man_1")==0){Output="dutch-mbrola-2";}
	else if(voice.compare("Dutch_Man_2")==0){Output="en-dutch";}
	else if(voice.compare("English_Woman")==0){Output="us-mbrola-1";}
	else if(voice.compare("English_Man_1")==0){Output="us-mbrola-2";}
	else if(voice.compare("English_Man_2")==0){Output="us-mbrola-3";}
	else if(voice.compare("Estonian_Man")==0){Output="estonian-mbrola";}
	else if(voice.compare("French_Woman_1")==0){Output="en-french";}
	else if(voice.compare("French_Man_1")==0){Output="en-french";}
	else if(voice.compare("French_Man_2")==0){Output="french-mbrola-1";}
	else if(voice.compare("French_Woman_2")==0){Output="french-mbrola-4";}
	else if(voice.compare("German_Woman_1")==0){Output="en-german-1";}
	else if(voice.compare("German_Man_1")==0){Output="en-german-2";}
	else if(voice.compare("German_Woman_2")==0){Output="en-german-3";}
	else if(voice.compare("German_Man_2")==0){Output="en-german-4";}
	else if(voice.compare("German_Woman_3")==0){Output="en-german-5";}
	else if(voice.compare("German_Man_3")==0){Output="en-german-6";}
	else if(voice.compare("German_Woman_4")==0){Output="german-mbrola-1";}
	else if(voice.compare("German_Man_4")==0){Output="german-mbrola-2";}
	else if(voice.compare("German_Woman_5")==0){Output="german-mbrola-3";}
	else if(voice.compare("German_Man_5")==0){Output="german-mbrola-4";}
	else if(voice.compare("German_Woman_6")==0){Output="german-mbrola-5";}
	else if(voice.compare("German_Man_6")==0){Output="german-mbrola-6";}
	else if(voice.compare("German_Woman_7")==0){Output="german-mbrola-7";}
	else if(voice.compare("Greek_Man_1")==0){Output="en-greek";}
	else if(voice.compare("Greek_Man_2")==0){Output="greek-mbrola-1";}
	else if(voice.compare("Hungarian_Woman_1")==0){Output="en-hungarian";}
	else if(voice.compare("Hungarian_Woman_2")==0){Output="hungarian-mbrola-1";}
	else if(voice.compare("Icelandic_Man")==0){Output="mbrola-icelandic";}
	else if(voice.compare("Indonesian_Man")==0){Output="indonesian-mbrola-1";}
	else if(voice.compare("Italian_Man")==0){Output="italian-mbrola-3";}
	else if(voice.compare("Italian_Woman")==0){Output="italian-mbrola-4";}
	else if(voice.compare("Latin_Man")==0){Output="latin-mbrola-1";}
	else if(voice.compare("Lithuanian_Man_1")==0){Output="lithuanian-mbrola-1";}
	else if(voice.compare("Lithuanian_Man_2")==0){Output="lithuanian-mbrola-2";}
	else if(voice.compare("Mexican_Man_1")==0){Output="mexican-mbrola-1";}
	else if(voice.compare("Mexican_Man_2")==0){Output="mexican-mbrola-2";}
	else if(voice.compare("Persian_Man")==0){Output="persian-mb-ir1";}
	else if(voice.compare("Persian_Woman")==0){Output="persian-mb-ir2";}
	else if(voice.compare("Polish_Woman_1")==0){Output="en-polish";}
	else if(voice.compare("Polish_Woman_2")==0){Output="polish-mbrola-1";}
	else if(voice.compare("Portugal_Woman")==0){Output="portugal-mbrola-1";}
	else if(voice.compare("Romanian_Man_1")==0){Output="en-romanian";}
	else if(voice.compare("Romanian_Man_2")==0){Output="romanian-mbrola-1";}
	else if(voice.compare("Spanish_Man_1")==0){Output="spanish-mbrola-1";}
	else if(voice.compare("Spanish_Man_2")==0){Output="spanish-mbrola-2";}
	else if(voice.compare("Swedish_Man_1")==0){Output="en-swedish";}
	else if(voice.compare("Swedish_Woman_1")==0){Output="en-swedish-f";}
	else if(voice.compare("Swedish_Man_2")==0){Output="swedish-mbrola-1";}
	else if(voice.compare("Swedish_Woman_2")==0){Output="swedish-mbrola-2";}
	else if(voice.compare("Turkish_Man")==0){Output="turkish-mbrola-1";}
	else if(voice.compare("Turkish_Woman")==0){Output="turkish-mbrola-1";}
 	else if(voice.compare("Venezuala_Man")==0){Output="venezuala-mbrola-1";}
 	else
		{cerr<<"Error in get_espeak_voiceName_from_voice_selection!\nUnrecognized name: "<<voice<<endl; exit(EXIT_FAILURE);}
	return Output;}

double extrapolate(double x,double x1,double x2,double y1,double y2)
	{if(x<x1 && x<x2)
		{return (y2-y1)*abs(x-x1)/(x2-x1)+y1;}
	else if(x>x1 && x>x2)
		{return (y2-y1)*(x-x1)/(x2-x1)+y2;}
	else if(x>=x1 && x<x2)
		{return interpolate(x,x1,x2,y1,y2);}
	else
		{cerr<<"Error in extrapolate!\nUnexpected scenario\n"; exit(EXIT_FAILURE);}
	}

double interpolate(double x,double x1,double x2,double y1,double y2)
	{return (y2-y1)*(x-x1)/(x2-x1)+y1;}

string write_clarinet_score(string noteVolume,string pitch,string noteDuration)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is the time in seconds of the physical string pluck action (this is a physical model)
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/physmodl_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_clarinet_score!\nInput clarinet score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 6 Lines
	for(int i=0;i<6;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	// p1 ;   Start  Dur    Amp      Pitch   Press  Filter     Embouchure  Reed Table
	Output+="i4 0 "+noteDuration+" "+noteVolume+" "+pitch+" 1.5 1000 0.2 1\n";
	Output+="e	; end of score\n";
	return Output;}

string write_dnoise_orchestra()
	{string inputNoiseFile="WHITE_NOISE.wav";
	string numberFilters="1024";
	string Output="";
	Output+="sr = 44100\n";
	Output+="ksmps = 32\n";
	Output+="nchnls = 2\n";
	Output+="0dbfs  = 1\n\n";
	Output+="; -A				specifies AIFF file format output\n";
	Output+="; -b {time}		specify the begin time in the noise reference sound file (default: 0)\n";
	Output+="; -B {sample}		specify the starting sample frame in the noise reference sound file (default: 0)\n";
	Output+="; -D {factor}		decimation factor (default: M/8)\n";
	Output+="; -e {time}		specify the end time in the noise reference sound file (default: end of file)\n";
	Output+="; -E {sample}		specify the final sample frame in the noise reference sound file (default: end of file)\n";
	Output+="; -f				specify float sound samples\n";
	Output+="; -h				skip sound file header (not valid for AIFF/WAV output)\n";
	Output+="; -i {refFile}		specify the input reference noise sound file\n";
	Output+="; -L {length}		input the synthesis window length (default: M)\n";
	Output+="; -m {threshold}	specify the minimum threshold gain of noise-gat when off in dB (default: -40)\n";
	Output+="; -M {length}		input the analysis window length (default: N-1, unless -w is specified)\n";
	Output+="; -n {num}		input the number of FFT frames to average over (default: 5)\n";
	Output+="; -N {num}		input the number of bandpass filters (default: 1024)\n";
	Output+="; -o {outFile}		specify output sound file\n";
	Output+="; -S {factor}		specify the sharpness factor of noise-gate turnoff (1,2,3,4 or 5, default: 1)\n";
	Output+="; -t {threshold}	specify the threshold above noise reference in dB (default: 30)\n";
	Output+="; -w {factor}		input a filter overlap factor (0,1,2 or 3, default: 2)\n";
	Output+="; -W				specifies WAV file format output\n\n\n";
	Output+="ires1 system_i 1,{{ dnoise -W -N"+numberFilters+" -i "+inputNoiseFile+" -o output.wav input.wav }}\n\n";
	Output+="instr 1\n";
	Output+="adnoise diskin2 \"output.wav\", 1\n";
	Output+="outs adnoise, adnoise\n";
	Output+="endin\n\n";
	return Output;}

string write_dnoise_score(string theDuration)
	{// note Duration should be in terms of seconds
	// pitch contains the frequency 
	string Output="";
	// Insert command
	Output+="i1 0 "+theDuration+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_drums_score(string drumType,string noteNumber,double noteDuration,string strikeVelocity,string noteVolume)
	{// note Duration should be in terms of beats
	// note volume shoule specify how loud the note should be from 0 being silence to 1 being maximum volume
	// tempo is the time in beats per minute (132 be default)
	// - this directly translates from Time_Per_Measure

	//cout<<"In write_drums_score\n"<<drumType<<" "<<noteDuration<<" "<<strikeVelocity<<" "<<noteVolume<<" "<<tempo<<endl;

	string Output="",p1="",p2="",p3="",p4="",p5="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/drums_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_drums_score!\nInput drum score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 636 Lines
	for(int i=0;i<636;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}

	if(drumType.compare("CRASH_CYMBAL_1")==0){p1="10"; p4="10"; noteVolume="";}
	else if(drumType.compare("CRASH_CYMBAL_2")==0){p1="10"; p4="11"; noteVolume="";}
	else if(drumType.compare("REVERSE_CYMBAL")==0){p1="10"; p4="12"; noteVolume="";}
	else if(drumType.compare("OPEN_HIHAT_1")==0){p1="10"; p4="13"; noteVolume="";}
	else if(drumType.compare("OPEN_HIHAT_2")==0){p1="10"; p4="14"; noteVolume="";}
	else if(drumType.compare("CLOSED_HIHAT")==0){p1="10"; p4="15"; noteVolume="";}
	else if(drumType.compare("TAMBOURINE")==0){p1="10"; p4="16"; noteVolume="";}
	else if(drumType.compare("RIDE_CYMBAL")==0){p1="10"; p4="17"; noteVolume="";}
	else if(drumType.compare("BASS_DRUM")==0){p1="20"; p4="20";}
	else if(drumType.compare("808_BASS_DRUM")==0){p1="21"; p4="25";}
	else if(drumType.compare("HAND_CLAP")==0){p1="30"; p4="30";}
	else if(drumType.compare("808_COWBELL")==0){p1="31"; p4="35";}
	else if(drumType.compare("808_OPEN_HIHAT")==0){p1="40"; p4="40";}
	else if(drumType.compare("808_CLOSED_HIHAT")==0){p1="40"; p4="41";}
	else if(drumType.compare("808_CYMBAL")==0){p1="40"; p4="42";}
	else if(drumType.compare("909_SNARE_1")==0){p1="50"; p4="50";}
	else if(drumType.compare("909_SNARE_2")==0){p1="50"; p4="51";}
	else if(drumType.compare("909_SNARE_3")==0){p1="50"; p4="52";}
	else if(drumType.compare("909_SNARE_4")==0){p1="50"; p4="53";}
	else if(drumType.compare("HIGH_TOM")==0){p1="51"; p4="57";}
	else if(drumType.compare("MID_TOM")==0){p1="51"; p4="58";}
	else if(drumType.compare("LOW_TOM")==0){p1="51"; p4="59";}
	else if(drumType.compare("RIM_SHOT")==0){p1="52"; p4="56";}
	else
		{cerr<<"Error in !\nUn-recognized drum type!\n"; exit(EXIT_FAILURE);}
	
	//Output+="i "+p1+" "+noteStartTime+" "+noteDuration+" "+p4+" "+strikeVelocity+" "+noteVolume+"\n";
	//
	// CSound i statement
	// i p1 p2 p3 p4 p5
	// p2 = noteStartTime (integer numbering order of notes)
	// p3 = noteDuration 
	//
	//int noteLength=256/noteDuration;
	//Output+="i "+p1+" "+startTime+" "+cnvrtNumToStrng(noteDuration,0)+" "+p4+" "+strikeVelocity+"\n";
	Output+="i "+p1+" 0 "+cnvrtNumToStrng(noteDuration,SIG_FIGS)+" "+p4+" "+strikeVelocity+"\n";
	Output+="e	; end of score\n";

	return Output;}

string write_flute_score(string noteVolume,string pitch,string noteDuration)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is the time in seconds of the physical string pluck action (this is a physical model)
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/physmodl_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_flute_score!\nInput flute score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 6 Lines
	for(int i=0;i<6;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	// p1 start duration amp pitch pressure Breath Feedback1 Feedback2
	//Output+="i3 0 "+noteDuration+" "+noteVolume+" "+pitch+" 0.9 0.036 0.4 0.4\n";
	Output+="i3 0 "+noteDuration+" "+noteVolume+" "+pitch+" 1 0 0.4 0.4\n";
	Output+="e	; end of score\n";
	return Output;}

string write_miramba_score(string freq)
	{string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/miramba_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_pluck_score!\nInput pluck score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 4 Lines
	for(int i=0;i<4;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	Output+="i 1 0 1 "+freq+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_piano_score(string noteDuration,string freq)
	{// note Duration should be in terms of seconds
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/piano_template.sco";	// Did not need one	
	// Determine Number of String needed
	string numberOfStrings;
	double freqValue=strtod(freq.c_str(),NULL);
	if(freqValue<80){numberOfStrings="2";}
	else if(freqValue>=80 && freqValue<220){numberOfStrings="3";}
	else if(freqValue>=220){numberOfStrings="4";}
	// Correct for Detuning
	freqValue=19*freqValue/20.0;
	freq=cnvrtNumToStrng(freqValue,3);
	// Insert command
	// p1 start duration 
	Output+="i1 0 "+noteDuration+" "+freq+" "+numberOfStrings+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_pluck_score(string noteVolume,string pitch)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is modeled as the natural decay, so
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/pluck_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_pluck_score!\nInput pluck score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 6 Lines
	for(int i=0;i<3;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	string method="1";
	Output+="i 1 0 "+pitch+" "+method+" "+noteVolume+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_pluck2_score(string noteVolume,string pitch)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is modeled as the natural decay, so
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/pluck_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_pluck2_score!\nInput pluck score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 6 Lines
	for(int i=0;i<3;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	string method="6";
	Output+="i 1 0 "+pitch+" "+method+" "+noteVolume+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_plucked_bass_score(string noteVolume,string pitch,string pluckDuration,string noteDuration)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is the time in seconds of the physical string pluck action (this is a physical model)
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/physmodl_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_plucked_bass_score!\nInput plucked bass score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 6 Lines
	for(int i=0;i<6;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	Output+="i2 0 "+noteDuration+" "+noteVolume+" "+pitch+" "+pluckDuration+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_plucked_string_score(string noteVolume,string pitch,string noteDuration)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is the time in seconds of the physical string pluck action (this is a physical model)
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/multifx_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_plucked_string_score!\nInput plucked string score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First 37 Lines
	for(int i=0;i<37;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert Desired Pluck
	Output+="i2 0 "+noteDuration+" "+noteVolume+" "+pitch+" 0 1 1\n";
	// Insert Mixer
	Output+="i100  0 "+noteDuration+" 1 1 0.5 1 1 0.5\n";
	// Insert Channel
	Output+="i110  0 "+noteDuration+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_saxophone_score(string noteDuration,string freq)
	{// note Duration should be in terms of seconds
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/saxophone_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_saxophone_score!\nInput flute score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First Line
	for(int i=0;i<1;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	//;1 start duration freq stiffness blowPos depth pressure
	string reedStiffness="30";
	string blowPosition="10";
	string lowFreqOscDepth="30";
	string blowPressure="50";
	Output+="i1 0 "+noteDuration+" "+freq+" "+reedStiffness+" "+blowPosition+" "+lowFreqOscDepth+" "+blowPressure+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_orchestra_for_soundFonts(string soundFontsFile)
	{string Output="",volume="100";
	// Sample Frequency
	Output+="sr = 44100\n";
	Output+="ksmps = 32\n";
	/// Number of Channels
	Output+="nchnls = 2\n";	
	Output+="0dbfs = 1\n\n";
	Output+="giengine fluidEngine\n";
	Output+="; soundfont path to manual/examples\n";
	Output+="isfnum\t fluidLoad \""+soundFontsFile+"\", giengine, 1\n";
	Output+="	 fluidProgramSelect giengine, 1, isfnum, 0, 0\n\n";
	Output+="instr 1\n\n";
	Output+="	mididefault   60, p3\n";
	Output+="	midinoteonkey p4, p5\n";
	Output+="ikey	init p4\n";
	Output+="ivel	init p5\n";
	Output+="	fluidNote giengine, 1, ikey, ivel\n\n";
	Output+="endin\n\n";
	Output+="instr 99\n\n";
	Output+="imvol  init "+volume+"\n";
	Output+="asigl, asigr fluidOut giengine\n";
	Output+="outs asigl*imvol, asigr*imvol\n\n";
	Output+="endin\n";
	return Output;}

string write_orchestra_for_frequency_shift_1_channel(string inputWaveFile)
	{string Output="";
	Output+="sr = 44100\n";
	Output+="ksmps = 32\n";
	Output+=";nchnls = 2\n";
	Output+="0dbfs  = 1\n\n";
	Output+="instr 1\n";
	Output+="  idur = p3\n";
	Output+="  ; Initial amount of frequency shift.\n";
	Output+="  ; It can be positive or negative.\n";
	Output+="  ibegshift = p4 \n";
	Output+="  ; Final amount of frequency shift.\n";
	Output+="  ; It can be positive or negative.\n";
	Output+="  iendshift = p5 \n\n";
	Output+="  ; A simple envelope for determining the amount of frequency shift.\n";
	Output+="  kfreq linseg ibegshift, idur, iendshift\n\n";
	Output+="  ; Use the sound of your choice.\n";
	Output+="  ain diskin2 \""+inputWaveFile+"\", 1, 0, 1\n\n";
	Output+="  ; Phase quadrature output derived from input signal.\n";
	Output+="  areal, aimag hilbert ain\n";
	Output+="  ; Quadrature oscillator.\n";
	Output+="  asin oscili 1, kfreq, 1\n";
	Output+="  acos oscili 1, kfreq, 1, .25\n\n";
	Output+="  ; Use a trigonometric identity. \n";
	Output+="  ; See the references for further details.\n";
	Output+="  amod1 = areal * acos\n";
	Output+="  amod2 = aimag * asin\n\n";
	Output+="  ; Both sum and difference frequencies can be output at once.\n";
	Output+="  ; aupshift corresponds to the sum frequencies.\n";
	Output+="  aupshift = (amod1 - amod2) * 0.7\n";
	Output+="  ; adownshift corresponds to the difference frequencies. \n";
	Output+="  adownshift = (amod1 + amod2) * 0.7\n\n";
	Output+="  ; Notice that the adding of the two together is\n";
	Output+="  ; identical to the output of ring modulation.\n\n";
	Output+="  outs aupshift, aupshift\n";
	Output+="endin\n";
	return Output;}

string write_orchestra_for_frequency_shift_2_channels(string inputWaveFile)
	{string Output="";
	Output+="sr = 44100\n";
	Output+="ksmps = 32\n";
	Output+="nchnls = 2\n";
	Output+="0dbfs  = 1\n\n";
	Output+="instr 1\n";
	Output+="  idur = p3\n";
	Output+="  ; Initial amount of frequency shift.\n";
	Output+="  ; It can be positive or negative.\n";
	Output+="  ibegshift = p4 \n";
	Output+="  ; Final amount of frequency shift.\n";
	Output+="  ; It can be positive or negative.\n";
	Output+="  iendshift = p5 \n\n";
	Output+="  ; A simple envelope for determining the amount of frequency shift.\n";
	Output+="  kfreq linseg ibegshift, idur, iendshift\n\n";
	Output+="  ; Use the sound of your choice.\n";
	Output+="  ain,ain diskin2 \""+inputWaveFile+"\", 1, 0, 1\n\n";
	Output+="  ; Phase quadrature output derived from input signal.\n";
	Output+="  areal, aimag hilbert ain\n";
	Output+="  ; Quadrature oscillator.\n";
	Output+="  asin oscili 1, kfreq, 1\n";
	Output+="  acos oscili 1, kfreq, 1, .25\n\n";
	Output+="  ; Use a trigonometric identity. \n";
	Output+="  ; See the references for further details.\n";
	Output+="  amod1 = areal * acos\n";
	Output+="  amod2 = aimag * asin\n\n";
	Output+="  ; Both sum and difference frequencies can be output at once.\n";
	Output+="  ; aupshift corresponds to the sum frequencies.\n";
	Output+="  aupshift = (amod1 - amod2) * 0.7\n";
	Output+="  ; adownshift corresponds to the difference frequencies. \n";
	Output+="  adownshift = (amod1 + amod2) * 0.7\n\n";
	Output+="  ; Notice that the adding of the two together is\n";
	Output+="  ; identical to the output of ring modulation.\n\n";
	Output+="  outs aupshift, aupshift\n";
	Output+="endin\n";
	return Output;}

string write_orchestra_for_soundFonts(string soundFontsFile,string filterCommand)
	{// Filter Command Format Examples:
	// OUT bqrez IN, freq, resonance, 1
	// hpOut,OUT,bpOut,brOut statevar IN, freq, Q
	// Check
	int checkPos=filterCommand.find("NONE",0),inPos,outPos;
	string filter_left="",filter_right="";
	if(checkPos==string::npos)
		{inPos=filterCommand.find(" IN,",0);
		if(inPos==string::npos){cerr<<"Error in write_orchestra_for_soundFonts\nImproper formating for filter command\n"<<filterCommand<<endl; exit(EXIT_FAILURE);}

		filter_left=filterCommand;
		filter_left.replace(inPos+1,2,"asigl");
		outPos=filter_left.find("OUT",0);
		if(outPos==string::npos){cerr<<"Error in write_orchestra_for_soundFonts\nImproper formating for filter command\n"<<filterCommand<<endl; exit(EXIT_FAILURE);}
		filter_left.replace(outPos,3,"asigl");

		filter_right=filterCommand;
		filter_right.replace(inPos+1,2,"asigr");
		outPos=filter_right.find("OUT",0);
		if(outPos==string::npos){cerr<<"Error in write_orchestra_for_soundFonts\nImproper formating for filter command\n"<<filterCommand<<endl; exit(EXIT_FAILURE);}
		filter_right.replace(outPos,3,"asigr");
		}
	string Output="",volume="100";
	// Sample Frequency
	Output+="sr = 44100\n";
	Output+="ksmps = 32\n";
	/// Number of Channels
	Output+="nchnls = 2\n";	
	Output+="0dbfs = 1\n\n";
	Output+="giengine fluidEngine\n";
	Output+="; soundfont path to manual/examples\n";
	Output+="isfnum\t fluidLoad \""+soundFontsFile+"\", giengine, 1\n";
	Output+="	 fluidProgramSelect giengine, 1, isfnum, 0, 0\n\n";
	Output+="instr 1\n\n";
	Output+="	mididefault   60, p3\n";
	Output+="	midinoteonkey p4, p5\n";
	Output+="ikey	init p4\n";
	Output+="ivel	init p5\n";
	Output+="	fluidNote giengine, 1, ikey, ivel\n\n";
	Output+="endin\n\n";
	Output+="instr 99\n\n";
	Output+="imvol  init "+volume+"\n";
	Output+="asigl, asigr fluidOut giengine\n";
	//if(filter_left.length()==0){Output+="outs asigl*imvol, asigr*imvol\n\n";}
	//else
	//	{Output+=filter_left+"\n";
	//	Output+=filter_right+"\n";
	//	Output+="outs lsig*imvol, rsig*imvol\n\n";
	//	}
	if(filter_left.length()>=0)
		{Output+=filter_left+"\n";
		Output+=filter_right+"\n";}
	Output+="outs asigl*imvol, asigr*imvol\n\n";
	Output+="endin\n";
	return Output;}

string write_score_for_frequency_shift(string noteDuration,string changeInFrequency)
	{// note Duration should be in terms of seconds
	string Output="";
	// Insert command
	Output+="; Sine table for quadrature oscillator.\n";
	Output+="f 1 0 16384 10 1\n\n";
	Output+="i 1 0 "+noteDuration+" "+changeInFrequency+" "+changeInFrequency+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_score_for_soundFonts(string noteDuration,string pitch,string strikeVelocity)
	{// note Duration should be in terms of seconds
	// pitch contains the frequency 
	string Output="";
	// Insert command
	Output+="i 1 0 "+noteDuration+" "+pitch+" "+strikeVelocity+"\n";
	Output+="i 99 0 "+noteDuration+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_strings_score(string noteVolume,string pitch)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is the time in seconds of the physical string pluck action (this is a physical model)
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/strings_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_strings_score!\nInput flute score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First Line
	for(int i=0;i<1;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	// p1 start duration amp pitch pressure Breath Feedback1 Feedback2
	string bowPressure="3";
	string bowPosition="0.127236";
	Output+="i 1 0 3 "+bowPressure+" "+bowPosition+" "+noteVolume+" "+pitch+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_strings2_score(string noteVolume,string pitch)
	{// note Duration should be in terms of seconds
	// note volume shoule specify how loud the note should be from 0 being silence to 50000 being maximum volume
	// pitch contains the frequency 
	// pluck duration is the time in seconds of the physical string pluck action (this is a physical model)
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/strings_template.sco";
	ifstream fIn;
	fIn.open(template_score_file.c_str());
	if(fIn.fail()){cerr<<"ERROR in write_strings2_score!\nInput flute score template file could not be opened.\n"<<template_score_file<<endl; exit(EXIT_FAILURE);}	
	int Sz=LINE_SIZE;
	char Val[Sz];
	string tmp;
	// Copy First Line
	for(int i=0;i<1;i++)
		{fIn.getline(Val,Sz);
		tmp=Val;
		Output+=tmp+"\n";}
	fIn.close();
	// Insert command
	// p1 start duration amp pitch pressure Breath Feedback1 Feedback2
	string bowPressure="3";
	string bowPosition="0.24";
	Output+="i 1 0 3 "+bowPressure+" "+bowPosition+" "+noteVolume+" "+pitch+"\n";
	Output+="e	; end of score\n";
	return Output;}

string write_tambourine_score(string noteDuration)
	{// note Duration should be in terms of seconds
	string Output="";
	string template_score_file=get_current_dir_name(); template_score_file+="/CSOUND/tambourine_template.sco";	// Did not need one	
	// Insert command
	// p1 start duration 
	Output+="i 1 0 "+noteDuration+" 0.6\n";
	Output+="e	; end of score\n";
	return Output;}

double* correct_sound_size(int desiredN,int actualN,double* inputArr)
	{if(desiredN<=0){cerr<<"Error in correct_sound_size!\nDesired size is zero or less\n"; exit(EXIT_FAILURE);}
	double* outputArr=new double[desiredN],tmp,frac;
	int Counter=0;
	if(desiredN<actualN)
		{// Down-Sample
		frac=(double)actualN/(double)desiredN;
		for(int i=0;i<actualN;i=i+frac)
			{outputArr[Counter]=inputArr[i];
			Counter++;}
		}
	else if(desiredN>actualN)
		{// Zero-Pad End until desired Length
		for(int i=0;i<actualN;i=i+frac)
			{outputArr[Counter]=inputArr[i];
			Counter++;}
		for(int i=0;i<desiredN-Counter;i++)
			{outputArr[i]=0;}
		}
	else if(desiredN==actualN)
		{for(int i=0;i<actualN;i=i+frac)
			{outputArr[i]=inputArr[i];}
		}
	else
		{cerr<<"Error in correct_sound_size!\nInprobable occurrence\n"; exit(EXIT_FAILURE);}
	return outputArr;}

bool waveFile_is_good(string waveFile)
	{SNDFILE *SoundFile;
	SF_INFO SoundFileInfo;
	int bufferSize=256, filetype = SF_ENDIAN_LITTLE | SF_FORMAT_RAW | SF_FORMAT_PCM_16 , intVal, returnValue;
	short buffer[bufferSize];
	for(int k=0;k<bufferSize;k++){buffer[k]=k;}
	
	filetype=SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	//if (! (file = sf_open (filename, SFM_WRITE, &sfinfo)))
	SoundFile=sf_open(waveFile.c_str(), SFM_READ, &SoundFileInfo);
	if(!SoundFile)
		{printf ("Line %d: sf_open_write failed with error : ", __LINE__);
		fflush(stdout);
		sf_perror(NULL);
		//exit(EXIT_FAILURE);
		return false;}

	returnValue=sf_seek(SoundFile,0,SEEK_SET);		// returns position offset in file cout<<returnValue<<endl;
	if(returnValue<0){cerr<<"Error seeking to beginning of file\n";}

	// Assess Wave File
	if(SoundFileInfo.format != filetype)
		{//printf ("Line %d: Returned format incorrect (0x%08X => 0x%08X).\n", __LINE__, filetype, SoundFileInfo.format) ;
		//exit(EXIT_FAILURE);
		}
	
	if(SoundFileInfo.frames < bufferSize)
		{printf ("Line %d: Incorrect number of.frames in file. (%d => %ld)\n", __LINE__, bufferSize, (long)SoundFileInfo.frames);
		//exit(EXIT_FAILURE);
		return false;}
	
	//if (SoundFileInfo.channels != 1)
	//	{printf ("Line %d: Incorrect number of channels in file.\n", __LINE__) ;
	//	exit (1) ;
	//	}
	sf_close(SoundFile);
	return true;
	}

void write_blank_drums_score_sound_file(string soundFile)
	{int width=150;
	ofstream fOut;
	fOut.open(soundFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_blank_drums_score_sound_file!\nCould not open sound file to write blank drums score.\n"<<soundFile<<endl; exit(EXIT_FAILURE);}	
	string Output="";
	Output+="# Title Here\n";
	Output+="#\n";
	Output+="#\n";
	Output+=repeat_string(" ",6)+"CRASH_CYMBAL_1 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",6)+"CRASH_CYMBAL_2 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",6)+"REVERSE_CYMBAL "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",9)+"RIDE_CYMBAL "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",10)+"808_CYMBAL "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",8)+"OPEN_HIHAT_1 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",8)+"OPEN_HIHAT_2 "+repeat_string(" ",width)+"\n";
	Output+="P |   808_OPEN_HIHAT "+repeat_string("-",width)+"\n";
	Output+="E |     CLOSED_HIHAT "+repeat_string(" ",width)+"\n";
	Output+="R | 808_CLOSED_HIHAT "+repeat_string("-",width)+"\n";
	Output+="C |       TAMBOURINE "+repeat_string(" ",width)+"\n";
	Output+="U |        BASS_DRUM "+repeat_string("-",width)+"\n";
	Output+="S |    808_BASS_DRUM "+repeat_string(" ",width)+"\n";
	Output+="S |        HAND_CLAP "+repeat_string("-",width)+"\n";
	Output+="I |      808_COWBELL "+repeat_string(" ",width)+"\n";
	Output+="O |      909_SNARE_1 "+repeat_string("-",width)+"\n";
	Output+="N |      909_SNARE_2 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",9)+"909_SNARE_3 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",9)+"909_SNARE_4 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",12)+"HIGH_TOM "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",13)+"MID_TOM "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",13)+"LOW_TOM "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",12)+"RIM_SHOT "+repeat_string(" ",width)+"\n\n";
	fOut<<Output;
	fOut.close();}

void write_demo_drums_score_sound_file(string soundFile)
	{int width=150;
	ofstream fOut;
	fOut.open(soundFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_demo_drums_score_sound_file!\nCould not open sound file to write blank drums score.\n"<<soundFile<<endl; exit(EXIT_FAILURE);}	
	//string Output="# Drums Demo\n#\n#\n      CRASH_CYMBAL_1  O8                                                                R8/22                                                                              \n      CRASH_CYMBAL_2 -R8-O8-------------------------------------------------------------R8/21------------------------------------------------------------------------------\n      REVERSE_CYMBAL  R4    O8                                                          R8/20                                                                              \n         RIDE_CYMBAL -R8/3-----O8-------------------------------------------------------R8/19------------------------------------------------------------------------------\n          808_CYMBAL  R8/4        O8                                                    R8/18                                                                              \n        OPEN_HIHAT_1 -R8/5-----------O8-------------------------------------------------R8/17------------------------------------------------------------------------------\n        OPEN_HIHAT_2  R8/6              O8                                              R8/16                                                                              \nP |   808_OPEN_HIHAT -R8/7-----------------O8-------------------------------------------R8/15------------------------------------------------------------------------------\nE |     CLOSED_HIHAT  R8/8                    O8                                        R8/14                                                                              \nR | 808_CLOSED_HIHAT -R8/9-----------------------O8-------------------------------------R8/13------------------------------------------------------------------------------\nC |       TAMBOURINE  R8/10                         O8                                  R8/12                                                                              \nU |        BASS_DRUM -R8/11----------------------------O8-------------------------------R8/11------------------------------------------------------------------------------\nS |    808_BASS_DRUM  R8/12                               O8                            R8/10                                                                              \nS |        HAND_CLAP -R8/13----------------------------------O8-------------------------R8/9-------------------------------------------------------------------------------\nI |      808_COWBELL  R8/14                                     O8                      R8/8                                                                               \nO |      909_SNARE_1 -R8/15----------------------------------------O8-------------------R8/7-------------------------------------------------------------------------------\nN |      909_SNARE_2  R8/16                                           O8                R8/6                                                                               \n         909_SNARE_3 -R8/17----------------------------------------------O8-------------R8/5-------------------------------------------------------------------------------\n         909_SNARE_4  R8/18                                                 O8          R8/4                                                                               \n            HIGH_TOM -R8/19----------------------------------------------------O8-------R8/3-------------------------------------------------------------------------------\n             MID_TOM  R8/20                                                       O8    R4                                                                                 \n             LOW_TOM -R8/21----------------------------------------------------------O8-R8---------------------------------------------------------------------------------\n            RIM_SHOT  R8/22                                                             O8                                                                                 \n";
	string Output="# Drums Demo\n#\n#\n";
	Output+=repeat_string(" ",6)+"CRASH_CYMBAL_1  O8"+repeat_string(" ",64)+"R8/22"+repeat_string(" ",78)+"\n";
	Output+=repeat_string(" ",6)+"CRASH_CYMBAL_2 -R8-O8"+repeat_string("-",61)+"R8/21"+repeat_string("-",78)+"\n";
	Output+=repeat_string(" ",6)+"REVERSE_CYMBAL  R4    O8"+repeat_string(" ",58)+"R8/20"+repeat_string(" ",78)+"\n";
	Output+=repeat_string(" ",6)+"   RIDE_CYMBAL -R8/3-----O8"+repeat_string("-",55)+"R8/19"+repeat_string("-",78)+"\n";
	Output+=repeat_string(" ",6)+"    808_CYMBAL  R8/4        O8"+repeat_string(" ",52)+"R8/18"+repeat_string(" ",78)+"\n";
	Output+=repeat_string(" ",6)+"  OPEN_HIHAT_1 -R8/5-----------O8"+repeat_string("-",49)+"R8/17"+repeat_string("-",78)+"\n";
	Output+=repeat_string(" ",6)+"  OPEN_HIHAT_2  R8/6              O8"+repeat_string(" ",46)+"R8/16"+repeat_string(" ",78)+"\n";
	Output+=               "P |   808_OPEN_HIHAT -R8/7-----------------O8"+repeat_string("-",43)+"R8/15"+repeat_string("-",78)+"\n";
	Output+=               "E |     CLOSED_HIHAT  R8/8                    O8"+repeat_string(" ",40)+"R8/14"+repeat_string(" ",78)+"\n";
	Output+=               "R | 808_CLOSED_HIHAT -R8/9-----------------------O8"+repeat_string("-",37)+"R8/13"+repeat_string("-",78)+"\n";
	Output+=               "C |       TAMBOURINE  R8/10                         O8"+repeat_string(" ",34)+"R8/12"+repeat_string(" ",78)+"\n";
	Output+=               "U |        BASS_DRUM -R8/11----------------------------O8"+repeat_string("-",31)+"R8/11"+repeat_string("-",78)+"\n";
	Output+=               "S |    808_BASS_DRUM  R8/12                               O8"+repeat_string(" ",28)+"R8/10"+repeat_string(" ",78)+"\n";
	Output+=               "S |        HAND_CLAP -R8/13----------------------------------O8"+repeat_string("-",25)+"R8/9"+repeat_string("-",78)+"\n";
	Output+=               "I |      808_COWBELL  R8/14                                     O8"+repeat_string(" ",22)+"R8/8"+repeat_string(" ",78)+"\n";
	Output+=               "O |      909_SNARE_1 -R8/15----------------------------------------O8"+repeat_string("-",19)+"R8/7"+repeat_string("-",78)+"\n";
	Output+=               "N |      909_SNARE_2  R8/16                                           O8"+repeat_string(" ",16)+"R8/6"+repeat_string(" ",78)+"\n";
	Output+=repeat_string(" ",6)+"   909_SNARE_3 -R8/17----------------------------------------------O8"+repeat_string("-",13)+"R8/5"+repeat_string("-",78)+"\n";
	Output+=repeat_string(" ",6)+"   909_SNARE_4  R8/18                                                 O8          R8/4"+repeat_string(" ",78)+"\n";
	Output+=repeat_string(" ",6)+"      HIGH_TOM -R8/19----------------------------------------------------O8-------R8/3"+repeat_string("-",78)+"\n";
	Output+=repeat_string(" ",6)+"       MID_TOM  R8/20                                                       O8    R4"+repeat_string(" ",78)+"\n";
	Output+=repeat_string(" ",6)+"       LOW_TOM -R8/21----------------------------------------------------------O8-R8"+repeat_string("-",78)+"\n";
	Output+=repeat_string(" ",6)+"      RIM_SHOT  R8/22                                                             O8"+repeat_string(" ",78)+"\n";
	fOut<<Output;
	fOut.close();}

void write_blank_misc_percussion_sound_file(string outFile)
	{int width=150;
	ofstream fOut;
	fOut.open(outFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_blank_misc_percussion_sound_file!\nCould not open sound file to write blank drums score.\n"<<outFile<<endl; exit(EXIT_FAILURE);}	
	string Output="# Title Here\n";
	Output+="#\n";
	Output+="#\n";
	Output+="    ROLL_HI_TRIANGLE "+repeat_string(" ",width)+"\n";
	Output+="   MUTED_HI_TRIANGLE "+repeat_string("-",width)+"\n";
	Output+="    OPEN_HI_TRIANGLE "+repeat_string(" ",width)+"\n";
	Output+="                GONG "+repeat_string("-",width)+"\n";
	Output+="            TAMTAM_A "+repeat_string(" ",width)+"\n";
	Output+="            TAMTAM_B "+repeat_string("-",width)+"\n";
	Output+="            TAMTAM_C "+repeat_string(" ",width)+"\n";
	Output+="   HI_FINGER_CYMBALS "+repeat_string("-",width)+"\n";
	Output+="   LO_FINGER_CYMBALS "+repeat_string(" ",width)+"\n";
	Output+="    SHAKE_TAMBOURINE "+repeat_string("-",width)+"\n";
	Output+="      HIT_TAMBOURINE "+repeat_string(" ",width)+"\n";
	Output+="     ROLL_TAMBOURINE "+repeat_string("-",width)+"\n";
	Output+="             COWBELL "+repeat_string(" ",width)+"\n";
	Output+="M |       VIBRA_SLAP "+repeat_string("-",width)+"\n";
	Output+="I |   HIT_HIGH_CONGA "+repeat_string(" ",width)+"\n";
	Output+="S |   TAP_HIGH_CONGA "+repeat_string("-",width)+"\n";
	Output+="C |    HIT_MID_CONGA "+repeat_string(" ",width)+"\n";
	Output+="  |    TAP_MID_CONGA "+repeat_string("-",width)+"\n";
	Output+="P |    HIT_LOW_CONGA "+repeat_string(" ",width)+"\n";
	Output+="E |    TAP_LOW_CONGA "+repeat_string("-",width)+"\n";
	Output+="R |           CABASA "+repeat_string(" ",width)+"\n";
	Output+="C |           SHAKER "+repeat_string("-",width)+"\n";
	Output+="U |    CRANK_RATCHET "+repeat_string(" ",width)+"\n";
	Output+="S |     FAST_RATCHET "+repeat_string("-",width)+"\n";
	Output+="S |     SLOW_RATCHET "+repeat_string(" ",width)+"\n";
	Output+="I |        CASTANETS "+repeat_string("-",width)+"\n";
	Output+="O |           CLAVES "+repeat_string(" ",width)+"\n";
	Output+="N |    HI_WOOD_BLOCK "+repeat_string("-",width)+"\n";
	Output+="       LO_WOOD_BLOCK "+repeat_string(" ",width)+"\n";
	Output+="    ROLL_LO_TRIANGLE "+repeat_string("-",width)+"\n";
	Output+="   MUTED_LO_TRIANGLE "+repeat_string(" ",width)+"\n";
	Output+="    OPEN_LO_TRIANGLE "+repeat_string("-",width)+"\n";
	Output+="        SLEIGH_BELLS "+repeat_string(" ",width)+"\n";
	Output+="         BELL_TREE_1 "+repeat_string("-",width)+"\n";
	Output+="         BELL_TREE_2 "+repeat_string(" ",width)+"\n";
	Output+="         BELL_TREE_3 "+repeat_string("-",width)+"\n";
	Output+="         BELL_TREE_4 "+repeat_string(" ",width)+"\n";
	Output+="         BELL_TREE_5 "+repeat_string("-",width)+"\n";
	Output+="        BAR_CHIMES_A "+repeat_string(" ",width)+"\n";
	Output+="        BAR_CHIMES_B "+repeat_string("-",width)+"\n";
	Output+="        BAR_CHIMES_C "+repeat_string(" ",width)+"\n";
	fOut<<Output;
	fOut.close();}

void write_demo_misc_percussion_sound_file(string outFile)
	{ofstream fOut;
	fOut.open(outFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_demo_misc_percussion_sound_file!\nCould not open sound file to write blank drums score.\n"<<outFile<<endl; exit(EXIT_FAILURE);}
	//string Output="# Miscellaneous Percussion Demo\n#\n#\n    ROLL_HI_TRIANGLE  O8                                                                                                                      R8/40                        \n   MUTED_HI_TRIANGLE -R8-O8-------------------------------------------------------------------------------------------------------------------R8/39------------------------\n    OPEN_HI_TRIANGLE  R4    O8                                                                                                                R8/38                        \n                GONG -R8/3-----O8-------------------------------------------------------------------------------------------------------------R8/37------------------------\n            TAMTAM_A  R8/4        O8                                                                                                          R8/36                        \n            TAMTAM_B -R8/5-----------O8-------------------------------------------------------------------------------------------------------R8/35------------------------\n            TAMTAM_C  R8/6              O8                                                                                                    R8/34                        \n   HI_FINGER_CYMBALS -R8/7-----------------O8-------------------------------------------------------------------------------------------------R8/33------------------------\n   LO_FINGER_CYMBALS  R8/8                    O8                                                                                              R8/32                        \n    SHAKE_TAMBOURINE -R8/9-----------------------O8-------------------------------------------------------------------------------------------R8/31------------------------\n      HIT_TAMBOURINE  R8/10                         O8                                                                                        R8/30                        \n     ROLL_TAMBOURINE -R8/11----------------------------O8-------------------------------------------------------------------------------------R8/29------------------------\n             COWBELL  R8/12                               O8                                                                                  R8/28                        \nM |       VIBRA_SLAP -R8/13----------------------------------O8-------------------------------------------------------------------------------R8/27------------------------\nI |   HIT_HIGH_CONGA  R8/14                                     O8                                                                            R8/26                        \nS |   TAP_HIGH_CONGA -R8/15----------------------------------------O8-------------------------------------------------------------------------R8/25------------------------\nC |    HIT_MID_CONGA  R8/16                                           O8                                                                      R8/24                        \n  |    TAP_MID_CONGA -R8/17----------------------------------------------O8-------------------------------------------------------------------R8/23------------------------\nP |    HIT_LOW_CONGA  R8/18                                                 O8                                                                R8/22                        \nE |    TAP_LOW_CONGA -R8/19----------------------------------------------------O8-------------------------------------------------------------R8/21------------------------\nR |           CABASA  R8/20                                                       O8                                                          R8/20                        \nC |           SHAKER -R8/21----------------------------------------------------------O8-------------------------------------------------------R8/19------------------------\nU |    CRANK_RATCHET  R8/22                                                             O8                                                    R8/18                        \nS |     FAST_RATCHET -R8/23----------------------------------------------------------------O8-------------------------------------------------R8/17------------------------\nS |     SLOW_RATCHET  R8/24                                                                   O8                                              R8/16                        \nI |        CASTANETS -R8/25----------------------------------------------------------------------O8-------------------------------------------R8/15------------------------\nO |           CLAVES  R8/26                                                                         O8                                        R8/14                        \nN |    HI_WOOD_BLOCK -R8/27----------------------------------------------------------------------------O8-------------------------------------R8/13------------------------\n       LO_WOOD_BLOCK  R8/28                                                                               O8                                  R8/12                        \n    ROLL_LO_TRIANGLE -R8/29----------------------------------------------------------------------------------O8-------------------------------R8/11------------------------\n   MUTED_LO_TRIANGLE  R8/30                                                                                     O8                            R8/10                        \n    OPEN_LO_TRIANGLE -R8/31----------------------------------------------------------------------------------------O8-------------------------R8/9-------------------------\n        SLEIGH_BELLS  R8/32                                                                                           O8                      R8/8                         \n         BELL_TREE_1 -R8/33----------------------------------------------------------------------------------------------O8-------------------R8/7-------------------------\n         BELL_TREE_2  R8/34                                                                                                 O8                R8/6                         \n         BELL_TREE_3 -R8/35----------------------------------------------------------------------------------------------------O8-------------R8/5-------------------------\n         BELL_TREE_4  R8/36                                                                                                       O8          R8/4                         \n         BELL_TREE_5 -R8/37----------------------------------------------------------------------------------------------------------O8-------R8/3-------------------------\n        BAR_CHIMES_A  R8/38                                                                                                             O8    R4                           \n        BAR_CHIMES_B -R8/39----------------------------------------------------------------------------------------------------------------O8-R8---------------------------\n        BAR_CHIMES_C  R8/40                                                                                                                   O8                           \n";
	string Output="# Miscellaneous Percussion Demo\n#\n#\n";
	Output+="    ROLL_HI_TRIANGLE  O8"+repeat_string(" ",118)+"R8/40"+repeat_string(" ",24)+"\n";
	Output+="   MUTED_HI_TRIANGLE -R8-O8"+repeat_string("-",115)+"R8/39"+repeat_string("-",24)+"\n";
	Output+="    OPEN_HI_TRIANGLE  R4    O8"+repeat_string(" ",112)+"R8/38"+repeat_string(" ",24)+"\n";
	Output+="                GONG -R8/3-----O8"+repeat_string("-",109)+"R8/37"+repeat_string("-",24)+"\n";
	Output+="            TAMTAM_A  R8/4        O8"+repeat_string(" ",106)+"R8/36"+repeat_string(" ",24)+"\n";
	Output+="            TAMTAM_B -R8/5-----------O8"+repeat_string("-",103)+"R8/35"+repeat_string("-",24)+"\n";
	Output+="            TAMTAM_C  R8/6              O8"+repeat_string(" ",100)+"R8/34"+repeat_string(" ",24)+"\n";
	Output+="   HI_FINGER_CYMBALS -R8/7-----------------O8"+repeat_string("-",97)+"R8/33"+repeat_string("-",24)+"\n";
	Output+="   LO_FINGER_CYMBALS  R8/8                    O8"+repeat_string(" ",94)+"R8/32"+repeat_string(" ",24)+"\n";
	Output+="    SHAKE_TAMBOURINE -R8/9-----------------------O8"+repeat_string("-",91)+"R8/31"+repeat_string("-",24)+"\n";
	Output+="      HIT_TAMBOURINE  R8/10                         O8"+repeat_string(" ",88)+"R8/30"+repeat_string(" ",24)+"\n";
	Output+="     ROLL_TAMBOURINE -R8/11----------------------------O8"+repeat_string("-",85)+"R8/29"+repeat_string("-",24)+"\n";
	Output+="             COWBELL  R8/12                               O8"+repeat_string(" ",82)+"R8/28"+repeat_string(" ",24)+"\n";
	Output+="M |       VIBRA_SLAP -R8/13----------------------------------O8"+repeat_string("-",79)+"R8/27"+repeat_string("-",24)+"\n";
	Output+="I |   HIT_HIGH_CONGA  R8/14                                     O8"+repeat_string(" ",76)+"R8/26"+repeat_string(" ",24)+"\n";
	Output+="S |   TAP_HIGH_CONGA -R8/15"+repeat_string("-",40)+"O8"+repeat_string("-",73)+"R8/25"+repeat_string("-",24)+"\n";
	Output+="C |    HIT_MID_CONGA  R8/16"+repeat_string(" ",43)+"O8"+repeat_string(" ",70)+"R8/24"+repeat_string(" ",24)+"\n";
	Output+="  |    TAP_MID_CONGA -R8/17"+repeat_string("-",46)+"O8"+repeat_string("-",67)+"R8/23"+repeat_string("-",24)+"\n";
	Output+="P |    HIT_LOW_CONGA  R8/18"+repeat_string(" ",49)+"O8"+repeat_string(" ",64)+"R8/22"+repeat_string(" ",24)+"\n";
	Output+="E |    TAP_LOW_CONGA -R8/19"+repeat_string("-",52)+"O8"+repeat_string("-",61)+"R8/21"+repeat_string("-",24)+"\n";
	Output+="R |           CABASA  R8/20"+repeat_string(" ",55)+"O8"+repeat_string(" ",58)+"R8/20"+repeat_string(" ",24)+"\n";
	Output+="C |           SHAKER -R8/21"+repeat_string("-",58)+"O8"+repeat_string("-",55)+"R8/19"+repeat_string("-",24)+"\n";
	Output+="U |    CRANK_RATCHET  R8/22"+repeat_string(" ",61)+"O8"+repeat_string(" ",52)+"R8/18"+repeat_string(" ",24)+"\n";
	Output+="S |     FAST_RATCHET -R8/23"+repeat_string("-",64)+"O8"+repeat_string("-",49)+"R8/17"+repeat_string("-",24)+"\n";
	Output+="S |     SLOW_RATCHET  R8/24"+repeat_string(" ",67)+"O8"+repeat_string(" ",46)+"R8/16"+repeat_string(" ",24)+"\n";
	Output+="I |        CASTANETS -R8/25"+repeat_string("-",70)+"O8"+repeat_string("-",43)+"R8/15"+repeat_string("-",24)+"\n";
	Output+="O |           CLAVES  R8/26"+repeat_string(" ",73)+"O8"+repeat_string(" ",40)+"R8/14"+repeat_string(" ",24)+"\n";
	Output+="N |    HI_WOOD_BLOCK -R8/27"+repeat_string("-",76)+"O8"+repeat_string("-",37)+"R8/13"+repeat_string("-",24)+"\n";
	Output+="       LO_WOOD_BLOCK  R8/28"+repeat_string(" ",79)+"O8"+repeat_string(" ",34)+"R8/12"+repeat_string(" ",24)+"\n";
	Output+="    ROLL_LO_TRIANGLE -R8/29"+repeat_string("-",82)+"O8"+repeat_string("-",31)+"R8/11"+repeat_string("-",24)+"\n";
	Output+="   MUTED_LO_TRIANGLE  R8/30"+repeat_string(" ",85)+"O8"+repeat_string(" ",28)+"R8/10"+repeat_string(" ",24)+"\n";
	Output+="    OPEN_LO_TRIANGLE -R8/31"+repeat_string("-",88)+"O8"+repeat_string("-",25)+"R8/9"+repeat_string("-",24)+"\n";
	Output+="        SLEIGH_BELLS  R8/32"+repeat_string(" ",91)+"O8"+repeat_string(" ",22)+"R8/8"+repeat_string(" ",24)+"\n";
	Output+="         BELL_TREE_1 -R8/33"+repeat_string("-",94)+"O8"+repeat_string("-",19)+"R8/7"+repeat_string("-",24)+"\n";
	Output+="         BELL_TREE_2  R8/34"+repeat_string(" ",97)+"O8"+repeat_string(" ",16)+"R8/6"+repeat_string(" ",24)+"\n";
	Output+="         BELL_TREE_3 -R8/35"+repeat_string("-",100)+"O8"+repeat_string("-",13)+"R8/5"+repeat_string("-",24)+"\n";
	Output+="         BELL_TREE_4  R8/36"+repeat_string(" ",103)+"O8"+repeat_string(" ",10)+"R8/4"+repeat_string(" ",24)+"\n";
	Output+="         BELL_TREE_5 -R8/37"+repeat_string("-",106)+"O8"+repeat_string("-",7)+"R8/3"+repeat_string("-",24)+"\n";
	Output+="        BAR_CHIMES_A  R8/38"+repeat_string(" ",109)+"O8    R4"+repeat_string(" ",24)+"\n";
	Output+="        BAR_CHIMES_B -R8/39"+repeat_string("-",112)+"O8-R8"+repeat_string("-",24)+"\n";
	Output+="        BAR_CHIMES_C  R8/40"+repeat_string(" ",115)+"O8"+repeat_string(" ",27)+"\n";
	fOut<<Output;
	fOut.close();}

void write_blank_music_score_sound_file(string soundFile)
	{int width=150;
	ofstream fOut;
	fOut.open(soundFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_blank_music_score_sound_file!\nCould not open sound file to write blank music score.\n"<<soundFile<<endl; exit(EXIT_FAILURE);}	
	string Output="";
	Output+="# Title Here\n";
	Output+="#\n";
	Output+="#\n";
	Output+=repeat_string(" ",4)+"G6 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"F6 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"E6 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"D6 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"C6 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"B5 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"A5 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"G5 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"F5 "+repeat_string("-",width)+"\n";
	Output+="T | E5 "+repeat_string(" ",width)+"\n";
	Output+="R | D5 "+repeat_string("-",width)+"\n";
	Output+="E | C5 "+repeat_string(" ",width)+"\n";
	Output+="B | B4 "+repeat_string("-",width)+"\n";
	Output+="L | A4 "+repeat_string(" ",width)+"\n";
	Output+="E | G4 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",4)+"F4 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"E4 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",4)+"D4 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"C4 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"B3 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"A3 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",4)+"G3 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"F3 "+repeat_string("-",width)+"\n";
	Output+="B | E3 "+repeat_string(" ",width)+"\n";
	Output+="A | D3 "+repeat_string("-",width)+"\n";
	Output+="S | C3 "+repeat_string(" ",width)+"\n";
	Output+="S | B2 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",4)+"A2 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"G2 "+repeat_string("-",width)+"\n";
	Output+=repeat_string(" ",4)+"F2 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"E2 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"D2 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"C2 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"B1 "+repeat_string(" ",width)+"\n";
	Output+=repeat_string(" ",4)+"A1 "+repeat_string(" ",width)+"\n\n";
	fOut<<Output;
	fOut.close();}

void write_blank_simple_drums_score_sound_file(string outFile)
	{int width=150;
	ofstream fOut;
	fOut.open(outFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_blank_drums_score_sound_file!\nCould not open sound file to write blank drums score.\n"<<outFile<<endl; exit(EXIT_FAILURE);}	
	string Output="";
	Output+="# Title Here\n";
	Output+="#\n";
	Output+="#\n";
	Output+="            BASS_DRUM_1 "+repeat_string(" ",width)+"\n";
	Output+="            BASS_DRUM_2 "+repeat_string("-",width)+"\n";
	Output+="       LEFT_HIT_SNARE_1 "+repeat_string(" ",width)+"\n";
	Output+="      ROUND_HIT_SNARE_1 "+repeat_string("-",width)+"\n";
	Output+="      RIGHT_HIT_SNARE_1 "+repeat_string(" ",width)+"\n";
	Output+="P |        ROLL_SNARE_1 "+repeat_string("-",width)+"\n";
	Output+="E |    LEFT_HIT_SNARE_2 "+repeat_string(" ",width)+"\n";
	Output+="R |   ROUND_HIT_SNARE_2 "+repeat_string("-",width)+"\n";
	Output+="C |   RIGHT_HIT_SNARE_2 "+repeat_string(" ",width)+"\n";
	Output+="U |         TAP_SNARE_2 "+repeat_string("-",width)+"\n";
	Output+="S |        ROLL_SNARE_2 "+repeat_string(" ",width)+"\n";
	Output+="S |    ROLL_A_CYMBALS_1 "+repeat_string("-",width)+"\n";
	Output+="I |  LO_CRASH_CYMBALS_1 "+repeat_string(" ",width)+"\n";
	Output+="O |    ROLL_B_CYMBALS_1 "+repeat_string("-",width)+"\n";
	Output+="N |  HI_CRASH_CYMBALS_1 "+repeat_string(" ",width)+"\n";
	Output+="   SHORT_ROLL_CYMBALS_2 "+repeat_string("-",width)+"\n";
	Output+="     LO_CRASH_CYMBALS_2 "+repeat_string(" ",width)+"\n";
	Output+="     MID_ROLL_CYMBALS_2 "+repeat_string("-",width)+"\n";
	Output+="    LONG_ROLL_CYMBALS_2 "+repeat_string(" ",width)+"\n";
	Output+="     HI_CRASH_CYMBALS_2 "+repeat_string("-",width)+"\n";
	fOut<<Output;
	fOut.close();}

void write_demo_simple_drums_score_sound_file(string outFile)
	{ofstream fOut;
	fOut.open(outFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_demo_misc_percussion_sound_file!\nCould not open sound file to write blank drums score.\n"<<outFile<<endl; exit(EXIT_FAILURE);}
	//string Output="# Simple Drums Demo\n#\n#\n            BASS_DRUM_1  O8                                                       R8/19                                                                                       \n            BASS_DRUM_2 -R8-O8----------------------------------------------------R8/18---------------------------------------------------------------------------------------\n       LEFT_HIT_SNARE_1  R4    O8                                                 R8/17                                                                                       \n      ROUND_HIT_SNARE_1 -R8/3-----O8----------------------------------------------R8/16---------------------------------------------------------------------------------------\n      RIGHT_HIT_SNARE_1  R8/4        O8                                           R8/15                                                                                       \nP |        ROLL_SNARE_1 -R8/5-----------O8----------------------------------------R8/14---------------------------------------------------------------------------------------\nE |    LEFT_HIT_SNARE_2  R8/6              O8                                     R8/13                                                                                       \nR |   ROUND_HIT_SNARE_2 -R8/7-----------------O8----------------------------------R8/12---------------------------------------------------------------------------------------\nC |   RIGHT_HIT_SNARE_2  R8/8                    O8                               R8/11                                                                                       \nU |         TAP_SNARE_2 -R8/9-----------------------O8----------------------------R8/10---------------------------------------------------------------------------------------\nS |        ROLL_SNARE_2  R8/10                         O8                         R8/9                                                                                        \nS |    ROLL_A_CYMBALS_1 -R8/11----------------------------O8----------------------R8/8----------------------------------------------------------------------------------------\nI |  LO_CRASH_CYMBALS_1  R8/12                               O8                   R8/7                                                                                        \nO |    ROLL_B_CYMBALS_1 -R8/13----------------------------------O8----------------R8/6----------------------------------------------------------------------------------------\nN |  HI_CRASH_CYMBALS_1  R8/14                                     O8             R8/5                                                                                        \n   SHORT_ROLL_CYMBALS_2 -R8/15----------------------------------------O8----------R8/4----------------------------------------------------------------------------------------\n     LO_CRASH_CYMBALS_2  R8/16                                           O8       R8/3                                                                                        \n     MID_ROLL_CYMBALS_2 -R8/17----------------------------------------------O8----R4------------------------------------------------------------------------------------------\n    LONG_ROLL_CYMBALS_2  R8/18                                                 O8 R8                                                                                          \n     HI_CRASH_CYMBALS_2 -R8/19----------------------------------------------------O8------------------------------------------------------------------------------------------\n";
	string Output="# Simple Drums Demo\n#\n#\n";
	Output+="            BASS_DRUM_1  O8"+repeat_string(" ",55)+"R8/19"+repeat_string(" ",87)+"\n";
	Output+="            BASS_DRUM_2 -R8-O8"+repeat_string("-",52)+"R8/18"+repeat_string("-",87)+"\n";
	Output+="       LEFT_HIT_SNARE_1  R4    O8"+repeat_string(" ",49)+"R8/17"+repeat_string(" ",87)+"\n";
	Output+="      ROUND_HIT_SNARE_1 -R8/3-----O8"+repeat_string("-",46)+"R8/16"+repeat_string("-",87)+"\n";
	Output+="      RIGHT_HIT_SNARE_1  R8/4        O8"+repeat_string(" ",43)+"R8/15"+repeat_string(" ",87)+"\n";
	Output+="P |        ROLL_SNARE_1 -R8/5-----------O8"+repeat_string("-",40)+"R8/14"+repeat_string("-",87)+"\n";
	Output+="E |    LEFT_HIT_SNARE_2  R8/6              O8"+repeat_string(" ",37)+"R8/13"+repeat_string(" ",87)+"\n";
	Output+="R |   ROUND_HIT_SNARE_2 -R8/7-----------------O8"+repeat_string("-",34)+"R8/12"+repeat_string("-",87)+"\n";
	Output+="C |   RIGHT_HIT_SNARE_2  R8/8                    O8"+repeat_string(" ",31)+"R8/11"+repeat_string(" ",87)+"\n";
	Output+="U |         TAP_SNARE_2 -R8/9-----------------------O8"+repeat_string("-",28)+"R8/10"+repeat_string("-",87)+"\n";
	Output+="S |        ROLL_SNARE_2  R8/10                         O8"+repeat_string(" ",25)+"R8/9"+repeat_string(" ",87)+"\n";
	Output+="S |    ROLL_A_CYMBALS_1 -R8/11----------------------------O8"+repeat_string("-",22)+"R8/8"+repeat_string("-",87)+"\n";
	Output+="I |  LO_CRASH_CYMBALS_1  R8/12                               O8"+repeat_string(" ",19)+"R8/7"+repeat_string(" ",87)+"\n";
	Output+="O |    ROLL_B_CYMBALS_1 -R8/13----------------------------------O8"+repeat_string("-",16)+"R8/6"+repeat_string("-",87)+"\n";
	Output+="N |  HI_CRASH_CYMBALS_1  R8/14                                     O8"+repeat_string(" ",13)+"R8/5"+repeat_string(" ",87)+"\n";
	Output+="   SHORT_ROLL_CYMBALS_2 -R8/15----------------------------------------O8"+repeat_string("-",10)+"R8/4"+repeat_string("-",87)+"\n";
	Output+="     LO_CRASH_CYMBALS_2  R8/16                                           O8"+repeat_string(" ",7)+"R8/3"+repeat_string(" ",87)+"\n";
	Output+="     MID_ROLL_CYMBALS_2 -R8/17----------------------------------------------O8----R4"+repeat_string("-",87)+"\n";
	Output+="    LONG_ROLL_CYMBALS_2  R8/18                                                 O8 R8"+repeat_string(" ",87)+"\n";
	Output+="     HI_CRASH_CYMBALS_2 -R8/19----------------------------------------------------O8"+repeat_string("-",87)+"\n";
	fOut<<Output;
	fOut.close();}

void write_expanded_music_input_file(string inFile,string blockNameLst,string blockDefLst,int numBlocks)
	{string delimiter=GLOBAL_DELIMITER;
	string *blocks=fill_string_array(blockNameLst,numBlocks,delimiter);
	string *blockValues=fill_string_array(blockDefLst,numBlocks,delimiter);
	int pos,pos2;
	// Expand Block Variables to be Written Out
	string Output="",currentBlock,currentBlockValue,tmp;
	bool SEARCHING;
	for(int a=0;a<numBlocks;a++)
		{currentBlock=blocks[a];
		currentBlockValue=remove_excess_white_space(blockValues[a]);
		for(int b=0;b<numBlocks;b++)
			{if(a!=b)
				{// Search for Other Blocks in Block
				pos=currentBlockValue.find(blocks[b],0);
				if(pos!=string::npos)
					{// Found, so expand with value
					SEARCHING=true;
					while(SEARCHING)
						{// Handle Extra White Space
						tmp=currentBlockValue.substr(pos+blocks[b].length(),1);
						if(tmp.compare(" ")==0)
							{currentBlockValue.replace(pos,blocks[b].length()+1,remove_excess_white_space(blockValues[b]));}
						else{currentBlockValue.replace(pos,blocks[b].length(),remove_excess_white_space(blockValues[b]));}
						pos=currentBlockValue.find(blocks[b],pos+1);
						if(pos==string::npos){SEARCHING=false; break;}
						}
					}
				}
			}
		// Update Block Values
		blockValues[a]=currentBlockValue;
		}
	// Last Block is the Output
	Output=blockValues[numBlocks-1];
	// Clean Up
	int numLines=count_delimiter(Output,"\n");
	pos=0; pos2=0;
	for(int k=0;k<numLines;k++)
		{// Extract Line
		pos2=Output.find("\n",pos);
		tmp=Output.substr(pos,pos2-pos-1);
		if(tmp.length()<=3){Output.replace(pos,tmp.length(),"#");}
		pos=pos2+1;					
		}
	//cout<<Output<<endl;
	//tmpFile=songFiles[i]+"expanded";
	ofstream fOut;
	fOut.open(inFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_expanded_music_input_file!\nTemporary input song file could not be opened.\n"<<inFile<<endl; exit(EXIT_FAILURE);}
	fOut<<Output<<endl;
	fOut.close();
	}

void write_gnuplot_scripts(string dataFile,double endTime)
	{string animationFldr=get_current_dir_name(); animationFldr+="/ANIMATION/";
	ofstream fOut;
	string animationFile=animationFldr+"animation.plt";
	// Write Animation Script (animation.plt)
	string Output="";
	Output+="#!/usr/bin/gnuplot\n\n\n";
	Output+="reset\n";
	Output+="# Specify output to generate\n";
	Output+="set terminal pngcairo size 1920,1080 enhanced font \'Verdana,10\' background rgb \"black\"\n\n";
	Output+="# color definitions\n";
	Output+="set palette rgb 3,9,9\n\n";
	Output+="# Hide Legend & Other things\n";
	Output+="unset key; unset colorbox; unset border; unset tics\n";
	Output+="set lmargin at screen 0.03\n";
	Output+="set bmargin at screen 0\n";
	Output+="set rmargin at screen 0.97\n";
	Output+="set tmargin at screen 1\n\n";
	Output+="set xrange [-0.05:0.15]\n";
	Output+="set yrange [-1.1:1.1]\n";
	Output+="set style data lines\n\n";
	Output+="t = 0\n";
	Output+="end_time = "+formatNumberString(cnvrtNumToStrng(endTime,SIG_FIGS))+"\n";
	Output+="plot \""+dataFile+"\" using 1:2 linetype 3 linecolor rgb \"white\" linewidth 1\n";
	//Output+="inputFile = \""+dataFile+"\"\n";
	Output+="load \'theShow.plt\'\n";
	fOut.open(animationFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_gnuplot_script\nOutput mp3 file could not be opened.\n"<<animationFile<<endl; exit(EXIT_FAILURE);}
	fOut<<Output;
	fOut.close();
	// Write Simulation Script (theShow.plt)
	string simulationFile=animationFldr+"theShow.plt";
	Output="#\n";
	Output+="t = t + 0.02\n\n";
	//Output+="plot \""+dataFile+"\" using 1:2 linetype 3 linecolor rgb \"white\" linewidth 1\n";
	Output+="set xrange [t-0.05:0.15+t]\n";
	Output+="replot\n";
	Output+="if(t<end_time) reread;\n";
	fOut.open(simulationFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_gnuplot_script\nOutput mp3 file could not be opened.\n"<<simulationFile<<endl; exit(EXIT_FAILURE);}
	fOut<<Output;
	fOut.close();
	}

void write_gnuplot_scripts_with_keys(string dataFile,string noteFile,double endTime,double xRange,string instrumentLabel,bool IS_DRUMS)
	{int Sz=LINE_SIZE,pos;
	char Val[Sz];
	//double xRange=0.8;
	string tmp,timeValue,timeLst="",noteValue,noteLst="",delimiter=GLOBAL_DELIMITER2,innerDelimiter=GLOBAL_DELIMITER;
	string command="";
	ofstream fOut;
	string animationFldr=extract_containing_folder(dataFile);
	string animationFile=animationFldr+"animation.plt";
	// Write Animation Script (animation.plt)
	string Output="";
	Output+="#!/usr/bin/gnuplot\n\n\n";
	Output+="reset\n";
	Output+="# Specify output to generate\n";
	Output+="set terminal pngcairo size 1920,1080 enhanced font \'Verdana,10\' background rgb \"black\"\n\n";
	Output+="# color definitions\n";
	Output+="set palette rgb 3,9,9\n\n";
	Output+="# Hide Legend & Other things\n";
	Output+="unset key; unset colorbox; unset border; unset tics\n";
	Output+="set lmargin at screen 0.03\n";
	Output+="set bmargin at screen 0\n";
	Output+="set rmargin at screen 0.97\n";
	Output+="set tmargin at screen 1\n\n";
	Output+="# Define Keyboard (A0-C8) [75 keys]\n";
	Output+="set macros\n";
	Output+="anchor=\"graph 0.5, char 1\"\n";
	Output+="rgbWhite=\"rgb 0xFFFFFF\"; rgbGrey=\"rgb 0x646464\"; rgbRed=\"rgb 0xFF0000\"; rgbGreen=\"rgb 0x00FF00\"; rgbBlue=\"rgb 0x0000FF\"; rgbBlack=\"rgb 0x000000\";\n";
	string keyColor="@rgbGreen", sharpKeyColor="@rgbGrey", pressKeyColor="@rgbWhite", pressKeyFontColor="@rgbBlack";
	Output+="keyFont=\"sans,16\"; keyWidth=5.25; keyHeight=2;\n";
	Output+="instrumentFont=\"sans,48\"\n";
	//Output+="instrFont=\"sans,40\"\n";
	Output+="shiftFactor=22\n";
	Output+="xOffset=0.032\n";
	if(!IS_DRUMS)
		{Output+=write_keyboard_labels(keyColor,sharpKeyColor);
		}
	Output+="# Instrument Label\n";
	Output+="set label 89 center at @anchor \"{/:Bold "+instrumentLabel+"}\" font instrumentFont textcolor @rgbBlue offset char 0,60\n";
	//
	//Output+="set xrange [-0.05:0.15]\n";
	Output+="set xrange [-0.05:"+formatNumberString(cnvrtNumToStrng(xRange-0.05,SIG_FIGS))+"]\n";
	Output+="set yrange [-1.1:1.1]\n";
	Output+="set style data lines\n\n";
	Output+="t = 0\n";
	Output+="end_time = "+formatNumberString(cnvrtNumToStrng(endTime,SIG_FIGS))+"\n";
	//Output+="plot \""+dataFile+"\" using 1:2 linetype 3 linecolor rgb \"white\" linewidth 1\n";
	bool SEARCHING=true;
	string otherDataFile;
	int Counter=1;
	otherDataFile=dataFile+"_"+cnvrtNumToStrng(Counter,0);
	if(!file_exist(otherDataFile)){Output+="plot \""+dataFile+"\" using 1:2 linetype 3 linecolor rgb \"white\" linewidth 1\n";}
	command="plot";
	while(SEARCHING)
		{otherDataFile=dataFile+"_"+cnvrtNumToStrng(Counter,0);
		if(file_exist(otherDataFile))
			{command+=" \""+otherDataFile+"\" using 1:2 linetype 3 linecolor rgb \"white\" linewidth 2,";
			Counter++;
			}
		else{SEARCHING=false; break;}
		}
	int numComponents=Counter-1;	
	double signalHeight,ampFactor,transFactor;
	string *instrLabels;
	if(command.length()>4)
		{// Define Font Size
		//if(numComponents==8){Output+="instrFont=\"sans,36\"\n";}
		//else if(numComponents==7){Output+="instrFont=\"sans,40\"\n";}
		//else{Output+="instrFont=\"sans,48\"\n";}
		Output+="instrFont=\"sans,36\"\n";
		// Plot Multi-Signal Plot
		Output+=command.substr(0,command.length()-1)+"\n";
		signalHeight=1.7/(double)numComponents; //signalHeight=2.0/(double)numComponents;
		ampFactor=signalHeight/2;
		instrLabels=new string[numComponents];
		instrLabels[0]="Right Hand"; instrLabels[1]="Left Hand";
		//instrLabels[0]="Brass"; instrLabels[1]="Bass"; instrLabels[2]="Piano"; instrLabels[3]="Guitar"; instrLabels[4]="Strings"; instrLabels[5]="Drums I"; instrLabels[6]="Drums II";
		//instrLabels[0]="Saxophone"; instrLabels[1]="1st Violin"; instrLabels[2]="2nd Violin"; instrLabels[3]="Guitar"; instrLabels[4]="Cello"; instrLabels[5]="Bass"; instrLabels[6]="Drums";
		//instrLabels[0]="Xylophone"; instrLabels[1]="Drums"; instrLabels[2]="Bass"; instrLabels[3]="Quick Hit"; instrLabels[4]="Low Strings"; instrLabels[5]="High Strings"; instrLabels[6]="Deport Hit"; instrLabels[7]="Justified Hit";
		for(int i=0;i<numComponents;i++)
			{transFactor=1-ampFactor-signalHeight*i;
			transFactor++;
			transFactor/=2;
			//Output+="set label "+cnvrtNumToStrng(i+500,0)+" front left at graph 0, graph "+formatNumberString(cnvrtNumToStrng(transFactor,SIG_FIGS))+" \"{/:Bold "+instrumentLabel+"}\" font instrumentFont textcolor @rgbBlue\n";
			Output+="set label "+cnvrtNumToStrng(i+500,0)+" front left at graph 0, graph "+formatNumberString(cnvrtNumToStrng(transFactor,SIG_FIGS))+" \"{/:Bold "+instrLabels[i]+"}\" font instrFont textcolor @rgbGreen\n";
			}
		}
	command="";
	//Output+="inputFile = \""+dataFile+"\"\n";
	Output+="load \'theShow.plt\'\n";
	fOut.open(animationFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_gnuplot_script_with_keys\nOutput mp3 file could not be opened.\n"<<animationFile<<endl; exit(EXIT_FAILURE);}
	fOut<<Output;
	fOut.close();
	// Write Simulation Script (theShow.plt)
	string simulationFile=animationFldr+"theShow.plt";
	Output="#\n";
	Output+="t = t + 0.02\n\n";
	//Output+="plot \""+dataFile+"\" using 1:2 linetype 3 linecolor rgb \"white\" linewidth 1\n";
	//Output+="set xrange [t-0.05:0.15+t]\n";
	Output+="set xrange [t-0.05:"+formatNumberString(cnvrtNumToStrng(xRange-0.05,SIG_FIGS))+"+t]\n";
	ifstream fIn;
	string *activeNotes,currentNotes,*time,startTime,finalTime,*theNotes,label,labelFile;
	int SongArraySize,numNotes,labelValue;
	if(!IS_DRUMS)
		{for(int i=0;i<88;i++)
			{Output+="set label "+cnvrtNumToStrng(i+1,0)+" textcolor "+keyColor+"; ";
			if(i+1==2 || i+1==5 || i+1==7 || i+1==10 || i+1==12 || i+1==14 || i+1==17 || i+1==19 || i+1==22 ||\
				i+1==24 || i+1==26 || i+1==29 || i+1==31 || i+1==34 || i+1==36 ||\
				i+1==38 || i+1==41 || i+1==43 || i+1==46 || i+1==48 || i+1==50 ||\
				i+1==53 || i+1==55 || i+1==58 || i+1==60 || i+1==62 || i+1==65 ||\
				i+1==67 || i+1==70 || i+1==72 || i+1==74 || i+1==77 || i+1==79 ||\
				i+1==82 || i+1==84 || i+1==86)
				{Output+="set label "+cnvrtNumToStrng(i+90,0)+" textcolor "+keyColor+"; ";
				Output+="set object "+cnvrtNumToStrng(i+1,0)+" rect fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3;\n";}
			else{Output+="set object "+cnvrtNumToStrng(i+1,0)+" rect fillstyle empty border 3;\n";}			
			}
		// Read Note File		
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
			if(noteValue.length()<=2||noteValue.compare("")==0){noteValue="R"+innerDelimiter;}
			noteLst+=noteValue+delimiter;
			//cout<<"|"<<timeValue<<"|"<<noteValue<<"|\n";
			fIn.getline(Val,Sz);}
		fIn.close();
		SongArraySize=count_delimiter(timeLst,delimiter);
		time=fill_string_array(timeLst,SongArraySize,delimiter);
		activeNotes=fill_string_array(noteLst,SongArraySize,delimiter);
		// if(t>=0 && t<0.06247166) set label 1 textcolor @rgbWhite; set label 50 textcolor @rgbWhite; set label 67 textcolor @rgbWhite;
		//cout<<noteLst<<endl;
		currentNotes=activeNotes[0]; startTime=time[0];
		for(int i=1;i<SongArraySize;i++)
			{if(activeNotes[i].compare(currentNotes)!=0)
				{// New Note Active
				finalTime=time[i];
				// Assess Notes
				numNotes=count_delimiter(currentNotes,innerDelimiter);
				theNotes=fill_string_array(currentNotes,numNotes,innerDelimiter);
				// Build Command
				command="";
				for(int j=0;j<numNotes;j++)
					{label=get_gnuplot_label_from_noteCode(theNotes[j]);
					//cout<<label<<"|"<<theNotes[j]<<endl;
					if(label.compare("-1")!=0)
						{labelValue=atoi(label.c_str());
						if(labelValue==2 || labelValue==5 || labelValue==7 || labelValue==10 || labelValue==12 || labelValue==14 || labelValue==17 || labelValue==19 ||\
							labelValue==22 || labelValue==24 || labelValue==26 || labelValue==29 || labelValue==31 || labelValue==34 || labelValue==36 ||\
							labelValue==38 || labelValue==41 || labelValue==43 || labelValue==46 || labelValue==48 || labelValue==50 || labelValue==53 ||\
							labelValue==55 || labelValue==58 || labelValue==60 || labelValue==62 || labelValue==65 || labelValue==67 || labelValue==70 ||\
							labelValue==72 || labelValue==74 || labelValue==77 || labelValue==79 || labelValue==82 || labelValue==84 || labelValue==86)
							{command+=" set label "+cnvrtNumToStrng(labelValue+89,0)+" textcolor "+pressKeyFontColor+";";}
						command+=" set label "+label+" textcolor "+pressKeyFontColor+";";
						command+=" set object "+label+" rect fillcolor "+pressKeyColor+" fillstyle solid 1.0;";}
					}
				delete [] theNotes;
				// Write Output
				if(command.length()>0){Output+="if(t>="+formatNumberString(startTime)+" && t<"+formatNumberString(finalTime)+")"+command+"\n";}
				// Update
				startTime=finalTime;
				currentNotes=activeNotes[i];
				}
			//cout<<i<<"/"<<SongArraySize<<"|"<<currentNotes<<"|"<<activeNotes[i]<<endl;
			}
		// Get Last Note
		finalTime=time[SongArraySize-1];
		// Assess Notes
		numNotes=count_delimiter(currentNotes,innerDelimiter);
		theNotes=fill_string_array(currentNotes,numNotes,innerDelimiter);
		// Build Command
		command="";
		for(int j=0;j<numNotes;j++)
			{label=get_gnuplot_label_from_noteCode(theNotes[j]);
			//cout<<label<<"|"<<theNotes[j]<<endl;
			if(label.compare("-1")!=0)
				{labelValue=atoi(label.c_str());
				if(labelValue==2 || labelValue==5 || labelValue==7 || labelValue==10 || labelValue==12 || labelValue==14 || labelValue==17 || labelValue==19 ||\
					labelValue==22 || labelValue==24 || labelValue==26 || labelValue==29 || labelValue==31 || labelValue==34 || labelValue==36 ||\
					labelValue==38 || labelValue==41 || labelValue==43 || labelValue==46 || labelValue==48 || labelValue==50 || labelValue==53 ||\
					labelValue==55 || labelValue==58 || labelValue==60 || labelValue==62 || labelValue==65 || labelValue==67 || labelValue==70 ||\
					labelValue==72 || labelValue==74 || labelValue==77 || labelValue==79 || labelValue==82 || labelValue==84 || labelValue==86)
					{command+=" set label "+cnvrtNumToStrng(labelValue+89,0)+" textcolor "+pressKeyFontColor+";";}
				command+=" set label "+label+" textcolor "+pressKeyFontColor+";";
				command+=" set object "+label+" rect fillcolor "+pressKeyColor+" fillstyle solid 1.0;";}
			}
		delete [] theNotes;
		// Write Output
		if(command.length()>0){Output+="if(t>="+formatNumberString(startTime)+" && t<"+formatNumberString(finalTime)+")"+command+"\n";}
		// Free Up Memory
		delete [] time; delete [] activeNotes;	
		}
	// Handle Labels File (if exists)
	// Define Labels File
	labelFile=extract_containing_folder(noteFile)+"labels";
	//cout<<labelFile<<endl;
	if(file_exist(labelFile))
		{// Read Label File		
		timeLst="";noteLst="";
		fIn.open(labelFile.c_str());
		if(fIn.fail()){cerr<<"ERROR !\nCould not open labels file.\n"<<labelFile<<endl; exit(EXIT_FAILURE);}		
		// Skip Header Line
		fIn.getline(Val,Sz);
		fIn.getline(Val,Sz);
		while(!fIn.eof())
			{tmp=Val;
			// Extract Time Value
			pos=tmp.find("\t",0);
			timeValue=tmp.substr(0,pos);
			timeLst+=timeValue+delimiter;
			// Extract Labels Value
			noteValue=tmp.substr(pos+1,tmp.length()-pos-1);
			if(noteValue.length()<=2||noteValue.compare("")==0){noteValue="R"+innerDelimiter;}
			noteLst+=noteValue+delimiter;
			//cout<<"|"<<timeValue<<"|"<<noteValue<<"|\n";
			fIn.getline(Val,Sz);}
		fIn.close();
		SongArraySize=count_delimiter(timeLst,delimiter);			
		time=fill_string_array(timeLst,SongArraySize,delimiter);		
		activeNotes=fill_string_array(noteLst,SongArraySize,delimiter);
		//
		currentNotes=activeNotes[0]; startTime=time[0];
		for(int i=1;i<SongArraySize;i++)
			{if(activeNotes[i].compare(currentNotes)!=0)
				{// New Note Active
				finalTime=time[i];
				// Assess Notes
				numNotes=count_delimiter(currentNotes,innerDelimiter);
				theNotes=fill_string_array(currentNotes,numNotes,innerDelimiter);
				// Build Command
				command="";
				for(int j=0;j<numNotes;j++)
					{command+=" set label 89 center at @anchor \"{/:Bold "+theNotes[j]+"}\" font instrumentFont textcolor @rgbBlue offset char 0,60";
					}
				delete [] theNotes;
				// Write Output
				if(command.length()>0){Output+="if(t>="+formatNumberString(startTime)+" && t<"+formatNumberString(finalTime)+")"+command+"\n";}
				// Update
				startTime=finalTime;
				currentNotes=activeNotes[i];
				}
			//cout<<i<<"/"<<SongArraySize<<"|"<<currentNotes<<"|"<<activeNotes[i]<<endl;
			}
		// Get Last Label
		finalTime=time[SongArraySize-1];
		// Assess Notes
		numNotes=count_delimiter(currentNotes,innerDelimiter);
		theNotes=fill_string_array(currentNotes,numNotes,innerDelimiter);
		// Build Command
		command="";
		for(int j=0;j<numNotes;j++)
			{command+=" set label 89 center at @anchor \"{/:Bold "+theNotes[j]+"}\" font instrumentFont textcolor @rgbBlue offset char 0,60\n";
			}
		delete [] theNotes;
		// Write Output
		if(command.length()>0){Output+="if(t>="+formatNumberString(startTime)+" && t<"+formatNumberString(finalTime)+")"+command+"\n";}
		}
	Output+="replot\n";
	Output+="if(t<end_time) reread;\n";
	fOut.open(simulationFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_gnuplot_script_with_keys\nOutput Simulation file could not be opened.\n"<<simulationFile<<endl; exit(EXIT_FAILURE);}
	fOut<<Output;
	fOut.close();
	}

string write_keyboard_labels(string keyColor,string sharpKeyColor)
	{string Output="";
	Output+="# 88 Key Labels\n";
	Output+="set label 1 front center at @anchor \"{/:Bold {/:Italic A_0}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-1)*keyWidth,2\n";
	Output+="set object 1 rectangle back at screen 0.009+xOffset, char 4.8 size char strlen(\"A_0\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 2 front center at @anchor \"{/:Bold {/:Italic A@^#_0}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-1.4)*keyWidth,2+keyHeight\n";
	Output+="set label 2+89 front center at @anchor \"{/:Bold {/:Italic B@^b_0}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-1.4)*keyWidth,2+2*keyHeight\n";
	Output+="set object 2 rectangle front at screen 0.018+xOffset, char 6 size char strlen(\"A_0\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 3 front center at @anchor \"{/:Bold {/:Italic B_0}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-1.8)*keyWidth,2\n";
	Output+="set object 3 rectangle back at screen 0.027+xOffset, char 4.8 size char strlen(\"B_0\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 4 front center at @anchor \"{/:Bold {/:Italic C_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-2.6)*keyWidth,2\n";
	Output+="set object 4 rectangle back at screen 0.045+xOffset, char 4.8 size char strlen(\"C_1\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 5 front center at @anchor \"{/:Bold {/:Italic C@^#_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-3.0)*keyWidth,2+keyHeight\n";
	Output+="set label 5+89 front center at @anchor \"{/:Bold {/:Italic D@^b_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-3.0)*keyWidth,2+2*keyHeight\n";
	Output+="set object 5 rectangle front at screen 0.054+xOffset, char 6 size char strlen(\"C_1\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 6 front center at @anchor \"{/:Bold {/:Italic D_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-3.5)*keyWidth,2\n";
	Output+="set object 6 rectangle back at screen 0.063+xOffset, char 4.8 size char strlen(\"D_1\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 7 front center at @anchor \"{/:Bold {/:Italic D@^#_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-3.9)*keyWidth,2+keyHeight\n";
	Output+="set label 7+89 front center at @anchor \"{/:Bold {/:Italic E@^b_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-3.9)*keyWidth,2+2*keyHeight\n";
	Output+="set object 7 rectangle front at screen 0.072+xOffset, char 6 size char strlen(\"D_1\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 8 front center at @anchor \"{/:Bold {/:Italic E_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-4.3)*keyWidth,2\n";
	Output+="set object 8 rectangle back at screen 0.081+xOffset, char 4.8 size char strlen(\"E_1\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 9 front center at @anchor \"{/:Bold {/:Italic F_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-5.1)*keyWidth,2\n";
	Output+="set object 9 rectangle back at screen 0.099+xOffset, char 4.8 size char strlen(\"F_1\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 10 front center at @anchor \"{/:Bold {/:Italic F@^#_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-5.5)*keyWidth,2+keyHeight\n";
	Output+="set label 10+89 front center at @anchor \"{/:Bold {/:Italic G@^b_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-5.5)*keyWidth,2+2*keyHeight\n";
	Output+="set object 10 rectangle front at screen 0.108+xOffset, char 6 size char strlen(\"F_1\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 11 front center at @anchor \"{/:Bold {/:Italic G_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-5.9)*keyWidth,2\n";
	Output+="set object 11 rectangle back at screen 0.117+xOffset, char 4.8 size char strlen(\"G_1\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 12 front center at @anchor \"{/:Bold {/:Italic G@^#_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-6.3)*keyWidth,2+keyHeight\n";
	Output+="set label 12+89 front center at @anchor \"{/:Bold {/:Italic A@^b_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-6.3)*keyWidth,2+2*keyHeight\n";
	Output+="set object 12 rectangle front at screen 0.126+xOffset, char 6 size char strlen(\"G_1\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 13 front center at @anchor \"{/:Bold {/:Italic A_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-6.8)*keyWidth,2\n";
	Output+="set object 13 rectangle back at screen 0.135+xOffset, char 4.8 size char strlen(\"A_1\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 14 front center at @anchor \"{/:Bold {/:Italic A@^#_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-7.2)*keyWidth,2+keyHeight\n";
	Output+="set label 14+89 front center at @anchor \"{/:Bold {/:Italic B@^b_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-7.2)*keyWidth,2+2*keyHeight\n";
	Output+="set object 14 rectangle front at screen 0.144+xOffset, char 6 size char strlen(\"A_1\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 15 front center at @anchor \"{/:Bold {/:Italic B_1}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-7.6)*keyWidth,2\n";
	Output+="set object 15 rectangle back at screen 0.153+xOffset, char 4.8 size char strlen(\"B_1\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 16 front center at @anchor \"{/:Bold {/:Italic C_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-8.4)*keyWidth,2\n";
	Output+="set object 16 rectangle back at screen 0.171+xOffset, char 4.8 size char strlen(\"C_2\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 17 front center at @anchor \"{/:Bold {/:Italic C@^#_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-8.8)*keyWidth,2+keyHeight\n";
	Output+="set label 17+89 front center at @anchor \"{/:Bold {/:Italic D@^b_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-8.8)*keyWidth,2+2*keyHeight\n";
	Output+="set object 17 rectangle front at screen 0.180+xOffset, char 6 size char strlen(\"C_2\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 18 front center at @anchor \"{/:Bold {/:Italic D_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-9.2)*keyWidth,2\n";
	Output+="set object 18 rectangle back at screen 0.189+xOffset, char 4.8 size char strlen(\"D_2\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 19 front center at @anchor \"{/:Bold {/:Italic D@^#_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-9.6)*keyWidth,2+keyHeight\n";
	Output+="set label 19+89 front center at @anchor \"{/:Bold {/:Italic E@^b_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-9.6)*keyWidth,2+2*keyHeight\n";
	Output+="set object 19 rectangle front at screen 0.198+xOffset, char 6 size char strlen(\"D_2\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 20 front center at @anchor \"{/:Bold {/:Italic E_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-10)*keyWidth,2\n";
	Output+="set object 20 rectangle back at screen 0.207+xOffset, char 4.8 size char strlen(\"E_2\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 21 front center at @anchor \"{/:Bold {/:Italic F_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-10.8)*keyWidth,2\n";
	Output+="set object 21 rectangle back at screen 0.225+xOffset, char 4.8 size char strlen(\"F_2\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 22 front center at @anchor \"{/:Bold {/:Italic F@^#_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-11.25)*keyWidth,2+keyHeight\n";
	Output+="set label 22+89 front center at @anchor \"{/:Bold {/:Italic G@^b_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-11.25)*keyWidth,2+2*keyHeight\n";
	Output+="set object 22 rectangle front at screen 0.234+xOffset, char 6 size char strlen(\"F_2\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 23 front center at @anchor \"{/:Bold {/:Italic G_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-11.65)*keyWidth,2\n";
	Output+="set object 23 rectangle back at screen 0.243+xOffset, char 4.8 size char strlen(\"G_2\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 24 front center at @anchor \"{/:Bold {/:Italic G@^#_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-12.05)*keyWidth,2+keyHeight\n";
	Output+="set label 24+89 front center at @anchor \"{/:Bold {/:Italic A@^b_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-12.05)*keyWidth,2+2*keyHeight\n";
	Output+="set object 24 rectangle front at screen 0.252+xOffset, char 6 size char strlen(\"G_2\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 25 front center at @anchor \"{/:Bold {/:Italic A_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-12.5)*keyWidth,2\n";
	Output+="set object 25 rectangle back at screen 0.261+xOffset, char 4.8 size char strlen(\"A_2\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 26 front center at @anchor \"{/:Bold {/:Italic A@^#_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-12.9)*keyWidth,2+keyHeight\n";
	Output+="set label 26+89 front center at @anchor \"{/:Bold {/:Italic B@^b_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-12.9)*keyWidth,2+2*keyHeight\n";
	Output+="set object 26 rectangle front at screen 0.270+xOffset, char 6 size char strlen(\"A_2\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 27 front center at @anchor \"{/:Bold {/:Italic B_2}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-13.3)*keyWidth,2\n";
	Output+="set object 27 rectangle back at screen 0.279+xOffset, char 4.8 size char strlen(\"B_2\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 28 front center at @anchor \"{/:Bold {/:Italic C_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-14.15)*keyWidth,2\n";
	Output+="set object 28 rectangle back at screen 0.297+xOffset, char 4.8 size char strlen(\"C_3\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 29 front center at @anchor \"{/:Bold {/:Italic C@^#_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-14.55)*keyWidth,2+keyHeight\n";
	Output+="set label 29+89 front center at @anchor \"{/:Bold {/:Italic D@^b_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-14.55)*keyWidth,2+2*keyHeight\n";
	Output+="set object 29 rectangle front at screen 0.306+xOffset, char 6 size char strlen(\"C_3\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 30 front center at @anchor \"{/:Bold {/:Italic D_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-14.95)*keyWidth,2\n";
	Output+="set object 30 rectangle back at screen 0.315+xOffset, char 4.8 size char strlen(\"D_3\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 31 front center at @anchor \"{/:Bold {/:Italic D@^#_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-15.4)*keyWidth,2+keyHeight\n";
	Output+="set label 31+89 front center at @anchor \"{/:Bold {/:Italic E@^b_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-15.4)*keyWidth,2+2*keyHeight\n";
	Output+="set object 31 rectangle front at screen 0.324+xOffset, char 6 size char strlen(\"D_3\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 32 front center at @anchor \"{/:Bold {/:Italic E_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-15.8)*keyWidth,2\n";
	Output+="set object 32 rectangle back at screen 0.333+xOffset, char 4.8 size char strlen(\"E_3\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 33 front center at @anchor \"{/:Bold {/:Italic F_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-16.6)*keyWidth,2\n";
	Output+="set object 33 rectangle back at screen 0.351+xOffset, char 4.8 size char strlen(\"F_3\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 34 front center at @anchor \"{/:Bold {/:Italic F@^#_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-17)*keyWidth,2+keyHeight\n";
	Output+="set label 34+89 front center at @anchor \"{/:Bold {/:Italic G@^b_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-17)*keyWidth,2+2*keyHeight\n";
	Output+="set object 34 rectangle front at screen 0.360+xOffset, char 6 size char strlen(\"F_3\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 35 front center at @anchor \"{/:Bold {/:Italic G_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-17.4)*keyWidth,2\n";
	Output+="set object 35 rectangle back at screen 0.369+xOffset, char 4.8 size char strlen(\"G_3\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 36 front center at @anchor \"{/:Bold {/:Italic G@^#_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-17.8)*keyWidth,2+keyHeight\n";
	Output+="set label 36+89 front center at @anchor \"{/:Bold {/:Italic A@^b_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-17.8)*keyWidth,2+2*keyHeight\n";
	Output+="set object 36 rectangle front at screen 0.378+xOffset, char 6 size char strlen(\"G_3\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 37 front center at @anchor \"{/:Bold {/:Italic A_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-18.25)*keyWidth,2\n";
	Output+="set object 37 rectangle back at screen 0.387+xOffset, char 4.8 size char strlen(\"A_3\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 38 front center at @anchor \"{/:Bold {/:Italic A@^#_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-18.65)*keyWidth,2+keyHeight\n";
	Output+="set label 38+89 front center at @anchor \"{/:Bold {/:Italic B@^b_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-18.65)*keyWidth,2+2*keyHeight\n";
	Output+="set object 38 rectangle front at screen 0.396+xOffset, char 6 size char strlen(\"A_3\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 39 front center at @anchor \"{/:Bold {/:Italic B_3}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-19.05)*keyWidth,2\n";
	Output+="set object 39 rectangle back at screen 0.405+xOffset, char 4.8 size char strlen(\"B_3\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 40 front center at @anchor \"{/:Bold {/:Italic C_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-19.9)*keyWidth,2\n";
	Output+="set object 40 rectangle back at screen 0.423+xOffset, char 4.8 size char strlen(\"C_4\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 41 front center at @anchor \"{/:Bold {/:Italic C@^#_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-20.25)*keyWidth,2+keyHeight\n";
	Output+="set label 41+89 front center at @anchor \"{/:Bold {/:Italic D@^b_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-20.25)*keyWidth,2+2*keyHeight\n";
	Output+="set object 41 rectangle front at screen 0.431+xOffset, char 6 size char strlen(\"C_4\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 42 front center at @anchor \"{/:Bold {/:Italic D_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-20.7)*keyWidth,2\n";
	Output+="set object 42 rectangle back at screen 0.440+xOffset, char 4.8 size char strlen(\"D_4\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 43 front center at @anchor \"{/:Bold {/:Italic D@^#_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-21.1)*keyWidth,2+keyHeight\n";
	Output+="set label 43+89 front center at @anchor \"{/:Bold {/:Italic E@^b_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-21.1)*keyWidth,2+2*keyHeight\n";
	Output+="set object 43 rectangle front at screen 0.449+xOffset, char 6 size char strlen(\"D_5\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 44 front center at @anchor \"{/:Bold {/:Italic E_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-21.5)*keyWidth,2\n";
	Output+="set object 44 rectangle back at screen 0.458+xOffset, char 4.8 size char strlen(\"E_4\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 45 front center at @anchor \"{/:Bold {/:Italic F_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-22.3)*keyWidth,2\n";
	Output+="set object 45 rectangle back at screen 0.476+xOffset, char 4.8 size char strlen(\"F_4\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 46 front center at @anchor \"{/:Bold {/:Italic F@^#_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-22.75)*keyWidth,2+keyHeight\n";
	Output+="set label 46+89 front center at @anchor \"{/:Bold {/:Italic G@^b_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-22.75)*keyWidth,2+2*keyHeight\n";
	Output+="set object 46 rectangle front at screen 0.485+xOffset, char 6 size char strlen(\"F_4\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 47 front center at @anchor \"{/:Bold {/:Italic G_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-23.15)*keyWidth,2\n";
	Output+="set object 47 rectangle back at screen 0.494+xOffset, char 4.8 size char strlen(\"G_4\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 48 front center at @anchor \"{/:Bold {/:Italic G@^#_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-23.55)*keyWidth,2+keyHeight\n";
	Output+="set label 48+89 front center at @anchor \"{/:Bold {/:Italic A@^b_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-23.55)*keyWidth,2+2*keyHeight\n";
	Output+="set object 48 rectangle front at screen 0.503+xOffset, char 6 size char strlen(\"G_4\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 49 front center at @anchor \"{/:Bold {/:Italic A_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-23.95)*keyWidth,2\n";
	Output+="set object 49 rectangle back at screen 0.512+xOffset, char 4.8 size char strlen(\"A_4\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 50 front center at @anchor \"{/:Bold {/:Italic A@^#_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-24.4)*keyWidth,2+keyHeight\n";
	Output+="set label 50+89 front center at @anchor \"{/:Bold {/:Italic B@^b_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-24.4)*keyWidth,2+2*keyHeight\n";
	Output+="set object 50 rectangle front at screen 0.521+xOffset, char 6 size char strlen(\"A_4\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 51 front center at @anchor \"{/:Bold {/:Italic B_4}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-24.8)*keyWidth,2\n";
	Output+="set object 51 rectangle back at screen 0.530+xOffset, char 4.8 size char strlen(\"B_4\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 52 front center at @anchor \"{/:Bold {/:Italic C_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-25.65)*keyWidth,2\n";
	Output+="set object 52 rectangle back at screen 0.548+xOffset, char 4.8 size char strlen(\"C_5\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 53 front center at @anchor \"{/:Bold {/:Italic C@^#_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-26)*keyWidth,2+keyHeight\n";
	Output+="set label 53+89 front center at @anchor \"{/:Bold {/:Italic D@^b_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-26)*keyWidth,2+2*keyHeight\n";
	Output+="set object 53 rectangle front at screen 0.557+xOffset, char 6 size char strlen(\"C_5\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 54 front center at @anchor \"{/:Bold {/:Italic D_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-26.45)*keyWidth,2\n";
	Output+="set object 54 rectangle back at screen 0.566+xOffset, char 4.8 size char strlen(\"D_5\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 55 front center at @anchor \"{/:Bold {/:Italic D@^#_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-26.85)*keyWidth,2+keyHeight\n";
	Output+="set label 55+89 front center at @anchor \"{/:Bold {/:Italic E@^b_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-26.85)*keyWidth,2+2*keyHeight\n";
	Output+="set object 55 rectangle front at screen 0.575+xOffset, char 6 size char strlen(\"D_5\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 56 front center at @anchor \"{/:Bold {/:Italic E_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-27.25)*keyWidth,2\n";
	Output+="set object 56 rectangle back at screen 0.584+xOffset, char 4.8 size char strlen(\"E_5\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 57 front center at @anchor \"{/:Bold {/:Italic F_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-28.1)*keyWidth,2\n";
	Output+="set object 57 rectangle back at screen 0.602+xOffset, char 4.8 size char strlen(\"F_5\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 58 front center at @anchor \"{/:Bold {/:Italic F@^#_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-28.5)*keyWidth,2+keyHeight\n";
	Output+="set label 58+89 front center at @anchor \"{/:Bold {/:Italic G@^b_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-28.5)*keyWidth,2+2*keyHeight\n";
	Output+="set object 58 rectangle front at screen 0.611+xOffset, char 6 size char strlen(\"F_5\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 59 front center at @anchor \"{/:Bold {/:Italic G_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-28.9)*keyWidth,2\n";
	Output+="set object 59 rectangle back at screen 0.620+xOffset, char 4.8 size char strlen(\"G_5\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 60 front center at @anchor \"{/:Bold {/:Italic G@^#_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-29.3)*keyWidth,2+keyHeight\n";
	Output+="set label 60+89 front center at @anchor \"{/:Bold {/:Italic A@^b_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-29.3)*keyWidth,2+2*keyHeight\n";
	Output+="set object 60 rectangle front at screen 0.629+xOffset, char 6 size char strlen(\"G_5\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 61 front center at @anchor \"{/:Bold {/:Italic A_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-29.75)*keyWidth,2\n";
	Output+="set object 61 rectangle back at screen 0.638+xOffset, char 4.8 size char strlen(\"A_5\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 62 front center at @anchor \"{/:Bold {/:Italic A@^#_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-30.15)*keyWidth,2+keyHeight\n";
	Output+="set label 62+89 front center at @anchor \"{/:Bold {/:Italic B@^b_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-30.15)*keyWidth,2+2*keyHeight\n";
	Output+="set object 62 rectangle front at screen 0.647+xOffset, char 6 size char strlen(\"A_5\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 63 front center at @anchor \"{/:Bold {/:Italic B_5}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-30.55)*keyWidth,2\n";
	Output+="set object 63 rectangle back at screen 0.656+xOffset, char 4.8 size char strlen(\"B_5\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 64 front center at @anchor \"{/:Bold {/:Italic C_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-31.4)*keyWidth,2\n";
	Output+="set object 64 rectangle back at screen 0.674+xOffset, char 4.8 size char strlen(\"C_6\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 65 front center at @anchor \"{/:Bold {/:Italic C@^#_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-31.8)*keyWidth,2+keyHeight\n";
	Output+="set label 65+89 front center at @anchor \"{/:Bold {/:Italic D@^b_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-31.8)*keyWidth,2+2*keyHeight\n";
	Output+="set object 65 rectangle front at screen 0.683+xOffset, char 6 size char strlen(\"C_6\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 66 front center at @anchor \"{/:Bold {/:Italic D_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-32.2)*keyWidth,2\n";
	Output+="set object 66 rectangle back at screen 0.692+xOffset, char 4.8 size char strlen(\"D_6\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 67 front center at @anchor \"{/:Bold {/:Italic D@^#_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-32.6)*keyWidth,2+keyHeight\n";
	Output+="set label 67+89 front center at @anchor \"{/:Bold {/:Italic E@^b_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-32.6)*keyWidth,2+2*keyHeight\n";
	Output+="set object 67 rectangle front at screen 0.701+xOffset, char 6 size char strlen(\"D_6\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 68 front center at @anchor \"{/:Bold {/:Italic E_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-33.05)*keyWidth,2\n";
	Output+="set object 68 rectangle back at screen 0.710+xOffset, char 4.8 size char strlen(\"E_6\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 69 front center at @anchor \"{/:Bold {/:Italic F_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-33.85)*keyWidth,2\n";
	Output+="set object 69 rectangle back at screen 0.728+xOffset, char 4.8 size char strlen(\"F_6\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 70 front center at @anchor \"{/:Bold {/:Italic F@^#_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-34.25)*keyWidth,2+keyHeight\n";
	Output+="set label 70+89 front center at @anchor \"{/:Bold {/:Italic G@^b_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-34.25)*keyWidth,2+2*keyHeight\n";
	Output+="set object 70 rectangle front at screen 0.737+xOffset, char 6 size char strlen(\"F_6\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 71 front center at @anchor \"{/:Bold {/:Italic G_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-34.65)*keyWidth,2\n";
	Output+="set object 71 rectangle back at screen 0.746+xOffset, char 4.8 size char strlen(\"G_6\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 72 front center at @anchor \"{/:Bold {/:Italic G@^#_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-35.05)*keyWidth,2+keyHeight\n";
	Output+="set label 72+89 front center at @anchor \"{/:Bold {/:Italic A@^b_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-35.05)*keyWidth,2+2*keyHeight\n";
	Output+="set object 72 rectangle front at screen 0.755+xOffset, char 6 size char strlen(\"G_6\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 73 front center at @anchor \"{/:Bold {/:Italic A_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-35.5)*keyWidth,2\n";
	Output+="set object 73 rectangle back at screen 0.764+xOffset, char 4.8 size char strlen(\"A_6\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 74 front center at @anchor \"{/:Bold {/:Italic A@^#_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-35.9)*keyWidth,2+keyHeight\n";
	Output+="set label 74+89 front center at @anchor \"{/:Bold {/:Italic B@^b_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-35.9)*keyWidth,2+2*keyHeight\n";
	Output+="set object 74 rectangle front at screen 0.773+xOffset, char 6 size char strlen(\"A_6\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 75 front center at @anchor \"{/:Bold {/:Italic B_6}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-36.3)*keyWidth,2\n";
	Output+="set object 75 rectangle back at screen 0.782+xOffset, char 4.8 size char strlen(\"B_6\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 76 front center at @anchor \"{/:Bold {/:Italic C_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-37.1)*keyWidth,2\n";
	Output+="set object 76 rectangle back at screen 0.800+xOffset, char 4.8 size char strlen(\"C_7\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 77 front center at @anchor \"{/:Bold {/:Italic C@^#_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-37.55)*keyWidth,2+keyHeight\n";
	Output+="set label 77+89 front center at @anchor \"{/:Bold {/:Italic D@^b_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-37.55)*keyWidth,2+2*keyHeight\n";
	Output+="set object 77 rectangle front at screen 0.809+xOffset, char 6 size char strlen(\"C_7\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 78 front center at @anchor \"{/:Bold {/:Italic D_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-38)*keyWidth,2\n";
	Output+="set object 78 rectangle back at screen 0.818+xOffset, char 4.8 size char strlen(\"D_7\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 79 front center at @anchor \"{/:Bold {/:Italic D@^#_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-38.4)*keyWidth,2+keyHeight\n";
	Output+="set label 79+89 front center at @anchor \"{/:Bold {/:Italic E@^b_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-38.4)*keyWidth,2+2*keyHeight\n";
	Output+="set object 79 rectangle front at screen 0.827+xOffset, char 6 size char strlen(\"D_7\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 80 front center at @anchor \"{/:Bold {/:Italic E_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-38.8)*keyWidth,2\n";
	Output+="set object 80 rectangle back at screen 0.836+xOffset, char 4.8 size char strlen(\"E_7\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 81 front center at @anchor \"{/:Bold {/:Italic F_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-39.6)*keyWidth,2\n";
	Output+="set object 81 rectangle back at screen 0.854+xOffset, char 4.8 size char strlen(\"F_7\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 82 front center at @anchor \"{/:Bold {/:Italic F@^#_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-40)*keyWidth,2+keyHeight\n";
	Output+="set label 82+89 front center at @anchor \"{/:Bold {/:Italic G@^b_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-40)*keyWidth,2+2*keyHeight\n";
	Output+="set object 82 rectangle front at screen 0.863+xOffset, char 6 size char strlen(\"F_7\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 83 front center at @anchor \"{/:Bold {/:Italic G_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-40.45)*keyWidth,2\n";
	Output+="set object 83 rectangle back at screen 0.872+xOffset, char 4.8 size char strlen(\"G_7\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 84 front center at @anchor \"{/:Bold {/:Italic G@^#_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-40.85)*keyWidth,2+keyHeight\n";
	Output+="set label 84+89 front center at @anchor \"{/:Bold {/:Italic A@^b_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-40.85)*keyWidth,2+2*keyHeight\n";
	Output+="set object 84 rectangle front at screen 0.881+xOffset, char 6 size char strlen(\"G_7\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 85 front center at @anchor \"{/:Bold {/:Italic A_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-41.25)*keyWidth,2\n";
	Output+="set object 85 rectangle back at screen 0.890+xOffset, char 4.8 size char strlen(\"A_7\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 86 front center at @anchor \"{/:Bold {/:Italic A@^#_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-41.65)*keyWidth,2+keyHeight\n";
	Output+="set label 86+89 front center at @anchor \"{/:Bold {/:Italic B@^b_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-41.65)*keyWidth,2+2*keyHeight\n";
	Output+="set object 86 rectangle front at screen 0.899+xOffset, char 6 size char strlen(\"A_7\")+1, char 4.1 fillcolor "+sharpKeyColor+" fillstyle solid 1.0 border 3\n";
	Output+="set label 87 front center at @anchor \"{/:Bold {/:Italic B_7}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-42.05)*keyWidth,2\n";
	Output+="set object 87 rectangle back at screen 0.908+xOffset, char 4.8 size char strlen(\"B_7\")+1, char 6.5 fillstyle empty border 3\n";
	Output+="set label 88 front center at @anchor \"{/:Bold {/:Italic C_8}}\" font keyFont textcolor "+keyColor+" offset char -(shiftFactor-42.85)*keyWidth,2\n";
	Output+="set object 88 rectangle back at screen 0.926+xOffset, char 4.8 size char strlen(\"C_8\")+1, char 6.5 fillstyle empty border 3\n";
	return Output;}

string get_gnuplot_label_from_noteCode(string Note)
	{string Output="-1";
	// Error Handling
	// Handle Staccato Encoding
	int pos=Note.find("X",0);
	if(pos!=string::npos){Note.replace(pos,1,"");}
	// Handle Sustained Encoding
	pos=Note.find("S",0);
	if(pos!=string::npos){Note.replace(pos,1,"");}
	//
	if(Note.compare("A0")==0){Output="1";}									// A Natural
	else if(Note.compare("As0")==0||Note.compare("Bf0")==0){Output="2";}			// A# Sharp / Bb Flat
	else if(Note.compare("B0")==0||Note.compare("Cf1")==0){Output="3";}			// B Natural
	else if(Note.compare("Bs0")==0||Note.compare("C1")==0){Output="4";}			// B# Sharp / C Natural	
	else if(Note.compare("Cs1")==0||Note.compare("Df1")==0){Output="5";}			// C# Sharp / Db Flat
	else if(Note.compare("D1")==0){Output="6";}								// D Natural
	else if(Note.compare("Ds1")==0||Note.compare("Ef1")==0){Output="7";}			// D# Sharp / Eb Flat
	else if(Note.compare("E1")==0||Note.compare("Ff1")==0){Output="8";}			// E Natural / Fb Flat
	else if(Note.compare("Es1")==0||Note.compare("F1")==0){Output="9";}			// F Natural / E# Sharp
	else if(Note.compare("Fs1")==0||Note.compare("Gf1")==0){Output="10";}			// F# Sharp / Gb Flat
	else if(Note.compare("G1")==0){Output="11";}								// G Natural
	else if(Note.compare("Gs1")==0||Note.compare("Af1")==0){Output="12";}			// G# Sharp / Ab Flat
	//
	else if(Note.compare("A1")==0){Output="13";}									// A Natural
	else if(Note.compare("As1")==0||Note.compare("Bf1")==0){Output="14";}			// A# Sharp / Bb Flat
	else if(Note.compare("B1")==0||Note.compare("Cf2")==0){Output="15";}			// B Natural
	else if(Note.compare("Bs1")==0||Note.compare("C2")==0){Output="16";}			// B# Sharp / C Natural	
	else if(Note.compare("Cs2")==0||Note.compare("Df2")==0){Output="17";}			// C# Sharp / Db Flat
	else if(Note.compare("D2")==0){Output="18";}								// D Natural
	else if(Note.compare("Ds2")==0||Note.compare("Ef2")==0){Output="19";}			// D# Sharp / Eb Flat
	else if(Note.compare("E2")==0||Note.compare("Ff2")==0){Output="20";}			// E Natural / Fb Flat
	else if(Note.compare("Es2")==0||Note.compare("F2")==0){Output="21";}			// F Natural / E# Sharp
	else if(Note.compare("Fs2")==0||Note.compare("Gf2")==0){Output="22";}			// F# Sharp / Gb Flat
	else if(Note.compare("G2")==0){Output="23";}								// G Natural
	else if(Note.compare("Gs2")==0||Note.compare("Af2")==0){Output="24";}			// G# Sharp / Ab Flat
	//
	else if(Note.compare("A2")==0){Output="25";}								// A Natural
	else if(Note.compare("As2")==0||Note.compare("Bf2")==0){Output="26";}			// A# Sharp / Bb Flat
	else if(Note.compare("B2")==0||Note.compare("Cf3")==0){Output="27";}			// B Natural
	else if(Note.compare("Bs2")==0||Note.compare("C3")==0){Output="28";}			// B# Sharp / C Natural	
	else if(Note.compare("Cs3")==0||Note.compare("Df3")==0){Output="29";}			// C# Sharp / Db Flat
	else if(Note.compare("D3")==0){Output="30";}								// D Natural
	else if(Note.compare("Ds3")==0||Note.compare("Ef3")==0){Output="31";}			// D# Sharp / Eb Flat
	else if(Note.compare("E3")==0||Note.compare("Ff3")==0){Output="32";}			// E Natural / Fb Flat
	else if(Note.compare("Es3")==0||Note.compare("F3")==0){Output="33";}			// F Natural / E# Sharp
	else if(Note.compare("Fs3")==0||Note.compare("Gf3")==0){Output="34";}			// F# Sharp / Gb Flat
	else if(Note.compare("G3")==0){Output="35";}								// G Natural
	else if(Note.compare("Gs3")==0||Note.compare("Af3")==0){Output="36";}			// G# Sharp / Ab Flat
	//
	else if(Note.compare("A3")==0){Output="37";}								// A Natural
	else if(Note.compare("As3")==0||Note.compare("Bf3")==0){Output="38";}			// A# Sharp / Bb Flat
	else if(Note.compare("B3")==0||Note.compare("Cf4")==0){Output="39";}			// B Natural
	else if(Note.compare("Bs3")==0||Note.compare("C4")==0){Output="40";}			// B# Sharp / C Natural	
	else if(Note.compare("Cs4")==0||Note.compare("Df4")==0){Output="41";}			// C# Sharp / Db Flat
	else if(Note.compare("D4")==0){Output="42";}								// D Natural
	else if(Note.compare("Ds4")==0||Note.compare("Ef4")==0){Output="43";}			// D# Sharp / Eb Flat
	else if(Note.compare("E4")==0||Note.compare("Ff4")==0){Output="44";}			// E Natural / Fb Flat
	else if(Note.compare("Es4")==0||Note.compare("F4")==0){Output="45";}			// F Natural / E# Sharp
	else if(Note.compare("Fs4")==0||Note.compare("Gf4")==0){Output="46";}			// F# Sharp / Gb Flat
	else if(Note.compare("G4")==0){Output="47";}								// G Natural
	else if(Note.compare("Gs4")==0||Note.compare("Af4")==0){Output="48";}			// G# Sharp / Ab Flat
	//
	else if(Note.compare("A4")==0){Output="49";}								// A Natural
	else if(Note.compare("As4")==0||Note.compare("Bf4")==0){Output="50";}			// A# Sharp / Bb Flat
	else if(Note.compare("B4")==0||Note.compare("Cf5")==0){Output="51";}			// B Natural
	else if(Note.compare("Bs4")==0||Note.compare("C5")==0){Output="52";}			// B# Sharp / C Natural	
	else if(Note.compare("Cs5")==0||Note.compare("Df5")==0){Output="53";}			// C# Sharp / Db Flat
	else if(Note.compare("D5")==0){Output="54";}								// D Natural
	else if(Note.compare("Ds5")==0||Note.compare("Ef5")==0){Output="55";}			// D# Sharp / Eb Flat
	else if(Note.compare("E5")==0||Note.compare("Ff5")==0){Output="56";}			// E Natural / Fb Flat
	else if(Note.compare("Es5")==0||Note.compare("F5")==0){Output="57";}			// F Natural / E# Sharp
	else if(Note.compare("Fs5")==0||Note.compare("Gf5")==0){Output="58";}			// F# Sharp / Gb Flat
	else if(Note.compare("G5")==0){Output="59";}								// G Natural
	else if(Note.compare("Gs5")==0||Note.compare("Af5")==0){Output="60";}			// G# Sharp / Ab Flat
	//
	else if(Note.compare("A5")==0){Output="61";}								// A Natural
	else if(Note.compare("As5")==0||Note.compare("Bf5")==0){Output="62";}			// A# Sharp / Bb Flat
	else if(Note.compare("B5")==0||Note.compare("Cf6")==0){Output="63";}			// B Natural
	else if(Note.compare("Bs5")==0||Note.compare("C6")==0){Output="64";}			// B# Sharp / C Natural	
	else if(Note.compare("Cs6")==0||Note.compare("Df6")==0){Output="65";}			// C# Sharp / Db Flat
	else if(Note.compare("D6")==0){Output="66";}								// D Natural
	else if(Note.compare("Ds6")==0||Note.compare("Ef6")==0){Output="67";}			// D# Sharp / Eb Flat
	else if(Note.compare("E6")==0||Note.compare("Ff6")==0){Output="68";}			// E Natural / Fb Flat
	else if(Note.compare("Es6")==0||Note.compare("F6")==0){Output="69";}			// F Natural / E# Sharp
	else if(Note.compare("Fs6")==0||Note.compare("Gf6")==0){Output="70";}			// F# Sharp / Gb Flat
	else if(Note.compare("G6")==0){Output="71";}								// G Natural
	else if(Note.compare("Gs6")==0||Note.compare("Af6")==0){Output="72";}			// G# Sharp / Ab Flat
	//
	else if(Note.compare("A6")==0){Output="73";}								// A Natural
	else if(Note.compare("As6")==0||Note.compare("Bf6")==0){Output="74";}			// A# Sharp / Bb Flat
	else if(Note.compare("B6")==0||Note.compare("Cf7")==0){Output="75";}			// B Natural
	else if(Note.compare("Bs6")==0||Note.compare("C7")==0){Output="76";}			// B# Sharp / C Natural	
	else if(Note.compare("Cs7")==0||Note.compare("Df7")==0){Output="77";}			// C# Sharp / Db Flat
	else if(Note.compare("D7")==0){Output="78";}								// D Natural
	else if(Note.compare("Ds7")==0||Note.compare("Ef7")==0){Output="79";}			// D# Sharp / Eb Flat
	else if(Note.compare("E7")==0||Note.compare("Ff7")==0){Output="80";}			// E Natural / Fb Flat
	else if(Note.compare("Es7")==0||Note.compare("F7")==0){Output="81";}			// F Natural / E# Sharp
	else if(Note.compare("Fs7")==0||Note.compare("Gf7")==0){Output="82";}			// F# Sharp / Gb Flat
	else if(Note.compare("G7")==0){Output="83";}								// G Natural
	else if(Note.compare("Gs7")==0||Note.compare("Af7")==0){Output="84";}			// G# Sharp / Ab Flat
	//
	else if(Note.compare("A7")==0){Output="85";}								// A Natural
	else if(Note.compare("As7")==0||Note.compare("Bf7")==0){Output="86";}			// A# Sharp / Bb Flat
	else if(Note.compare("B7")==0||Note.compare("Cf8")==0){Output="87";}			// B Natural
	else if(Note.compare("Bs7")==0||Note.compare("C8")==0){Output="88";}			// B# Sharp / C Natural	
	//
	else if(Note.compare("R")==0){Output="-1";}
	else
		{cerr<<"Error in get_gnuplot_label_from_noteCode\nUnrecognized Note ("<<Note<<")\n";}
	return Output;}

void write_wave_file(string inFile,double *Data,int numPnts)
	{SNDFILE *SoundFile;
	SF_INFO SoundFileInfo;
	int bufferSize=256, filetype = SF_ENDIAN_LITTLE | SF_FORMAT_RAW | SF_FORMAT_PCM_16 , intVal, returnValue;
	short buffer[bufferSize];
	for(int k=0;k<bufferSize;k++){buffer[k]=k;}
	
	filetype=SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	SoundFileInfo.channels=2;
	SoundFileInfo.samplerate=SAMPLE_RATE;
	SoundFileInfo.format=filetype;
	SoundFileInfo.frames=numPnts;

	//if (! (file = sf_open (filename, SFM_WRITE, &sfinfo)))
	SoundFile=sf_open(inFile.c_str(), SFM_WRITE, &SoundFileInfo);
	if(!SoundFile)
		{printf ("Line %d: sf_open_write failed at write_wave_file for double with error :\n", __LINE__);
		fflush(stdout);
		sf_perror(NULL);
		exit(EXIT_FAILURE);
		}

	//sf_readf_double(SoundFile, Output, SoundFileInfo.frames);	//fill the array with sample values, a frame equals on sample per channel 
	intVal=sf_write_double(SoundFile,Data,numPnts);
	//intVal=sf_readf_short(SoundFile, Output, SoundFileInfo.frames);
	//cout<<intVal<<endl;
	if( intVal != numPnts)
		{
		if(intVal>0)
			{//printf ("Line %d: short read (%d).\nNumber of Frames: %d\nNumber of Channels: %d\n%s\n", __LINE__, intVal, SoundFileInfo.frames, SoundFileInfo.channels,inFile.c_str()) ;
			//exit(EXIT_FAILURE);
			//intVal=sf_write_float(SoundFile,Data,numPnts);
			}
		else if(intVal==0)
			{// End of File (EOF) Reach need to seek to top
			returnValue=sf_seek(SoundFile,0,SEEK_SET);		//cout<<returnValue<<endl;
			}
		else{cerr<<"So we got here\n";}
		}
	sf_close(SoundFile);
	}

void write_wave_file(string inFile,SAMPLE *Data,int numPnts)
	{SNDFILE *SoundFile;
	SF_INFO SoundFileInfo;
	int bufferSize=256, filetype = SF_ENDIAN_LITTLE | SF_FORMAT_RAW | SF_FORMAT_PCM_16 , intVal, returnValue;
	short buffer[bufferSize];
	for(int k=0;k<bufferSize;k++){buffer[k]=k;}
	
	filetype=SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	SoundFileInfo.channels=2;
	SoundFileInfo.samplerate=SAMPLE_RATE;
	SoundFileInfo.format=filetype;
	SoundFileInfo.frames=numPnts;

	//if (! (file = sf_open (filename, SFM_WRITE, &sfinfo)))
	SoundFile=sf_open(inFile.c_str(), SFM_WRITE, &SoundFileInfo);
	if(!SoundFile)
		{printf ("Line %d: sf_open_write failed at write_wave_file with error :\n", __LINE__);
		fflush(stdout);
		sf_perror(NULL);
		exit(EXIT_FAILURE);
		}

	//sf_readf_double(SoundFile, Output, SoundFileInfo.frames);	//fill the array with sample values, a frame equals on sample per channel 
	intVal=sf_write_float(SoundFile,Data,numPnts);
	//intVal=sf_readf_short(SoundFile, Output, SoundFileInfo.frames);
	//cout<<intVal<<endl;
	if( intVal != numPnts)
		{
		if(intVal>0)
			{printf ("Line %d: short read (%d).\nNumber of Frames: %d\nNumber of Channels: %d\n%s\n", __LINE__, intVal, SoundFileInfo.frames, SoundFileInfo.channels,inFile.c_str()) ;
			//exit(EXIT_FAILURE);
			}
		else if(intVal==0)
			{// End of File (EOF) Reach need to seek to top
			returnValue=sf_seek(SoundFile,0,SEEK_SET);		//cout<<returnValue<<endl;
			intVal=sf_write_double(SoundFile,(double*)Data,numPnts);
			//cout<<intVal<<endl;
			}
		else{cerr<<"So we got here\n";}
		}
	sf_close(SoundFile);
	}
