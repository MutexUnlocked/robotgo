// Copyright 2016 The MutexUnlocked Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// https://github.com/MutexUnlocked/robotgo/blob/master/LICENSE
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

#include "os.h"

#if defined(IS_MACOSX)
	#include "../hook/darwin/input_c.h"
	#include "../hook/darwin/hook_c.h"
	#include "../hook/darwin/event_c.h"
	#include "../hook/darwin/properties_c.h"
#elif defined(USE_X11)
	//#define USE_XKBCOMMON 0
	#include "../hook/x11/input_c.h"
	#include "../hook/x11/hook_c.h"
	#include "../hook/x11/event_c.h"
	#include "../hook/x11/properties_c.h"
#elif defined(IS_WINDOWS)
	#include "../hook/windows/input_c.h"
	#include "../hook/windows/hook_c.h"
	#include "../hook/windows/event_c.h"
	#include "../hook/windows/properties_c.h"
#endif

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../hook/iohook.h"
#include "../chan/eb_chan.h"

eb_chan events;
bool sending = false;

int vccode[100];
int codesz;

char *cevent;
// uint16_t *cevent;
int cstatus = 1;
int event_status;
int rrevent;

int add_hook(dispatcher_t dispatch);
void add_event_async();
int add_event(char *key_event);
int stop_event();

void dispatch_proc_end(iohook_event * const event);
// int allEvent(char *key_event);
int allEvent(char *key_event, int vcode[], int size);

// NOTE: The following callback executes on the same thread that hook_run() is called
// from.

struct _MEvent {
	uint8_t id;
	size_t mask;
	uint16_t keychar;
	// char *keychar;
	size_t x;
	uint8_t y;
	uint8_t bytesPerPixel;
};

typedef struct _MEvent MEvent;
// typedef MMBitmap *MMBitmapRef;

MEvent mEvent;


bool loggerProc(unsigned int level, const char *format, ...) {
	bool status = false;

	va_list args;
	switch (level) {
		#ifdef USE_DEBUG
		case LOG_LEVEL_DEBUG:
		case LOG_LEVEL_INFO:
			va_start(args, format);
			status = vfprintf(stdout, format, args) >= 0;
			va_end(args);
			break;
		#endif

		case LOG_LEVEL_WARN:
		case LOG_LEVEL_ERROR:
			va_start(args, format);
			status = vfprintf(stderr, format, args) >= 0;
			va_end(args);
			break;
	}

	return status;
}