/*
 * Copyright 2016 HSAEYZ.
 *
 * Author: Yin Jinzhong <yinjinzhong@hangsheng.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _DRIVERS_TEF_665X_H
#define _DRIVERS_TEF_665X_H

#define RADIODEV_IOC_MAGIC  'r'

#define RADIODEV_IOCTURNTO	_IOW(RADIODEV_IOC_MAGIC, 0, int)
#define RADIODEV_IOCGETOPSTATUS	_IOR(RADIODEV_IOC_MAGIC, 1, int)
#define RADIODEV_IOCSETCLOCK	_IOW(RADIODEV_IOC_MAGIC, 2, int)
#define RADIODEV_IOCACTIVATE	_IO (RADIODEV_IOC_MAGIC, 3)
#define RADIODEV_IOCGETSTATUS	_IOR(RADIODEV_IOC_MAGIC, 4, int)
#define RADIODEV_IOCSETMUTE	_IOW(RADIODEV_IOC_MAGIC, 5, int)
#define RADIODEV_IOCSETVOL	_IOW(RADIODEV_IOC_MAGIC, 6, int)
#define RADIODEV_IOCSETPWR	_IOW(RADIODEV_IOC_MAGIC, 7, int)
#define RADIODEV_IOCSETCEQ	_IOW(RADIODEV_IOC_MAGIC, 8, int)

#define RADIODEV_IOCGETDATA	_IOR(RADIODEV_IOC_MAGIC, 9, int)
#define RADIODEV_IOCGETAS	_IOR(RADIODEV_IOC_MAGIC,10, int)

#define RADIODEV_IOCSETFMVALUE	_IOW(RADIODEV_IOC_MAGIC,11, int)
#define RADIODEV_IOCSETAMVALUE	_IOW(RADIODEV_IOC_MAGIC,12, int)

#define RADIODEV_IOCGETSTEREO	_IOR(RADIODEV_IOC_MAGIC,13, int)
#define RADIODEV_IOCSETSTEREO	_IOW(RADIODEV_IOC_MAGIC,14, int)

#define RADIODEV_IOC_MAXNR 15

char* ioctl_msg[RADIODEV_IOC_MAXNR] = {
	"RADIODEV_IOCTURNTO",
	"RADIODEV_IOCGETOPSTATUS",
	"RADIODEV_IOCSETCLOCK",
	"RADIODEV_IOCACTIVATE",
	"RADIODEV_IOCGETSTATUS",
	"RADIODEV_IOCSETMUTE",
	"RADIODEV_IOCSETVOL",
	"RADIODEV_IOCSETPWR",
	"RADIODEV_IOCSETCEQ",
	"RADIODEV_IOCGETDATA",
	"RADIODEV_IOCGETAS",
	"RADIODEV_IOCSETFMVALUE",
	"RADIODEV_IOCSETAMVALUE",
	"RADIODEV_IOCGETSTEREO",
	"RADIODEV_IOCSETSTEREO"
};

typedef enum {
	tune_action_standby	= 0, 
	tune_action_preset	,	/* Tune to new program with short mute time */
	tune_action_search	,	/* Tune to new program and stay muted */
	tune_action_af_update	,	/* Tune to alternative frequency, store quality and tune back with inaudible mute */
	tune_action_jump	,	/* Tune to alternative frequency with short inaudible mute  */
	tune_action_check	,	/* Tune to alternative frequency and stay muted */
	tune_action_end		,	/* Release the mute of a Search/Check action (frequency is ignored) */
} tune_mode;

typedef struct {
	tune_mode mode;
	unsigned short  freq;
} tune_to_t;

typedef enum {
	tune_power_on		= 0,
	tune_boot_state		,
	tune_idle_state		,
	tune_wait_active	,
	tune_active_state	,
	tune_not_exist		,
	tune_last		,
} tune_status;

typedef enum {
	clk_default		= 0,
	clk_ext			,
} clk_mode;

typedef struct {
	unsigned int frequency;
	clk_mode clk_sel;
} ref_clk;

typedef enum {
	dis_mute		= 0,
	en_mute			,
} mute_mode;


typedef enum {
	pwr_on			= 0,
	pwr_off			,
} power_sw;

typedef enum {
	isam			=0,
	isfm			,
} is_fm;

typedef struct {
	is_fm	fm;
	char status;
} q_status;


typedef struct {
	is_fm	fm;
	char	usn;
	char	wam;
	int16_t	offset;
	char	level;
} q_data;

typedef struct {
	is_fm	fm;
	char	resault;
} s_resault;

#endif



