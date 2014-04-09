/*
 * Copyright (c) 2011-2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//Process 217854097 (PROJECT) terminated SIGSEGV code=1 fltno=11 ip=0011a9f0(com.example.PROJECT.testDev_ple_PROJECT135a2c08@main+0x3c40) ref=00323000

#include <errno.h>
#include <fcntl.h>
#include <gulliver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/termio.h>
#include <sys/types.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/asoundlib.h>


#include <assert.h>
#include <bps/bps.h>
#include <screen/screen.h>
#include <bps/navigator.h>
#include <bps/audiomixer.h>
#include <bps/audiodevice.h>
#include <bps/dialog.h>
//#include "dialogutil.h"
#include <bps/screen.h>
#include <bps/event.h>
#include <stdarg.h>
#include <malloc.h>



#include <EGL/egl.h>
#include <GLES/gl.h>

#include "gestures/types.h"
#include "gestures/tap.h"
#include <sys/queue.h>
#include "input/event_types.h"
#include "gestures/bucket.h"
#include <sys/netmgr.h>
//#include "bbutil.h"
#include "input/screen_helpers.h"
#include <bps/mmrenderer.h>
#include <mm/renderer.h>

//Qt Libraries
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QToolButton>
#include <QDial>
#include <QSlider>
#include <QObject>
//End of Qt Libraries

#include "mainwidget.h"



extern float volume;
extern bool volumechange;
#define MSG_SIZE 1024
static char msg[MSG_SIZE];
static struct sigevent mmr_sigevent;

#if ! defined(DEFAULT_AUDIO_OUT)
    #define DEFAULT_AUDIO_OUT "audio:default"
#endif


#define RINGBUFFERSIZE 1048576
//#define TRUE	1
//#define FALSE	0
#define POLYPHONIC 32
using namespace std;
#include <cstring>
#include <string>


/*extern char* path1;
extern char* path2;
extern char* path3;*/

extern char* WAV_RELATIVE_PATH ;
extern char* WAV_RELATIVE_PATH_2;
extern char* WAV_RELATIVE_PATH_3;

extern bool changepath;

//int y1 = snprintf(WAV_RELATIVE_PATH, PATH_MAX, "%s",path1);
//int y2 = snprintf(WAV_RELATIVE_PATH_2, PATH_MAX, "%s",path2);
//int y3 = snprintf(WAV_RELATIVE_PATH_3, PATH_MAX, "%s",path3);

#ifndef max
	#define max(a,b) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#endif


int screen_val;
screen_event_t screen_event;


int chanid1; //creating channel id for message channnels
pid_t pid = 0;// creating process id for message queue, no same process pid = 0;
int status; //status of message receive buffer


// create file to store waves
FILE   *file1;
FILE   *file2;

//variables to store WAV data
int     mSamples;
int     mSampleRate;
int     mSampleChannels;
int     mSampleBits;

int     mSamples2;
int     mSampleRate2;
int     mSampleChannels2;
int     mSampleBits2;


//defining tranfefsamples to store WAV in memory
char* TransferSamples (FILE *file1, int  mSamples, int mSampleRate, int mSampleChannels, int mSampleBits);
int AllocMem();



//storing WAV path for file1 and file2
char input_file[PATH_MAX];
char input_file2[PATH_MAX];

//current working directory
char cwd[PATH_MAX];

//Data Structure for PlayItem
typedef struct{
	char *start;
	unsigned int LoopStart;
	unsigned int LoopEnd;
	char LoopFlag;
	unsigned int Length;
	char id;
	char note;
	char NewFlag;
	char Valid;
}PlayItem;

//Create Array of PlayItem
PlayItem *PlayArray[POLYPHONIC], *LastUsedPtrT1;

//points to the next avaliable play Item
int NextFreePlayArrayElement;

//Data Struture for Ring Buffer
struct RingBuf{
	char* ptr;
	char *CurrentWritePos;
	char *CurrentReadPos;
	int FillCount;
	pthread_mutex_t mutex;
	pthread_cond_t condvar;
}MyRingBuffer;

int	 vectorID=0;

//Data Structure for Messsage between Schedular and Thread1
typedef struct
{
    char*   Soundptr;
    long    SoundLen;
	char	NoteOn;
	char	NoteValue;
	char id;
} message;

//Define RIFF and WAVE id
const char *kRiffId = "RIFF";
const char *kWaveId = "WAVE";

//Data Structure for RIFF Tag
typedef struct
{
    char    tag[4];
    long    length;
}
RiffTag;

//Data Structure for RIFF Header
typedef struct
{
    char    Riff[4];
    long    Size;
    char    Wave[4];
}RiffHdr;

//Data Structure for Wav Header
typedef struct
{
    short   FormatTag;
    short   Channels;
    long    SamplesPerSec;
    long    AvgBytesPerSec;
    short   BlockAlign;
    short   BitsPerSample;
}WaveHdr;


//Reserving Memory for Pitch Shifed Sound
char   *mSampleBfr1F1, *mSampleBfr1F1high, *mSampleBfr2F1, *mSampleBfr2F1upa,  *mSampleBfr2F1upb;
char   *mSampleBfr2F1upc, *mSampleBfr2F1upd, *mSampleBfr2F1upe, *mSampleBfr2F1upf, *mSampleBfr2F1upg, *mSampleBfr2F1uph;
char   *mSampleBfr2F1upi, *mSampleBfr2F1upj, *mSampleBfr2F1upk,*mSampleBfr2F1high, *mSampleBfr2F1higha, *mSampleBfr2F1highb;
char   *mSampleBfr2F1highc, *mSampleBfr2F1highd, *mSampleBfr2F1highe, *mSampleBfr2F1highf, *mSampleBfr2F1highg, *mSampleBfr2F1highh;
char   *mSampleBfr2F1highi, *mSampleBfr2F1highj, *mSampleBfr2F1highk, *mSampleBfr2F1highcc;

//Store Length of Pitch Shifted Sound
size_t	lenbfrF1,lenbfrF1high, lenbfrF2, lenbfrF2upa, lenbfrF2upb, lenbfrF2upc, lenbfrF2upd, lenbfrF2upe, lenbfrF2upf, lenbfrF2upg, lenbfrF2uph;
size_t	lenbfrF2upi, lenbfrF2upj, lenbfrF2upk, lenbfrF2high,lenbfrF2higha, lenbfrF2highb, lenbfrF2highc, lenbfrF2highd, lenbfrF2highe;
size_t  lenbfrF2highf, lenbfrF2highg, lenbfrF2highh, lenbfrF2highi, lenbfrF2highj, lenbfrF2highk, lenbfrF2highcc;

//Declare a structure to describe the parameters of a PCM capture/playback channel
snd_pcm_t *pcm_handle;
snd_pcm_channel_info_t pi;
snd_mixer_t *mixer_handle;
snd_mixer_group_t group;
snd_pcm_channel_params_t pp,pp2;
snd_pcm_channel_setup_t setup;
fd_set  rfds, wfds;

int bsize;

unsigned long RingBufferActiveLength=0;
char* EndOfSoundptr;
char* Thread1Mix, *Thread1Mix2, *Thread2Mix;
int TotalNoteOn;


//Declare Threads
void* Thread1(void* arg);
void* PlayThread(void*);
void* Mixer(void* arg);
void* Scheduler(void* arg);


//prints error message
int err(char *messages)
{
    snprintf(msg, MSG_SIZE, "%s\n%s", messages, strerror(errno));
    return -1;
}

// finding wave header tag
int FindTag (FILE * fp, const char *tag)
{
    int retVal;
    RiffTag tagBfr = {"", 0};
    retVal = 0;

    // Keep reading until we find the tag or hit the EOF.
    while (fread ((unsigned char *) &tagBfr, sizeof (tagBfr), 1, fp))
    {
        // If this is our tag, set the length and break.
        if (strncmp (tag, tagBfr.tag, sizeof tagBfr.tag) == 0)
        {	retVal = ENDIAN_LE32(tagBfr.length);
            break;
        }

        // Skip ahead the specified number of bytes in the stream
        fseek (fp, tagBfr.length, SEEK_CUR);
    }
    // Return the result of our operation
    return (retVal);
}

//a function to check WAV header
int CheckHdr (FILE * fp)
{
    RiffHdr riffHdr = {"", 0};

    /* Read the header and make sure that this is indeed a Wave file. */
    if (fread ((unsigned char *) &riffHdr, sizeof (RiffHdr), 1, fp) == 0)
        return 0;

    if (strncmp (riffHdr.Riff, kRiffId, strlen (kRiffId)) ||
        strncmp (riffHdr.Wave, kWaveId, strlen (kWaveId)))
        return -1;

    return 0;
}

int dev_raw (int fd)
{
    struct termios termios_p;

    if (tcgetattr (fd, &termios_p))
        return (-1);

    termios_p.c_cc[VMIN] = 1;
    termios_p.c_cc[VTIME] = 0;
    termios_p.c_lflag &= ~(ECHO | ICANON | ISIG |
        ECHOE | ECHOK | ECHONL);
    termios_p.c_oflag &= ~(OPOST);
    return (tcsetattr (fd, TCSANOW, &termios_p));
}

int dev_unraw (int fd)
{
    struct termios termios_p;

    if (tcgetattr (fd, &termios_p))
        return (-1);

    termios_p.c_lflag |= (ECHO | ICANON | ISIG |
        ECHOE | ECHOK | ECHONL);
    termios_p.c_oflag |= (OPOST);
    return (tcsetattr (fd, TCSAFLUSH, &termios_p));
}



//*****************************************************************************
/* *INDENT-OFF* */
#ifdef __USAGE
%C[Options] *

Options:
    -a[card#:]<dev#>  the card & device number to play out on
#endif
/* *INDENT-ON* */
//*****************************************************************************

//Handle BB navigator events
static void handle_navigator_event(bps_event_t *event)
{
	switch (bps_event_get_code(event))
	{	case NAVIGATOR_SWIPE_DOWN:
			fprintf(stderr,"\nSwipe down event");
			break;
		case NAVIGATOR_EXIT:
			fprintf(stderr,"\nExit event");
			exit(-1);
			break;
		default:
			break;
	}
}


//Handle BB audio events
void handle_audio_event(bps_event_t *event)
{

	volume = audiomixer_event_get_output_level(event, AUDIOMIXER_OUTPUT_HEADPHONE);

	volume = audiomixer_event_get_output_level(event, AUDIOMIXER_OUTPUT_SPEAKER);

	audiomixer_event_get_input_level(event, AUDIOMIXER_INPUT);

	if (BPS_SUCCESS != audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, volume))
	{   fprintf(stderr,"Failed to decrease the volume.\n");        }

	if (BPS_SUCCESS != audiomixer_set_output_level(AUDIOMIXER_OUTPUT_HEADPHONE, volume))
	{   fprintf(stderr,"Failed to decrease the volume.\n");    }
}


void setFileParams(int x){

	WaveHdr wavHdr1;
	WaveHdr wavHdr2;
//	SmplHdr smplHdr1,smplHdr2;

	int rtn;
	long int save_pos, data_start_pos;
//	int loop_start;
//	int loop_end;
//	int samples2, samples3;


	if(x==0){

		rtn = snprintf(input_file, PATH_MAX, "%s/%s", cwd, WAV_RELATIVE_PATH);
			if (rtn > PATH_MAX - 1) {
				fprintf(stderr, "error line 422");

			}
		if ((file1 = fopen (input_file,"r")) == 0)
			//	return err ("file open #1");
			fprintf(stderr, "after fopen\n");
			if (CheckHdr (file1) == -1)
				fprintf(stderr, "Check Header #1\n");

			mSamples = FindTag (file1, "fmt ");
//			samples2 = FindTag (file1, "smpl");
			fread (&wavHdr1, sizeof (wavHdr1), 1, file1);
			fseek (file1, (mSamples - sizeof (WaveHdr)), SEEK_CUR);
//			fread (&smplHdr1, sizeof(smplHdr1), 1, file1);
//			fseek (file1, (samples2 - sizeof(smplHdr1)), SEEK_CUR);
			save_pos = ftell(file1);
			data_start_pos = save_pos+8;
			printf("\n data starts at %ld",save_pos);
			mSampleRate = ENDIAN_LE32(wavHdr1.SamplesPerSec);
			mSampleChannels = ENDIAN_LE16(wavHdr1.Channels);
			mSampleBits = ENDIAN_LE16(wavHdr1.BitsPerSample);
//			loop_start = ENDIAN_LE16(smplHdr1.Start);
//			loop_end = ENDIAN_LE16(smplHdr1.End);
//			loopon = loop_start;
//			loopoff = loop_end;
			mSamples = FindTag (file1, "data");
	}

	else if(x==1){

		rtn = snprintf(input_file2, PATH_MAX, "%s/%s", cwd, WAV_RELATIVE_PATH_2);
			if (rtn > PATH_MAX - 1) {
				fprintf(stderr, "error line 422");

				}

		if ((file2 = fopen (input_file2,"r")) == 0)
				//	return err ("file open #1");
			fprintf(stderr, "after fopen\n");
			if (CheckHdr (file2) == -1)
				fprintf(stderr, "Check Header #1\n");

			mSamples2 = FindTag (file2, "fmt ");
//			fread (&smplHdr2, sizeof(smplHdr2), 1, file2);
//			fseek (file2, (samples3 - sizeof(smplHdr2)), SEEK_CUR);
			fread (&wavHdr2, sizeof (wavHdr2), 1, file2);
			fseek (file2, (mSamples2 - sizeof (WaveHdr)), SEEK_CUR);
			save_pos = ftell(file2); //returns current position of stream specified by file2
			data_start_pos = save_pos+8;
			printf("\n data starts at %ld",save_pos);
			mSampleRate2 = ENDIAN_LE32(wavHdr2.SamplesPerSec);
			mSampleChannels2 = ENDIAN_LE16(wavHdr2.Channels);
			mSampleBits2 = ENDIAN_LE16(wavHdr2.BitsPerSample);
			mSamples2 = FindTag (file2, "data");

//			samples3 = FindTag (file2, "smpl");


//			loop_start = ENDIAN_LE16(smplHdr2.Start);
//			loop_end = ENDIAN_LE16(smplHdr2.End);
//			loopon = loop_start;
//			loopoff = loop_end;

	}
}


//used for handl error for audio volume control
#define COLOR_PURPLE 0xffff00ff
#define ERR(code) [code] = #code
static const char *errlist[] = {};

#undef ERR
#define NERRS ( sizeof(errlist) / sizeof(errlist[0]) )
static void mmrerror( mmr_context_t *ctxt, const char *errmsg ) {
    const mmr_error_info_t *err = mmr_error_info( ctxt );
    unsigned errcode = err->error_code;
    const char *name;
    if ( errcode >= NERRS || ( name = errlist[ errcode ] ) == NULL ) {
        name = "bad error code";
    }
}



int main(int argc, char *argv[])
{
	bps_initialize(); //intialise blackberry platform services

	//intialising Qt
	qputenv("QT_QPA_FONTDIR", "/usr/fonts/font_repository/monotype");
	QCoreApplication::addLibraryPath("app/native/lib");

	QApplication a(argc, argv);
	MainWidget w; //creates a window

	//Intialise multitouch for keyboard
	QList<QAbstractButton *> toolButtons = w.findChildren<QAbstractButton *>();
	foreach (QAbstractButton *toolButton, toolButtons)// DO NOT PUT SEMI-COLON IN HERE. THIS IS A FUNCTION
	{toolButton->setAttribute(Qt::WA_AcceptTouchEvents);}

	//intialise multitouch for slider and dial
	QList<QAbstractSlider *> sliders = w.findChildren<QAbstractSlider *>();
	foreach (QAbstractSlider *slider, sliders)// DO NOT PUT SEMI-COLON IN HERE. THIS IS A FUNCTION
	{slider->setAttribute(Qt::WA_AcceptTouchEvents);}

	//display the GUI on playbook screen
	 w.showFullScreen();

	int fragsize = -1;
	int     card = -1;
	int     dev = 0;

	WaveHdr wavHdr1;
	WaveHdr wavHdr2;

	int rtn;
	int jj;
	int count = 0;
	float offset;

	long int save_pos, data_start_pos;
	int trigger_flag;
	int got_char_flag;

	int    n, N = 0, c;
	char chid1[1];
	char chid2[1];
	int pitchDirection;

	//define parameter for threads and attribute
	struct sched_param Schedstruct, param;
	pthread_attr_t attr;

	//declaring thread ids
	pthread_t Thread1id, Thread2id, Mixerid, Schedulerid, PlayThreadid;

	/* create mutex with priority inheritance*/
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&MyRingBuffer.mutex, NULL);
	pthread_cond_init( &MyRingBuffer.condvar,NULL);

	//Allocate memory for PlayItems
	int i;
	for (i = 0; i<POLYPHONIC; i++)
	{
		PlayArray[i] = (PlayItem * )malloc(sizeof(PlayItem) );
		PlayArray[i]->Valid = FALSE;
	}
	NextFreePlayArrayElement = 0;



	getcwd(cwd, PATH_MAX);

	//opening sound card
	setvbuf (stdin, NULL, _IONBF, 0);
	if (card == -1)
	{
		if ((rtn = snd_pcm_open_preferred (&pcm_handle, &card, &dev, SND_PCM_OPEN_PLAYBACK)) < 0)
			fprintf(stderr, "device open\n");
	}
	else
	{
		if ((rtn = snd_pcm_open (&pcm_handle, card, dev, SND_PCM_OPEN_PLAYBACK)) < 0)
			fprintf(stderr, "device open\n");
	}

	//calling function audio setup
	setFileParams(0);
	setFileParams(1);

	printf ("SampleRate = %d, Channels = %d, SampleBits = %d\n", mSampleRate, mSampleChannels, mSampleBits);
	/* disabling mmap is not actually required in this example but it is included to
	 * demonstrate how it is used when it is required.
	 */

	if ((rtn = snd_pcm_plugin_set_disable (pcm_handle, PLUGIN_DISABLE_MMAP)) < 0)
	{
		fprintf (stderr, "snd_pcm_plugin_set_disable failed: %s\n", snd_strerror (rtn));
		return -1;
	}

	//initialize sound card playback and mixer group
	memset (&pi, 0, sizeof (pi));
	pi.channel = SND_PCM_CHANNEL_PLAYBACK;
	if ((rtn = snd_pcm_plugin_info (pcm_handle, &pi)) < 0)
	{
		fprintf (stderr, "snd_pcm_plugin_info failed: %s\n", snd_strerror (rtn));
		return -1;
	}

	memset (&pp, 0, sizeof (pp));

	pp.mode = SND_PCM_MODE_BLOCK;
	pp.channel = SND_PCM_CHANNEL_PLAYBACK;
	pp.start_mode = SND_PCM_START_FULL;
	pp.stop_mode = SND_PCM_STOP_STOP;

	pp.buf.block.frag_size = pi.max_fragment_size;
	 if (fragsize != -1)
	        pp.buf.block.frag_size = fragsize;
	pp.buf.block.frags_max = 1;
	pp.buf.block.frags_min = 1;

	pp.format.interleave = 1;

	pp.format.rate = mSampleRate;
	pp.format.voices = mSampleChannels;

	if (mSampleBits == 8)
		pp.format.format = SND_PCM_SFMT_U8;
	else
		pp.format.format = SND_PCM_SFMT_S16_LE;

	if ((rtn = snd_pcm_plugin_params (pcm_handle, &pp)) < 0)
	{
		fprintf (stderr, "snd_pcm_plugin_params failed: %s\n", snd_strerror (rtn));
		return -1;
	}

	if ((rtn = snd_pcm_plugin_prepare (pcm_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0)
		fprintf (stderr, "snd_pcm_plugin_prepare failed: %s\n", snd_strerror (rtn));


	memset (&setup, 0, sizeof (setup));
	memset (&group, 0, sizeof (group));
	setup.channel = SND_PCM_CHANNEL_PLAYBACK;
	setup.mixer_gid = &group.gid;


	if ((rtn = snd_pcm_plugin_setup (pcm_handle, &setup)) < 0)
	{
		fprintf (stderr, "snd_pcm_plugin_setup failed: %s\n", snd_strerror (rtn));
		return -1;
	}
	printf ("Format %s \n", snd_pcm_get_format_name (setup.format.format));
	printf ("Frag Size %d \n", setup.buf.block.frag_size);
	printf ("Rate %d \n", setup.format.rate);
	bsize = setup.buf.block.frag_size;

	if (group.gid.name[0] == 0)
	{
		printf ("Mixer Pcm Group [%s] Not Set \n", group.gid.name);
		exit (-1);
	}
	printf ("Mixer Pcm Group [%s]\n", group.gid.name);
	if ((rtn = snd_mixer_open (&mixer_handle, card, setup.mixer_device)) < 0)
	{
		fprintf (stderr, "snd_mixer_open failed: %s\n", snd_strerror (rtn));
		return -1;
	}




	FD_ZERO (&rfds);
	FD_ZERO (&wfds);
	n = 1;

	//Allocate memory for pitch sifted notes
	AllocMem();

	//Transfer samples to base note
    mSampleBfr1F1 = TransferSamples (file1, mSamples, mSampleRate, mSampleChannels, mSampleBits);
	mSampleBfr1F1high = TransferSamples (file2, mSamples2, mSampleRate2, mSampleChannels2, mSampleBits2);

	//allocating memory for Ring Buffer
	MyRingBuffer.ptr = (char*)malloc(RINGBUFFERSIZE);

	//insert silence into ring buffer
	memset (MyRingBuffer.ptr, 0, RINGBUFFERSIZE);
	MyRingBuffer.FillCount = 0;
	EndOfSoundptr = MyRingBuffer.ptr;

		const char *ctxtname = "testplayer"; //audio context name
		const char *inputtype = "track";
		const char *mmrname = NULL;
		const char *audioout = NULL;
		mmr_connection_t *connection = NULL; //audio connection id
		mmr_context_t *ctxt = NULL; //declare audio context
	    mode_t mode = S_IRUSR | S_IXUSR;
	    int audio_oid; // output ID
	    strm_dict_t *aoparams = NULL; // input parameters

	    audioout = DEFAULT_AUDIO_OUT;

	    connection = mmr_connect( mmrname ); //connect to audio
	    ctxt = mmr_context_create( connection, ctxtname, 0, mode ); //creaate an audio context
	    audio_oid = mmr_output_attach( ctxt, audioout, "audio" ); //attach context to audio output
	    if ( aoparams && mmr_output_parameters( ctxt, audio_oid, aoparams ) ) {
	        mmrerror( ctxt, "output parameters (audio)" );
	    }
	    //attach wav files to audio context
	     mmr_input_attach( ctxt, input_file, inputtype );
	     mmr_input_attach( ctxt, input_file2, inputtype );

	     //Getting and setting volume for Headphone and speaker
	     audiomixer_get_output_level(AUDIOMIXER_OUTPUT_SPEAKER, &volume);
	     audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, volume);
	     audiomixer_get_output_level(AUDIOMIXER_OUTPUT_HEADPHONE, &volume);
	     audiomixer_set_output_level(AUDIOMIXER_OUTPUT_HEADPHONE, volume);


	// create channels for message queue
	chanid1 = ChannelCreate(NULL);

    //setting attribute for threads
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
	Schedstruct.sched_priority = 8;
	pthread_attr_setschedparam(&attr, &Schedstruct);


		if (BPS_SUCCESS != navigator_request_events(0)) {
			fprintf(stderr, "Error requesting navigator events: %s", strerror(errno));
			exit(-1);
		}
		if (BPS_SUCCESS != navigator_rotation_lock(true)) {
			fprintf(stderr, "navigator_rotation_lock failed\n");
			exit(-1);
		}
		if (BPS_SUCCESS != dialog_request_events(0)) {
			fprintf(stderr, "Error requesting dialog events: %s", strerror(errno));
			exit(-1);
		}

		if (BPS_SUCCESS != audiodevice_request_events(0)) {
		   fprintf(stderr, "Error requesting audio device events: %s", strerror(errno));
		    exit(-1);
		}

	//creating Thread1, Mixer and Play thread
	chid1[0] = 1;
	pthread_create(&Thread1id, &attr, &Thread1, chid1);
	pthread_create(&Mixerid, &attr, &Mixer, NULL);
	pthread_create(&Schedulerid, &attr, &Scheduler, NULL);

	//creating and setting Scheduler thread to the highest priority
	Schedstruct.sched_priority = 9;
	pthread_attr_setschedparam(&attr, &Schedstruct);
	pthread_create(&PlayThreadid, &attr, &PlayThread, NULL);

	return a.exec();

	//joining thread
	pthread_join(Thread1id, NULL);
	pthread_join(Mixerid, NULL);
	pthread_join(Schedulerid, NULL);


	success:
	n = snd_pcm_plugin_flush (pcm_handle, SND_PCM_CHANNEL_PLAYBACK);

	rtn = snd_mixer_close (mixer_handle);
	rtn = snd_pcm_close (pcm_handle);
	fclose (file1);
	fclose (file2);

	free(mSampleBfr1F1);
	free(mSampleBfr1F1high);
	free(mSampleBfr2F1);
	free(mSampleBfr2F1upa);
	free(mSampleBfr2F1upb);
	free(mSampleBfr2F1upc);
	free(mSampleBfr2F1upd);
	free(mSampleBfr2F1upe);
	free(mSampleBfr2F1upf);
	free(mSampleBfr2F1upg);
	free(mSampleBfr2F1uph);
	free(mSampleBfr2F1upi);
	free(mSampleBfr2F1upj);
	free(mSampleBfr2F1upk);
	free(mSampleBfr2F1high);
	free(mSampleBfr2F1higha);
	free(mSampleBfr2F1highb);
	free(mSampleBfr2F1highc);
	free(mSampleBfr2F1highd);
	free(mSampleBfr2F1highe);
	free(mSampleBfr2F1highf);
	free(mSampleBfr2F1highg);
	free(mSampleBfr2F1highh);
	free(mSampleBfr2F1highi);
	free(mSampleBfr2F1highj);
	free(mSampleBfr2F1highk);

	return 0;
 }

//Allocate memory for pitch shifted note
int AllocMem(){

	int TotalSamplesCh,TotalSamplesCh2,n, n2;

	 TotalSamplesCh = (mSamples * 8)/(mSampleBits*mSampleChannels); //this is no. of samples in file per channel
	 TotalSamplesCh2 = (mSamples2 * 8)/(mSampleBits2*mSampleChannels2); //this is no. of samples in file per channel

	lenbfrF2 = TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1 = (char *)malloc (lenbfrF2);
	if(mSampleBfr2F1==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upa=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upa = (char *)malloc (lenbfrF2upa);
	if(mSampleBfr2F1upa==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upb=TotalSamplesCh*mSampleChannels*mSampleBits/8;
		mSampleBfr2F1upb = (char *)malloc (lenbfrF2upb);
		if(mSampleBfr2F1upb==NULL){
			fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
			exit(0);
		}

	lenbfrF2upc=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upc = (char *)malloc (lenbfrF2upc);
	if(mSampleBfr2F1upc==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upd=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upd = (char *)malloc (lenbfrF2upd);
	if(mSampleBfr2F1upd==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upe=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upe = (char *)malloc (lenbfrF2upe);
	if(mSampleBfr2F1upe==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upf=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upf = (char *)malloc (lenbfrF2upf);
	if(mSampleBfr2F1upf==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upg=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upg = (char *)malloc (lenbfrF2upg);
	if(mSampleBfr2F1upg==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2uph=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1uph = (char *)malloc (lenbfrF2uph);
	if(mSampleBfr2F1uph==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upi=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upi = (char *)malloc (lenbfrF2upi);
	if(mSampleBfr2F1upi==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upj=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upj = (char *)malloc (lenbfrF2upj);
	if(mSampleBfr2F1upj==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2uph=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1uph = (char *)malloc (lenbfrF2uph);
	if(mSampleBfr2F1uph==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upi=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upi = (char *)malloc (lenbfrF2upi);
	if(mSampleBfr2F1upi==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upj=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upj = (char *)malloc (lenbfrF2upj);
	if(mSampleBfr2F1upj==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2upk=TotalSamplesCh*mSampleChannels*mSampleBits/8;
	mSampleBfr2F1upk = (char *)malloc (lenbfrF2upk);
	if(mSampleBfr2F1upk==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2 space");
		exit(0);
	}

	lenbfrF2high = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1high = (char *)malloc (lenbfrF2high);
	if(mSampleBfr2F1high==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2high space");
		exit(0);
	}

	lenbfrF2higha = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1higha = (char *)malloc (lenbfrF2higha);
	if(mSampleBfr2F1higha==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highb = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highb = (char *)malloc (lenbfrF2highb);
	if(mSampleBfr2F1highb==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highc = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highc = (char *)malloc (lenbfrF2highc);
	if(mSampleBfr2F1highc==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highd = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highd = (char *)malloc (lenbfrF2highd);
	if(mSampleBfr2F1highd==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highe = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highe = (char *)malloc (lenbfrF2highe);
	if(mSampleBfr2F1highe==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highf = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highf = (char *)malloc (lenbfrF2highf);
	if(mSampleBfr2F1highf==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highg = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highg = (char *)malloc (lenbfrF2highg);
	if(mSampleBfr2F1highg==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highh = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highh = (char *)malloc (lenbfrF2highh);
	if(mSampleBfr2F1highh==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highi = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highi = (char *)malloc (lenbfrF2highi);
	if(mSampleBfr2F1highi==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highj = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highj = (char *)malloc (lenbfrF2highj);
	if(mSampleBfr2F1highj==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highk = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highk = (char *)malloc (lenbfrF2highk);
	if(mSampleBfr2F1highk==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

	lenbfrF2highcc = TotalSamplesCh2*mSampleChannels2*mSampleBits2/8;
	mSampleBfr2F1highcc = (char *)malloc (lenbfrF2highk);
	if(mSampleBfr2F1highcc==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF2higha space");
		exit(0);
	}

return 0;

}

//Transfer sample to Base Note
char* TransferSamples (FILE *file1, int  mSamples, int mSampleRate, int mSampleChannels, int mSampleBits)
{
    int TotalSamplesCh;
	char* myptr;

    TotalSamplesCh = (mSamples * 8)/(mSampleBits*mSampleChannels); //this is no. of samples in file per channel
    fprintf(stderr,"\ntotal no of samples/channel = %d",TotalSamplesCh);

	lenbfrF1= TotalSamplesCh*mSampleChannels*mSampleBits/8;
	myptr = (char *)malloc (lenbfrF1);
	if(myptr==NULL){
		fprintf(stderr,"\nproblem allocating mSampleBfrF1 space");
		exit(0);
	}
    return myptr;
}




void* Thread1(void* myid)
{
	message	in; //declaring a data structure for message from scheduler

	//Variables used for Initializing Message Receive and Message Reply
	int ret_val;
	int rcvid;
	char* replybuff[50];
	int rbyte = sizeof(replybuff);
	int sbyte = sizeof(in);

	PlayItem *PlayEventptr; //creating a PlayItem pointer

	//connect to message channel
	int coid = ConnectAttach( 0 ,pid, chanid1, 0 , NULL);

	int count = 0;
	int i, j;
	j=1;

	while(1){
			//receive message from Scheduler thread
			rcvid = MsgReceive (chanid1, (char*) &in, sbyte, NULL);

			//Send reply to scheduler to confirm message received
			MsgReply (rcvid, status, replybuff, rbyte);



		if(in.NoteOn == TRUE){

			/* reserve the resource */
			pthread_mutex_lock(&mutex);

				TotalNoteOn++;

			/* unlock the semaphore to release the resource */
			pthread_mutex_unlock(&mutex);

				//store information in PlayArray
				PlayEventptr = PlayArray[NextFreePlayArrayElement];
				LastUsedPtrT1 = PlayEventptr;
				PlayEventptr->start = in.Soundptr;
				PlayEventptr->NewFlag = TRUE;
				PlayEventptr->Length = in.SoundLen;
				PlayEventptr->LoopStart = (int)(0.5*(in.SoundLen/bsize))*bsize;
				PlayEventptr->LoopEnd = (int)(0.75*(in.SoundLen/bsize))*bsize;
				PlayEventptr->LoopFlag = FALSE;
				PlayEventptr->id = in.id;
				PlayEventptr->note = in.NoteValue;
				PlayEventptr->Valid = TRUE;

				//point to next avaliable PlayItem
				NextFreePlayArrayElement =(NextFreePlayArrayElement+1)%POLYPHONIC;

		}
		else if(in.NoteOn == FALSE){
			for(i=0;i<POLYPHONIC;i++){
				if((PlayArray[i]->id == in.id)&&(PlayArray[i]->note ==in.NoteValue)){

					/* reserve the resource */
					pthread_mutex_lock(&mutex);

					TotalNoteOn--;

					/* unlock the semaphore to release the resource */
					pthread_mutex_unlock(&mutex);

					//turn off looping
					PlayArray[i]->LoopFlag = FALSE;

					break;
				}

			}
		}
	}// end of while

	pthread_exit(NULL);
	return 0;
}


//Scan Play Item array and transfer a copy to the Ring Buffer
void* Mixer(void* arg)
{
	char *temp, *silence;
	char emptyArray;
	int vectorID[POLYPHONIC];
	int vectorCount;
	int k, EraseCount, EraseFlag, Erased;
	char *SavedStart[POLYPHONIC];

	//allocated memory for temp valid PlayItems are copied to temprojec
	temp = (char*)malloc(bsize);
	silence = (char*)malloc(bsize);

	EraseFlag = FALSE;
	MyRingBuffer.CurrentWritePos = MyRingBuffer.ptr;
	int j;
	for(j = 0;j<bsize;j++)
	{
		silence[j] = 0;
	}

	while(1)
	{

		k=0;
		EraseCount = 0;
		emptyArray = TRUE;
		memset(temp,0,bsize);
		int i;
		for(i=0;i<POLYPHONIC;i++)
		{
			if(PlayArray[i]->Valid == FALSE)
			{
				continue; // ignore this array element, increment i and back to top of the for loop.
			}
			emptyArray = FALSE;
			if(	PlayArray[i]->NewFlag == TRUE){
				SavedStart[i] = PlayArray[i]->start;
				PlayArray[i]->NewFlag = FALSE;
			}

			int j;
			for(j = 0;j<bsize;j++)
			{
					temp[j]=(temp[j]+PlayArray[i]->start[j])/2;

			}
			(PlayArray[i]->start)+=bsize;

			if((PlayArray[i]->start)>=SavedStart[i]+(PlayArray[i]->Length)){ //play is complete so remove array entry
					PlayArray[i]->Valid = FALSE;
			}
			else if((PlayArray[i]->start>=SavedStart[i]+PlayArray[i]->LoopEnd)&&(PlayArray[i]->LoopFlag==TRUE))
			{
					PlayArray[i]->start = SavedStart[i]+PlayArray[i]->LoopStart;

			}
		}
		if(emptyArray == FALSE){
			memcpy(MyRingBuffer.CurrentWritePos,temp,bsize);
			//copy latest data ring buffer

			pthread_mutex_lock(&MyRingBuffer.mutex);
			while(MyRingBuffer.FillCount == bsize)
			{
				pthread_cond_wait(&MyRingBuffer.condvar,&MyRingBuffer.mutex);
			}
			MyRingBuffer.FillCount = bsize;
			pthread_cond_signal(&MyRingBuffer.condvar);
			pthread_mutex_unlock(&MyRingBuffer.mutex);
		}
	}
	return 0;
}

//writes to the card fragment by fragment (bsize)
void* PlayThread(void* arg)
{
	int rtn, N=0;
	int n, ret_val, written = 0;
	char RingBufferCleared = TRUE;;
	struct mq_attr Mattr;
	message	in;
	snd_pcm_channel_status_t status;

	//set up sound card for playback
    FD_SET (STDIN_FILENO, &rfds);
    FD_SET (snd_mixer_file_descriptor (mixer_handle), &rfds);
    FD_SET (snd_pcm_file_descriptor (pcm_handle, SND_PCM_CHANNEL_PLAYBACK), &wfds);
    rtn = max (snd_mixer_file_descriptor (mixer_handle), snd_pcm_file_descriptor (pcm_handle, SND_PCM_CHANNEL_PLAYBACK));
    if (select (rtn + 1,NULL, &wfds, NULL, NULL) == -1){
	    printf("\nselect error");
	    exit(0);
    }


	N=0;
   	n = bsize;
	MyRingBuffer.CurrentReadPos = MyRingBuffer.ptr;
	//Read from Ring Buffer and send to Sound car for Play
	while(1){

			pthread_mutex_lock(&MyRingBuffer.mutex);
			while(MyRingBuffer.FillCount ==0){
				pthread_cond_wait(&MyRingBuffer.condvar,&MyRingBuffer.mutex);
			}

			if (FD_ISSET (snd_pcm_file_descriptor (pcm_handle, SND_PCM_CHANNEL_PLAYBACK), &wfds)){
	         	  	written = snd_pcm_plugin_write (pcm_handle, MyRingBuffer.CurrentReadPos, n);
		            if (written <  n){
     	                memset (&status, 0, sizeof (status));
		                status.channel = SND_PCM_CHANNEL_PLAYBACK;
					    if (snd_pcm_plugin_status (pcm_handle, &status) < 0){
							// get the runtime status
		                    		fprintf (stderr, "underrun: playback channel status error\n");
		                    		exit (1);
	                  	}
	                  	if (status.status == SND_PCM_STATUS_READY ||status.status == SND_PCM_STATUS_UNDERRUN){
				                // need to reprepare the card if there was an underrun
		                    if (snd_pcm_plugin_prepare (pcm_handle, SND_PCM_CHANNEL_PLAYBACK) < 0){
		                        fprintf (stderr, "underrun: playback channel prepare error\n");
		                        exit (1);
			                }
		                }
                           //try writing out what wasn't written in the last attempt (i.e. what's left of the fragment).
		                if (written < 0)  // if written negative then there was an error and nothing was written
		                    written = 0;   // so send the whole lot again
	               		written += snd_pcm_plugin_write (pcm_handle, MyRingBuffer.CurrentReadPos + written, n - written);
		            }
     		     }
				MyRingBuffer.FillCount =0;
				pthread_cond_signal(&MyRingBuffer.condvar);
				pthread_mutex_unlock(&MyRingBuffer.mutex);

		}

	pthread_exit(NULL);
	return 0;
}


void* Scheduler(void* arg)
{

	    int  rtn;
		int jj,TotalSamplesCh,TotalSamplesCh2;
		int count = 0;
		float offset;
	    int  n, n2, N = 0, c;
		int ival,index,i;
		char* ptr1=0;
		size_t size;

		//Array of offset for pitch shift
		//float offsetArray[13]= {1, 1.059463, 1.122462, 1.189207, 1.259921, 1.334839, 1.414213, 1.498307, 1.587401, 1.681792, 1.781797, 1.887748, 2};
		float offsetArray[13]= {1, 1.059463, 1.122462, 1.189207, 1.259921, 1.334839, 1.414213, 1.498307, 1.587401, 1.681792, 1.781797, 1.887748, 2};

		key	KeyIn;//data structure to receive message from MainWidget Class

		//Declare variable for Message queue
		int ret_val,ret_val1;
		int rcvid;
		char* replybuff1[50];
		int rbyte1 = sizeof(replybuff1);
		int sbyte1 = sizeof(KeyIn);

		//connect to message channel to Thread1
		int coid = ConnectAttach( 0 ,pid, chanid1, 0 , NULL);
		//connect to message channel from MainWidget
		int coid1 = ConnectAttach(0 ,pid1, chanid2, 0 , NULL);

		int T1id[POLYPHONIC][2];
		message Thread1Msg;

			//Reply Buffer Variable
			char* replybuff[50];
			int sbyte = sizeof(Thread1Msg);
			int rbyte = sizeof(replybuff);

			Thread1Msg.NoteOn = FALSE;

			int j;

			//set Note id to zero
			for(j=0;j<POLYPHONIC;j++){
				T1id[j][0]=0;
				T1id[j][1]=0;
			}



			TotalSamplesCh = (mSamples * 8)/(mSampleBits*mSampleChannels); //this is no. of samples in file per channel
			TotalSamplesCh2 = (mSamples2 * 8)/(mSampleBits2*mSampleChannels2); //this is no. of samples in file per channel

			if ((n = fread (mSampleBfr1F1, 1, mSamples, file1)) < 0)  //read the samples from disk into mSampleBfr1
			printf("\nproblem reading file to buffer");

			if ((n2 = fread (mSampleBfr1F1high, 1, mSamples2, file2)) < 0)  //read the samples from disk into mSampleBfr1
			printf("\nproblem reading file to buffer");

			while(1){

				if(volumechange == true)
				{
				  //Scan for Volume Change Events
				  audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, volume);
				  audiomixer_set_output_level(AUDIOMIXER_OUTPUT_HEADPHONE, volume);
				  volumechange = false;
					  if (BPS_SUCCESS != audiodevice_request_events(0))
					  { fprintf(stderr, "Error requesting audio device events: %s", strerror(errno));
						exit(-1);
					  }
				}

				//Receive message from MainWidget Class
				rcvid = MsgReceive (chanid2, (char*) &KeyIn, sbyte1, NULL);
				MsgReply (rcvid, status1, replybuff1, rbyte1);

				//Read Which Note is Playing Ist Octave
				if (KeyIn.note=="c"){ptr1=mSampleBfr2F1, size=lenbfrF2;}
				else if (KeyIn.note=="c#"){ptr1=mSampleBfr2F1upa, size=lenbfrF2upa;}
			    else if (KeyIn.note=="d"){ptr1=mSampleBfr2F1upb, size=lenbfrF2upb; }
		        else if (KeyIn.note=="d#"){ptr1=mSampleBfr2F1upc, size=lenbfrF2upc;}
				else if (KeyIn.note=="e"){ptr1=mSampleBfr2F1upd, size=lenbfrF2upd; }
				else if (KeyIn.note=="f"){ptr1=mSampleBfr2F1upe, size=lenbfrF2upe; }
				else if (KeyIn.note=="f#"){ptr1=mSampleBfr2F1upf, size=lenbfrF2upf;}
				else if (KeyIn.note=="g"){ptr1=mSampleBfr2F1upg, size=lenbfrF2upg; }
				else if (KeyIn.note=="g#"){ptr1=mSampleBfr2F1uph, size=lenbfrF2uph;}
				else if (KeyIn.note=="a"){ptr1=mSampleBfr2F1upi, size=lenbfrF2upi; }
				else if (KeyIn.note=="a#"){ptr1=mSampleBfr2F1upj, size=lenbfrF2upj;}
				else if (KeyIn.note=="b"){ptr1=mSampleBfr2F1upk, size=lenbfrF2upk; }

				//Read Which Note is Playing 2nd octave
				else if (KeyIn.note=="hc"){ptr1=mSampleBfr2F1high, size=lenbfrF2high;}
				else if (KeyIn.note=="hc#"){ptr1=mSampleBfr2F1higha, size=lenbfrF2higha;}
				else if (KeyIn.note=="hd"){ptr1=mSampleBfr2F1highb, size=lenbfrF2highb; }
				else if (KeyIn.note=="hd#"){ptr1=mSampleBfr2F1highc, size=lenbfrF2highc;}
				else if (KeyIn.note=="he"){ptr1=mSampleBfr2F1highd, size=lenbfrF2highd; }
				else if (KeyIn.note=="hf"){ptr1=mSampleBfr2F1highe, size=lenbfrF2highe; }
				else if (KeyIn.note=="hf#"){ptr1=mSampleBfr2F1highf, size=lenbfrF2highf;}
			    else if (KeyIn.note=="hg"){ptr1=mSampleBfr2F1highg, size=lenbfrF2highg; }
				else if (KeyIn.note=="hg#"){ptr1=mSampleBfr2F1highh, size=lenbfrF2highh;}
				else if (KeyIn.note=="ha"){ptr1=mSampleBfr2F1highi, size=lenbfrF2highi; }
				else if (KeyIn.note=="ha#"){ptr1=mSampleBfr2F1highj, size=lenbfrF2highj;}
				else if (KeyIn.note=="hb"){ptr1=mSampleBfr2F1highk, size=lenbfrF2highk; }

				//C note on third octave
				else if (KeyIn.note=="hcc"){ptr1=mSampleBfr2F1highcc, size=lenbfrF2highcc; }

				//this section decimates the base note to create higher pitches
				if (KeyIn.value==1){

							//read offset value
							offset=offsetArray[KeyIn.offsetValue];

							index = 0;

							if(mSampleChannels == 1)
							{  /* mono */
								jj = 0;
								while(1)
								{	ival = 2*((int)(index*(offset)));

									if(ival>n-1||ival==n-1)
									{
										break;
									}
										*(ptr1+jj) = *(mSampleBfr1F1+ival);
										*(ptr1+jj+1) = *(mSampleBfr1F1+ival+1);
										index++;
										jj = jj+2;
								}
							}
								else
								{	/* stereo */
									jj = 0;
									while(1)
									{	ival = 4*((int)(index*(offset)));

										if(ival>n-1||ival==n-1){
											break;
										}
										if((ival+3)>n)break;

										*(ptr1+jj) = *(mSampleBfr1F1+ival);
										*(ptr1+jj+1) = *(mSampleBfr1F1+ival+1);
										*(ptr1+jj+2) = *(mSampleBfr1F1+ival+2);
										*(ptr1+jj+3) = *(mSampleBfr1F1+ival+3);

										index++;
										jj = jj+4;
									}

								}


							//send Note On message to thread 1
							Thread1Msg.NoteOn = TRUE;
							Thread1Msg.NoteValue = 'x';
							T1id[i][0] = 'x';
							T1id[i][1] = i;
							Thread1Msg.id = i+1;
							int h = i++;
							T1id[i][1] = ((h) % POLYPHONIC) + 1;
							Thread1Msg.Soundptr = ptr1;
							Thread1Msg.SoundLen = size;

							status = MsgSend(coid, (const char*) &Thread1Msg, sbyte, replybuff, rbyte);
							pthread_mutex_lock(&mutex);
							KeyIn.value=0;
							pthread_mutex_unlock(&mutex);

						}

				//When key release is received
						else if( KeyIn.value==2){
							Thread1Msg.NoteOn = FALSE;
							Thread1Msg.NoteValue = 'x';
							int j;

							for(j=0;j<POLYPHONIC;j++){
								if(T1id[j][0] == 'x'){
								Thread1Msg.id = T1id[j][1];
								T1id[j][1] = 0;
								T1id[j][0] = 0;
								break;
								}
							}
							status = MsgSend(coid, (const char*) &Thread1Msg, sbyte, replybuff, rbyte);
							pthread_mutex_lock(&mutex);
							KeyIn.value=0;
							pthread_mutex_unlock(&mutex);
						}

					}

	pthread_exit(NULL);
	return 0;
}
