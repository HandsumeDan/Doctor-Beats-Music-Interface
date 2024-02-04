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
	string soundFile=get_current_dir_name(); soundFile+="/demo_drum_score";
	write_demo_drums_score_sound_file(soundFile);
	cout<<"Wrote demo Drums score to:\n"<<soundFile<<endl;
}
