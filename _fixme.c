
#include <time.h>

#include "quakedef.h"
#ifdef GLQUAKE
#include "gl_model.h"
#include "gl_local.h"
#endif
#include "cdaudio.h"
#ifdef WITH_KEYMAP
#include "keymap.h"
#endif // WITH_KEYMAP 
#include "resource.h"
#include "winquake.h"

// cvars.

cvar_t	cl_multiview = {"cl_multiview", "0" };
cvar_t	cl_mvdisplayhud = {"cl_mvdisplayhud", "1"};
cvar_t	cl_mvhudvertical = {"cl_mvhudvertical", "0"};
cvar_t	cl_mvhudflip = {"cl_mvhudflip", "0"};

cvar_t	r_lerpframes = {"r_lerpframes", "1"};
cvar_t	gl_clear = {"gl_clear", "0"};
cvar_t	gl_polyblend = {"gl_polyblend", "1"}; // 0
cvar_t	cl_mvinset = {"cl_mvinset", "0"};
cvar_t	cl_mvinsethud = {"cl_mvinsethud", "1"};
cvar_t	cl_mvinsetcrosshair = {"cl_mvinsetcrosshair", "1"};

// view origin
vec3_t		vup, vpn, vright;
vec3_t		r_origin;

// screen size info
refdef_t	r_refdef;
refdef2_t	r_refdef2;

// video/input.

HWND		mainwindow;

int				window_center_x, window_center_y;
RECT			window_rect;

unsigned d_8to24table[256];

void Draw_BeginDisc (void) {}
void Draw_EndDisc (void) {}
void Draw_InitConback (void) {}

void Mod_ClearAll (void) {}
void Mod_Init (void) {}
model_t *Mod_ForName (char *name, qbool crash) {return NULL;}

void R_InitParticles (void) {}
void R_ParticleTrail (vec3_t start, vec3_t end, vec3_t *trail_origin, trail_type_t type) {}