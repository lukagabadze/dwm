/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static int swallowfloating          = 0;   static const int topbar             = 1;        /* 0 means bottom bar */ static const char *fonts[]          = { "monospace:size=8" };
static const char dmenufont[]       = "monospace:size=8";


/* color set */
#include "./colors.h"


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ "st",       NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	//{ "firefox",  NULL,       NULL,             0,            0,           0,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ ALTMOD|ShiftMask,             KEY,      tagnthmon,      {.i = TAG } },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *screenshotTmp[] = {"screenshot", "-t", NULL  };
static const char *screenshotSave[] = {"screenshot", "-n", NULL  };

static const char *incBrightness[] = {"brightness", "50", "inc", NULL};
static const char *decBrightness[] = {"brightness", "50", "dec", NULL};

static const char *incVolume[] = {"amixer", "set", "Master", "5%+", NULL};
static const char *decVolume[] = {"amixer", "set", "Master", "5%-", NULL};
static const char *toggleVolume[] = {"pactl", "set-sink-mute", "0", "toggle", NULL};

static const char *incBacklight[] = {"kbd-backlight", "-i" , NULL};
static const char *decBacklight[] = {"kbd-backlight", "-d" , NULL};
static const char *toggleBacklight[] = {"kbd-backlight", "-t" , NULL};

static const char *launchNvim[] = {"st", "nvim", NULL};
static const char *launchRanger[] = {"st", "ranger", NULL};
static const char *launchBrowser[] = {"firefox", NULL};

static const char *emojiCopy[] = {"emoji", NULL};
static const char *screenLock[] = {"xlock", NULL};

static const char *mountPartition[] = {"mount-partition", NULL};
static const char *umountPartition[] = {"umount-partition", NULL};

static const char *toggleThinklight[] = {"toggleThinklight", NULL};



static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,												XK_z,			 spawn,		       {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_v,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_p,      quit,           {0} },

	{MODKEY,                        XK_Print,  spawn,         {.v = screenshotTmp}},
	{MODKEY|ShiftMask,              XK_Print,  spawn,         {.v = screenshotSave}},


	{MODKEY,                        XK_n,      spawn,         {.v = launchNvim}},
	{MODKEY,                        XK_r,      spawn,         {.v = launchRanger}},
	{MODKEY,                        XK_w,      spawn,         {.v = launchBrowser}},

	{MODKEY,                        XK_F3,     spawn,         {.v = incVolume}},
	{MODKEY,                        XK_F2,     spawn,         {.v = decVolume}},
	{MODKEY,                        XK_F1,     spawn,         {.v = toggleVolume}},

	{MODKEY,                        XK_F6,     spawn,         {.v = incBrightness}},
	{MODKEY,                        XK_F5,     spawn,         {.v = decBrightness}},
	{MODKEY,                        XK_F9,     spawn,         {.v = toggleBacklight}},
	{MODKEY,                        XK_F11,    spawn,         {.v = decBacklight}},
	{MODKEY,                        XK_F12,    spawn,         {.v = incBacklight}},

	{MODKEY,                        XK_Home,   spawn,         {.v = mountPartition}},
	{MODKEY,                        XK_End,    spawn,         {.v = umountPartition}},

	{MODKEY,                        XK_Prior,  spawn,         {.v = toggleThinklight }},

	{MODKEY,                        XK_o,      spawn,         {.v = emojiCopy}},
	{MODKEY,                        XK_End,    spawn,         {.v = screenLock}},

	/* dual monitor setup */
  { MODKEY|ShiftMask,             XK_j,      focusnthmon,    {.i = -1 } },
  { MODKEY|ShiftMask,             XK_k,      focusnthmon,    {.i = +1 } },


  { MODKEY|ShiftMask,             XK_Tab,      focusnthmon,    {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

