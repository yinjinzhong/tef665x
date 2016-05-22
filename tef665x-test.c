#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>

#include "../tef665x/tef665x.h"

int radio_cmd, radio_arg;
char radio_device[100] = "/dev/tef665x";


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
			strcpy(radio_device, argv[++i]);
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

	if ((fd = open(radio_device, O_RDWR)) < 0) {
		printf("Unable to open %s\n", radio_device);
		return -1;
	}

	return fd;
}

int main(int argc,char **argv)
{
	int fd;  
	int cmd;
	int arg = 0;
	tune_to_t tmp;

	if (process_cmdline(argc, argv) < 0) {
		return -1;
	}

	fd = radio_setup();

	sleep(1);

	printf("cmd = %d, arg = %ld\n", radio_cmd, radio_arg);
	switch (radio_cmd) {
		case 0:
			cmd = RADIODEV_IOCGETSTATUS;
		break;

		case 1:
		break;

		case 2:
			cmd = RADIODEV_IOCTURNTO;
			tmp.mode = 2;
			tmp.freq = 2512;
		break;

		default:
		break;
	}

	if (ioctl(fd, cmd, &tmp) < 0) {
		printf("Call cmd fail\n");
		return -1;
	}

	close(fd);

	return 0;
} 
