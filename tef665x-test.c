
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <signal.h>
#include <stdint.h>
#include <inttypes.h>


#include "../tef665x/tef665x.h"

<<<<<<< HEAD
#ifndef VERSION
#define VERSION "ver: 0.0.1"
#endif

#define CLEAR(x) memset (&(x), 0, sizeof (x))
=======

int r_s_start = 9000;
int r_s_stop  = 10800;
int vol = 10;
>>>>>>> V01

int radio_cmd, radio_arg;
static char* deviceName = "/dev/tef665x";

static int continuous = 0;


/**
SIGINT interput handler
*/
void StopContCapture(int sig_id) {
	printf("stoping continuous capture\n");
	continuous = 0;
}

void InstallSIGINTHandler() {
	struct sigaction sa;
	CLEAR(sa);
	
	sa.sa_handler = StopContCapture;
	if(sigaction(SIGINT, &sa, 0) != 0)
	{
		fprintf(stderr,"could not install SIGINT handler, continuous capture disabled");
		continuous = 0;
	}
}

int process_cmdline(int argc, char **argv)
{
	int i;

	radio_cmd=0;
	radio_arg=0;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-c") == 0) {
			radio_cmd = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-d") == 0) {
			strcpy(deviceName, argv[++i]);
		}
		else if (strcmp(argv[i], "-s") == 0){
			r_s_start = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-e") == 0){
			r_s_stop = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-v") == 0){
			vol = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-h") == 0) {
			printf("\n\nTEF665x -- NXP radio test help --\n\n" \
			"Syntax: radio-test\n"\
			" -c <radio ctrl command>\n" \
			" -d <radio select, /dev/radio>\n"\
			" -s <radio serch start,[9000]>\n"\
			" -e <radio serch end,[10800]>\n"\
			" -v <radio volume,[10]>\n");
			return -1;
		}
	}

	return 0;
}

int radio_start(void)
{
	int fd = 0;

	if ((fd = open(deviceName, O_RDWR)) < 0) {
		printf("Unable to open %s\n", deviceName);
		return -1;
	}

	return fd;
}

<<<<<<< HEAD
/**
	print usage information
*/
static void usage(FILE* fp, int argc, char** argv)
{
	fprintf(fp,
		"Usage: %s [options]\n\n"
		"Options:\n"
		"-d | --device name   Video device name [/dev/tef665x]\n"
		"-h | --help          Print this message\n"
		"-o | --output        Set output filename\n"
		"-q | --quality       Set quality (0-100)\n"
		"-m | --mmap          Use memory mapped buffers\n"
		"-r | --read          Use read() calls\n"
		"-u | --userptr       Use application allocated buffers\n"
		"-c | --continuous    Do continous capture, stop with SIGINT.\n"
		"-v | --version       Print version\n"
		"",
		argv[0]);
	}

static const char short_options [] = "d:ho:q:mruW:H:I:ea:vc";

static const struct option
long_options [] = {
	{ "device",     required_argument,      NULL,           'd' },
	{ "help",       no_argument,            NULL,           'h' },
	{ "output",     required_argument,      NULL,           'o' },
	{ "quality",    required_argument,      NULL,           'q' },
	{ "mmap",       no_argument,            NULL,           'm' },
	{ "read",       no_argument,            NULL,           'r' },
	{ "userptr",    no_argument,            NULL,           'u' },
	{ "command",    required_argument,	NULL,           'e' },
	{ "arg",    	required_argument,	NULL,           'a' },
	{ "version",	no_argument,		NULL,		'v' },
	{ "continuous",	no_argument,		NULL,		'c' },
	{ 0, 0, 0, 0 }
};
=======
int radio_close(int fd)
{
	return close(fd);
}

>>>>>>> V01

int main(int argc,char **argv)
{
	char ch;

	int fd;  
	int cmd;
	int arg = 0;
	tune_to_t tmp;
	tune_status status;
	q_data   qdata;
	int freq_s = 0;

	radio_cmd=0;
	radio_arg=0;

	for (;;) {
		int index, c = 0;

		system("clear");

		c = getopt_long(argc, argv, short_options, long_options, &index);

		if (-1 == c) {
			usage(stdout, argc, argv);
			break;
		}

		switch (c) {
			case 0: /* getopt_long() flag */
				break;

			case 'd':
				deviceName = optarg;
				break;

			case 'h':
				usage(stdout, argc, argv);
				exit(EXIT_SUCCESS);

			case 'o':
				break;

			case 'q':
				break;

			case 'm':

				fprintf(stderr, "You didn't compile for mmap support.\n");
				exit(EXIT_FAILURE);
				break;

			case 'r':
				fprintf(stderr, "You didn't compile for read support.\n");
				exit(EXIT_FAILURE);
				break;

			case 'u':
				fprintf(stderr, "You didn't compile for userptr support.\n");
				exit(EXIT_FAILURE);
				break;

			case 'v':
				printf("Version: %s\n", VERSION);
				exit(EXIT_SUCCESS);
				break;

			case 'e':
				radio_cmd = atoi(optarg);
				break;

			case 'c':
				// set flag for continuous capture, interuptible by sigint
				continuous = 1;
				InstallSIGINTHandler();
				break;

			case 'a':
				radio_arg = atoi(optarg);
				break;

			default:
				usage(stderr, argc, argv);
				exit(EXIT_FAILURE);
		}
	}

<<<<<<< HEAD
	fd = radio_setup();
	sleep(1);

	printf("cmd = %d, arg = %ld\n", radio_cmd, radio_arg);
/*	switch (radio_cmd) {*/
/*		case 0:*/
/*			cmd = RADIODEV_IOCGETSTATUS;*/
/*		break;*/

/*		case 1:*/
/*			//cmd = RADIODEV_IOCINIT;*/
/*		break;*/

/*		case 2:*/
/*			cmd = RADIODEV_IOCTURNTO;*/
/*		break;*/

/*		default:*/
/*		break;*/
/*	}*/

/*	ioctl(fd, cmd, radio_arg);*/
=======
	fd = radio_start();

	usleep(1);

	printf("cmd = %d, arg = %ld\n", radio_cmd, radio_arg);
	switch (radio_cmd) {
		case 0:
		{
			cmd = RADIODEV_IOCGETOPSTATUS;
			if (ioctl(fd, cmd, &status) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}
			printf("The status = %01x\n", status);
			break;
		}
		case 2:
		{
			/* Read the opration results */
			cmd = RADIODEV_IOCGETOPSTATUS;
			if (ioctl(fd, cmd, &status) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}
			printf ("Device status = %d\n", status);

			for (freq_s = r_s_start; freq_s < r_s_stop; freq_s+=10)
			{
				printf ("Set freq = %d\n", freq_s);
				/* Start search */
				cmd = RADIODEV_IOCTURNTO;
				tmp.mode = 2;
				tmp.freq = freq_s;

				if (ioctl(fd, cmd, &tmp) < 0) {
					printf("Call cmd fail\n");
					return -1;
				}

				usleep(60000);

				/* Read search results */
				cmd = RADIODEV_IOCGETDATA;
				qdata.fm = isfm;
				if (ioctl(fd, cmd, &qdata) < 0) {
					printf("Call cmd fail\n");
					return -1;
				}
				printf ("Get Data: fm = %d, usn = %d, wam = %d, offset = %d, level = %d\n",
					qdata.fm, qdata.usn, qdata.wam, qdata.offset, qdata.level);
				//printf ("Ned Data: fm = 32, usn > 27, wam > 23, offset >100, level < 20\n");

				if ((qdata.usn>27)||(qdata.wam>23)||(qdata.offset>100)||(qdata.level < 20)) continue;
				else break;
			}

			/* Reset to new frequency */
			cmd = RADIODEV_IOCTURNTO;
			tmp.mode = 1;
			tmp.freq = freq_s;

			printf ("Preset freq = %d\n", freq_s);

			if (ioctl(fd, cmd, &tmp) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}

			/* Setup the volume */
			cmd = RADIODEV_IOCSETVOL;
			if (ioctl(fd, cmd, &vol) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}
			break;
		}
		default:
			break;
	}

>>>>>>> V01

        printf("Press q exit\n",ch);

	while( (ch=getchar())!='q' ){
		putchar(ch);
	}

	radio_close(fd);

	return 0;
}

