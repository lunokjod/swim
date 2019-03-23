#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <time.h>

#include "sharek.hpp"


// layout icons
#include "res/layout_cascade.xbm"
#include "res/layout_dwindle.xbm"
#include "res/layout_free.xbm"
#include "res/layout_monocle.xbm"
#include "res/layout_spiral.xbm"
#include "res/layout_tile.xbm"
#include "res/layout_grid.xbm"

// slstat icons
#include "res/keyboard_img.xbm"
#include "res/keyboard_caps_img.xbm"
#include "res/keyboard_num_img.xbm"
#include "res/keyboard_all_img.xbm"

#include "res/battery_notcharging_img.xbm"
#include "res/battery_charging_img.xbm"
#include "res/battery_img.xbm"

#include "res/battery_80_img.xbm"
#include "res/battery_64_img.xbm"
#include "res/battery_48_img.xbm"
#include "res/battery_32_img.xbm"
#include "res/battery_16_img.xbm"

#include "res/battery_empty_img.xbm"


//#include "res/cpu_img.xbm"
/*
class Swim {
public:
	Swim() {

	}
	~Swim() {

	}
};
*/
unsigned int duskTime = 19 *60*60; // from 20h
unsigned int nightTime = 20 *60*60; // to 21h
unsigned int lastMode = 0; // 0 day, 1 dusk, 2 night

time_t
day_seconds() {
    time_t t1, t2;
    struct tm tms;
    time(&t1);
    localtime_r(&t1, &tms);
    tms.tm_hour = 0;
    tms.tm_min = 0;
    tms.tm_sec = 0;
    t2 = mktime(&tms);
    return t1 - t2;
}

void checkDayNightMonitorMode() {

	time_t secondsTilMidnight =  day_seconds();
	if ( secondsTilMidnight > nightTime ) {
		// night mode!
		if ( 2 != lastMode ) {
			fprintf(stdout, "Night mode"EOL);
			spawn("xrandr --output eDP-1 --gamma 1.1:0.8:0.7 --brightness 0.8");
			lastMode = 2;
		}
	} else if ( secondsTilMidnight > duskTime ) {
		// dusk mode (incremental)
		if ( 1 != lastMode ) {
			fprintf(stdout, "Dusk mode"EOL);
			lastMode = 1;
	    }
	} else {
		// day mode!
		if ( 0 != lastMode ) {
			fprintf(stdout, "Day mode"EOL);
			spawn("xrandr --output eDP-1 --gamma 1.0:1.0:1.0 --brightness 1.0");
			lastMode = 0;
		}
	}
}
void drw_image(Drw *drw, Window win, char *imgdata, int x, int y, int w, int h, int invert) {
	if (!drw || !drw->scheme) {
		return;
	}

	XSetForeground(drw->dpy, drw->gc, invert ? drw->scheme[ColBg].pixel : drw->scheme[ColFg].pixel);
	XSetBackground(drw->dpy, drw->gc, invert ? drw->scheme[ColFg].pixel : drw->scheme[ColBg].pixel);

	Pixmap image = XCreatePixmapFromBitmapData(drw->dpy, drw->drawable, imgdata, w, h, drw->scheme[ColBg].pixel, drw->scheme[ColFg].pixel, 1);

	XCopyPlane(drw->dpy, image, win, drw->gc,0, 0, w, h, x, y, 1);

	XFreePixmap(drw->dpy, image);

}

void drw_circle(Drw *drw, int x, int y, unsigned int r, int filled, int invert) {
	if (!drw || !drw->scheme)
		return;
	XSetForeground(drw->dpy, drw->gc, invert ? drw->scheme[ColBg].pixel : drw->scheme[ColFg].pixel);
	
//	XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, x+r, y+r);

	XDrawArc(drw->dpy, drw->drawable, drw->gc, x, y, r, r, filled, 360*64);
}
/*
void remove_spaces(const char *input, char *result)
{
  int i, j = 0;
  for (i = 0; input[i] != '\0'; i++) {
    if (!isspace((unsigned char) input[i])) {
      result[j++] = input[i];
    }
  }
  result[j] = '\0';
}
*/
unsigned int getAppNumber(Monitor *m, unsigned int tag) {
	unsigned int count = 0;
	Client *c;
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		if ( c->tags && tag ) {
			count++;
		}
	}
	return count;
}

void parseStatus(char * status, Monitor * m) {
	if ( NULL == status ) {
		return;
	}
	if ( 0 == strlen(status) ) {
		return;
	}
	
	char * statusCopy = strdup(status);
	/*malloc(strlen(status)+1);
	strcpy(statusCopy, status);
	*/

	/*
		{ cpu_freq,            "%s\n",		 ""},
		{ load_avg,            "%s\n",            "" },
		{ cpu_perc,            "%s\n",        "" },
		{ battery_state,       "%s\n",            "BAT1" },
		{ battery_perc,        "%s\n",           "BAT1" },
		{ battery_remaining,   "%s\n",           "BAT1" },
		{ datetime,            "%s\n",        "%F %R" },
		{ keyboard_indicators, "%s\n",          "c|n" },
	*/

	char *cpu_freq, *load_avg, *cpu_perc, *battery_state, *battery_perc, *battery_remaining, *datetime, *keyboard_indicators;

	cpu_freq = NULL;
	load_avg = NULL;
	cpu_perc = NULL;
	battery_state = NULL;
	battery_perc = NULL;
	battery_remaining = NULL;
	datetime = NULL;
	keyboard_indicators=NULL;

	char * token;
	unsigned int tokens=0;
	token = strtok(statusCopy, ",");
	while( token != NULL ) {
		tokens++;
		if ( 0 == strlen(token) ) {
			continue;
		}
		if        ( 1 == tokens ) {
			cpu_freq = strdup(token);
		} else if ( 2 == tokens ) {
			load_avg = strdup(token);
		} else if ( 3 == tokens ) {
			cpu_perc = strdup(token);
		} else if ( 4 == tokens ) {
			battery_state = strdup(token);
		} else if ( 5 == tokens ) {
			battery_perc = strdup(token);
		} else if ( 6 == tokens ) {
			battery_remaining = strdup(token);
		} else if ( 7 == tokens ) {
			datetime = strdup(token);
		} else if ( 8 == tokens ) {
			keyboard_indicators = strdup(token);
		}
		token = strtok(NULL, ",");
	}

//sw = TEXTW(stext) - lrpad + 2;
//drw_text(drw, m->ww - sw, 0, sw, bh, 0, stext, 0);
sw = m->ww/4;
size_t rightPadding = 0;

	drw_setscheme(drw, scheme[SchemeNorm]);

	if ( NULL != datetime ) {

//		drw_setscheme(drw, scheme[SchemeBlack]);

		size_t textSize = TEXTW(datetime);
		rightPadding += textSize;
		drw_text(drw,  m->ww - ((rightPadding - lrpad) +9) , 0, textSize, bh, 0, datetime, 0);
		free(datetime);
	}

	if ( NULL != keyboard_indicators ) {
		int capslock = 0;
		if ( 'C' == keyboard_indicators[0] ) {
			capslock = 1;
		}  
		int numlock = 0;
		if ( 'N' == keyboard_indicators[1] ) {
			numlock = 1;
		}  



		rightPadding+=keyboard_img_width;
		if ( 0 == capslock ) {
			if ( 0 == numlock ) {
				drw_image(drw, drw->drawable, (char*)keyboard_img_bits, m->ww - rightPadding, 1, keyboard_img_width, keyboard_img_height, 0);
			} else {
				drw_image(drw, drw->drawable, (char*)keyboard_num_img_bits, m->ww -rightPadding, 1, keyboard_num_img_width, keyboard_num_img_height, 0);
			}
		} else {
			if ( 0 == numlock ) {
				drw_image(drw, drw->drawable, (char*)keyboard_caps_img_bits, m->ww -rightPadding, 1, keyboard_caps_img_width, keyboard_caps_img_height, 0);
			} else {
				drw_image(drw, drw->drawable, (char*)keyboard_all_img_bits, m->ww -rightPadding, 1, keyboard_all_img_width, keyboard_all_img_height, 0);
			}			
		}
		free(keyboard_indicators);
	}

	if ( NULL != battery_remaining ) {
		char *realRemain;
		realRemain = battery_remaining+1;


		if ( 0 != strlen(realRemain)) {
			size_t textSize = TEXTW(realRemain);
			rightPadding += textSize;
			drw_text(drw,  m->ww - ((rightPadding - lrpad) +9) , 0, textSize-lrpad, bh, 0, realRemain, 0);
		}
		free(battery_remaining);
	}

	if ( NULL != battery_perc ) {
		rightPadding+=battery_img_width;
		int battLevel = atoi(battery_perc);

//		drw_image(drw, drw->drawable, (char*)battery_empty_img_bits, m->ww - rightPadding, 1, battery_empty_img_width, battery_empty_img_height, 0);
//		int imagePart = battLevel * battery_img_width / 100;
//		slog("battLevel * battery_img_width / 100: %d * %d / 100 = %d"EOL, battLevel, battery_img_width, imagePart);
		if ( battLevel < 16 ) {
			drw_setscheme(drw, scheme[SchemeAlert]);
		}

//@WIP
		//drw_image(drw, drw->drawable, (char*)battery_img_bits, m->ww - rightPadding, 1, battery_img_width -imagePart, battery_img_height, 0);


		if      ( battLevel > 96 )
			drw_image(drw, drw->drawable, (char*)battery_img_bits, m->ww - rightPadding, 1, battery_img_width, battery_img_height, 0);
		else if ( battLevel > 80 )
			drw_image(drw, drw->drawable, (char*)battery_80_img_bits, m->ww - rightPadding, 1, battery_80_img_width, battery_80_img_height, 0);
		else if ( battLevel > 64 )
			drw_image(drw, drw->drawable, (char*)battery_64_img_bits, m->ww - rightPadding, 1, battery_64_img_width, battery_64_img_height, 0);
		else if ( battLevel > 48 )
			drw_image(drw, drw->drawable, (char*)battery_48_img_bits, m->ww - rightPadding, 1, battery_48_img_width, battery_48_img_height, 0);
		else if ( battLevel > 32 )
			drw_image(drw, drw->drawable, (char*)battery_32_img_bits, m->ww - rightPadding, 1, battery_32_img_width, battery_32_img_height, 0);
		else if ( battLevel > 16 )
			drw_image(drw, drw->drawable, (char*)battery_16_img_bits, m->ww - rightPadding, 1, battery_16_img_width, battery_16_img_height, 0);
		else
			drw_image(drw, drw->drawable, (char*)battery_empty_img_bits, m->ww - rightPadding, 1, battery_empty_img_width, battery_empty_img_height, 0);
		if ( NULL != battery_state ) {
			if ( '?' == battery_state[0] ) {
					drw_image(drw, drw->drawable, (char*)battery_notcharging_img_bits, m->ww - rightPadding, 1, battery_notcharging_img_width, battery_notcharging_img_height, 0);
			} else if ( '+' == battery_state[0] ) {
					drw_image(drw, drw->drawable, (char*)battery_charging_img_bits, m->ww - rightPadding, 1, battery_charging_img_width, battery_charging_img_height, 0);
			}
			free(battery_state);
		}

		char * buffer = malloc(strlen(battery_perc)+2);
		sprintf(buffer,"%s%%", battery_perc);
		size_t textSize = TEXTW(buffer);
		rightPadding += textSize;
		drw_text(drw,  m->ww - ((rightPadding - lrpad) +9) , 0, textSize-lrpad, bh, 0, buffer, 0);
		free(buffer);

		sw = rightPadding;


		free(battery_perc);
		drw_setscheme(drw, scheme[SchemeNorm]);

	}

 



	if ( NULL != cpu_freq ) {
//		slog("cpu_freq: %s"EOL, cpu_freq);
		free(cpu_freq);
	}
	if ( NULL != load_avg ) {
//		slog("load_avg: %s"EOL, load_avg);
		free(load_avg);
	}
	if ( NULL != cpu_perc ) {

//		slog("cpu_perc: %s"EOL, cpu_perc);
		free(cpu_perc);
	}
	free(statusCopy);
}

void sharekDrawbar(Monitor *m) {
	// background
	drw_setscheme(drw, scheme[SchemeNorm]);
	drw_rect(drw, 0, 0, m->ww, bh, 1, 1);

	// draw root title

	/* draw status first so it can be overdrawn by tags later */
	if (m == selmon) { /* status is only drawn on selected monitor */
		//@TODO graphical mode!!!
		drw_setscheme(drw, scheme[SchemeNorm]);
		// draw text style
		/*
		sw = TEXTW(stext) - lrpad + 2;
		drw_text(drw, m->ww - sw, 0, sw, bh, 0, stext, 0);
		*/
		if ( strlen(stext) > 0 ) {
			parseStatus(stext, m);
		}

	}



	int boxs = drw->fonts->h / 9;
	int boxw = drw->fonts->h / 6 + 2;
	
	Client *c;
	unsigned int occ = 0;
	for (c = m->clients; c; c = c->next) {
		occ |= c->tags;
		/*
		if (c->isurgent) {
			urg |= c->tags;
		}*/
	}

	// draw tags
	int x = 0;
	int w;
	int urg =0; // hardcoded denote no urgent notification enabled @TODO
	for (int i = 0; i < LENGTH(tags); i++) {
		w = TEXTW(tags[i]);
		drw_setscheme(drw, scheme[m->tagset[m->seltags] & 1 << i ? SchemeSel : SchemeActive]);

		drw_text(drw, x, 0, w, bh, lrpad / 2, tags[i], urg & 1 << i);

		// draw tags decorations
		if (occ & 1 << i) {

			// upper line
			drw_rect(drw, x + boxw, boxs-3, w - ( 2 * boxw + 1), boxw,
			    m == selmon && selmon->sel && selmon->sel->tags & 1 << i,
			    urg & 1 << i);
			// bottom line
			drw_rect(drw, x + boxw, bh-2, w - ( 2 * boxw + 1), boxw,
			    m == selmon && selmon->sel && selmon->sel->tags & 1 << i,
			    urg & 1 << i);

		}
  
		x += w;
	}
	/* 
	// @TODO draw window dots
	x = 0; // begin again
	for (int i = 0; i < LENGTH(tags); i++) {
		w = TEXTW(tags[i]);
		//unsigned int appnum = getAppNumber(m, 1 << i);

//		for (c = m->clients; c; c = c->next) {
//			//@TODO test if cient is attached to this tag
//		}
		x += w;
	}*/


	// layout
	drw_setscheme(drw, scheme[SchemeBW]);

	switch (m->ltsymbol[0]) {
		case 'T':
			drw_image(drw, drw->drawable, (char*)layout_tile_bits, x, 1, layout_tile_width, layout_tile_height, 0);
			x+=layout_tile_width;
			break;
		case 'M':
			drw_image(drw, drw->drawable, (char*)layout_monocle_bits, x, 1, layout_monocle_width, layout_monocle_height, 0);
			x+=layout_monocle_width;
			break;
		case 'G':
			drw_image(drw, drw->drawable, (char*)layout_grid_bits, x, 1, layout_grid_width, layout_grid_height, 0);
			x+=layout_grid_width;
			break;
		case 'S':
			drw_image(drw, drw->drawable, (char*)layout_spiral_bits, x, 1, layout_spiral_width, layout_spiral_height, 0);
			x+=layout_spiral_width;
			break;
		case 'D':
			drw_image(drw, drw->drawable, (char*)layout_dwindle_bits, x, 1, layout_dwindle_width, layout_dwindle_height, 0);
			x+=layout_dwindle_width;
			break;
		case 'C':
			drw_image(drw, drw->drawable, (char*)layout_cascade_bits, x, 1, layout_cascade_width, layout_cascade_height, 0);
			x+=layout_cascade_width;
			break;
		case 'F':
		default:
			drw_image(drw, drw->drawable, (char*)layout_free_bits, x, 1, layout_free_width, layout_free_height, 0);
			x+=layout_free_width;

	}



	// draw title
/*
	//@TODO extract icon from _NET_WM_ICON
	auto shit = getatomprop(netatom[NetWMIcon]);
*/
	if ((w = m->ww - sw - x) > bh) {
		if (m->sel) {
			drw_setscheme(drw, scheme[m == selmon ? SchemeSel : SchemeNorm]);
			drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0);
		} else {
				drw_setscheme(drw, scheme[SchemeNorm]);
				drw_rect(drw, x, 0, w, bh, 1, 1);
		}
	}

	// copy icon
//@TODO	CRASHES
//	XCopyArea(drw->dpy, testPixmap, m->barwin, drw->gc, 0, 0, gimp_image.width, gimp_image.height, 0, 0);



/*
maybe with this?
	XPutImage(drw->dpy, testPixmap, )

XPutImage(display, d, gc, image, src_x, src_y, dest_x, dest_y, width, height)
        Display *display;
        Drawable d;
        GC gc;
        XImage *image;
        int src_x, src_y;
        int dest_x, dest_y;
        unsigned int width, height;
*/



	// flip
	drw_map(drw, m->barwin, 0, 0, m->ww, bh);
/*
void drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h)
{
	if (!drw)
		return;

	XCopyArea(drw->dpy, drw->drawable, win, drw->gc, x, y, w, h, x, y);
*/
}

void sharek_init() {
	logd = fopen(LOGFILENAME,"a");
	setbuf (logd, NULL);

	slog("-> Number5 is alive!"EOL);

}

void sharek_deinit() {
	//slog("dealloc testPixmap: %p <- %ld"EOL, (void*)drw->dpy, testPixmap);
	//XFreePixmap(drw->dpy, testPixmap);

	slog("-> See you soon!"EOL);
	fclose(logd);
}

void desktopClick(const Arg *arg) {
	slog("@TODO Click on destop"EOL);
}

void grid(Monitor *m) {
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for(rows = 0; rows <= n/2; rows++)
		if(rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - cw * cols : 0;
		resize(c, cx, cy, cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, False);
		i++;
	}
}

void cascade(Monitor *m) {
	size_t startPadding = 0;
	static size_t padding = 50;
	Client *c;
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		resize(c, m->wx+startPadding, m->wy+startPadding,
			(m->ww /2),
			(m->wh /2),
			  0);
		focus(c);
		startPadding += padding;
	}
}
