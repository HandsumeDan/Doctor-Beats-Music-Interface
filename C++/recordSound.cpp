# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <string>
# include "portaudio.h"
# include "FourierTransform.h"
# include "musicUtils.h"

int main(int argc,char** argv,char** envp)
{
	string outputFile;
	double timeDuration;

	int ch;
	while((ch=getopt(argc,argv,"d:o:"))!=EOF)
		{switch(ch)
			{case 'd':
				// Time Duration (seconds)
				timeDuration=strtod(optarg,NULL);
				break;
			case 'o':
				// Song Output File
				outputFile=optarg;
				break;
			default:
				exit(EXIT_FAILURE);}}

    PaStreamParameters  inputParameters,
                        outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;
    int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    SAMPLE              max, val;
    double              average;

    //printf("patest_record.c\n");
	fflush(stdout);

	data.maxFrameIndex = totalFrames = timeDuration * SAMPLE_RATE; /* Record for a few seconds. */
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);
    data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
    if( data.recordedSamples == NULL )
    {
        printf("Could not allocate record array.\n");
        goto done;
    }
    for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;

    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    }
    inputParameters.channelCount = 2;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              recordCallback,
              &data );
    if( err != paNoError ) goto done;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

    while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        //Pa_Sleep(1000);
        printf("index = %d\n", data.frameIndex ); fflush(stdout);
    }
    if( err < 0 ) goto done;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto done;

	printf("Done.\n"); fflush(stdout);

	double maxValue;
	int index;
	// Normalize Values to hold unit variance (biggest magnitude = 1)
	maxValue=abs_maximum_of_array(data.recordedSamples,numSamples,index);
	for(int j=0;j<numSamples;j++)
		{//cout<<j<<"|"<<data.recordedSamples[j]<<endl;
		data.recordedSamples[j]/=maxValue;}

	// Write Output Wave file after De-Noising
	string tmpFile=get_current_dir_name(); tmpFile+="/WAV/temp_input.wav";
	write_wave_file(tmpFile,data.recordedSamples,numSamples);

	// De-Noise Recording
	dnoise_song(tmpFile,outputFile,envp);

done:
    Pa_Terminate();
    if( data.recordedSamples )       /* Sure it is NULL or valid. */
        free( data.recordedSamples );
    if( err != paNoError )
    {
        fprintf( stderr, "An error occurred while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        err = 1;          /* Always return 0 or 1, but no other return codes. */
    }
    return err;

}
