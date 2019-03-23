#ifndef __SWIM__CONFIG__
#define __SWIM__CONFIG__

//#define CLASSIC
#define DEBUG
#define LINUX

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_purple[]      = "#753388";
static const char col_yellow[]      = "#eedd39";
static const char col_darkcyan[]    = "#222737";
static const char col_lime[]        = "#a7db4b";
static const char col_black[]       = "#000000";
static const char col_red[]         = "#FF0000";
static const char col_white[]       = "#FFFFFF";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
// classic blue
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
	[SchemeActive] = { col_gray3, col_darkcyan, col_gray2 },
	[SchemeStatus]  = { col_black, col_cyan,  col_cyan  },
	[SchemeAlert] = { col_gray3, col_red, col_gray3 },
	[SchemeBlack] = { col_black, col_gray3, col_black },
	[SchemeTest] = { col_red, col_lime, col_lime },
	[SchemeBW] = { col_white, col_black, col_white },
};



/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *tags[] = { "All", "work", "and", "no", "play", "makes", "Jack", "a","dull", "boy" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",       NULL,       NULL,       1 ,           0,           -1 },
	{ "Sublime_text",  NULL,       NULL,       2 ,           0,           -1 },
	{ "Evince",        NULL,       NULL,       1 << 3 ,      0,           -1 },
//	{ "st-256color",   NULL,       NULL,       1 << 2 ,      0,           -1 },
	{ "Thunderbird",   NULL,       NULL,       1 << 8 ,      0,           -1 },
	{ "Surf",   	   NULL,       NULL,       0      ,      0,           -1 },
	{ "tabbed-surf",   NULL,       NULL,       0      ,      0,           -1 },
	{ "xpad",          NULL,       NULL,       0      ,      1,           -1 },
	{ "Telegram",      NULL,       NULL,       1 << 8 ,      0,           -1 },

/* in case of no rules */
//	{ NULL,		NULL,		NULL,		0,				False,			-1},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "sharek.cpp"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "F",      NULL },    /* no layout function means floating behavior */
	{ "M",      monocle },
	{ "G",      grid },
 	{ "S",      spiral },
 	{ "D",      dwindle },
	{ "C",      cascade },
/*
	{ "[]=",      tile },
	{ "<><",      NULL },
	{ "[M]",      monocle },
	{ "HHH",      grid },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	{ "^*.",      cascade },
*/
};

/* key definitions */
#define MODKEY Mod4Mask

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//static const char *termcmd[]  = { "st","-e","/usr/bin/fish", "-t", "'TharMinaL'",NULL };
static const char *termcmd[]  = { "st","-e","/usr/bin/fish", NULL };
static const char *suspendcmd[]  = { "systemctl", "suspend",NULL };
static const char *mylockcmd[]  = { "slock", NULL };

static const char *webbrowsercmd[]  = { "surf-open", NULL };

#ifdef FREEBSD
static const char *myexitcmd[]  = { "killall","xinit", NULL };
#endif
#ifdef LINUX
static const char *myexitcmd[]  = { "killall","--signal","15","xinit", NULL };
#endif

static const char *mykillcmd[]  = { "xkill", NULL };
static const char *screenshootcmd[]  = { "import","-window","root", "/home/sharek/Documents/Screenshoot.png", NULL };

static const char *mynightmodecmd[]  = { "xrandr","--output", "eDP-1", "--gamma","1.1:0.8:0.7", "--brightness", "0.8", NULL };
static const char *mydaymodecmd[]  = { "xrandr","--output", "eDP-1", "--gamma","1.1:1.0:1.0", "--brightness", "1.0", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */

	// basic commands
	{ MODKEY,	                    XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = webbrowsercmd } }, // my_b start browser

	{ MODKEY|ShiftMask,             XK_k,      spawn,          {.v = mykillcmd } },
	
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = myexitcmd } },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },

	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = mylockcmd } }, // my_n lockscreen
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = suspendcmd } },

//	{ MODKEY|ShiftMask,             XK_s,      lockAndSuspend,  {0} },

	{ MODKEY|ControlMask,           XK_n,      spawn,          {.v = mynightmodecmd } },
	{ MODKEY|ControlMask,           XK_d,      spawn,          {.v = mydaymodecmd } },
	

	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ControlMask,           XK_b,      togglebar,      {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_s,      spawn,		   {.v = screenshootcmd } },

	// layout
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[6]} },

	// tags	
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },

	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
//	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
//	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
//	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
//	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkRootWin,           0,              Button1,        desktopClick,   {0} },
//	{ ClkRootWin,           0,              Button1,        spawn,          {.v = termcmd } },
};

#endif
