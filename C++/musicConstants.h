# include <string>

using namespace std;

# ifndef MusicConstants_H
# define MusicConstants_H

	# define SAMPLE_RATE 44100
	# define FRAMES_PER_BUFFER 512
	# define NUM_CHANNELS 2
	// Number of Decimal Places are Significant
	# define SIG_FIGS 8
	// Maximum Number of Characters for Input Per Line
	# define LINE_SIZE 1500000
	// Global Delimiters
	# define GLOBAL_DELIMITER "?"
	# define GLOBAL_DELIMITER2 "`"
	// Root Frequencies
	// C0 (lowest note) - B0
	# define C0_FREQ 16.35160	// B# Sharp / C Natural
	# define C0s_FREQ 17.32391	// C# Sharp / Db Flat
	# define D0_FREQ 18.35405	// D Natural
	# define D0s_FREQ 19.44544	// D# Sharp / Eb Flat
	# define E0_FREQ 20.60172	// E Natural / Fb Flat
	# define F0_FREQ 21.82676	// F Natural / E# Sharp
	# define F0s_FREQ 23.12465	// F# Sharp / Gb Flat
	# define G0_FREQ 24.49971	// G Natural
	# define G0s_FREQ 25.95654	// G# Sharp / Ab Flat
	# define A0_FREQ 27.50000	// A Natural
	# define A0s_FREQ 29.13524	// A# Sharp / Bb Flat
	# define B0_FREQ 30.86771	// B Natural / C Flat
	// C1 - B1
	# define C1_FREQ 32.70320	// B# Sharp / C Natural
	# define C1s_FREQ 34.64783	// C# Sharp / Db Flat
	# define D1_FREQ 36.70810	// D Natural
	# define D1s_FREQ 38.89087	// D# Sharp / Eb Flat
	# define E1_FREQ 41.20344	// E Natural / Fb Flat
	# define F1_FREQ 43.65353	// F Natural / E# Sharp
	# define F1s_FREQ 46.24930	// F# Sharp / Gb Flat
	# define G1_FREQ 48.99943	// G Natural
	# define G1s_FREQ 51.91309	// G# Sharp / Ab Flat
	# define A1_FREQ 55.00000	// A Natural
	# define A1s_FREQ 58.27047	// A# Sharp / Bb Flat
	# define B1_FREQ 61.73541	// B Natural / C Flat
	// C2 - B2
	# define C2_FREQ 65.40639	// B# Sharp / C Natural
	# define C2s_FREQ 69.29566	// C# Sharp / Db Flat
	# define D2_FREQ 73.41619	// D Natural
	# define D2s_FREQ 77.78175	// D# Sharp / Eb Flat
	# define E2_FREQ 82.40689	// E Natural / Fb Flat
	# define F2_FREQ 87.30706	// F Natural / E# Sharp
	# define F2s_FREQ 92.49861	// F# Sharp / Gb Flat
	# define G2_FREQ 97.99886	// G Natural
	# define G2s_FREQ 103.8262	// G# Sharp / Ab Flat
	# define A2_FREQ 110.0000	// A Natural
	# define A2s_FREQ 116.5409	// A# Sharp / Bb Flat
	# define B2_FREQ 123.4708	// B Natural / C Flat
	// C3 - B3
	# define C3_FREQ 130.8128	// B# Sharp / C Natural
	# define C3s_FREQ 138.5913	// C# Sharp / Db Flat
	# define D3_FREQ 146.8324	// D Natural
	# define D3s_FREQ 155.5635	// D# Sharp / Eb Flat
	# define E3_FREQ 164.8138	// E Natural / Fb Flat
	# define F3_FREQ 174.6141	// F Natural / E# Sharp
	# define F3s_FREQ 184.9972	// F# Sharp / Gb Flat
	# define G3_FREQ 195.9977	// G Natural
	# define G3s_FREQ 207.6523	// G# Sharp / Ab Flat
	# define A3_FREQ 220.0000	// A Natural
	# define A3s_FREQ 233.0819	// A# Sharp / Bb Flat
	# define B3_FREQ 246.9417	// B Natural / C Flat
	// C4 - B4
	# define C4_FREQ 261.6256	// B# Sharp / C Natural
	# define C4s_FREQ 277.1826	// C# Sharp / Db Flat
	# define D4_FREQ 293.6648	// D Natural
	# define D4s_FREQ 311.1270	// D# Sharp / Eb Flat
	# define E4_FREQ 329.6276	// E Natural / Fb Flat
	# define F4_FREQ 349.2282	// F Natural / E# Sharp
	# define F4s_FREQ 369.9944	// F# Sharp / Gb Flat
	# define G4_FREQ 391.9954	// G Natural
	# define G4s_FREQ 415.3047	// G# Sharp / Ab Flat
	# define A4_FREQ 440.0000	// A Natural
	# define A4s_FREQ 466.1638	// A# Sharp / Bb Flat
	# define B4_FREQ 493.8833	// B Natural / C Flat
	// C5 - B5
	# define C5_FREQ 523.2511	// B# Sharp / C Natural
	# define C5s_FREQ 554.3653	// C# Sharp / Db Flat
	# define D5_FREQ 587.3295	// D Natural
	# define D5s_FREQ 622.2540	// D# Sharp / Eb Flat
	# define E5_FREQ 659.2551	// E Natural / Fb Flat
	# define F5_FREQ 698.4565	// F Natural / E# Sharp
	# define F5s_FREQ 739.9888	// F# Sharp / Gb Flat
	# define G5_FREQ 783.9909	// G Natural
	# define G5s_FREQ 830.6094	// G# Sharp / Ab Flat
	# define A5_FREQ 880.0000	// A Natural
	# define A5s_FREQ 932.3275	// A# Sharp / Bb Flat
	# define B5_FREQ 987.7666	// B Natural / C Flat
	// C6 - B6
	# define C6_FREQ 1046.502	// B# Sharp / C Natural
	# define C6s_FREQ 1108.731	// C# Sharp / Db Flat
	# define D6_FREQ 1174.659	// D Natural
	# define D6s_FREQ 1244.508	// D# Sharp / Eb Flat
	# define E6_FREQ 1318.510	// E Natural / Fb Flat
	# define F6_FREQ 1396.913	// F Natural / E# Sharp
	# define F6s_FREQ 1479.978	// F# Sharp / Gb Flat
	# define G6_FREQ 1567.982	// G Natural
	# define G6s_FREQ 1661.219	// G# Sharp / Ab Flat
	# define A6_FREQ 1760.000	// A Natural
	# define A6s_FREQ 1864.655	// A# Sharp / Bb Flat
	# define B6_FREQ 1975.533	// B Natural / C Flat
	// C7 - B7
	# define C7_FREQ 2093.005	// B# Sharp / C Natural
	# define C7s_FREQ 2217.461	// C# Sharp / Db Flat
	# define D7_FREQ 2349.318	// D Natural
	# define D7s_FREQ 2489.016	// D# Sharp / Eb Flat
	# define E7_FREQ 2637.020	// E Natural / Fb Flat
	# define F7_FREQ 2793.826	// F Natural / E# Sharp
	# define F7s_FREQ 2959.955	// F# Sharp / Gb Flat
	# define G7_FREQ 3135.963	// G Natural
	# define G7s_FREQ 3322.438	// G# Sharp / Ab Flat
	# define A7_FREQ 3520.000	// A Natural
	# define A7s_FREQ 3729.310	// A# Sharp / Bb Flat
	# define B7_FREQ 3951.066	// B Natural / C Flat
	// C8 - B8
	# define C8_FREQ 4186.009	// B# Sharp / C Natural
	# define C8s_FREQ 4434.922 	// C# Sharp / Db Flat
	# define D8_FREQ 4698.636	// D Natural
	# define D8s_FREQ 4978.032	// D# Sharp / Eb Flat
	# define E8_FREQ 5274.041	// E Natural / Fb Flat
	# define F8_FREQ 5587.652	// F Natural / E# Sharp
	# define F8s_FREQ 5919.911	// F# Sharp / Gb Flat
	# define G8_FREQ 6271.927	// G Natural
	# define G8s_FREQ 6644.875	// G# Sharp / Ab Flat
	# define A8_FREQ 7040.000	// A Natural
	# define A8s_FREQ 7458.620	// A# Sharp / Bb Flat
	# define B8_FREQ 7902.133	// B Natural / C Flat
	//  Keyboard Frequency Layout
	//     ||||      ||||  ||||      |||| |||| ||||     |||| ||||      |||| |||| ||||                  |||| ||||      |||| |||| ||||       
	//    |A#0|     |C#1| |D#1|     |F#1||G#1||A#1|    |C#2||D#2|     |F#2||G#2||A#2|                 |C#7||D#7|     |F#7||G#7||A#7|       
	// | A0 | B0 | C1 | D1 | E1 | F1 | G1 | A1 | B1 | C2 | D2 | E2 | F2 | G2 | A2 | B2 | C3 | ... | C7 | D7 | E7 | F7 | G7 | A7 | B7 | C8 |
	// ----- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----       ---- ---- ---- ---- ---- ---- ---- ----
	//
	# define ACCEPTED_ERROR_PERCENT 1
	// Display Comments (0 is on, 1 is off)
	# define VERBOSE true
	// Important Tools/Executables
	# define CSOUND_EXE "/usr/bin/csound"
	# define ESPEAK_EXE "/usr/bin/espeak"
	// eSpeak Voice Names/Types
	static const int numVoiceSpeakers=62;
	const string espeakVoiceSpeakers[numVoiceSpeakers]={"Afrikaans_Man_1","Afrikaans_Man_2","Brazil_Man_1","Brazil_Woman_1","Brazil_Man_2","Brazil_Woman_2",\
									"British_Man","Croatian_Man","Czech_Man","Dutch_Man_1","Dutch_Man_2","English_Woman","English_Man_1","English_Man_2",\
									"Estonian_Man","French_Woman_1","French_Man_1","French_Man_2","French_Woman_2","German_Woman_1","German_Man_1","German_Woman_2",\
									"German_Man_2","German_Woman_3","German_Man_3","German_Woman_4","German_Man_4","German_Woman_5","German_Man_5","German_Woman_6",\
									"German_Man_6","German_Woman_7","Greek_Man_1","Greek_Man_2","Hungarian_Woman_1","Hungarian_Woman_2","Icelandic_Man",\
									"Indonesian_Man","Italian_Man","Italian_Woman","Latin_Man","Lithuanian_Man_1","Lithuanian_Man_2","Mexican_Man_1","Mexican_Man_2",\
									"Persian_Man","Persian_Woman","Polish_Woman_1","Polish_Woman_2","Portugal_Woman","Romanian_Man_1","Romanian_Man_2","Spanish_Man_1",\
									"Spanish_Man_2","Swedish_Man_1","Swedish_Woman_1","Swedish_Man_2","Swedish_Woman_2","Turkish_Man","Turkish_Woman","Venezuala_Man"};

	//
	// 84 Percussion Instruments + 266 Instruments + 62 voices = 412 options
	//
	// Sound Fonts (266) File Constants (located in CSOUND folder)
	# define ACCORDION_HIT_SOUNDFONT "SoundFonts/Acchit.sf2"
	# define ACOUSTIC_GUITAR_SOUNDFONT "SoundFonts/07AcousticGuitar.sf2"
	# define AEON_FLUX_GUITAR_SOUNDFONT "SoundFonts/Fluxguit.sf2"
	# define AGGRESSIVE_ORGAN_SOUNDFONT "SoundFonts/Aggorg.sf2"
	# define ALIEN_HIT_SOUNDFONT "SoundFonts/Alienhit.sf2"
	# define BARHARP_SOUNDFONT "SoundFonts/Barharp.sf2"
	# define BERLIN_TRUMPET_SOUNDFONT "SoundFonts/Berltrmp.sf2"
	# define BJORKESTRA_HIT_SOUNDFONT "SoundFonts/Bjorkhit.sf2"
	# define BOSTON_BRASS_SOUNDFONT "SoundFonts/Boston_Brass.sf2"
	# define BOTTLE_BLOW_SOUNDFONT "SoundFonts/Bottleblow.sf2"
	# define BOUNCE_BOW_SOUNDFONT "SoundFonts/Bounce_Bow.sf2"
	# define BOY_CHOIR_SOUNDFONT "SoundFonts/Boychoir.sf2"
	# define BRASS_SOUNDFONT "SoundFonts/R-brass.sf2"
	# define BRASS_SQUIRT_SOUNDFONT "SoundFonts/Brassqrt.sf2"
	# define CLARINET_SOUNDFONT "SoundFonts/Klezclar.sf2"
	# define CLASSIC_HIT_SOUNDFONT "SoundFonts/Classhit.sf2"
	# define CRAZY_ORGAN_SOUNDFONT "SoundFonts/Crazyorg.sf2"
	# define CRYPT_ORGAN_SOUNDFONT "SoundFonts/Cryptorg.sf2"
	# define CRYSTAL_RHODES_PIANO_SOUNDFONT "SoundFonts/Crysrhod.sf2"
	# define DEEP_STRING_SOUNDFONT "SoundFonts/Deep_String.sf2"
	# define DEPORT_HIT_SOUNDFONT "SoundFonts/deport_hit.sf2"
	# define DIDGERIDOO_SOUNDFONT "SoundFonts/Ddoo.sf2"
	# define DIRTY_ELECTRIC_ORGAN_SOUNDFONT "SoundFonts/Dirtelec.sf2"
	# define DIRTY_STRINGS_SOUNDFONT "SoundFonts/Dirtstrn.sf2"
	# define ECHO_METAL_GUITAR_SOUNDFONT "SoundFonts/Echogtr.sf2"
	# define ENDORIAN_BASS_SOUNDFONT "SoundFonts/Endobass.sf2"
	# define ENSEMBLE_VIOLIN_SOUNDFONT "SoundFonts/ensemble_violin.sf2"
	# define FANTASTIC_STRINGS_SOUNDFONT "SoundFonts/Fantstrn.sf2"
	# define FLUGELHORNS_SOUNDFONT "SoundFonts/flugelhorns.sf2"
	# define FLUGELRADIO_SOUNDFONT "SoundFonts/flugelradio.sf2"
	# define FLUTE_SOUNDFONT "SoundFonts/Kelflute.sf2"
	# define FRET_GUITAR_SOUNDFONT "SoundFonts/Fretguit.sf2"
	# define FUNKY_GUITAR_SOUNDFONT "SoundFonts/FUNKFRET.sf2"
	# define GERMAN8_HARPSICHORD_SOUNDFONT "SoundFonts/German8_Harpsichord.sf2"
	# define GLOCKENSPIEL_SOUNDFONT "SoundFonts/Virtual_Orchestra/Percussion/glockenspiel.sf2"
	# define GOTHIC_ORGAN_SOUNDFONT "SoundFonts/Gothorgn.sf2"
	# define GRAND_PIANO_SOUNDFONT "SoundFonts/Grandpno.sf2"
	# define HARP_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/harp-sustain.sf2"
	# define HARPSICHORD_SOUNDFONT "SoundFonts/R-harpsi.sf2"
	# define HORN1_SOUNDFONT "SoundFonts/Horn1.sf2"
	# define JUSTIFIED_HIT_SOUNDFONT "SoundFonts/Justhit.sf2"
	# define JV_1080_BASS_SOUNDFONT "SoundFonts/Jvbass.sf2"
	# define KICK_ARSE_BASS_SOUNDFONT "SoundFonts/Kick-arse_bass.sf2"
	# define LAID_BASS_SOUNDFONT "SoundFonts/Laidbass.sf2"
	# define LEAD_GUITARS_SOUNDFONT "SoundFonts/3_Lead_Guitars.sf2"
	# define MAGNIFICENT_ORGAN_SOUNDFONT "SoundFonts/Maggoth2.sf2"
	# define MEAN_SAXOPHONE_SOUNDFONT "SoundFonts/Meansax.sf2"
	# define METALLIC_BASS_SOUNDFONT "SoundFonts/Metbass.sf2"
	# define MISCELLANEOUS_PERCUSSION_SOUNDFONT "SoundFonts/Virtual_Orchestra/Percussion/misc.sf2"
	# define MONKEY_THUMP_SOUNDFONT "SoundFonts/Monkey.sf2"
	# define MOURN_SAXOPHONE_SOUNDFONT "SoundFonts/Mournsax.sf2"
	# define MULTI_GUITAR_SOUNDFONT "SoundFonts/Multigit.sf2"
	# define NEO_ORGAN_SOUNDFONT "SoundFonts/Neoorgan.sf2"
	# define NIN_GUITAR_SOUNDFONT "SoundFonts/Ninguit.sf2"
	# define NUMB_HIT_SOUNDFONT "SoundFonts/Numbhit.sf2"
	# define PAGAN_WHISTLE_SOUNDFONT "SoundFonts/Pagan_Whistle.sf2"
	# define PHANTOM_HIT_SOUNDFONT "SoundFonts/Phanthit.sf2"
	# define PHANTOM_ORGAN_SOUNDFONT "SoundFonts/Phantorg.sf2"
	# define PIANO_SET_SOUNDFONT "SoundFonts/Pianoset.sf2"
	# define QUICK_HIT_SOUNDFONT "SoundFonts/Quickhit.sf2"
	# define REGULAR_1ST_VIOLIN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/1st-violin-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_2ND_VIOLIN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/2nd-violin-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_ALL_BRASS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/all-brass-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_ALL_STRINGS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/all-strings-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_ALL_WOODWINDS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/all-woodwinds-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_ALTO_FLUTE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/alto-flute-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_BASS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/bass-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_BASS_CLARINET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/bass-clarinet-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_BASS_TROMBONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/bass-trombone-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_BASSOON_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/bassoon-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_CELLO_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/cello-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_CLARINET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/clarinet-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_CONTRABASSOON_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/contrabassoon-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_ENGLISH_HORN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/english-horn-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_FLUTE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/flute-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_FRENCH_HORN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/french-horn-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_OBOE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/oboe-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_PICCOLO_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/piccolo-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_TROMBONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/trombone-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_TRUMPET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/trumpet-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_TUBA_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/tuba-SOLO-normal-mod-wheel.sf2"
	# define REGULAR_VIOLA_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/viola-SOLO-normal-mod-wheel.sf2"
	# define ROBOTECH_HIT_SOUNDFONT "SoundFonts/Robohit.sf2"
	# define ROCK_GUITAR_SOUNDFONT "SoundFonts/Emu_Rockgtr.sf2"
	# define ROCK_HIT_SOUNDFONT "SoundFonts/rockhit.sf2"
	# define ROCK_ORGAN_SOUNDFONT "SoundFonts/Rockorg1.sf2"
	# define ROLAND_BASS1_SOUNDFONT "SoundFonts/R-bass1.sf2"
	# define ROLAND_BASS2_SOUNDFONT "SoundFonts/R-bass2.sf2"
	# define ROLAND_DULCIMER_SOUNDFONT "SoundFonts/R-dulcim.sf2"
	# define ROLAND_PIANO_SOUNDFONT "SoundFonts/R-piano.sf2"
	# define ROLAND_STRINGS_SOUNDFONT "SoundFonts/R-string.sf2"
	# define ROLAND_VIOLIN_SOUNDFONT "SoundFonts/R-violin.sf2"
	# define SEAGULL_ACOUSTIC_GUITAR_SOUNDFONT "SoundFonts/Seagull_Acoustic_Guitar.sf2"
	# define SEINFELD_BASS_SOUNDFONT "SoundFonts/Seinbass.sf2"
	# define SESSION_GUITAR_SOUNDFONT "SoundFonts/Sessgit.sf2"
	# define SIMPLE_DRUMS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Percussion/bassdrum-snare-cymbals.sf2"
	# define SITAR_SOUNDFONT "SoundFonts/Sitar.sf2"
	# define SLEEPTALK_HIT_SOUNDFONT "SoundFonts/Sleephit.sf2"
	# define SOFT_HORN_SOUNDFONT "SoundFonts/Softhorn.sf2"
	# define SOLID_BASS_SOUNDFONT "SoundFonts/Solid_Bass.sf2"
	# define STACCATO_1ST_VIOLIN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/1st-violin-SOLO-staccato.sf2"
	# define STACCATO_2ND_VIOLIN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/2nd-violin-SOLO-staccato.sf2"
	# define STACCATO_ALL_BRASS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/all-brass-SOLO-staccato.sf2"
	# define STACCATO_ALL_STRINGS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/all-strings-SOLO-staccato.sf2"
	# define STACCATO_ALL_WOODWINDS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/all-woodwinds-SOLO-staccato.sf2"
	# define STACCATO_ALTO_FLUTE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/alto-flute-SOLO-staccato.sf2"
	# define STACCATO_BASS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/bass-SOLO-staccato.sf2"
	# define STACCATO_BASS_CLARINET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/bass-clarinet-SOLO-staccato.sf2"
	# define STACCATO_BASS_TROMBONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/bass-trombone-SOLO-staccato.sf2"
	# define STACCATO_BASSOON_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/bassoon-SOLO-staccato.sf2"
	# define STACCATO_CELLO_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/cello-SOLO-staccato.sf2"
	# define STACCATO_CLARINET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/clarinet-SOLO-staccato.sf2"
	# define STACCATO_CONTRABASSOON_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/contrabassoon-SOLO-staccato.sf2"
	# define STACCATO_ENGLISH_HORN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/english-horn-SOLO-staccato.sf2"
	# define STACCATO_FLUTE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/flute-SOLO-staccato.sf2"
	# define STACCATO_FRENCH_HORN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/french-horn-SOLO-staccato.sf2"
	# define STACCATO_OBOE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/oboe-SOLO-staccato.sf2"
	# define STACCATO_PICCOLO_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/piccolo-SOLO-staccato.sf2"
	# define STACCATO_TROMBONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/trombone-SOLO-staccato.sf2"
	# define STACCATO_TRUMPET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/trumpet-SOLO-staccato.sf2"
	# define STACCATO_TUBA_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/tuba-SOLO-staccato.sf2"
	# define STACCATO_VIOLA_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/viola-SOLO-staccato.sf2"
	# define STAGE_GUITAR_SOUNDFONT "SoundFonts/Stagegtr.sf2"
	# define STAR_WARS_HIT_SOUNDFONT "SoundFonts/Starhit.sf2"
	# define STEEL_GUITAR_SOUNDFONT "SoundFonts/Steelgit.sf2"
	# define STEINWAY_GRAND_PIANO_SOUNDFONT "SoundFonts/Steinway_Grand_Piano_1.2.sf2"
	# define STRAIGHT_STRINGS_SOUNDFONT "SoundFonts/straight_strings.sf2"
	# define STRAT_MARSHALL_SOUNDFONT "SoundFonts/Strat_Marshall.sf2"
	# define STRINGS_WITH_PIANO_SOUNDFONT "SoundFonts/Strpiano.sf2"
	# define STUD_BASS_SOUNDFONT "SoundFonts/Studbass.sf2"
	# define SUBPIANO_SOUNDFONT "SoundFonts/Subpiano.sf2"
	# define SUSTAINED_1ST_VIOLIN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/1st-violin-SOLO-sustain.sf2"
	# define SUSTAINED_2ND_VIOLIN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/2nd-violin-SOLO-sustain.sf2"
	# define SUSTAINED_ALL_BRASS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/all-brass-SOLO-sustain.sf2"
	# define SUSTAINED_ALL_STRINGS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/all-strings-SOLO-sustain.sf2"
	# define SUSTAINED_ALL_WOODWINDS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/all-woodwinds-SOLO-sustain.sf2"
	# define SUSTAINED_ALTO_FLUTE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/alto-flute-SOLO-sustain.sf2"
	# define SUSTAINED_BASS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/bass-SOLO-sustain.sf2"
	# define SUSTAINED_BASS_CLARINET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/bass-clarinet-SOLO-sustain.sf2"
	# define SUSTAINED_BASS_TROMBONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/bass-trombone-SOLO-sustain.sf2"
	# define SUSTAINED_BASSOON_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/bassoon-SOLO-sustain.sf2"
	# define SUSTAINED_CELLO_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/cello-SOLO-sustain.sf2"
	# define SUSTAINED_CLARINET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/clarinet-SOLO-sustain.sf2"
	# define SUSTAINED_CONTRABASSOON_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/contrabassoon-SOLO-sustain.sf2"
	# define SUSTAINED_ENGLISH_HORN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/english-horn-SOLO-sustain.sf2"
	# define SUSTAINED_FLUTE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/flute-SOLO-sustain.sf2"
	# define SUSTAINED_FRENCH_HORN_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/french-horn-SOLO-sustain.sf2"
	# define SUSTAINED_OBOE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/oboe-SOLO-sustain.sf2"
	# define SUSTAINED_PICCOLO_SOUNDFONT "SoundFonts/Virtual_Orchestra/Woodwinds/piccolo-SOLO-sustain.sf2"
	# define SUSTAINED_TROMBONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/trombone-SOLO-sustain.sf2"
	# define SUSTAINED_TRUMPET_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/trumpet-SOLO-sustain.sf2"
	# define SUSTAINED_TUBA_SOUNDFONT "SoundFonts/Virtual_Orchestra/Brass/tuba-SOLO-sustain.sf2"
	# define SUSTAINED_VIOLA_SOUNDFONT "SoundFonts/Virtual_Orchestra/Strings/viola-SOLO-sustain.sf2"
	# define T_HIT_SOUNDFONT "SoundFonts/t-hit.sf2"
	# define TIMPANI_SOUNDFONT "SoundFonts/Virtual_Orchestra/Percussion/timpani-hit.sf2"
	# define TIRED_ACCORDION_SOUNDFONT "SoundFonts/Tired_Accordion.sf2"
	# define TRUMPETS_SOUNDFONT "SoundFonts/5_Trumpets.sf2"
	# define TUBULAR_BELLS_SOUNDFONT "SoundFonts/Virtual_Orchestra/Percussion/tubular-bells.sf2"
	# define TWILIGHT_ZONE_HIT_SOUNDFONT "SoundFonts/Zonehit.sf2"
	# define U20_PIANO_SOUNDFONT "SoundFonts/U20PIANO.sf2"
	# define VELOCITY_GRAND_PIANO_SOUNDFONT "SoundFonts/Velocity_Grand_Piano.sf2"
	# define VIBRAPHONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Percussion/vibraphone-open.sf2"
	# define VIOLIN_SOUNDFONT "SoundFonts/Violin.sf2"
	# define VIOLIN_PIZZICATO_SOUNDFONT "SoundFonts/Viopiz2.sf2"
	# define WHISTLE_SOUNDFONT "SoundFonts/Whistsolo.sf2"
	# define WORSHIP_CHOIR_SOUNDFONT "SoundFonts/Worshchr.sf2"
	# define XYLOPHONE_SOUNDFONT "SoundFonts/Virtual_Orchestra/Percussion/xylophone.sf2"

	// Drums (23) Constants
	# define DRUM_01 "CRASH_CYMBAL_1"
	# define DRUM_02 "CRASH_CYMBAL_2"
	# define DRUM_03 "REVERSE_CYMBAL"
	# define DRUM_04 "RIDE_CYMBAL"
	# define DRUM_05 "808_CYMBAL"
	# define DRUM_06 "OPEN_HIHAT_1"
	# define DRUM_07 "OPEN_HIHAT_2"
	# define DRUM_08 "808_OPEN_HIHAT"
	# define DRUM_09 "CLOSED_HIHAT"
	# define DRUM_10 "808_CLOSED_HIHAT"
	# define DRUM_11 "TAMBOURINE"
	# define DRUM_12 "BASS_DRUM"
	# define DRUM_13 "808_BASS_DRUM"
	# define DRUM_14 "HAND_CLAP"
	# define DRUM_15 "808_COWBELL"
	# define DRUM_16 "909_SNARE_1"
	# define DRUM_17 "909_SNARE_2"
	# define DRUM_18 "909_SNARE_3"
	# define DRUM_19 "909_SNARE_4"
	# define DRUM_20 "HIGH_TOM"
	# define DRUM_21 "MID_TOM"
	# define DRUM_22 "LOW_TOM"
	# define DRUM_23 "RIM_SHOT"

	// Miscellaneous Percussion (41) Constants
	# define MISCELLANEOUS_PERCUSSION_01 "ROLL_HI_TRIANGLE"
	# define MISCELLANEOUS_PERCUSSION_02 "MUTED_HI_TRIANGLE"
	# define MISCELLANEOUS_PERCUSSION_03 "OPEN_HI_TRIANGLE"
	# define MISCELLANEOUS_PERCUSSION_04 "GONG"
	# define MISCELLANEOUS_PERCUSSION_05 "TAMTAM_A"
	# define MISCELLANEOUS_PERCUSSION_06 "TAMTAM_B"
	# define MISCELLANEOUS_PERCUSSION_07 "TAMTAM_C"
	# define MISCELLANEOUS_PERCUSSION_08 "HI_FINGER_CYMBALS"
	# define MISCELLANEOUS_PERCUSSION_09 "LO_FINGER_CYMBALS"
	# define MISCELLANEOUS_PERCUSSION_10 "SHAKE_TAMBOURINE"
	# define MISCELLANEOUS_PERCUSSION_11 "HIT_TAMBOURINE"
	# define MISCELLANEOUS_PERCUSSION_12 "ROLL_TAMBOURINE"
	# define MISCELLANEOUS_PERCUSSION_13 "COWBELL"
	# define MISCELLANEOUS_PERCUSSION_14 "VIBRA_SLAP"
	# define MISCELLANEOUS_PERCUSSION_15 "HIT_HIGH_CONGA"
	# define MISCELLANEOUS_PERCUSSION_16 "TAP_HIGH_CONGA"
	# define MISCELLANEOUS_PERCUSSION_17 "HIT_MID_CONGA"
	# define MISCELLANEOUS_PERCUSSION_18 "TAP_MID_CONGA"
	# define MISCELLANEOUS_PERCUSSION_19 "HIT_LOW_CONGA"
	# define MISCELLANEOUS_PERCUSSION_20 "TAP_LOW_CONGA"
	# define MISCELLANEOUS_PERCUSSION_21 "CABASA"
	# define MISCELLANEOUS_PERCUSSION_22 "SHAKER"
	# define MISCELLANEOUS_PERCUSSION_23 "CRANK_RATCHET"
	# define MISCELLANEOUS_PERCUSSION_24 "FAST_RATCHET"
	# define MISCELLANEOUS_PERCUSSION_25 "SLOW_RATCHET"
	# define MISCELLANEOUS_PERCUSSION_26 "CASTANETS"
	# define MISCELLANEOUS_PERCUSSION_27 "CLAVES"
	# define MISCELLANEOUS_PERCUSSION_28 "HI_WOOD_BLOCK"
	# define MISCELLANEOUS_PERCUSSION_29 "LO_WOOD_BLOCK"
	# define MISCELLANEOUS_PERCUSSION_30 "ROLL_LO_TRIANGLE"
	# define MISCELLANEOUS_PERCUSSION_31 "MUTED_LO_TRIANGLE"
	# define MISCELLANEOUS_PERCUSSION_32 "OPEN_LO_TRIANGLE"
	# define MISCELLANEOUS_PERCUSSION_33 "SLEIGH_BELLS"
	# define MISCELLANEOUS_PERCUSSION_34 "BELL_TREE_1"
	# define MISCELLANEOUS_PERCUSSION_35 "BELL_TREE_2"
	# define MISCELLANEOUS_PERCUSSION_36 "BELL_TREE_3"
	# define MISCELLANEOUS_PERCUSSION_37 "BELL_TREE_4"
	# define MISCELLANEOUS_PERCUSSION_38 "BELL_TREE_5"
	# define MISCELLANEOUS_PERCUSSION_39 "BAR_CHIMES_A"
	# define MISCELLANEOUS_PERCUSSION_40 "BAR_CHIMES_B"
	# define MISCELLANEOUS_PERCUSSION_41 "BAR_CHIMES_C"

	// Simple Drums (20) Constants
	# define SIMPLE_DRUM_01 "BASS_DRUM_1"
	# define SIMPLE_DRUM_02 "BASS_DRUM_2"
	# define SIMPLE_DRUM_03 "LEFT_HIT_SNARE_1"
	# define SIMPLE_DRUM_04 "ROUND_HIT_SNARE_1"
	# define SIMPLE_DRUM_05 "RIGHT_HIT_SNARE_1"
	# define SIMPLE_DRUM_06 "ROLL_SNARE_1"
	# define SIMPLE_DRUM_07 "LEFT_HIT_SNARE_2"
	# define SIMPLE_DRUM_08 "ROUND_HIT_SNARE_2"
	# define SIMPLE_DRUM_09 "RIGHT_HIT_SNARE_2"
	# define SIMPLE_DRUM_10 "TAP_SNARE_2"
	# define SIMPLE_DRUM_11 "ROLL_SNARE_2"
	# define SIMPLE_DRUM_12 "ROLL_A_CYMBALS_1"
	# define SIMPLE_DRUM_13 "LO_CRASH_CYMBALS_1"
	# define SIMPLE_DRUM_14 "ROLL_B_CYMBALS_1"
	# define SIMPLE_DRUM_15 "HI_CRASH_CYMBALS_1"
	# define SIMPLE_DRUM_16 "SHORT_ROLL_CYMBALS_2"
	# define SIMPLE_DRUM_17 "LO_CRASH_CYMBALS_2"
	# define SIMPLE_DRUM_18 "MID_ROLL_CYMBALS_2"
	# define SIMPLE_DRUM_19 "LONG_ROLL_CYMBALS_2"
	# define SIMPLE_DRUM_20 "HI_CRASH_CYMBALS_2"

# endif
