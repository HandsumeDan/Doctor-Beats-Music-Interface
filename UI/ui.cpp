# include <cmath>
# include <cstdio>
# include <cstring>
# include <fstream>
# include <iostream>
# include <sstream>
# include <string>
# include <fcntl.h>					// Defines O_CREAT flag
# include <csignal>
//# include <thread>
# include <sys/reg.h>
# include <sys/syscall.h>			// Defines OS System Calls
//# include <sys/ptrace.h>
# include <sys/types.h>
# include <sys/user.h>
# include <sys/wait.h>
# include <sys/stat.h>				// Defines chmod function
# include <unistd.h>
//# include <boost/filesystem.hpp>		// create_directory

# include <QtWidgets>
# include <QProcess>

# include "ui.h"

using namespace std;

UI::UI(QWidget *parent) : QWidget(parent)
	{// Initialize the SOUND folder
	soundFldr=get_current_dir_name(); soundFldr+="/SOUND/";
	// Define Song History File
	songHistoryFile=get_current_dir_name(); songHistoryFile+="/.history";	
	songHistoryBox=new QComboBox;
	connect(songHistoryBox,SIGNAL(activated(int)),this,SLOT(loadSong()));
	// Load Song History
	loadHistory();
	QLabel *songHistoryLabel=new QLabel(tr("- Song History -")); songHistoryLabel->setAlignment(Qt::AlignCenter);

	tempo=new QLineEdit; tempo->setText(tr(NO_VALUE)); tempo->setAlignment(Qt::AlignCenter);
	songName=new QLineEdit; songName->setText(tr(NO_VALUE)); songName->setAlignment(Qt::AlignCenter);
	songNameLabel=new QLabel(tr("Song Name:")); songNameLabel->setAlignment(Qt::AlignRight);
	QLabel* tempoLabel=new QLabel(tr("Tempo:")); tempoLabel->setAlignment(Qt::AlignRight);

	QPushButton* openSongButton=new QPushButton(tr("Open Song File"));
	openSongButton->setFixedWidth(200);//openSongButton->setFixedHeight(200);
	connect(openSongButton,SIGNAL(clicked()),this,SLOT(open_song_file()));

	addSoundButton=new QPushButton(tr("Add a Sound"));
	addSoundButton->setFixedWidth(200);
	connect(addSoundButton,SIGNAL(clicked()),this,SLOT(addSoundTab()));

	remSoundButton=new QPushButton(tr("Remove a Sound")); remSoundButton->hide();
	remSoundButton->setFixedWidth(200);
	connect(remSoundButton,SIGNAL(clicked()),this,SLOT(removeSoundTab()));

	QPushButton *writeBlanksDemosButton=new QPushButton(tr("Write Scores")); writeBlanksDemosButton->setFixedWidth(200);
	connect(writeBlanksDemosButton,SIGNAL(clicked()),this,SLOT(launchwriteDemoAndBlankFiles()));
	QPushButton *viewInstrumentsButton=new QPushButton(tr("Instruments")); viewInstrumentsButton->setFixedWidth(200);
	connect(viewInstrumentsButton,SIGNAL(clicked()),this,SLOT(launchviewInstrumentsList()));

	recordAudioButton=new QPushButton(tr("Record Audio")); recordAudioButton->setFixedWidth(200); recordAudioButton->hide();
	connect(recordAudioButton,SIGNAL(clicked()),this,SLOT(record_audio()));	
	QPushButton *writeWaveformButton=new QPushButton(tr("Write Song")); writeWaveformButton->setFixedWidth(200);
	connect(writeWaveformButton,SIGNAL(clicked()),this,SLOT(write_song_input_file()));
	// Sound Tab
	soundTab = new QTabWidget;
	// Define Solution Conditions Tab
	QWidget *tab1 = new QWidget;
	//
	QLabel* categoryLabel=new QLabel(tr("Category:")); categoryLabel->setAlignment(Qt::AlignRight);
	QLabel* instrumentLabel=new QLabel(tr("Instrument:")); instrumentLabel->setAlignment(Qt::AlignRight);
	QLabel* amplitudeLabel=new QLabel(tr("Amplitude:")); amplitudeLabel->setAlignment(Qt::AlignRight);
	QLabel* filterLabel=new QLabel(tr("Filter:")); filterLabel->setAlignment(Qt::AlignRight);
	QLabel* hiddenLabel=new QLabel; hiddenLabel->setFixedWidth(100);
	QLabel* hiddenLabel1=new QLabel;
	QLabel* hiddenLabel2=new QLabel;
	QLabel* hiddenLabel4=new QLabel;
	categoryBox[0]=new QComboBox;
	amplitude[0]=new QLineEdit; amplitude[0]->setText(tr("1.0")); amplitude[0]->setAlignment(Qt::AlignCenter);
	for(int i=0;i<numCategories;i++){categoryBox[0]->addItem(tr(categories[i].c_str()));}
	connect(categoryBox[0],SIGNAL(activated(int)),this,SLOT(loadInstrumentComboBox(int)));
	instrumentBox[0]=new QComboBox;
	connect(instrumentBox[0],SIGNAL(activated(int)),this,SLOT(loadInstrumentLabel(int)));
	filterTypeBox[0]=new QComboBox;
	for(int i=0;i<numFilterTypes;i++){filterTypeBox[0]->addItem(tr(filterTypes[i].c_str()));}
	connect(filterTypeBox[0],SIGNAL(activated(int)),this,SLOT(loadFilterComboBox(int)));
	filterBox[0]=new QComboBox; filterBox[0]->hide();
	connect(filterBox[0],SIGNAL(activated(int)),this,SLOT(loadFilterInput(int)));
	filterInput1Label[0]=new QLabel; filterInput1Label[0]->setAlignment(Qt::AlignRight); filterInput1Label[0]->hide();
	filterInput1[0]=new QLineEdit; filterInput1[0]->setText(tr(NO_VALUE)); filterInput1[0]->setAlignment(Qt::AlignCenter); filterInput1[0]->hide();
	filterInput2Label[0]=new QLabel; filterInput2Label[0]->setAlignment(Qt::AlignRight); filterInput2Label[0]->hide();
	filterInput2[0]=new QLineEdit; filterInput2[0]->setText(tr(NO_VALUE)); filterInput2[0]->setAlignment(Qt::AlignCenter); filterInput2[0]->hide();

	QPushButton* openButton=new QPushButton(tr("Open a Sound File"));
	connect(openButton,SIGNAL(clicked()),this,SLOT(open_sound_file()));

	soundFileLabel[0]=new QLabel; soundFileLabel[0]->setText(tr(NO_VALUE)); soundFileLabel[0]->setAlignment(Qt::AlignCenter);
	instrLabel[0]=new QLabel; instrLabel[0]->setText(tr(NO_VALUE)); instrLabel[0]->setAlignment(Qt::AlignCenter);

	//
	QGridLayout *tab1hbox = new QGridLayout;
	tab1hbox->setSizeConstraint(QLayout::SetFixedSize);
	tab1hbox->addWidget(categoryLabel,0,0,1,1); tab1hbox->addWidget(categoryBox[0],0,1,1,1); tab1hbox->addWidget(hiddenLabel,0,2,1,1); tab1hbox->addWidget(openButton,0,3,1,1);
	tab1hbox->addWidget(instrumentLabel,1,0,1,1); tab1hbox->addWidget(instrumentBox[0],1,1,1,1); tab1hbox->addWidget(soundFileLabel[0],1,2,1,2);
	tab1hbox->addWidget(amplitudeLabel,2,0,1,1); tab1hbox->addWidget(amplitude[0],2,1,1,1); tab1hbox->addWidget(instrLabel[0],2,2,1,2);
	tab1hbox->addWidget(filterLabel,3,0,1,1); tab1hbox->addWidget(filterTypeBox[0],3,1,1,1); tab1hbox->addWidget(filterInput1Label[0],3,2,1,1); tab1hbox->addWidget(filterInput1[0],3,3,1,1);
	tab1hbox->addWidget(hiddenLabel4,4,0,1,1); tab1hbox->addWidget(filterBox[0],4,1,1,1); tab1hbox->addWidget(filterInput2Label[0],4,2,1,1); tab1hbox->addWidget(filterInput2[0],4,3,1,1);
	tab1->setLayout(tab1hbox);

	soundTab->addTab(tab1, tr("&0"));
	
	QGroupBox *theBox=new QGroupBox;
	QGridLayout *boxLayout=new QGridLayout;
	boxLayout->setSizeConstraint(QLayout::SetFixedSize);
	boxLayout->addWidget(songNameLabel,0,0,1,1); boxLayout->addWidget(songName,0,1,1,1); boxLayout->addWidget(hiddenLabel2,0,2,1,1); boxLayout->addWidget(openSongButton,0,3,1,1);
	boxLayout->addWidget(tempoLabel,1,0,1,1); boxLayout->addWidget(tempo,1,1,1,1); boxLayout->addWidget(songHistoryLabel,1,3,1,1);
	boxLayout->addWidget(addSoundButton,2,0,1,1); boxLayout->addWidget(remSoundButton,2,1,1,1); boxLayout->addWidget(songHistoryBox,2,3,1,1);
	boxLayout->addWidget(soundTab,3,0,1,4);
	boxLayout->addWidget(writeBlanksDemosButton,4,0,1,1); boxLayout->addWidget(viewInstrumentsButton,4,1,1,1); boxLayout->addWidget(recordAudioButton,4,2,1,1); boxLayout->addWidget(writeWaveformButton,4,3,1,1);
	theBox->setLayout(boxLayout);

	QGroupBox *bottomBox=new QGroupBox;
	QGridLayout *bLayout=new QGridLayout;
	denoiseBox=new QCheckBox; denoiseBox->setText(tr("De-Noise Song"));
	playbackBox=new QCheckBox; playbackBox->setText(tr("Playback Output"));
	mp3Box=new QCheckBox; mp3Box->setText(tr("Convert to .mp3"));
	saveHistoryBox=new QCheckBox; saveHistoryBox->setText(tr("Save Song to History"));
	sonicVisualizerBox=new QCheckBox; sonicVisualizerBox->setText(tr("Use Sonic Visualizer on Output"));
	bLayout->addWidget(denoiseBox,0,0,1,1); bLayout->addWidget(playbackBox,0,1,1,1); bLayout->addWidget(mp3Box,0,2,1,1); bLayout->addWidget(saveHistoryBox,0,3,1,1);
	bLayout->addWidget(sonicVisualizerBox,1,0,1,1);
	bottomBox->setLayout(bLayout);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->addWidget(theBox,0,0,8,4);
	mainLayout->addWidget(bottomBox,8,0,1,4);
	setLayout(mainLayout);
	setWindowTitle(tr("Dr. Beats Music Interface"));
	//resize(QDesktopWidget().availableGeometry(this).size());
}

string UI::generateFilterCommand(int tabIndex)
	{string Output;
	int filterTypeIndex=filterTypeBox[tabIndex]->currentIndex();
	int filterIndex=filterBox[tabIndex]->currentIndex();
	string theFilterType=filterTypes[filterTypeIndex],theFilter;
	QString qTmp;
	string tmp,freq,value2;
	double lowerFreq,upperFreq;
	if(theFilterType.compare("HIGH-PASS")==0)
		{theFilter=highPassFilters[filterIndex];
		if(theFilter.compare("Butterworth")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 1, "+value2+", 0";}
				}
			}
		else if(theFilter.compare("Butterworth (2nd order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else{Output="OUT butterhp IN, "+freq;}
			}
		else if(theFilter.compare("Chebyshev I")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 1, "+value2+", 1";}
				}
			}
		else if(theFilter.compare("Chebyshev II")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 1, "+value2+", 2";}
				}
			}
		else if(theFilter.compare("Elliptical")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 1, "+value2+", 3";}
				}
			}
		else if(theFilter.compare("Recursive (1st order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else{Output="OUT atone IN, "+freq;}
			}
		else if(theFilter.compare("Resonant (2nd order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT bqrez IN, "+freq+", "+value2+", 1";}
				}
			}
		else if(theFilter.compare("State Variable")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT,lpOut,bpOut,brOut statevar IN, "+freq+", "+value2;}
				}
			}
		else if(theFilter.compare("State Variable (2nd order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="lpOut,OUT,bpOut svfilter IN, "+freq+", "+value2;}
				}
			}
		else{}
		}
	else if(theFilterType.compare("LOW-PASS")==0)
		{theFilter=lowPassFilters[filterIndex];
		if(theFilter.compare("Butterworth")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 0, "+value2+", 0";}
				}
			}
		else if(theFilter.compare("Butterworth (2nd order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else{Output="OUT butterlp IN, "+freq;}
			}
		else if(theFilter.compare("Chebyshev I")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 0, "+value2+", 1";}
				}
			}
		else if(theFilter.compare("Chebyshev II")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 0, "+value2+", 2";}
				}
			}
		else if(theFilter.compare("Elliptical")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT clfilt IN, "+freq+", 0, "+value2+", 3";}
				}
			}
		else if(theFilter.compare("Recursive (1st order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else{Output="OUT tone IN, "+freq;}
			}
		else if(theFilter.compare("Resonant")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT lowres IN, "+freq+", "+value2+", 0";}
				}
			}
		else if(theFilter.compare("Resonant II")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT rezzy IN, "+freq+", "+value2+", 0";}
				}
			}
		else if(theFilter.compare("Resonant (2nd order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT bqrez IN, "+freq+", "+value2+", 0";}
				}
			}
		else if(theFilter.compare("State Variable")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="hpOut,OUT,bpOut,brOut statevar IN, "+freq+", "+value2;}
				}
			}
		else if(theFilter.compare("State Variable (2nd order)")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT,hpOut,bpOut svfilter IN, "+freq+", "+value2;}
				}
			}
		else{}
		}
	else if(theFilterType.compare("BAND-PASS")==0)
		{theFilter=bandPassFilters[filterIndex];
		if(theFilter.compare("Butterworth")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				lowerFreq=strtod(freq.c_str(),NULL);
				upperFreq=strtod(value2.c_str(),NULL);
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT butterbp IN, "+freq+", "+formatNumberString(cnvrtNumToStrng(upperFreq-lowerFreq,8));}
				}
			}
		else if(theFilter.compare("Resonant")==0)
			{qTmp=filterInput1[tabIndex]->text();
			freq=qTmp.toStdString();
			if(freq.compare(NO_VALUE)==0){Output="NONE";}
			else
				{qTmp=filterInput2[tabIndex]->text();
				value2=qTmp.toStdString();
				lowerFreq=strtod(freq.c_str(),NULL);
				upperFreq=strtod(value2.c_str(),NULL);
				if(value2.compare(NO_VALUE)==0){Output="NONE";}
				else{Output="OUT reson IN, "+freq+", "+formatNumberString(cnvrtNumToStrng(upperFreq-lowerFreq,8));}
				}}
		else{}
		}
	else{}

	return Output;}

void UI::readFilterCommand(string filterCommand,int tabIndex)
	{// Parse Filter Command
	double lowerFreq,upperFreq;
	// Check for NONE
	int pos=filterCommand.find("NONE",0);
	if(pos!=string::npos){return;}
	pos=filterCommand.find(" IN,",0);
	int pos2=filterCommand.rfind(" ",pos-1);
	// Extract function name
	string filterFunction=filterCommand.substr(pos2+1,pos-pos2-1);
	string commandOutput=filterCommand.substr(0,pos2);
	string commandInput=filterCommand.substr(pos+4,filterCommand.length()-pos-4);
	string firstInput,secondInput="",thirdInput="",fourthInput="";
	string firstOutput,secondOutput="",thirdOutput="",fourthOutput="";
	string theFilterType,theFilter;
	pos=commandInput.find(",",0);
	if(pos==string::npos){firstInput=remove_excess_white_space(commandInput);}
	else{firstInput=remove_excess_white_space(commandInput.substr(0,pos));}
	filterInput1[tabIndex]->show(); filterInput1[tabIndex]->setText(tr(firstInput.c_str()));
	pos2=commandOutput.find(",",0);
	if(pos2==string::npos){firstOutput=remove_excess_white_space(commandOutput);}
	else{firstOutput=remove_excess_white_space(commandOutput.substr(0,pos2));}

	if(filterFunction.compare("atone")==0)
		{theFilterType="HIGH-PASS";
		theFilter="Recursive (1st order)";}
	else if(filterFunction.compare("bqrez")==0)
		{theFilter="Resonant (2nd order)";
		pos2=commandInput.find(",",pos+1);
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,pos2-pos-1));
		pos=pos2;
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(secondInput.c_str()));
		thirdInput=remove_excess_white_space(commandInput.substr(pos+1,commandInput.length()-pos-1));
		if(thirdInput.compare("0")==0){theFilterType="LOW-PASS";}
		else if(thirdInput.compare("1")==0){theFilterType="HIGH-PASS";}
		}
	else if(filterFunction.compare("butterbp")==0)
		{theFilterType="BAND-PASS";
		theFilter="Butterworth";
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,commandInput.length()-pos-1));
		lowerFreq=strtod(firstInput.c_str(),NULL);
		upperFreq=strtod(secondInput.c_str(),NULL);
		upperFreq+=lowerFreq;
		secondInput=formatNumberString(cnvrtNumToStrng(upperFreq,8));
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(secondInput.c_str()));}
	else if(filterFunction.compare("butterhp")==0)
		{theFilterType="HIGH-PASS";
		theFilter="Butterworth";}
	else if(filterFunction.compare("butterlp")==0)
		{theFilterType="LOW-PASS";
		theFilter="Butterworth";}
	else if(filterFunction.compare("clfilt")==0)
		{pos2=commandInput.find(",",pos+1);
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,pos2-pos-1));
		pos=pos2;
		if(secondInput.compare("0")==0){theFilterType="LOW-PASS";}
		else if(secondInput.compare("1")==0){theFilterType="HIGH-PASS";}
		pos2=commandInput.find(",",pos+1);
		thirdInput=remove_excess_white_space(commandInput.substr(pos+1,pos2-pos-1));
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(thirdInput.c_str()));
		pos=pos2;
		fourthInput=remove_excess_white_space(commandInput.substr(pos+1,commandInput.length()-pos-1));
		if(fourthInput.compare("0")==0){theFilter="Butterworth";}
		else if(fourthInput.compare("1")==0){theFilter="Chebyshev I";}
		else if(fourthInput.compare("2")==0){theFilter="Chebyshev II";}
		else if(fourthInput.compare("3")==0){theFilter="Elliptical";}
		}
	else if(filterFunction.compare("statevar")==0)
		{theFilter="State Variable";
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,commandInput.length()-pos-1));
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(secondInput.c_str()));
		
		pos=commandOutput.find(",",pos2+1);
		secondOutput=remove_excess_white_space(commandOutput.substr(pos2+1,pos-pos2-1));
		pos2=pos;
		pos=commandOutput.find(",",pos2+1);
		thirdOutput=remove_excess_white_space(commandOutput.substr(pos2+1,pos-pos2-1));
		pos2=pos;
		fourthOutput=remove_excess_white_space(commandOutput.substr(pos+1,commandOutput.length()-pos-1));
		if(firstOutput.compare("OUT")==0){theFilterType="HIGH-PASS";}
		if(secondOutput.compare("OUT")==0){theFilterType="LOW-PASS";}
		if(thirdOutput.compare("OUT")==0){theFilterType="BAND-PASS";}
		}
	else if(filterFunction.compare("svfilter")==0)
		{theFilter="State Variable (2nd order)";
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,commandInput.length()-pos-1));
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(secondInput.c_str()));

		pos=commandOutput.find(",",pos2+1);
		secondOutput=remove_excess_white_space(commandOutput.substr(pos2+1,pos-pos2-1));
		pos2=pos;
		pos=commandOutput.find(",",pos2+1);
		thirdOutput=remove_excess_white_space(commandOutput.substr(pos2+1,pos-pos2-1));
		if(firstOutput.compare("OUT")==0){theFilterType="LOW-PASS";}
		if(secondOutput.compare("OUT")==0){theFilterType="HIGH-PASS";}
		if(thirdOutput.compare("OUT")==0){theFilterType="BAND-PASS";}
		}
	else if(filterFunction.compare("tone")==0)
		{theFilterType="LOW-PASS";
		theFilter="Recursive (1st order)";}
	else if(filterFunction.compare("lowres")==0)
		{theFilterType="LOW-PASS";
		theFilter="Resonant";
		pos2=commandInput.find(",",pos+1);
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,pos2-pos-1));
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(secondInput.c_str()));
		}
	else if(filterFunction.compare("rezzy")==0)
		{theFilterType="LOW-PASS";
		theFilter="Resonant II";
		pos2=commandInput.find(",",pos+1);
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,pos2-pos-1));
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(secondInput.c_str()));
		}
	else if(filterFunction.compare("reson")==0)
		{theFilterType="BAND-PASS";
		theFilter="Resonant";
		secondInput=remove_excess_white_space(commandInput.substr(pos+1,commandInput.length()-pos-1));
		lowerFreq=strtod(firstInput.c_str(),NULL);
		upperFreq=strtod(secondInput.c_str(),NULL);
		upperFreq+=lowerFreq;
		secondInput=formatNumberString(cnvrtNumToStrng(upperFreq,8));
		filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(secondInput.c_str()));}

	int index=-1;
	for(int i=0;i<numFilterTypes;i++)
		{if(filterTypes[i].compare(theFilterType)==0){index=i; break;}
		}
	if(index==-1){cerr<<"Error!\n"; exit(EXIT_FAILURE);}
	filterTypeBox[tabIndex]->setCurrentIndex(index);
	// Update Instrument
	loadFilterComboBox(index);
	index=-1;
	if(theFilterType.compare("HIGH-PASS")==0)
		{for(int i=0;i<numHighPassFilters;i++){if(highPassFilters[i].compare(theFilter)==0){index=i; break;}}
		}
	else if(theFilterType.compare("LOW-PASS")==0)
		{for(int i=0;i<numLowPassFilters;i++){if(lowPassFilters[i].compare(theFilter)==0){index=i; break;}}
		}
	else if(theFilterType.compare("BAND-PASS")==0)
		{for(int i=0;i<numBandPassFilters;i++){if(bandPassFilters[i].compare(theFilter)==0){index=i; break;}}
		}
	if(index!=-1){filterBox[tabIndex]->setCurrentIndex(index);}

	}

void UI::loadFilterComboBox(int index)
	{int tabIndex=soundTab->currentIndex();
	string theFilterType=filterTypes[index];
	
	filterBox[tabIndex]->clear(); filterBox[tabIndex]->show();

	if(theFilterType.compare("HIGH-PASS")==0)
		{for(int i=0;i<numHighPassFilters;i++){filterBox[tabIndex]->addItem(tr(highPassFilters[i].c_str()));}
		}
	else if(theFilterType.compare("LOW-PASS")==0)
		{for(int i=0;i<numLowPassFilters;i++){filterBox[tabIndex]->addItem(tr(lowPassFilters[i].c_str()));}
		}
	else if(theFilterType.compare("BAND-PASS")==0)
		{for(int i=0;i<numBandPassFilters;i++){filterBox[tabIndex]->addItem(tr(bandPassFilters[i].c_str()));}
		}
	else if(theFilterType.compare("NONE")==0)
		{filterInput1[tabIndex]->setText(tr(NO_VALUE)); filterInput1[tabIndex]->hide();
		filterInput1Label[tabIndex]->hide();
		filterInput2[tabIndex]->setText(tr(NO_VALUE)); filterInput2[tabIndex]->hide();
		filterInput2Label[tabIndex]->hide();
		filterBox[tabIndex]->hide();}
	else
		{}
	}

void UI::loadInstrumentComboBox(int index)
	{int tabIndex=soundTab->currentIndex();
	string theCategory=categories[index];
	
	instrumentBox[tabIndex]->clear();

	if(theCategory.compare("BASS")==0)
		{for(int i=0;i<numBassInstruments;i++){instrumentBox[tabIndex]->addItem(tr(bassInstruments[i].c_str()));}
		}
	else if(theCategory.compare("BRASS")==0)
		{for(int i=0;i<numBrassInstruments;i++){instrumentBox[tabIndex]->addItem(tr(brassInstruments[i].c_str()));}
		}
	else if(theCategory.compare("DIGITAL")==0)
		{for(int i=0;i<numDigitalInstruments;i++){instrumentBox[tabIndex]->addItem(tr(digitalInstruments[i].c_str()));}
		}
	else if(theCategory.compare("GUITARS")==0)
		{for(int i=0;i<numGuitarInstruments;i++){instrumentBox[tabIndex]->addItem(tr(guitarInstruments[i].c_str()));}
		}
	else if(theCategory.compare("ORGANS")==0)
		{for(int i=0;i<numOrganInstruments;i++){instrumentBox[tabIndex]->addItem(tr(organInstruments[i].c_str()));}
		}
	else if(theCategory.compare("ORCHESTRA HITS")==0)
		{for(int i=0;i<numOrcHitInstruments;i++){instrumentBox[tabIndex]->addItem(tr(orcHitInstruments[i].c_str()));}
		}
	else if(theCategory.compare("PERCUSSION")==0)
		{for(int i=0;i<numPercussionInstruments;i++){instrumentBox[tabIndex]->addItem(tr(percussionInstruments[i].c_str()));}
		}
	else if(theCategory.compare("PIANO")==0)
		{for(int i=0;i<numPianoInstruments;i++){instrumentBox[tabIndex]->addItem(tr(pianoInstruments[i].c_str()));}
		}
	else if(theCategory.compare("STRINGS")==0)
		{for(int i=0;i<numStringInstruments;i++){instrumentBox[tabIndex]->addItem(tr(stringInstruments[i].c_str()));}
		}
	else if(theCategory.compare("VOICE")==0)
		{for(int i=0;i<numVoiceInstruments;i++){instrumentBox[tabIndex]->addItem(tr(voiceInstruments[i].c_str()));}
		}
	else if(theCategory.compare("WOODWINDS")==0)
		{for(int i=0;i<numWoodwindInstruments;i++){instrumentBox[tabIndex]->addItem(tr(woodwindInstruments[i].c_str()));}
		}
	else if(theCategory.compare("--MORE--")==0)
		{}
	else
		{}
	}

void UI::loadInstrumentComboBox(int catIndex,int tabIndex)
	{//int tabIndex=soundTab->currentIndex();
	string theCategory=categories[catIndex];
	
	instrumentBox[tabIndex]->clear();

	if(theCategory.compare("BASS")==0)
		{for(int i=0;i<numBassInstruments;i++){instrumentBox[tabIndex]->addItem(tr(bassInstruments[i].c_str()));}
		}
	else if(theCategory.compare("BRASS")==0)
		{for(int i=0;i<numBrassInstruments;i++){instrumentBox[tabIndex]->addItem(tr(brassInstruments[i].c_str()));}
		}
	else if(theCategory.compare("DIGITAL")==0)
		{for(int i=0;i<numDigitalInstruments;i++){instrumentBox[tabIndex]->addItem(tr(digitalInstruments[i].c_str()));}
		}
	else if(theCategory.compare("GUITARS")==0)
		{for(int i=0;i<numGuitarInstruments;i++){instrumentBox[tabIndex]->addItem(tr(guitarInstruments[i].c_str()));}
		}
	else if(theCategory.compare("ORGANS")==0)
		{for(int i=0;i<numOrganInstruments;i++){instrumentBox[tabIndex]->addItem(tr(organInstruments[i].c_str()));}
		}
	else if(theCategory.compare("ORCHESTRA HITS")==0)
		{for(int i=0;i<numOrcHitInstruments;i++){instrumentBox[tabIndex]->addItem(tr(orcHitInstruments[i].c_str()));}
		}
	else if(theCategory.compare("PERCUSSION")==0)
		{for(int i=0;i<numPercussionInstruments;i++){instrumentBox[tabIndex]->addItem(tr(percussionInstruments[i].c_str()));}
		}
	else if(theCategory.compare("PIANO")==0)
		{for(int i=0;i<numPianoInstruments;i++){instrumentBox[tabIndex]->addItem(tr(pianoInstruments[i].c_str()));}
		}
	else if(theCategory.compare("STRINGS")==0)
		{for(int i=0;i<numStringInstruments;i++){instrumentBox[tabIndex]->addItem(tr(stringInstruments[i].c_str()));}
		}
	else if(theCategory.compare("VOICE")==0)
		{for(int i=0;i<numVoiceInstruments;i++){instrumentBox[tabIndex]->addItem(tr(voiceInstruments[i].c_str()));}
		}
	else if(theCategory.compare("WOODWINDS")==0)
		{for(int i=0;i<numWoodwindInstruments;i++){instrumentBox[tabIndex]->addItem(tr(woodwindInstruments[i].c_str()));}
		}
	else if(theCategory.compare("--MORE--")==0)
		{}
	else
		{}
	}

void UI::loadFilterInput(int index)
	{int tabIndex=soundTab->currentIndex();
	int filterTypeIndex=filterTypeBox[tabIndex]->currentIndex();
	int filterIndex=index;

	string theFilterType=filterTypes[filterTypeIndex];
	string theFilter,firstLabel="",secondLabel="";
	if(theFilterType.compare("HIGH-PASS")==0)
		{theFilter=highPassFilters[filterIndex];
		if(theFilter.compare("Butterworth")==0||theFilter.compare("Chebyshev I")==0||theFilter.compare("Chebyshev II")==0||theFilter.compare("Elliptical")==0)
			{firstLabel="CutOff Frequency (Hz):";
			secondLabel="Number of Poles:";}
		else if(theFilter.compare("Butterworth (2nd order)")==0||theFilter.compare("Recursive (1st order)")==0)
			{firstLabel="CutOff Frequency (Hz):";}
		else if(theFilter.compare("Resonant (2nd order)")==0)
			{firstLabel="CutOff Frequency (Hz):";
			secondLabel="Resonance Value:";}
		else if(theFilter.compare("State Variable")==0||theFilter.compare("State Variable (2nd order)")==0)
			{firstLabel="CutOff Frequency (Hz):";
			secondLabel="Q Value:";}
		else{}
		}
	else if(theFilterType.compare("LOW-PASS")==0)
		{theFilter=lowPassFilters[filterIndex];
		if(theFilter.compare("Butterworth")==0||theFilter.compare("Chebyshev I")==0||theFilter.compare("Chebyshev II")==0||theFilter.compare("Elliptical")==0)
			{firstLabel="CutOff Frequency (Hz):";
			secondLabel="Number of Poles:";}
		else if(theFilter.compare("Butterworth (2nd order)")==0||theFilter.compare("Recursive (1st order)")==0)
			{firstLabel="CutOff Frequency (Hz):";}
		else if(theFilter.compare("Resonant")==0||theFilter.compare("Resonant II")==0||theFilter.compare("Resonant (2nd order)")==0)
			{firstLabel="CutOff Frequency (Hz):";
			secondLabel="Resonance Value:";}
		else if(theFilter.compare("State Variable")==0||theFilter.compare("State Variable (2nd order)")==0)
			{firstLabel="CutOff Frequency (Hz):";
			secondLabel="Q Value:";}
		else{}
		}
	else if(theFilterType.compare("BAND-PASS")==0)
		{theFilter=bandPassFilters[filterIndex];
		if(theFilter.compare("Butterworth")==0||theFilter.compare("Resonant")==0)
			{firstLabel="Lower CutOff Frequency (Hz):";
			secondLabel="Upper CutOff Frequency (Hz):";}
		else{}
		}
	else{}

	// Update Filter Input Labels
	if(firstLabel.length()>0)
		{filterInput1[tabIndex]->show(); filterInput1[tabIndex]->setText(tr(NO_VALUE));
		filterInput1Label[tabIndex]->show(); filterInput1Label[tabIndex]->setText(tr(firstLabel.c_str()));}
	else{filterInput1Label[tabIndex]->hide(); filterInput1[tabIndex]->hide();}
	if(secondLabel.length()>0)
		{filterInput2[tabIndex]->show(); filterInput2[tabIndex]->setText(tr(NO_VALUE));
		filterInput2Label[tabIndex]->show(); filterInput2Label[tabIndex]->setText(tr(secondLabel.c_str()));}
	else{filterInput2Label[tabIndex]->hide(); filterInput2[tabIndex]->hide();}
	
	}

void UI::loadInstrumentLabel(int index)
	{int tabIndex=soundTab->currentIndex();
	int catIndex=categoryBox[tabIndex]->currentIndex();
	int instrIndex=index;
	//cout<<tabIndex<<"|"<<index<<"|"<<categoryBox[tabIndex]->currentIndex()<<endl;
	//string theInstrument=instrumentBox[index]->itemData(instrumentBox[index]->currentIndex());
	//QVariant theInstrument=instrumentBox[tabIndex]->itemData(instrumentBox[tabIndex]->currentIndex());
	//QString theString=theInstrument.toString();
	//cout<<theString.toStdString()<<endl;
	string theCategory=categories[catIndex];
	string theInstrument;
	if(theCategory.compare("BASS")==0){theInstrument=bassInstruments[instrIndex];}
	else if(theCategory.compare("BRASS")==0){theInstrument=brassInstruments[instrIndex];}
	else if(theCategory.compare("DIGITAL")==0){theInstrument=digitalInstruments[instrIndex];}
	else if(theCategory.compare("GUITARS")==0){theInstrument=guitarInstruments[instrIndex];}
	else if(theCategory.compare("ORGANS")==0){theInstrument=organInstruments[instrIndex];}
	else if(theCategory.compare("ORCHESTRA HITS")==0){theInstrument=orcHitInstruments[instrIndex];}
	else if(theCategory.compare("PERCUSSION")==0){theInstrument=percussionInstruments[instrIndex];}
	else if(theCategory.compare("PIANO")==0){theInstrument=pianoInstruments[instrIndex];}
	else if(theCategory.compare("STRINGS")==0){theInstrument=stringInstruments[instrIndex];}
	else if(theCategory.compare("VOICE")==0){theInstrument=voiceInstruments[instrIndex];}
	else if(theCategory.compare("WOODWINDS")==0){theInstrument=woodwindInstruments[instrIndex];}
	else if(theCategory.compare("--MORE--")==0){}
	else
		{}
	//cout<<theInstrument<<endl;
	instrLabel[tabIndex]->setText(tr(theInstrument.c_str()));
	}

void UI::loadSong()
	{QString qSongName=songHistoryBox->currentText();
	string theSongName=qSongName.toStdString();
	string songFldr=soundFldr+theSongName+"/";
	string songInputFile=songFldr+"Song_Input";
	songName->setText(tr(theSongName.c_str()));
	numSound=1;
	if(numSound<=1)
		{// Hide Removal Button
		remSoundButton->hide();}
	int N=soundTab->count();
	for(int i=1;i<N;i++)
		{soundTab->removeTab(i);
		soundFileLabel[i]->setText(tr(NO_VALUE));
		instrLabel[i]->setText(tr(NO_VALUE));
		filterInput1[i]->setText(tr(NO_VALUE)); filterInput1[i]->hide();
		filterInput1Label[i]->hide();
		filterInput2[i]->setText(tr(NO_VALUE)); filterInput2[i]->hide();
		filterInput2Label[i]->hide();
		}
	soundFileLabel[0]->setText(tr(NO_VALUE));
	instrLabel[0]->setText(tr(NO_VALUE));
	filterInput1[0]->setText(tr(NO_VALUE)); filterInput1[0]->hide();
	filterInput1Label[0]->hide();
	filterInput2[0]->setText(tr(NO_VALUE)); filterInput2[0]->hide();
	filterInput2Label[0]->hide();
	// Erase Data from deleted tab
	//pH[N-1]->setText(tr(NO_VALUE));pHLabel[N-1]->setText(tr("<b><span style=\"color:red;\">pH</span></b>:"));
	read_song_input_file(songInputFile);
	}

void UI::addSoundTab()
	{//int tabIndex=scTab->currentIndex();
	// Increase NUmber of Solution Conditions
	numSound++;
	if(numSound>1)
		{// Show Removal Button
		remSoundButton->show();}

	// Define New Tab Index
	int N=soundTab->count();
	// Define Sound Tab
	QWidget *tab1 = new QWidget;
	//
	QLabel* categoryLabel=new QLabel(tr("Category:")); categoryLabel->setAlignment(Qt::AlignRight);
	QLabel* instrumentLabel=new QLabel(tr("Instrument:")); instrumentLabel->setAlignment(Qt::AlignRight);
	QLabel* amplitudeLabel=new QLabel(tr("Amplitude:")); amplitudeLabel->setAlignment(Qt::AlignRight);
	QLabel* filterLabel=new QLabel(tr("Filter:")); filterLabel->setAlignment(Qt::AlignRight);
	QLabel* hiddenLabel=new QLabel; hiddenLabel->setFixedWidth(100);
	QLabel* hiddenLabel1=new QLabel;
	QLabel* hiddenLabel2=new QLabel;
	QLabel* hiddenLabel4=new QLabel;
	categoryBox[N]=new QComboBox;
	amplitude[N]=new QLineEdit; amplitude[N]->setText(tr("1.0")); amplitude[N]->setAlignment(Qt::AlignCenter);
	for(int i=0;i<numCategories;i++){categoryBox[N]->addItem(tr(categories[i].c_str()));}
	//connect(selectSoluteButton_1,SIGNAL(activated(const QString&)),this,SLOT(solute_defined(const QString&)));
	connect(categoryBox[N],SIGNAL(activated(int)),this,SLOT(loadInstrumentComboBox(int)));
	instrumentBox[N]=new QComboBox;
	connect(instrumentBox[N],SIGNAL(activated(int)),this,SLOT(loadInstrumentLabel(int)));
	//instrumentLabel[N]=new QLabel; instrumentLabel[N]->setText(tr(NO_VALUE)); instrumentLabel[N]->setAlignment(Qt::AlignCenter);
	filterTypeBox[N]=new QComboBox;
	for(int i=0;i<numFilterTypes;i++){filterTypeBox[N]->addItem(tr(filterTypes[i].c_str()));}
	connect(filterTypeBox[N],SIGNAL(activated(int)),this,SLOT(loadFilterComboBox(int)));
	filterBox[N]=new QComboBox; filterBox[N]->hide();
	connect(filterBox[N],SIGNAL(activated(int)),this,SLOT(loadFilterInput(int)));
	filterInput1Label[N]=new QLabel; filterInput1Label[N]->setAlignment(Qt::AlignRight); filterInput1Label[N]->hide();
	filterInput1[N]=new QLineEdit; filterInput1[N]->setText(tr(NO_VALUE)); filterInput1[N]->setAlignment(Qt::AlignCenter); filterInput1[N]->hide();
	filterInput2Label[N]=new QLabel; filterInput2Label[N]->setAlignment(Qt::AlignRight); filterInput2Label[N]->hide();
	filterInput2[N]=new QLineEdit; filterInput2[N]->setText(tr(NO_VALUE)); filterInput2[N]->setAlignment(Qt::AlignCenter); filterInput2[N]->hide();

	QPushButton* openButton=new QPushButton(tr("Open a Sound File"));
	connect(openButton,SIGNAL(clicked()),this,SLOT(open_sound_file()));
	soundFileLabel[N]=new QLabel; soundFileLabel[N]->setText(tr(NO_VALUE)); soundFileLabel[N]->setAlignment(Qt::AlignCenter);
	instrLabel[N]=new QLabel; instrLabel[N]->setText(tr(NO_VALUE)); instrLabel[N]->setAlignment(Qt::AlignCenter);
	//
	QGridLayout *tab1hbox = new QGridLayout;
	tab1hbox->setSizeConstraint(QLayout::SetFixedSize);
	tab1hbox->addWidget(categoryLabel,0,0,1,1); tab1hbox->addWidget(categoryBox[N],0,1,1,1); tab1hbox->addWidget(hiddenLabel,0,2,1,1); tab1hbox->addWidget(openButton,0,3,1,1);
	tab1hbox->addWidget(instrumentLabel,1,0,1,1); tab1hbox->addWidget(instrumentBox[N],1,1,1,1); tab1hbox->addWidget(soundFileLabel[N],1,2,1,2);
	tab1hbox->addWidget(amplitudeLabel,2,0,1,1); tab1hbox->addWidget(amplitude[N],2,1,1,1); tab1hbox->addWidget(instrLabel[N],2,2,1,2);
	tab1hbox->addWidget(filterLabel,3,0,1,1); tab1hbox->addWidget(filterTypeBox[N],3,1,1,1); tab1hbox->addWidget(filterInput1Label[N],3,2,1,1); tab1hbox->addWidget(filterInput1[N],3,3,1,1);
	tab1hbox->addWidget(hiddenLabel4,4,0,1,1); tab1hbox->addWidget(filterBox[N],4,1,1,1); tab1hbox->addWidget(filterInput2Label[N],4,2,1,1); tab1hbox->addWidget(filterInput2[N],4,3,1,1);
	tab1->setLayout(tab1hbox);

	string tmp="&"+cnvrtNumToStrng(N,0);
	soundTab->addTab(tab1, tr(tmp.c_str()));
	}

string UI::get_instrument_category(string theInstrument)
	{// Compile all Instruments into one list
	string lst="",catLst="",delimiter=";",theCategory;
	for(int a=0;a<numCategories;a++)
		{theCategory=categories[a];
		if(theCategory.compare("BASS")==0)
			{for(int i=0;i<numBassInstruments;i++){lst+=bassInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("BRASS")==0)
			{for(int i=0;i<numBrassInstruments;i++){lst+=brassInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("DIGITAL")==0)
			{for(int i=0;i<numDigitalInstruments;i++){lst+=digitalInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("GUITARS")==0)
			{for(int i=0;i<numGuitarInstruments;i++){lst+=guitarInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("ORGANS")==0)
			{for(int i=0;i<numOrganInstruments;i++){lst+=organInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("ORCHESTRA HITS")==0)
			{for(int i=0;i<numOrcHitInstruments;i++){lst+=orcHitInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("PERCUSSION")==0)
			{for(int i=0;i<numPercussionInstruments;i++){lst+=percussionInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("PIANO")==0)
			{for(int i=0;i<numPianoInstruments;i++){lst+=pianoInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("STRINGS")==0)
			{for(int i=0;i<numStringInstruments;i++){lst+=stringInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("VOICE")==0)
			{for(int i=0;i<numVoiceInstruments;i++){lst+=voiceInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("WOODWINDS")==0)
			{for(int i=0;i<numWoodwindInstruments;i++){lst+=woodwindInstruments[i]+delimiter; catLst+=theCategory+delimiter;}
			}
		else if(theCategory.compare("--MORE--")==0)
			{}
		else
			{}
		}
	
	int numInstruments=count_delimiter(lst,delimiter);
	string *allInstruments=fill_string_array(lst,numInstruments,delimiter);
	string *allCategories=fill_string_array(catLst,numInstruments,delimiter);
	int index=-1;
	for(int i=0;i<numInstruments;i++)
		{if(allInstruments[i].compare(theInstrument)==0){index=i; break;}
		}
	if(index==-1){cerr<<"Error instrument not found!\n"; exit(EXIT_FAILURE);}
	return allCategories[index];}

int UI::get_instrument_index(string theInstrument,string theCategory)
	{int index=-1;
	if(theCategory.compare("BASS")==0)
		{for(int i=0;i<numBassInstruments;i++){if(bassInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("BRASS")==0)
		{for(int i=0;i<numBrassInstruments;i++){if(brassInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("DIGITAL")==0)
		{for(int i=0;i<numDigitalInstruments;i++){if(digitalInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("GUITARS")==0)
		{for(int i=0;i<numGuitarInstruments;i++){if(guitarInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("ORGANS")==0)
		{for(int i=0;i<numOrganInstruments;i++){if(organInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("ORCHESTRA HITS")==0)
		{for(int i=0;i<numOrcHitInstruments;i++){if(orcHitInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("PERCUSSION")==0)
		{for(int i=0;i<numPercussionInstruments;i++){if(percussionInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("PIANO")==0)
		{for(int i=0;i<numPianoInstruments;i++){if(pianoInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("STRINGS")==0)
		{for(int i=0;i<numStringInstruments;i++){if(stringInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("VOICE")==0)
		{for(int i=0;i<numVoiceInstruments;i++){if(voiceInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("WOODWINDS")==0)
		{for(int i=0;i<numWoodwindInstruments;i++){if(woodwindInstruments[i].compare(theInstrument)==0){index=i; break;}}
		}
	else if(theCategory.compare("--MORE--")==0)
		{}
	else
		{}
	// Error Check
	if(index==-1){cerr<<"Error in get_instrument_index!\nIndex could not be determined.\n"; exit(EXIT_FAILURE);}
	return index;}

void UI::removeSoundTab()
	{// Reduce Number of Solution Conditions
	numSound--;
	if(numSound<=1)
		{// Hide Removal Button
		remSoundButton->hide();}
	int N=soundTab->count();
	soundTab->removeTab(N-1);
	// Erase Data from deleted tab
	//pH[N-1]->setText(tr(NO_VALUE));pHLabel[N-1]->setText(tr("<b><span style=\"color:red;\">pH</span></b>:"));
	soundFileLabel[N-1]->setText(tr(NO_VALUE));
	instrLabel[N-1]->setText(tr(NO_VALUE));
	}

void UI::open_song_file()
	{QStringList files=QFileDialog::getOpenFileNames(this,tr("files"),tr(soundFldr.c_str()),tr("All files (*)") );
	//for(int i=0;i<files.count();i++){cout<<i<<" "<<files.at(i).toLocal8Bit().constData()<<endl;}
	// Number of PDB Files Input by USer
	int N=files.count();
	int tabIndex=soundTab->currentIndex(),pos,pos2;
	string tmp,tmp2;//=files.at(0).toLocal8Bit().constData();
	for(int i=0;i<N;i++){tmp=files.at(i).toLocal8Bit().constData();}
	//
	if(tmp.length()>0)
		{
/*		numSound=1;
		if(numSound<=1)
			{// Hide Removal Button
			remSoundButton->hide();}
		N=soundTab->count();
		for(int i=1;i<N;i++)
			{soundTab->removeTab(i);
			soundFileLabel[i]->setText(tr(NO_VALUE));
			instrLabel[i]->setText(tr(NO_VALUE));
			filterInput1[i]->setText(tr(NO_VALUE)); filterInput1[i]->hide();
			filterInput1Label[i]->hide();
			filterInput2[i]->setText(tr(NO_VALUE)); filterInput2[i]->hide();
			filterInput2Label[i]->hide();
			}
		soundFileLabel[0]->setText(tr(NO_VALUE));
		instrLabel[0]->setText(tr(NO_VALUE));
		filterInput1[0]->setText(tr(NO_VALUE)); filterInput1[0]->hide();
		filterInput1Label[0]->hide();
		filterInput2[0]->setText(tr(NO_VALUE)); filterInput2[0]->hide();
		filterInput2Label[0]->hide();
*/
		pos=tmp.find(soundFldr,0);
		if(pos!=string::npos)
			{pos=tmp.rfind("/");
			pos2=tmp.rfind("/",pos-1);
			//cout<<"|"<<tmp.substr(pos2+1,pos-pos2-1)<<"|"<<endl;
			tmp2=tmp.substr(pos2+1,pos-pos2-1);
			songName->setText(tr(tmp2.c_str()));
			}
		read_song_input_file(tmp);}
	}

void UI::open_sound_file()
	{QStringList files=QFileDialog::getOpenFileNames(this,tr("files"),tr(soundFldr.c_str()),tr("All files (*)") );
	//for(int i=0;i<files.count();i++){cout<<i<<" "<<files.at(i).toLocal8Bit().constData()<<endl;}
	int N=files.count();
	int tabIndex=soundTab->currentIndex();
	string tmp;//=files.at(0).toLocal8Bit().constData();
	for(int i=0;i<N;i++){tmp=files.at(i).toLocal8Bit().constData();}
	//
	if(tmp.length()>0)
		{soundFiles[tabIndex]=tmp;
		soundFileLabel[tabIndex]->setText(tmp.c_str());
		}
	}

void UI::loadHistory()
	{ifstream fIn;
	ofstream fOut;
	fIn.open(songHistoryFile.c_str());
	if(fIn.fail())
		{//cerr<<"Error in loadHistory!\n"; exit(EXIT_FAILURE);
		// Generate Empty file
		fOut.open(songHistoryFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut.fail()){cerr<<"ERROR in loadHistory!!!\nFile could not be opened.\n"<<songHistoryFile<<endl;exit(EXIT_FAILURE);}
		fOut.close();
		// Re-Attempt to open
		fIn.open(songHistoryFile.c_str());
		if(fIn.fail()){cerr<<"Error in loadHistory!\n"; exit(EXIT_FAILURE);}
		}
	int Sz=15000;
	char Val[Sz];
	string tmp,lst="",delimiter=";";
	fIn.getline(Val,Sz);		//cout<<Val<<endl;
	while(!fIn.eof())
		{tmp=Val;		//cout<<tmp<<endl;
		// Append to List
		lst+=tmp+delimiter;
		fIn.getline(Val,Sz);}
	fIn.close();
	int N=0;
	if(lst.length()>0){N=count_delimiter(lst,delimiter);}
	string *songNames;
	if(N>0)
		{songNames=fill_string_array(lst,N,delimiter);
		// Load Song History Combo Box
		songHistoryBox->clear();
		for(int i=0;i<N;i++){songHistoryBox->addItem(tr(songNames[i].c_str()));}
		}
	}

void UI::read_song_input_file(string inFile)
	{//syntheticSong S;
	ifstream fIn;
	fIn.open(inFile.c_str());
	if(fIn.fail()){cerr<<"ERROR in UI::read_song_input_file!\nInput music file could not be opened.\n"<<inFile<<endl; exit(EXIT_FAILURE);}
	bool FILE_FOUND=false, AMP_FOUND=false, SEARCHING;
	int Sz=1500000, pos, pos2, N, Counter=0;
	char Val[Sz];
	string tmp, tmp2, soundFile, soundAmp, soundType, filter, fileLst="", ampLst="", delimiter="?",content, inputValue, lst="",stLst="", filterLst="",innerDelimiter="`";
	bool TEMPO_FOUND=false;
	//double timePerMeasure;
	string timePerMeasure;
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
				//timePerMeasure=strtod(inputValue.c_str(),NULL);
				timePerMeasure=inputValue;
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
				else{cerr<<"Error in UI::read_song_input_file\nUn-predicted error handling sound file:\n"<<soundFile<<"\n"; exit(EXIT_FAILURE);}
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
					else{cerr<<"Error in UI::read_song_input_file\nUn-predicted error handling amplitude:\n"<<soundAmp<<"\n"; exit(EXIT_FAILURE);}
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
				else{cerr<<"Error in UI::read_song_input_file\nUn-predicted error handling amplitude:\n"<<soundType<<"\n"; exit(EXIT_FAILURE);}
				// Record Sound Type For Later
				stLst+=soundType+delimiter;
				}
			// Handle Possible Case of Filter
			pos=tmp.find("FILTER_",0);
			if(pos!=string::npos)
				{// Extract Sound Type Information
				if(tmp.find(" = ",0)!=string::npos){filter=tmp.substr(tmp.find(" = ",0)+3,tmp.length()-tmp.find(" = ",0)-3);}
				else if(tmp.find("= ",0)!=string::npos){filter=tmp.substr(tmp.find("= ",0)+2,tmp.length()-tmp.find("= ",0)-2);}
				else if(tmp.find(" =",0)!=string::npos){filter=tmp.substr(tmp.find(" =",0)+2,tmp.length()-tmp.find(" =",0)-2);}
				else if(tmp.find("=",0)!=string::npos){filter=tmp.substr(tmp.find("=",0)+1,tmp.length()-tmp.find("=",0)-1);}
				else if(tmp.find("=",0)==string::npos){cerr<<"Error in read_song_input_file\nBadly formatted sound type for component ("<<Counter+1<<")\n"<<soundType<<"\n"; exit(EXIT_FAILURE);}
				else{cerr<<"Error in UI::read_song_input_file\nUn-predicted error handling amplitude:\n"<<soundType<<"\n"; exit(EXIT_FAILURE);}
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

	// Update Tempo
	tempo->setText(tr(timePerMeasure.c_str()));
	int numComps=Counter,index;
	string *amplitudes=fill_string_array(ampLst,numComps,delimiter);
	// Extract Amplitude Values
	lst="";
	for(int i=0;i<numComps;i++)
		{pos=amplitudes[i].find("=",0);
		if(pos!=string::npos)
			{// Extract
			lst+=remove_excess_white_space(amplitudes[i].substr(pos+1,amplitudes[i].length()-pos-2))+delimiter;
			}
		}
	string *ampValues=fill_string_array(lst,numComps,delimiter);
	string *soundFiles=fill_string_array(fileLst,numComps,delimiter);
	string *soundTypes=fill_string_array(stLst,numComps,delimiter);
	string *filterCommands=fill_string_array(filterLst,numComps,delimiter);
	string theCategory,theFilterType;
	for(int i=0;i<numComps;i++)
		{if(i>0)
			{// Generate New Sound Tab
			addSoundTab();}
		// Update Amplitude
		amplitude[i]->setText(tr(ampValues[i].c_str())); amplitude[i]->setAlignment(Qt::AlignCenter);
		// Determine Category from Instrument/SoundType
		theCategory=get_instrument_category(soundTypes[i]);
		index=-1;
		for(int a=0;a<numCategories;a++){if(categories[a].compare(theCategory)==0){index=a; break;}}
		if(index==-1){cerr<<"Error could not find category\n"; exit(EXIT_FAILURE);}
		// Update Instrument Category
		categoryBox[i]->setCurrentIndex(index);
		// Update Instrument
		loadInstrumentComboBox(index,i);
		instrumentBox[i]->setCurrentIndex(get_instrument_index(soundTypes[i],theCategory));
		// Update Instrument Label
		instrLabel[i]->setText(tr(soundTypes[i].c_str()));
		// Update Sound File Label
		soundFileLabel[i]->setText(tr(soundFiles[i].c_str()));
		// Update Filter Input (if any)
		//cout<<"|"<<filterCommands[i]<<"|"<<endl;
		if(filterCommands[i].length()>0)
			{readFilterCommand(filterCommands[i],i);
			}
		}
	}

void UI::record_audio()
	{// Define Song Folder in SOUND folder
	QString qTmp=songName->text();
	string theSongName=qTmp.toStdString();

	if(theSongName.compare(NO_VALUE)==0 || theSongName.length()==0){cerr<<"Error!\nNeed a song name\n"; return;}	
	string songFldr=soundFldr+theSongName+"/";
	string waveFile=songFldr+theSongName+".wav";
	string audioFile=songFldr+"RECORDING.wav";
	string finalOutputFile=songFldr+"MERGED.wav";

	string record_with_playback=get_current_dir_name(); record_with_playback+="/bin/record_with_playback.exe";
	string record_sound=get_current_dir_name(); record_sound+="/bin/record_sound.exe";
	string merge_wave_files=get_current_dir_name(); merge_wave_files+="/bin/merge_wave_files.exe";
	string cmd="\""+record_with_playback+"\" -i \""+waveFile+"\" -e \""+record_sound+"\" -o \""+audioFile+"\"";
	int PID=fork(),status2,result;
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
/*		char* argv[]={the_executable.c_str(),"-i",songFile.c_str(),NULL};
		char* envp[]={NULL};
		//freopen(logFile.c_str(),"w",stderr);
		result=syscall(SYS_execve,the_executable.c_str(),argv,envp);
		if(result<0)
			{cerr<<"ERROR!\n";
			exit(EXIT_FAILURE);}
*/
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR running record_with_playback.exe!\n"; exit(EXIT_FAILURE);}
		exit(EXIT_SUCCESS);
		}
	else if(PID>0){waitpid(PID,&status2,0);}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	cout<<"WAV Recording Complete!\n";

	sleep(2);

	// Merge Wave Files
	cmd="\""+merge_wave_files+"\" -a 0.1 -b 1 -i \""+waveFile+"\" -j \""+audioFile+"\" -o \""+finalOutputFile+"\"";
	result=system(cmd.c_str());
	if(result<0){cerr<<"ERROR running record_with_playback.exe!\n"; exit(EXIT_FAILURE);}
	cout<<"WAV Merging Complete!\n";
	}

void UI::updateHistoryFile(string songNm)
	{ifstream fIn;
	ofstream fOut;
	fIn.open(songHistoryFile.c_str());
	int Sz=15000,pos;
	char Val[Sz];
	bool ALREADY_EXIST;
	string cpy="",tmp,inputName,inputVal;
	if(fIn.fail())
		{//cout<<"Failed to open .history\n";
		//cpy=".apbsFldr:\n.filesFldr:\n.hydroproFldr:\n.msmsFldr:\n.pdb2pqrFldr:\n.zpredFldr:\n";
		//fOut.open(refFile.c_str(),ofstream::out|ofstream::trunc);
		fOut.open(songHistoryFile.c_str(),ofstream::out|ofstream::trunc);
		if(fOut.fail()){cerr<<"ERROR in updateHistoryFile!!!\nFile could not be opened.\n"<<songHistoryFile<<endl;exit(EXIT_FAILURE);}
		tmp=songNm+"\n";
		fOut<<tmp;
		fOut.close();
		}
	else
		{// .history exists
		// Copy & Update File
		//cout<<"Opened .history\n";
		ALREADY_EXIST=false;
		fIn.getline(Val,Sz);		//cout<<Val<<endl;
		while(!fIn.eof())
			{tmp=Val;		//cout<<tmp<<endl;
			// Compare line with input song name
			if(tmp.compare(songNm)==0)
				{ALREADY_EXIST=true;
				break;}
			fIn.getline(Val,Sz);}
		fIn.close();
		//
		if(!ALREADY_EXIST)
			{// Add song name to .history
			fOut.open(songHistoryFile.c_str(),ofstream::out|ofstream::app);
			if(fOut.fail()){cerr<<"ERROR in updateHistoryFile!!!\nFile could not be opened.\n"<<songHistoryFile<<endl;exit(EXIT_FAILURE);}
			tmp=songNm+"\n";
			fOut<<tmp;
			fOut.close();
			}
		}
	}

void UI::write_song_input_file()
	{string currFldr=get_current_dir_name(); currFldr+="/";
	//
	QString tmp=songName->text();
	string theSongName=tmp.toStdString();

	if(theSongName.compare(NO_VALUE)==0)
		{cerr<<"User Input Error!\nNeed a song name\n";
		return;}	
	string songFldr=soundFldr+theSongName+"/";
	// Make New Directory for Song Name
	//cout<<songFldr<<endl;
	int check,width=80;
	if(!directory_exist(songFldr)){check=mkdir(songFldr.c_str(),0777);}
	string songFile=songFldr+"Song_Input";
	string Output="#\n";
	Output+="# Song Input\n";
	Output+="#\n";
	Output+="# Sound Types\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# ORCHESTRA HITS\n";
	Output+="# accordion_hit | alien_hit | bjorkestra_hit | classic_hit | deport_hit | justified_hit\n";
	Output+="# numb_hit | phantom_hit | quick_hit | robotech_hit | rock_hit | sleeptalk_hit\n";
	Output+="# star_wars_hit | t_hit | twilight_zone_hit\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# VOICE\n";
	Output+="# boy_choir | worship_choir\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# BRASS\n";
	Output+="# brass | flugelhorns | trumpet | trumpets | soft_horn | berlin_trumpet | boston_brass \n";
	Output+="# monkey_thump | flugelradio | brass_squirt\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# GUITAR\n";
	Output+="# guitar | rock_guitar | lead_guitars | sitar | steel_guitar | funky_guitar | seagull_acoustic_guitar  \n";
	Output+="# multi_guitar | fret_guitar | session_guitar | acoustic_guitar | stage_guitar | echo_metal_guitar\n";
	Output+="# strat_marshall | aeon_flux_guitar | NIN_guitar\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# PIANO/HARP\n";
	Output+="# piano | steinway_grand_piano | velocity_grand_piano | harpsichord | grand_piano | barharp\n";
	Output+="# german8_harpsichord | roland_piano | piano_set | crystal_rhodes_piano | strings_with_piano\n";
	Output+="# U20_piano \n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# BASS\n";
	Output+="# plucked_bass | solid_bass | kick_arse_bass | stud_bass | laid_bass | roland_bass | roland_bass2 \n";
	Output+="# endorian_bass | jv_1080_bass | metallic_bass | seinfeld_bass\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# ORGAN\n";
	Output+="# tired_accordion\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# WIND\n";
	Output+="# mean_saxophone | mourn_saxophone | clarinet | flute | whistle | bottle_blow\n";
	Output+="# pagan_whistle | didgeridoo \n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# STRINGS\n";
	Output+="# pluck | pluck2 | strings | strings2 | straight_strings | plucked_string | bounce_bow \n";
	Output+="# deep_string | violin | ensemble_violin | violin_pizzicato | dirty_strings\n";
	Output+="# fantastic_strings | roland_dulcimer | roland_violin | roland_strings | strings_with_piano\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# PERCUSSION\n";
	Output+="# drums\n";
	Output+="# miramba\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# DIGITAL\n";
	Output+="# sinusoid\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# In Sound Files,\n";
	Output+="#	O	<- regular note\n";
	Output+="#	S	<- sustained note\n";
	Output+="#	X	<- staccato note\n";
	Output+="# "+repeat_string("-",width)+"\n";
	Output+="# Tempo (seconds)\n";
	// Get Time Per Measure
	tmp=tempo->text();
	string timePerMeasure=tmp.toStdString();
	if(timePerMeasure.compare(NO_VALUE)==0)
		{cerr<<"User Input Error!\nNeed a tempo (i.e. the time per measure)\n";
		return;}
	Output+="TIME_PER_MEASURE = "+timePerMeasure+"\n";
	Output+="# -------------------------------------------------------------------------\n";
	int N=numSound;
	string amp,filePath,instrument,newSoundFile,inputValue,filterCommand;
	for(int i=0;i<N;i++)
		{// Get Amplitude
		tmp=amplitude[i]->text();
		amp=tmp.toStdString();
		if(amp.compare(NO_VALUE)==0){return;}
		// Get File Path
		tmp=soundFileLabel[i]->text();
		filePath=tmp.toStdString();
		// Copy Sound File into Song Folder
		newSoundFile=songFldr+extract_file_name(filePath);
		if(newSoundFile.compare(filePath)!=0)
			{if(!copyFile(filePath,newSoundFile)){cerr<<"Error in UI::write_song_file!\nCould not copy sound file ("<<filePath<<") to song folder ("<<newSoundFile<<")\n"; exit(EXIT_FAILURE);}}		
		if(filePath.compare(NO_VALUE)==0)
			{cerr<<"User Input Error!\nNeed to select a sound file for component #"+cnvrtNumToStrng(i+1,0)+"\n";
			return;}
		// Get Instrument
		tmp=instrLabel[i]->text();
		instrument=tmp.toStdString();
		if(instrument.compare(NO_VALUE)==0)
			{cerr<<"User Input Error!\nNeed to select an instrument for component #"+cnvrtNumToStrng(i+1,0)+"\n";
			return;}
		// Get Filter Input
		filterCommand=generateFilterCommand(i);
		//
		Output+="# Component "+cnvrtNumToStrng(i+1,0)+"\n";
		//Output+="FILE_"+cnvrtNumToStrng(i+1,0)+" = "+filePath+"\n";
		Output+="FILE_"+cnvrtNumToStrng(i+1,0)+" = "+newSoundFile+"\n";
		Output+="AMPLITUDE_"+cnvrtNumToStrng(i+1,0)+" = "+amp+"\n";
		Output+="SOUND_"+cnvrtNumToStrng(i+1,0)+" = "+instrument+"\n";
		Output+="FILTER_"+cnvrtNumToStrng(i+1,0)+" = "+filterCommand+"\n";
		Output+="# -------------------------------------------------------------------------\n";
		}
	ofstream fOut;
	fOut.open(songFile.c_str(),ofstream::out|ofstream::trunc);
	if(fOut.fail()){cerr<<"ERROR in write_song_input_file!\nCould not open song input file to write blank drums score.\n"<<songFile<<endl; exit(EXIT_FAILURE);}
	fOut<<Output;
	fOut.close();

	string write_wave=get_current_dir_name(); write_wave+="/bin/write_wave_file.exe";
	string denoise_wave=get_current_dir_name(); denoise_wave+="/bin/denoise_song.exe";
	string outputWaveFile=songFldr+theSongName+".wav";
	string cmd="\""+write_wave+"\" -i \""+songFile+"\" -o \""+outputWaveFile+"\"";
	int PID=fork(),status2,result;
	//string logFile=get_current_dir_name(); logFile+="/log_stderr";
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
/*		char* argv[]={the_executable.c_str(),"-i",songFile.c_str(),NULL};
		char* envp[]={NULL};
		//freopen(logFile.c_str(),"w",stderr);
		result=syscall(SYS_execve,the_executable.c_str(),argv,envp);
		if(result<0)
			{cerr<<"ERROR!\n";
			exit(EXIT_FAILURE);}
*/
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR running write_wave_file.exe!\n"; exit(EXIT_FAILURE);}
		exit(EXIT_SUCCESS);
		}
	else if(PID>0){waitpid(PID,&status2,0);}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}

	// Update Song History
	if(saveHistoryBox->isChecked()){updateHistoryFile(theSongName); loadHistory();}

	// De-Noise Sound
	string tmpInputFile;
	if(denoiseBox->isChecked())
		{cout<<"De-Noising Song...";
		tmpInputFile=get_current_dir_name(); tmpInputFile+="/WAV/temp_input.wav";
		copyFile(outputWaveFile,tmpInputFile);
		cmd="\""+denoise_wave+"\" -i \""+tmpInputFile+"\" -o \""+outputWaveFile+"\"";
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR running denoise_song.exe!\n"; exit(EXIT_FAILURE);}
		cout<<"Done!\n";
		}

	// Convert to .mp3 Format
	string mp3File;
	if(mp3Box->isChecked())
		{cout<<"Starting .mp3 Conversion...";
		mp3File=songFldr+theSongName+".mp3";
		cmd="ffmpeg -v verbose -r 44100 -ac 2 -i \""+outputWaveFile+"\" \""+mp3File+"\"";
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR running ffmpeg!\n"; exit(EXIT_FAILURE);}
		cout<<"Done!\nWrote "<<mp3File<<endl;
		}

	// Sonic Visualizer
	if(sonicVisualizerBox->isChecked())
		{cmd="sonic-visualiser \""+outputWaveFile+"\"";
		PID=fork();
		if(PID==0)
			{// Run in Child Process, next time fork is executed it gives child process PID
			cout<<"Starting Sonic Visualiser...";
			result=system(cmd.c_str());
			if(result<0){cerr<<"ERROR running sonic-visualiser!\n"; exit(EXIT_FAILURE);}
			cout<<"Done!\n";
			exit(EXIT_SUCCESS);
			}
		else if(PID>0){/*waitpid(PID,&status2,0);*/}
		else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
		else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
		}

	// Audio Playback
	if(playbackBox->isChecked())
		{cmd="aplay \""+outputWaveFile+"\"";
		PID=fork();
		if(PID==0)
			{// Run in Child Process, next time fork is executed it gives child process PID
			cout<<"Starting Audio Playback...";
			result=system(cmd.c_str());
			if(result<0){cerr<<"ERROR running aplay!\n"; exit(EXIT_FAILURE);}
			cout<<"Done!\n";
			exit(EXIT_SUCCESS);
			}
		else if(PID>0){/*waitpid(PID,&status2,0);*/}
		else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
		else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
		}

	//exit(EXIT_SUCCESS);
	cout<<"WAV Output Finished!\n";
	//
	recordAudioButton->show();
	}

void UI::launchviewInstrumentsList()
	{VIL=new viewInstrumentsList();
	VIL->show();}

void UI::launchwriteDemoAndBlankFiles()
	{DBF=new writeDemoAndBlankFiles();
	DBF->show();}

viewInstrumentsList::viewInstrumentsList(QWidget *parent) : QWidget(parent)
	{int totalInstruments=0;
	// Generate Instrument List in HTML
	string lst="",tmp;
	// BASS
	lst+="<u>"+cnvrtNumToStrng(numBassInstruments,0)+" <b>BASS</b> Instruments</u><br>";
	for(int i=0;i<numBassInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+bassInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numBassInstruments;
	// BRASS
	lst+="<u>"+cnvrtNumToStrng(numBrassInstruments,0)+" <b>BRASS</b> Instruments</u><br>";
	for(int i=0;i<numBrassInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+brassInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numBrassInstruments;
	// DIGITAL
	lst+="<u>"+cnvrtNumToStrng(numDigitalInstruments,0)+" <b>DIGITAL</b> Instruments</u><br>";
	for(int i=0;i<numDigitalInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+digitalInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numDigitalInstruments;
	// GUITARS
	lst+="<u>"+cnvrtNumToStrng(numGuitarInstruments,0)+" <b>GUITARS</b> Instruments</u><br>";
	for(int i=0;i<numGuitarInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+guitarInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numGuitarInstruments;
	// ORGANS
	lst+="<u>"+cnvrtNumToStrng(numOrganInstruments,0)+" <b>ORGANS</b> Instruments</u><br>";
	for(int i=0;i<numOrganInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+organInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numOrganInstruments;
	// ORCHESTRA HITS
	lst+="<u>"+cnvrtNumToStrng(numOrcHitInstruments,0)+" <b>ORCHESTRA HITS</b> Instruments</u><br>";
	for(int i=0;i<numOrcHitInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+orcHitInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numOrcHitInstruments;
	// PERCUSSION
	lst+="<u>"+cnvrtNumToStrng(numPercussionInstruments,0)+" <b>PERCUSSION</b> Instruments</u><br>";
	for(int i=0;i<numPercussionInstruments;i++)
		{tmp=percussionInstruments[i];
		lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+tmp+"</b><br>";
		if(tmp.compare("drums")==0)
			{for(int j=0;j<num_drums;j++){lst+="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;+&nbsp;"+drums_components[j]+"<br>";}
			}
		else if(tmp.compare("misc_percussion")==0)
			{for(int j=0;j<num_misc_percussion;j++){lst+="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;+&nbsp;"+misc_percussion_components[j]+"<br>";}
			}
		else if(tmp.compare("simple_drums")==0)
			{for(int j=0;j<num_simple_drums;j++){lst+="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;+&nbsp;"+simple_drums_components[j]+"<br>";}}
		}
	lst+="<br>"; totalInstruments+=numPercussionInstruments+num_drums+num_misc_percussion+num_simple_drums-3;
	// PIANO
	lst+="<u>"+cnvrtNumToStrng(numPianoInstruments,0)+" <b>PIANO</b> Instruments</u><br>";
	for(int i=0;i<numPianoInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+pianoInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numPianoInstruments;
	// STRINGS
	lst+="<u>"+cnvrtNumToStrng(numStringInstruments,0)+" <b>STRINGS</b> Instruments</u><br>";
	for(int i=0;i<numStringInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+stringInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numStringInstruments;
	// VOICE
	lst+="<u>"+cnvrtNumToStrng(numVoiceInstruments,0)+" <b>VOICE</b> Instruments</u><br>";
	for(int i=0;i<numVoiceInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+voiceInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numVoiceInstruments;
	// WOODWINDS
	lst+="<u>"+cnvrtNumToStrng(numWoodwindInstruments,0)+" <b>WOODWINDS</b> Instruments</u><br>";
	for(int i=0;i<numWoodwindInstruments;i++){lst+="&nbsp;&nbsp;&nbsp;-&nbsp;<b>"+woodwindInstruments[i]+"</b><br>";}
	lst+="<br>"; totalInstruments+=numWoodwindInstruments;
	// Total Instruments
	lst+="<u><b>Total Instruments</b></u><br>";
	lst+=cnvrtNumToStrng(totalInstruments,0)+"<br>";

	QLabel *theInstrumentsList=new QLabel;
	theInstrumentsList->setText(lst.c_str());
	theInstrumentsList->setAlignment(Qt::AlignLeft);

	QGroupBox* eBox=new QGroupBox;
	//eBox->setFont(font);
	QGridLayout *elayout = new QGridLayout;
	elayout->addWidget(theInstrumentsList,0,0,1,1);
	eBox->setLayout(elayout);

	// Improved Executable Group Box
	QScrollArea* scrollArea=new QScrollArea();
	scrollArea->setWidget(eBox);  
	scrollArea->setWidgetResizable(true);  
	QGridLayout *exelayout=new QGridLayout;
	exelayout->addWidget(scrollArea);
	QGroupBox* exeBox=new QGroupBox;//(tr("|-PROGRAM FILE PATHS-|"));
	//exeBox->setFont(font);
	exeBox->setLayout(exelayout);exeBox->setAlignment(Qt::AlignLeft);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->addWidget(exeBox,0,0,3,2);
	setLayout(mainLayout);
	setWindowTitle(tr("Instruments List"));
	}

writeDemoAndBlankFiles::writeDemoAndBlankFiles(QWidget *parent) : QWidget(parent)
	{// Blanks
	QLabel* blanksLabel=new QLabel(tr("Write Blank Scores")); blanksLabel->setAlignment(Qt::AlignLeft);
	QPushButton *writeBlankDrumsScoreButton=new QPushButton(tr("Blank Drums")); writeBlankDrumsScoreButton->setFixedWidth(200);
	connect(writeBlankDrumsScoreButton,SIGNAL(clicked()),this,SLOT(write_blank_drums_score()));
	QPushButton *writeBlankMusicScoreButton=new QPushButton(tr("Blank Music")); writeBlankMusicScoreButton->setFixedWidth(200);
	connect(writeBlankMusicScoreButton,SIGNAL(clicked()),this,SLOT(write_blank_music_score()));
	QPushButton *writeBlankSimpleDrumsScoreButton=new QPushButton(tr("Blank Simple Drums")); writeBlankSimpleDrumsScoreButton->setFixedWidth(200);
	connect(writeBlankSimpleDrumsScoreButton,SIGNAL(clicked()),this,SLOT(write_blank_simple_drums_score()));
	QPushButton *writeBlankMiscPercussionScoreButton=new QPushButton(tr("Blank Misc Percussion")); writeBlankMiscPercussionScoreButton->setFixedWidth(200);
	connect(writeBlankMiscPercussionScoreButton,SIGNAL(clicked()),this,SLOT(write_blank_misc_percussion_score()));

	QGroupBox *topBox=new QGroupBox;
	QGridLayout *boxLayout=new QGridLayout;
	boxLayout->setSizeConstraint(QLayout::SetFixedSize);
	boxLayout->addWidget(blanksLabel,0,0,1,2);
	boxLayout->addWidget(writeBlankDrumsScoreButton,1,0,1,1); boxLayout->addWidget(writeBlankMusicScoreButton,1,1,1,1);
	boxLayout->addWidget(writeBlankSimpleDrumsScoreButton,2,0,1,1); boxLayout->addWidget(writeBlankMiscPercussionScoreButton,2,1,1,1);
	topBox->setLayout(boxLayout);

	// Demos
	QPushButton *writeDemoDrumsScoreButton=new QPushButton(tr("Demo Drums")); writeDemoDrumsScoreButton->setFixedWidth(200);
	connect(writeDemoDrumsScoreButton,SIGNAL(clicked()),this,SLOT(write_demo_drums_score()));
	QLabel* demosLabel=new QLabel(tr("Write Demo Scores")); demosLabel->setAlignment(Qt::AlignLeft);
	QPushButton *writeDemoSimpleDrumsScoreButton=new QPushButton(tr("Demo Simple Drums")); writeDemoSimpleDrumsScoreButton->setFixedWidth(200);
	connect(writeDemoSimpleDrumsScoreButton,SIGNAL(clicked()),this,SLOT(write_demo_simple_drums_score()));
	QPushButton *writeDemoMiscPercussionScoreButton=new QPushButton(tr("Demo Misc Percussion")); writeDemoMiscPercussionScoreButton->setFixedWidth(200);
	connect(writeDemoMiscPercussionScoreButton,SIGNAL(clicked()),this,SLOT(write_demo_misc_percussion_score()));

	QGroupBox *bottomBox=new QGroupBox;
	QGridLayout *bLayout=new QGridLayout;
	bLayout->addWidget(demosLabel,0,0,1,1);
	bLayout->addWidget(writeDemoDrumsScoreButton,1,0,1,1);
	bLayout->addWidget(writeDemoSimpleDrumsScoreButton,2,0,1,1); bLayout->addWidget(writeDemoMiscPercussionScoreButton,2,1,1,1);
	bottomBox->setLayout(bLayout);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->addWidget(topBox,0,0,3,2);
	mainLayout->addWidget(bottomBox,4,0,3,2);
	setLayout(mainLayout);
	setWindowTitle(tr("Blank/Empty Scores and Demos"));
	}

void writeDemoAndBlankFiles::write_blank_drums_score()
	{string the_executable=get_current_dir_name(); the_executable+="/bin/write_blank_drums_score.exe";
	string cmd=the_executable;
	int PID=fork(),status2,result;
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR in writeDemoAndBlankFiles::write_blank_drums_score()!\n"; exit(EXIT_FAILURE);}
		}
	else if(PID>0){//waitpid(PID,&status2,0);
				}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	return;}

void writeDemoAndBlankFiles::write_demo_drums_score()
	{string the_executable=get_current_dir_name(); the_executable+="/bin/write_demo_drums_score.exe";
	string cmd=the_executable;
	int PID=fork(),status2,result;
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR in writeDemoAndBlankFiles::write_demo_drums_score()!\n"; exit(EXIT_FAILURE);}
		}
	else if(PID>0){//waitpid(PID,&status2,0);
				}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	return;}

void writeDemoAndBlankFiles::write_blank_simple_drums_score()
	{string the_executable=get_current_dir_name(); the_executable+="/bin/write_blank_simple_drums_score.exe";
	string cmd=the_executable;
	int PID=fork(),status2,result;
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR in writeDemoAndBlankFiles::write_blank_simple_drums_score()!\n"; exit(EXIT_FAILURE);}
		}
	else if(PID>0){//waitpid(PID,&status2,0);
				}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	return;}

void writeDemoAndBlankFiles::write_demo_simple_drums_score()
	{string the_executable=get_current_dir_name(); the_executable+="/bin/write_demo_simple_drums_score.exe";
	string cmd=the_executable;
	int PID=fork(),status2,result;
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR in writeDemoAndBlankFiles::write_demo_simple_drums_score()!\n"; exit(EXIT_FAILURE);}
		}
	else if(PID>0){//waitpid(PID,&status2,0);
				}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	return;}

void writeDemoAndBlankFiles::write_blank_misc_percussion_score()
	{string the_executable=get_current_dir_name(); the_executable+="/bin/write_blank_misc_percussion_score.exe";
	string cmd=the_executable;
	int PID=fork(),status2,result;
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR in writeDemoAndBlankFiles::write_blank_misc_percussion_score()!\n"; exit(EXIT_FAILURE);}
		}
	else if(PID>0){//waitpid(PID,&status2,0);
				}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	return;}

void writeDemoAndBlankFiles::write_demo_misc_percussion_score()
	{string the_executable=get_current_dir_name(); the_executable+="/bin/write_demo_misc_percussion_score.exe";
	string cmd=the_executable;
	int PID=fork(),status2,result;
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR in writeDemoAndBlankFiles::write_blank_misc_percussion_score()!\n"; exit(EXIT_FAILURE);}
		}
	else if(PID>0){//waitpid(PID,&status2,0);
				}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	return;}

void writeDemoAndBlankFiles::write_blank_music_score()
	{string the_executable=get_current_dir_name(); the_executable+="/bin/write_blank_music_score.exe";
	string cmd=the_executable;
	int PID=fork(),status2,result;
	//string logFile=get_current_dir_name(); logFile+="/log_stderr";
	if(PID==0)
		{// Run in Child Process, next time fork is executed it gives child process PID
		result=system(cmd.c_str());
		if(result<0){cerr<<"ERROR in writeDemoAndBlankFiles::write_blank_music_score()!\n"; exit(EXIT_FAILURE);}
		}
	else if(PID>0){//waitpid(PID,&status2,0);
				}
	else if(PID==-1){cerr<<"Fork failed."<<endl;exit(EXIT_FAILURE);}
	else{cerr<<"Output of fork() unpredicted:\n"<<PID<<endl;}
	return;}

string cnvrtNumToStrng(int Num,int numberAfterDecimalpoint)
	{stringstream ss;
	ss.setf(ios::fixed);
	if(numberAfterDecimalpoint>0)
		{ss.setf(ios::showpoint);}
	ss.precision(numberAfterDecimalpoint);
	ss<<Num;
	return ss.str();}

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

bool copyFile(string srcFile,string destFile)
	{ifstream theSource(srcFile.c_str(),ifstream::in|ios::binary);
	ofstream theDestination(destFile.c_str(),ofstream::out|ofstream::trunc|ios::binary);
	theDestination<<theSource.rdbuf();
	return theSource && theDestination;}

bool directory_exist(string fPath)
	{struct stat sb;
	if(stat(fPath.c_str(),&sb)==0 && S_ISDIR(sb.st_mode)){return true;}
	else{return false;}}

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

string remove_excess_white_space(string inStr)
	{int startIndex,endIndex;
	string tmp;
	// Check from front
	for(int i=0;i<inStr.length();i++){tmp=inStr[i]; if(tmp.compare(" ")!=0){startIndex=i; break;}}
	// Check from back
	for(int i=inStr.length()-1;i>=0;i--){tmp=inStr[i]; if(tmp.compare(" ")!=0){endIndex=i+1; break;}}
	string Output=inStr.substr(startIndex,endIndex-startIndex);
	return Output;}

string repeat_string(string Input,int Num){string Output="";for(int i=0;i<Num;i++){Output+=Input;}return Output;}

