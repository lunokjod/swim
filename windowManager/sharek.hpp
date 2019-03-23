#ifndef __SHAREK__DWM___
#define __SHAREK__DWM___

#include <stdio.h>

#define LOGFILENAME "/home/sharek/log/dwm-sharek.log"

#define EOL "\n"

FILE *logd = NULL;

#ifdef DEBUG
#define slog(...) \
	if ( NULL != logd ) { \
		fprintf(logd,##__VA_ARGS__); \
	}
#else
#define slog(...)
#endif


//Pixmap testPixmap;

float brightness = 1.0;

void drw_circle(Drw *drw, int x, int y, unsigned int r, int filled, int invert);
//void slog(char * whatToLog);
void sharek_init();
void sharek_deinit();
void desktopClick(const Arg *arg);
void cascade(Monitor *m);

#endif
