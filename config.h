/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 3;  /* border pixel of windows */
static const unsigned int snap     = 32;  /* snap pixel */
static const unsigned int gappih   = 10;  /* horiz inner gap between windows */
static const unsigned int gappiv   = 10;  /* vert inner gap between windows */
static const unsigned int gappoh   = 10;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov   = 3;  /* vert outer gap between windows and screen edge */
static const int smartgaps         = 0;  /* 1 means no outer gap when there is only one window */
static const int showbar           = 1;  /* 0 means no bar */
static const int topbar            = 0;  /* 0 means bottom bar */
static const int horizpadbar       = 10;  /* horizontal padding for statusbar */
static const int vertpadbar        = 10;  /* vertical padding for statusbar */
static const char *fonts[]         = {
    "Input Mono: size=12: antialias=true: hinting=true",
    "FontAwesome: size=10",
};
static const char dmenufont[]      = "Input Mono: weight=bold: size=12";
static const char drac_bg[]        = "#282a36";
static const char drac_bg3[]       = "#1e1f29";
static const char drac_sel[]       = "#6272a4";
static const char drac_sel2[]      = "#bd93f9";
static const char drac_sel3[]      = "#383a59";
static const char drac_fg[]        = "#f8f8f2";
static const char drac_green[]     = "#50fa7b";
static const char drac_cyan[]      = "#8be9fd";
static const char drac_grey[]      = "#44475a";
static const char *colors[][3]     = {
    /*               fg         bg         border   */
    [SchemeNorm] = { drac_fg,   drac_bg3,  drac_sel  },
    [SchemeSel]  = { drac_sel2, drac_bg,   drac_sel2  },
};
static const float mfact           = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster           = 1;  /* number of clients in master area */
static const int resizehints       = 1;  /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen    = 1;  /* 1 will force focus on the fullscreen window */
static const char *tags[]          = { "1", "2", "3", "4", "5" };
static const Layout layouts[]      = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};
static const Rule rules[] = {
    /*  class      instance    title       tags mask     isfloating   monitor */
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
#include <X11/XF86keysym.h>
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *termcmd[]  = { "kitty", NULL };
static const char *dmenucmd[] = { "dmenu_run" };
static const char *pscrncmd[] = { "printscreen",  NULL};
static const char *sscrncmd[] = { "selectscreen",  NULL};
static const char *brupcmd[]  = { "light", "-A", ".33", NULL };
static const char *brdwcmd[]  = { "light", "-U", ".33", NULL };
static const char *volucmd[]  = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *voldcmd[]  = { "pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *volmcmd[]  = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *micmcmd[]  = { "pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };
static Key keys[]             = {
    /* modifier         key                       function        argument */
    { MODKEY,           XK_p,                     spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask, XK_Return,                spawn,          {.v = termcmd } },
    { MODKEY,           XK_b,                     togglebar,      {0} },
    { MODKEY,           XK_j,                     focusstack,     {.i = +1 } },
    { MODKEY,           XK_k,                     focusstack,     {.i = -1 } },
    { MODKEY,           XK_i,                     incnmaster,     {.i = +1 } },
    { MODKEY,           XK_d,                     incnmaster,     {.i = -1 } },
    { MODKEY,           XK_h,                     setmfact,       {.f = -0.05} },
    { MODKEY,           XK_l,                     setmfact,       {.f = +0.05} },
    { MODKEY|Mod4Mask,  XK_0,                     togglegaps,     {0} },
    { MODKEY,           XK_Return,                zoom,           {0} },
    { MODKEY,           XK_Tab,                   view,           {0} },
    { MODKEY|ShiftMask, XK_c,                     killclient,     {0} },
    { MODKEY,           XK_t,                     setlayout,      {.v = &layouts[0]} },
    { MODKEY,           XK_f,                     setlayout,      {.v = &layouts[1]} },
    { MODKEY,           XK_m,                     setlayout,      {.v = &layouts[2]} },
    { MODKEY,           XK_space,                 setlayout,      {0} },
    { MODKEY|ShiftMask, XK_space,                 togglefloating, {0} },
    { MODKEY,           XK_0,                     view,           {.ui = ~0 } },
    { MODKEY|ShiftMask, XK_0,                     tag,            {.ui = ~0 } },
    { MODKEY,           XK_comma,                 focusmon,       {.i = -1 } },
    { MODKEY,           XK_period,                focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask, XK_comma,                 tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask, XK_period,                tagmon,         {.i = +1 } },
    { 0,                XK_Print,                 spawn,          {.v = printscreen } },
    { MODKEY,           XK_Print,                 spawn,          {.v = selectscreen } },
    { 0,                XF86XK_MonBrightnessUp,   spawn,          {.v = brupcmd } },
    { 0,                XF86XK_MonBrightnessDown, spawn,          {.v = brdwcmd } },
    { 0,                XF86XK_AudioRaiseVolume,  spawn,          {.v = volucmd} },
    { 0,                XF86XK_AudioLowerVolume,  spawn,          {.v = voldcmd} },
    { 0,                XF86XK_AudioMute,         spawn,          {.v = volmcmd} },
    { 0,                XF86XK_AudioMicMute,       spawn,         {.v = micmcmd} },
    TAGKEYS(            XK_1,                      0)
    TAGKEYS(            XK_2,                      1)
    TAGKEYS(            XK_3,                      2)
    TAGKEYS(            XK_4,                      3)
    TAGKEYS(            XK_5,                      4)
    TAGKEYS(            XK_6,                      5)
    TAGKEYS(            XK_7,                      6)
    TAGKEYS(            XK_8,                      7)
    TAGKEYS(            XK_9,                      8)
    { MODKEY|ShiftMask, XK_q,      quit,           {0} },
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

