# include <string>

// Computational Constants
# define MAX_SOUNDS 100
# define LINE_INPUT_SIZE 100
# define NO_VALUE "----"

using namespace std;

# ifndef UI_H
# define UI_H

# include <QWidget>
//# include <QTextToSpeech>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class Button;
# include <QDialog>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QDial;
class QGroupBox;
class QLabel;
class QLineEdit;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QScrollBar;
class QSlider;
class QSpinBox;
class QTabWidget;
class QTableWidget;
class QTextEdit;
QT_END_NAMESPACE

// Supporting User Interface for Viewing Instrument List
class viewInstrumentsList : public QWidget
{
    Q_OBJECT
public:
	viewInstrumentsList(QWidget *parent = 0);
	
	// Instrument Categories
	static const int numCategories=12;
	string categories[numCategories]={"BASS","BRASS","DIGITAL","GUITARS","ORGANS","ORCHESTRA HITS","PERCUSSION","PIANO","STRINGS","VOICE","WOODWINDS","--MORE--"};
	// Bass
	static const int numBassInstruments=11;
	string bassInstruments[numBassInstruments]={"endorian_bass","jv_1080_bass","kick_arse_bass","laid_bass","metallic_bass","plucked_bass",\
										"roland_bass","roland_bass2","seinfeld_bass","solid_bass","stud_bass"};
	// Brass
	static const int numBrassInstruments=18;
	string brassInstruments[numBrassInstruments]={"berlin_trumpet","boston_brass","brass","brass_squirt","flugelhorns","flugelradio",\
						"mean_saxophone","monkey_thump","mourn_saxophone","orchestra_all_brass","orchestra_bass_trombone","orchestra_french_horn",\
						"orchestra_trombone","orchestra_trumpet","orchestra_tuba","soft_horn","trumpet","trumpets"};
	// Digital
	static const int numDigitalInstruments=3;
	string digitalInstruments[numDigitalInstruments]={"sawtooth","sinusoid","triangle_wave"};
	// Guitars
	static const int numGuitarInstruments=16;
	string guitarInstruments[numGuitarInstruments]={"aeon_flux_guitar","acoustic_guitar","echo_metal_guitar","fret_guitar","funky_guitar","guitar",\
						"lead_guitars","multi_guitar","NIN_guitar","rock_guitar","seagull_acoustic_guitar","session_guitar",\
						"sitar","stage_guitar","steel_guitar","strat_marshall"};
	// Orchestra Hits
	static const int numOrcHitInstruments=15;
	string orcHitInstruments[numOrcHitInstruments]={"accordion_hit","alien_hit","bjorkestra_hit","classic_hit","deport_hit","justified_hit",\
											"numb_hit","phantom_hit","quick_hit","robotech_hit","rock_hit","sleeptalk_hit",\
											"star_wars_hit","t_hit","twilight_zone_hit"};
	// Organ
	static const int numOrganInstruments=10;
	string organInstruments[numOrganInstruments]={"aggressive_organ","crazy_organ","crypt_organ","dirty_electric_organ","gothic_organ","magnificent_organ",\
											"neo_organ","phantom_organ","rock_organ","tired_accordion"};	
	// Piano
	static const int numPianoInstruments=12;
	string pianoInstruments[numPianoInstruments]={"barharp","crystal_rhodes_piano","german8_harpsichord","grand_piano","harpsichord","piano",\
						"piano_set","roland_piano","steinway_grand_piano","strings_with_piano","U20_piano","velocity_grand_piano"};
	// Strings
	static const int numStringInstruments=24;
	string stringInstruments[numStringInstruments]={"bounce_bow","deep_string","dirty_strings","ensemble_violin","fantastic_strings","harp","orchestra_1st_violin",\
										"orchestra_2nd_violin","orchestra_all_strings","orchestra_bass","orchestra_cello","orchestra_viola","pluck",\
										"pluck2","plucked_string","roland_dulcimer","roland_strings","roland_violin","straight_strings",\
										"strings","strings2","strings_with_piano","violin","violin_pizzicato"};		
	// Voice
	static const int numVoiceInstruments=63;
	string voiceInstruments[numVoiceInstruments]={"boy_choir","worship_choir","Afrikaans_Man_1","Afrikaans_Man_2","Brazil_Man_1","Brazil_Woman_1","Brazil_Man_2","Brazil_Woman_2",\
									"British_Man","Croatian_Man","Czech_Man","Dutch_Man_1","Dutch_Man_2","English_Woman","English_Man_1","English_Man_2",\
									"Estonian_Man","French_Woman_1","French_Man_1","French_Man_2","French_Woman_2","German_Woman_1","German_Man_1","German_Woman_2",\
									"German_Man_2","German_Woman_3","German_Man_3","German_Woman_4","German_Man_4","German_Woman_5","German_Man_5","German_Woman_6",\
									"German_Man_6","German_Woman_7","Greek_Man_1","Greek_Man_2","Hungarian_Woman_1","Hungarian_Woman_2","Icelandic_Man",\
									"Indonesian_Man","Italian_Man","Italian_Woman","Latin_Man","Lithuanian_Man_1","Lithuanian_Man_2","Mexican_Man_1","Mexican_Man_2",\
									"Persian_Man","Persian_Woman","Polish_Woman_1","Polish_Woman_2","Portugal_Woman","Romanian_Man_1","Romanian_Man_2","Spanish_Man_1",\
									"Spanish_Man_2","Swedish_Man_1","Swedish_Woman_1","Swedish_Man_2","Swedish_Woman_2","Turkish_Man","Turkish_Woman","Venezuala_Man"};
	// Woodwinds
	static const int numWoodwindInstruments=16;
	string woodwindInstruments[numWoodwindInstruments]={"bottle_blow","clarinet","didgeridoo","flute","orchestra_all_woodwinds","orchestra_alto_flute",\
											"orchestra_bass_clarinet","orchestra_bassoon","orchestra_clarinet","orchestra_contrabassoon",\
											"orchestra_english_horn","orchestra_flute","orchestra_oboe","orchestra_piccolo","pagan_whistle","whistle"};
	// Percussion
	static const int numPercussionInstruments=9;
	string percussionInstruments[numPercussionInstruments]={"drums","glockenspiel","miramba","misc_percussion","simple_drums","timpani","tubular_bells","vibraphone","xylophone"};

	// Percussion: drums Components
	static const int num_drums=23;
	string drums_components[num_drums]={"CRASH_CYMBAL_1","CRASH_CYMBAL_2","REVERSE_CYMBAL","RIDE_CYMBAL","808_CYMBAL","OPEN_HIHAT_1","OPEN_HIHAT_2",\
								"808_OPEN_HIHAT","CLOSED_HIHAT","808_CLOSED_HIHAT","TAMBOURINE","BASS_DRUM","808_BASS_DRUM","HAND_CLAP",\
								"808_COWBELL","909_SNARE_1","909_SNARE_2","909_SNARE_3","909_SNARE_4","HIGH_TOM","MID_TOM","LOW_TOM","RIM_SHOT"};

	// Percussion: misc_percussion Components
	static const int num_misc_percussion=41;
	string misc_percussion_components[num_misc_percussion]={"ROLL_HI_TRIANGLE","MUTED_HI_TRIANGLE","OPEN_HI_TRIANGLE","GONG","TAMTAM_A","TAMTAM_B","TAMTAM_C",\
												"HI_FINGER_CYMBALS","LO_FINGER_CYMBALS","SHAKE_TAMBOURINE","HIT_TAMBOURINE","ROLL_TAMBOURINE","COWBELL",\
												"VIBRA_SLAP","HIT_HIGH_CONGA","TAP_HIGH_CONGA","HIT_MID_CONGA","TAP_MID_CONGA","HIT_LOW_CONGA","TAP_LOW_CONGA",\
												"CABASA","SHAKER","CRANK_RATCHET","FAST_RATCHET","SLOW_RATCHET","CASTANETS","CLAVES","HI_WOOD_BLOCK","LO_WOOD_BLOCK",\
												"ROLL_LO_TRIANGLE","MUTED_LO_TRIANGLE","OPEN_LO_TRIANGLE","SLEIGH_BELLS","BELL_TREE_1","BELL_TREE_2","BELL_TREE_3",\
												"BELL_TREE_4","BELL_TREE_5","BAR_CHIMES_A","BAR_CHIMES_B","BAR_CHIMES_C"};
	
	// Percussion: simple_drums Components
	static const int num_simple_drums=20;
	string simple_drums_components[num_simple_drums]={"BASS_DRUM_1","BASS_DRUM_2","LEFT_HIT_SNARE_1","ROUND_HIT_SNARE_1","RIGHT_HIT_SNARE_1","ROLL_SNARE_1","LEFT_HIT_SNARE_2",\
											"ROUND_HIT_SNARE_2","RIGHT_HIT_SNARE_2","TAP_SNARE_2","ROLL_SNARE_2","ROLL_A_CYMBALS_1","LO_CRASH_CYMBALS_1",\
											"ROLL_B_CYMBALS_1","HI_CRASH_CYMBALS_1","SHORT_ROLL_CYMBALS_2","LO_CRASH_CYMBALS_2","MID_ROLL_CYMBALS_2",\
											"LONG_ROLL_CYMBALS_2","HI_CRASH_CYMBALS_2"};
};

// Supporting User Interface for Writing Demo and Blank Sound Score Files
class writeDemoAndBlankFiles : public QWidget
{
    Q_OBJECT
public:
	writeDemoAndBlankFiles(QWidget *parent = 0);	
	//
	//void defineParameters(string iFile);
public slots:
	void write_blank_drums_score();
	void write_demo_drums_score();
	void write_blank_music_score();
	void write_blank_simple_drums_score();
	void write_demo_simple_drums_score();
	void write_blank_misc_percussion_score();
	void write_demo_misc_percussion_score();

};

// Main User Interface
class UI : public QWidget
{
    Q_OBJECT

public:
	UI(QWidget *parent = 0);
	// Sounds Tab
	QTabWidget* soundTab;
	// Add a Sound Button
	QPushButton* addSoundButton;
	// Remove a Sound Button
	QPushButton* remSoundButton;
	// Important Labels/Text Input
	QLineEdit *songName;
	QLabel *songNameLabel;
	QLineEdit *tempo;
	
	QComboBox *categoryBox[MAX_SOUNDS];
	QComboBox *filterTypeBox[MAX_SOUNDS];
	QComboBox *filterBox[MAX_SOUNDS];
	QComboBox *instrumentBox[MAX_SOUNDS];
	QLabel *instrLabel[MAX_SOUNDS];
	QComboBox *songHistoryBox;
	//QPushButton *openSoundFile[MAX_SOUNDS];
	string *soundFiles=new string[MAX_SOUNDS];
	QLineEdit *amplitude[MAX_SOUNDS];
	QLabel *filterInput1Label[MAX_SOUNDS];
	QLineEdit *filterInput1[MAX_SOUNDS];
	QLabel *filterInput2Label[MAX_SOUNDS];
	QLineEdit *filterInput2[MAX_SOUNDS];
	QLabel *soundFileLabel[MAX_SOUNDS];
	QCheckBox *denoiseBox;
	QCheckBox *playbackBox;
	QCheckBox *mp3Box;
	QCheckBox *saveHistoryBox;
	QCheckBox *sonicVisualizerBox;
	// History File of Songs
	string songHistoryFile;
	// Button for Recording from Mic
	QPushButton *recordAudioButton;

	// Filter Types
	static const int numFilterTypes=4;
	string filterTypes[numFilterTypes]={"HIGH-PASS","LOW-PASS","BAND-PASS","NONE"};
	// High Pass Filters
	static const int numHighPassFilters=9;
	string highPassFilters[numHighPassFilters]={"Butterworth",				/* out clfilt in, freq, 1, numPoles, 0 */
										"Butterworth (2nd order)", 	/* out butterhp in, freq */
										"Chebyshev I",				/* out clfilt in, freq, 1, numPoles, 1 */
										"Chebyshev II",			/* out clfilt in, freq, 1, numPoles, 2 */
										"Elliptical",				/* out clfilt in, freq, 1, numPoles, 3 */
										"Recursive (1st order)",		/* out atone in, freq */
										"Resonant (2nd order)",		/* out bqrez in, freq, resonance, 1 */
										"State Variable",			/* hpOut,lpOut,bpOut,brOut statevar in, freq, Q */
										"State Variable (2nd order)"	/* lpOut,hpOut,bpOut svfilter in, freq, Q */
										};
	// Low Pass Filters
	static const int numLowPassFilters=11;
	string lowPassFilters[numLowPassFilters]={"Butterworth",				/* out clfilt in, freq, 0, numPoles, 0 */
										"Butterworth (2nd order)", 	/* out butterlp in, freq */
										"Chebyshev I",				/* out clfilt in, freq, 0, numPoles, 1 */
										"Chebyshev II",			/* out clfilt in, freq, 0, numPoles, 2 */
										"Elliptical",				/* out clfilt in, freq, 0, numPoles, 3 */
										"Recursive (1st order)",		/* out tone in, freq */
										"Resonant",				/* out lowres in, freq, resonance */
										"Resonant II",				/* out rezzy in, freq, resonance */
										"Resonant (2nd order)",		/* out bqrez in, freq, resonance, 0 */
										"State Variable",			/* hpOut,lpOut,bpOut,brOut statevar in, freq, Q */
										"State Variable (2nd order)"	/* lpOut,hpOut,bpOut svfilter in, freq, Q */
										};
	// Band Pass Filters
	static const int numBandPassFilters=2;
	string bandPassFilters[numBandPassFilters]={"Butterworth",				/* out butterbp in, freq, bandWidth */
										"Resonant"				/* out reson in, freq, bandWidth */
										};	
	

	// Instrument Categories
	static const int numCategories=12;
	string categories[numCategories]={"BASS","BRASS","DIGITAL","GUITARS","ORGANS","ORCHESTRA HITS","PERCUSSION","PIANO","STRINGS","VOICE","WOODWINDS","--MORE--"};
	// Bass
	static const int numBassInstruments=11;
	string bassInstruments[numBassInstruments]={"endorian_bass","jv_1080_bass","kick_arse_bass","laid_bass","metallic_bass","plucked_bass",\
										"roland_bass","roland_bass2","seinfeld_bass","solid_bass","stud_bass"};
	// Brass
	static const int numBrassInstruments=18;
	string brassInstruments[numBrassInstruments]={"berlin_trumpet","boston_brass","brass","brass_squirt","flugelhorns","flugelradio",\
						"mean_saxophone","monkey_thump","mourn_saxophone","orchestra_all_brass","orchestra_bass_trombone","orchestra_french_horn",\
						"orchestra_trombone","orchestra_trumpet","orchestra_tuba","soft_horn","trumpet","trumpets"};
	// Digital
	static const int numDigitalInstruments=3;
	string digitalInstruments[numDigitalInstruments]={"sawtooth","sinusoid","triangle_wave"};
	// Guitars
	static const int numGuitarInstruments=16;
	string guitarInstruments[numGuitarInstruments]={"aeon_flux_guitar","acoustic_guitar","echo_metal_guitar","fret_guitar","funky_guitar","guitar",\
						"lead_guitars","multi_guitar","NIN_guitar","rock_guitar","seagull_acoustic_guitar","session_guitar",\
						"sitar","stage_guitar","steel_guitar","strat_marshall"};
	// Orchestra Hits
	static const int numOrcHitInstruments=15;
	string orcHitInstruments[numOrcHitInstruments]={"accordion_hit","alien_hit","bjorkestra_hit","classic_hit","deport_hit","justified_hit",\
											"numb_hit","phantom_hit","quick_hit","robotech_hit","rock_hit","sleeptalk_hit",\
											"star_wars_hit","t_hit","twilight_zone_hit"};
	// Organ
	static const int numOrganInstruments=10;
	string organInstruments[numOrganInstruments]={"aggressive_organ","crazy_organ","crypt_organ","dirty_electric_organ","gothic_organ","magnificent_organ",\
											"neo_organ","phantom_organ","rock_organ","tired_accordion"};	
	// Piano
	static const int numPianoInstruments=12;
	string pianoInstruments[numPianoInstruments]={"barharp","crystal_rhodes_piano","german8_harpsichord","grand_piano","harpsichord","piano",\
						"piano_set","roland_piano","steinway_grand_piano","strings_with_piano","U20_piano","velocity_grand_piano"};
	// Strings
	static const int numStringInstruments=24;
	string stringInstruments[numStringInstruments]={"bounce_bow","deep_string","dirty_strings","ensemble_violin","fantastic_strings","harp","orchestra_1st_violin",\
										"orchestra_2nd_violin","orchestra_all_strings","orchestra_bass","orchestra_cello","orchestra_viola","pluck",\
										"pluck2","plucked_string","roland_dulcimer","roland_strings","roland_violin","straight_strings",\
										"strings","strings2","strings_with_piano","violin","violin_pizzicato"};		
	// Voice
	static const int numVoiceInstruments=63;
	string voiceInstruments[numVoiceInstruments]={"boy_choir","worship_choir","Afrikaans_Man_1","Afrikaans_Man_2","Brazil_Man_1","Brazil_Woman_1","Brazil_Man_2","Brazil_Woman_2",\
									"British_Man","Croatian_Man","Czech_Man","Dutch_Man_1","Dutch_Man_2","English_Woman","English_Man_1","English_Man_2",\
									"Estonian_Man","French_Woman_1","French_Man_1","French_Man_2","French_Woman_2","German_Woman_1","German_Man_1","German_Woman_2",\
									"German_Man_2","German_Woman_3","German_Man_3","German_Woman_4","German_Man_4","German_Woman_5","German_Man_5","German_Woman_6",\
									"German_Man_6","German_Woman_7","Greek_Man_1","Greek_Man_2","Hungarian_Woman_1","Hungarian_Woman_2","Icelandic_Man",\
									"Indonesian_Man","Italian_Man","Italian_Woman","Latin_Man","Lithuanian_Man_1","Lithuanian_Man_2","Mexican_Man_1","Mexican_Man_2",\
									"Persian_Man","Persian_Woman","Polish_Woman_1","Polish_Woman_2","Portugal_Woman","Romanian_Man_1","Romanian_Man_2","Spanish_Man_1",\
									"Spanish_Man_2","Swedish_Man_1","Swedish_Woman_1","Swedish_Man_2","Swedish_Woman_2","Turkish_Man","Turkish_Woman","Venezuala_Man"};
	// Woodwinds
	static const int numWoodwindInstruments=16;
	string woodwindInstruments[numWoodwindInstruments]={"bottle_blow","clarinet","didgeridoo","flute","orchestra_all_woodwinds","orchestra_alto_flute",\
											"orchestra_bass_clarinet","orchestra_bassoon","orchestra_clarinet","orchestra_contrabassoon",\
											"orchestra_english_horn","orchestra_flute","orchestra_oboe","orchestra_piccolo","pagan_whistle","whistle"};
	// Percussion
	static const int numPercussionInstruments=9;
	string percussionInstruments[numPercussionInstruments]={"drums","glockenspiel","miramba","misc_percussion","simple_drums","timpani","tubular_bells","vibraphone","xylophone"};

	// Percussion: drums Components
	static const int num_drums=23;
	string drums_components[num_drums]={"CRASH_CYMBAL_1","CRASH_CYMBAL_2","REVERSE_CYMBAL","RIDE_CYMBAL","808_CYMBAL","OPEN_HIHAT_1","OPEN_HIHAT_2",\
								"808_OPEN_HIHAT","CLOSED_HIHAT","808_CLOSED_HIHAT","TAMBOURINE","BASS_DRUM","808_BASS_DRUM","HAND_CLAP",\
								"808_COWBELL","909_SNARE_1","909_SNARE_2","909_SNARE_3","909_SNARE_4","HIGH_TOM","MID_TOM","LOW_TOM","RIM_SHOT"};

	// Percussion: misc_percussion Components
	static const int num_misc_percussion=41;
	string misc_percussion_components[num_misc_percussion]={"ROLL_HI_TRIANGLE","MUTED_HI_TRIANGLE","OPEN_HI_TRIANGLE","GONG","TAMTAM_A","TAMTAM_B","TAMTAM_C",\
												"HI_FINGER_CYMBALS","LO_FINGER_CYMBALS","SHAKE_TAMBOURINE","HIT_TAMBOURINE","ROLL_TAMBOURINE","COWBELL",\
												"VIBRA_SLAP","HIT_HIGH_CONGA","TAP_HIGH_CONGA","HIT_MID_CONGA","TAP_MID_CONGA","HIT_LOW_CONGA","TAP_LOW_CONGA",\
												"CABASA","SHAKER","CRANK_RATCHET","FAST_RATCHET","SLOW_RATCHET","CASTANETS","CLAVES","HI_WOOD_BLOCK","LO_WOOD_BLOCK",\
												"ROLL_LO_TRIANGLE","MUTED_LO_TRIANGLE","OPEN_LO_TRIANGLE","SLEIGH_BELLS","BELL_TREE_1","BELL_TREE_2","BELL_TREE_3",\
												"BELL_TREE_4","BELL_TREE_5","BAR_CHIMES_A","BAR_CHIMES_B","BAR_CHIMES_C"};
	
	// Percussion: simple_drums Components
	static const int num_simple_drums=20;
	string simple_drums_components[num_simple_drums]={"BASS_DRUM_1","BASS_DRUM_2","LEFT_HIT_SNARE_1","ROUND_HIT_SNARE_1","RIGHT_HIT_SNARE_1","ROLL_SNARE_1","LEFT_HIT_SNARE_2",\
											"ROUND_HIT_SNARE_2","RIGHT_HIT_SNARE_2","TAP_SNARE_2","ROLL_SNARE_2","ROLL_A_CYMBALS_1","LO_CRASH_CYMBALS_1",\
											"ROLL_B_CYMBALS_1","HI_CRASH_CYMBALS_1","SHORT_ROLL_CYMBALS_2","LO_CRASH_CYMBALS_2","MID_ROLL_CYMBALS_2",\
											"LONG_ROLL_CYMBALS_2","HI_CRASH_CYMBALS_2"};

	// The Number of Sounds Current in the interface
	int numSound=1;

	// SOUND Folder Containing User Written Sheet Music
	string soundFldr;

public slots:
	string generateFilterCommand(int tabIndex);
	string get_instrument_category(string theInstrument);
	int get_instrument_index(string theInstrument,string theCategory);
	void launchviewInstrumentsList();
	void launchwriteDemoAndBlankFiles();
	void loadFilterComboBox(int index);
	void loadFilterInput(int index);
	void loadInstrumentComboBox(int index);
	void loadInstrumentComboBox(int catIndex,int tabIndex);
	void loadInstrumentLabel(int index);
	void loadSong();
	void addSoundTab();
	void removeSoundTab();
	void loadHistory();
	void open_song_file();
	void open_sound_file();
	void readFilterCommand(string filterCommand,int tabIndex);
	void read_song_input_file(string inFile);
	void record_audio();
	void updateHistoryFile(string songNm);
	void write_song_input_file();
private:
	writeDemoAndBlankFiles *DBF;
	viewInstrumentsList *VIL;
};

string cnvrtNumToStrng(int Num,int numberAfterDecimalpoint);
int count_delimiter(string Data,string delimiter);
bool copyFile(string srcFile,string destFile);
bool directory_exist(string fPath);
string extract_file_name(string fPath);
string* fill_string_array(string Data,int numPnts,string delimiter);
string formatNumberString(string Num);
string remove_excess_white_space(string inStr);
string repeat_string(string Input,int Num);

# endif

//⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹ ⁺ ⁻ ⁼ ⁽ ⁾
//ᵃ ᵇ ᶜ ᵈ ᵉ ᶠ ᵍ ʰ ⁱ ʲ ᵏ ˡ ᵐ ⁿ ᵒ ᵖ ʳ ˢ ᵗ ᵘ ᵛ ʷ ˣ ʸ ᶻ 
//ᴬ ᴮ ᴰ ᴱ ᴳ ᴴ ᴵ ᴶ ᴷ ᴸ ᴹ ᴺ ᴼ ᴾ ᴿ ᵀ ᵁ ⱽ ᵂ 
//ₐ ₑ ₕ ᵢ ⱼ ₖ ₗ ₘ ₙ ₒ ₚ ᵣ ₛ ₜ ᵤ ᵥ ₓ
//ᵅ ᵝ ᵞ ᵟ ᵋ ᶿ ᶥ ᶲ ᵠ ᵡ 
//ᵦ ᵧ ᵨ ᵩ ᵪ  
//1₀ ₁ ₂ ₃ ₄ ₅ ₆ ₇ ₈ ₉ ₊ ₋ ₌ ₍ ₎
