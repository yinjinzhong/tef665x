#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>


#include "../tef665x/tef665x.h"


#define r_s_start 9400
#define r_s_stop  9550

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
		else if (strcmp(argv[i], "-d") == 0) {
			strcpy(radio_device, argv[++i]);
		}
		else if (strcmp(argv[i], "-h") == 0) {
			printf("\n\nTEF665x -- NXP radio test help --\n\n" \
			"Syntax: radio-test\n"\
			" -c <radio ctrl command>\n" \
			" -d <radio select, /dev/radio>\n");
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
	int vol = 12;
	q_data   qdata;
	int freq_s = 0;

	if (process_cmdline(argc, argv) < 0) {
		return -1;
	}

	fd = radio_start();

	sleep(1);

	printf("cmd = %d, arg = %ld\n", radio_cmd, radio_arg);
	switch (radio_cmd) {
		case 0:
			cmd = RADIODEV_IOCGETOPSTATUS;
			if (ioctl(fd, cmd, &status) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}
			printf("The status = %01x\n", status);
		break;

		case 2:
			/* 获取设备状态 */
			cmd = RADIODEV_IOCGETOPSTATUS;
			if (ioctl(fd, cmd, &status) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}
			printf ("Device status = %d\n", status);

			for (freq_s = r_s_start; freq_s < r_s_stop; freq_s+=10)
			{
				printf ("Set freq = %d\n", freq_s);
				/* 搜索 */
				cmd = RADIODEV_IOCTURNTO;
				tmp.mode = 2;
				tmp.freq = freq_s;

				if (ioctl(fd, cmd, &tmp) < 0) {
					printf("Call cmd fail\n");
					return -1;
				}

				sleep(1);

				/* 查询结果 */
				cmd = RADIODEV_IOCGETDATA;
				qdata.fm = 32;
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

			/* 设置频率 */
			cmd = RADIODEV_IOCTURNTO;
			tmp.mode = 1;
			tmp.freq = freq_s;

			printf ("Preset freq = %d\n", freq_s);

			if (ioctl(fd, cmd, &tmp) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}

			/* 设置声音大小 */
			cmd = RADIODEV_IOCSETVOL;
			if (ioctl(fd, cmd, &vol) < 0) {
				printf("Call cmd fail\n");
				return -1;
			}
		break;

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

