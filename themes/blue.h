static const char norm_fg[] = "#bfd0e8";
static const char norm_bg[] = "#000000";
static const char norm_border[] = "#8591a2";

static const char sel_fg[] = "#bfd0e8";
static const char sel_bg[] = "#028bd5";
static const char sel_border[] = "#bfd0e8";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
};
