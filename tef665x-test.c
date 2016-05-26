/*
 * For tef665x radio test program
 * Copyright 2016 HSAEYZ.
 *
 * Author: Yin Jinzhong <yinjinzhong@hangsheng.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>


#include "../tef665x/tef665x.h"


int r_s_start = 9000;
int r_s_stop  = 10800;
int vol = 10;

int radio_cmd, radio_arg;
char radio_device[30] = "/dev/tef665x";


int process_cmdline(int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-c") == 0)
			radio_cmd = atoi(argv[++i]);
		else if (strcmp(argv[i], "-d") == 0)
			strcpy(radio_device, argv[++i]);
		else if (strcmp(argv[i], "-s") == 0)
			r_s_start = atoi(argv[++i]);
		else if (strcmp(argv[i], "-e") == 0)
			r_s_stop = atoi(argv[++i]);
		else if (strcmp(argv[i], "-v") == 0)
			vol = atoi(argv[++i]);
		else if (strcmp(argv[i], "-h") == 0) {
			printf("\n\nTEF665x -- NXP radio test help --\n\n" \
			"Syntax: radio-test\n"\
			" -c <radio ctrl command>\n" \
			" -d <radio select, /dev/radio>\n"\
			" -s <radio serch start,[9000]>\n"\
			" -e <radio serch end,[10800]>\n"\
			" -v <radio volume,[10]>\n"\
			" -h <radio test help.>");
			return -1;
		}
	}

	return 0;
}

int radio_start(void)
{
	int fd = 0;

	if ((fd = open(radio_device, O_RDWR)) < 0) {
		printf("Unable to open %s\n", radio_device);
		return -1;
	}

	return fd;
}

int radio_close(int fd)
{
	return close(fd);
}


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

	if (process_cmdline(argc, argv) < 0) {
		return -1;
	}

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


        printf("Press q exit\n",ch);

	while( (ch=getchar())!='q' ){
		putchar(ch);
	}

	radio_close(fd);

	return 0;
}

