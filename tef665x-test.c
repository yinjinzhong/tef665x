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

#ifndef VERSION
#define VERSION "ver: 0.0.1"
#endif

int radio_cmd, radio_arg;
static char* deviceName = "/dev/tef665x";


int process_cmdline(int argc, char **argv)
{
	int i;

	radio_cmd=0;
	radio_arg=0;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-c") == 0) {
			radio_cmd = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-a") == 0) {
			radio_arg = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-d") == 0) {
			strcpy(deviceName, argv[++i]);
		}
		else if (strcmp(argv[i], "-h") == 0) {
			printf("\n\nTEF665x -- NXP radio test help --\n\n" \
			"Syntax: radio-test\n"\
			" -c <radio ctrl command>\n" \
			" -a <radio ctrl arg>\n" \
			" -d <radio select, /dev/radio>\n");
			return -1;
		}
	}

	return 0;
}

int radio_setup(void)
{
	int fd = 0;

	if ((fd = open(deviceName, O_RDWR)) < 0) {
		printf("Unable to open %s\n", deviceName);
		return -1;
	}

	return fd;
}

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
		"-v | --version       Print version\n"
		"",
		argv[0]);
	}

static const char short_options [] = "d:ho:q:mruW:H:I:vc";

static const struct option
long_options [] = {
	{ "device",     required_argument,      NULL,           'd' },
	{ "help",       no_argument,            NULL,           'h' },
	{ "output",     required_argument,      NULL,           'o' },
	{ "quality",    required_argument,      NULL,           'q' },
	{ "mmap",       no_argument,            NULL,           'm' },
	{ "read",       no_argument,            NULL,           'r' },
	{ "userptr",    no_argument,            NULL,           'u' },
	{ "version",	no_argument,		NULL,		'v' },
	{ 0, 0, 0, 0 }
};

int main(int argc,char **argv)
{
	int fd;  
	int cmd;

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
				// print help
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

			default:
				usage(stderr, argc, argv);
				exit(EXIT_FAILURE);
		}
	}

	fd = radio_setup();
	sleep(1);


/*	printf("cmd = %d, arg = %ld\n", radio_cmd, radio_arg);*/
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

	close(fd);

	return 0;
}

