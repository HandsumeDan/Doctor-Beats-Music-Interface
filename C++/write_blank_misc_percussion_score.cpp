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
	string soundFile=get_current_dir_name(); soundFile+="/blank_misc_percussion_score";
	write_blank_misc_percussion_sound_file(soundFile);
	cout<<"Wrote blank Miscellaneous Percussion score to:\n"<<soundFile<<endl;
}



