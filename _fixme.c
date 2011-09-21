
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

cvar_t	vid_wideaspect = {"vid_wideaspect", "0", CVAR_NO_RESET | CVAR_SILENT};

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

cvar_t	scr_menudrawhud		= {"scr_menudrawhud", "0"};
cvar_t	scr_menualpha		= {"scr_menualpha", "0.7"};

cvar_t	r_fullbrightSkins = {"r_fullbrightSkins", "1", 0};
cvar_t	gl_max_size			= {"gl_max_size", "2048", 0};
cvar_t	gl_picmip			= {"gl_picmip", "0"};
cvar_t	gl_textureless = {"gl_textureless", "0", 0};
cvar_t  vid_hwgammacontrol	= { "vid_hwgammacontrol", 	"2" };
cvar_t	r_drawflat   = {"r_drawflat", "0", 0};
cvar_t	r_novis = {"r_novis", "0"};
cvar_t	r_conwidth			= { "vid_conwidth",			"640" };
cvar_t  vid_flashonactivity = { "vid_flashonactivity",	"1", };
cvar_t	gl_texturemode		= {"gl_texturemode", "GL_LINEAR_MIPMAP_LINEAR", 0};
cvar_t	gl_anisotropy		= {"gl_anisotropy","1", 0};
cvar_t	r_fullscreen		= { "vid_fullscreen",		"1" };
cvar_t	r_displayRefresh	= { "vid_displayfrequency", "0"};
cvar_t	r_colorbits			= { "vid_colorbits",		"0" };
cvar_t	r_mode				= { "vid_mode",				"3" };

// view origin
vec3_t		vup, vpn, vright;
vec3_t		r_origin;

// screen size info
refdef_t	r_refdef;
refdef2_t	r_refdef2;

// video/input.

static mpic_t			pic_dummy;

mpic_t			*draw_disc;

qbool vid_hwgamma_enabled = false;
float vid_gamma = 1.0;

HWND		mainwindow;

int				window_center_x, window_center_y;
RECT			window_rect;

int		char_textures[MAX_CHARSETS];

unsigned d_8to24table[256];

void VID_Init (unsigned char *palette) {}
void VID_Shutdown (void) {}
void VID_SetCaption (char *text) {}
void VID_NotifyActivity(void) {}
void VID_SetDeviceGammaRamp (unsigned short *ramps) {}

void Draw_Init (void) {}
void Draw_BeginDisc (void) {}
void Draw_EndDisc (void) {}
void Draw_InitConback (void) {}
void Draw_Character (int x, int y, int num) {}
void Draw_TileClear (int x, int y, int w, int h) {}
void Draw_Pic (int x, int y, mpic_t *pic) {}
void Draw_String (int x, int y, const char *str) {}
mpic_t *Draw_CachePic (char *path) {return &pic_dummy;}
void Draw_SSubPic(int x, int y, mpic_t *gl, int srcx, int srcy, int width, int height, float scale) {}
void Draw_SColoredString (int x, int y, const wchar *text, clrinfo_t *clr, int clr_cnt, int red, float scale) {}
void Draw_AlphaFillRGB (int x, int y, int w, int h, color_t color) {}
void Draw_SString (int x, int y, const char *str, float scale) {}
void Draw_AlphaPic (int x, int y, mpic_t *pic, float alpha) {}
void Draw_AlphaString (int x, int y, char *str, float alpha) {}
void Draw_Fill (int x, int y, int w, int h, byte c) {}
void Draw_AlphaFill (int x, int y, int w, int h, byte c, float alpha) {}
void Draw_Crosshair(void) {}
void Draw_AlphaLineRGB (int x_start, int y_start, int x_end, int y_end, float thickness, color_t color) {}
void Draw_SAlphaPic (int x, int y, mpic_t *gl, float alpha, float scale) {}
void Draw_AlphaRectangleRGB (int x, int y, int w, int h, float thickness, qbool fill, color_t color) {}
mpic_t *Draw_CachePicSafe (const char *path, qbool crash, qbool only24bit) {return &pic_dummy;}
mpic_t *Draw_CacheWadPic (char *name) {return &pic_dummy;}
void Draw_ColoredString (int x, int y, const char *str, int red) {}
void Draw_SAlphaSubPic2 (int x, int y, mpic_t *pic, int src_x, int src_y, int src_width, int src_height, float scale_x, float scale_y, float alpha) {}
void Draw_TextBox (int x, int y, int width, int lines) {}
void Draw_CharacterW (int x, int y, wchar num) {}
void Draw_ColoredString3W (int x, int y, const wchar *text, clrinfo_t *clr, int clr_cnt, int red) {}
void Draw_SCharacter (int x, int y, int num, float scale) {}
void Draw_ConsoleBackground (int lines) {}
void Draw_StringW (int x, int y, const wchar *ws) {}
void Draw_AlphaSubPic (int x, int y, mpic_t *pic, int srcx, int srcy, int width, int height, float alpha) {}
void Draw_Alt_String (int x, int y, const char *str) {}
void Draw_STransPic (int x, int y, mpic_t *gl, float scale) {}
void Draw_SetOverallAlpha(float opacity) {}
void Draw_AlphaCircle (int x, int y, float radius, float thickness, qbool fill, byte c, float alpha) {}
void Draw_AlphaPieSlice (int x, int y, float radius, float startangle, float endangle, float thickness, qbool fill, byte c, float alpha) {}
void Draw_SPic (int x, int y, mpic_t *gl, float scale) {}
void Draw_SAlt_String (int x, int y, const char *text, float scale) {}
void Draw_AlphaLine (int x_start, int y_start, int x_end, int y_end, float thickness, byte c, float alpha) {}
void Draw_AlphaCircleOutline (int x, int y, float radius, float thickness, byte color, float alpha) {}
void Draw_AlphaCircleFill (int x, int y, float radius, byte color, float alpha) {}
void Draw_AlphaCircleOutlineRGB (int x, int y, float radius, float thickness, color_t color) {}
void Draw_AlphaCircleRGB (int x, int y, float radius, float thickness, qbool fill, color_t color) {}
void Draw_ColoredString3 (int x, int y, const char *text, clrinfo_t *clr, int clr_cnt, int red) {}
void Draw_Polygon(int x, int y, vec3_t *vertices, int num_vertices, qbool fill, color_t color) {}
void Draw_TransPic (int x, int y, mpic_t *pic) {}
void Draw_FadeScreen (float alpha) {}
void Draw_SubPic(int x, int y, mpic_t *pic, int srcx, int srcy, int width, int height) {}
void Draw_BigString (int x, int y, const char *text, clrinfo_t *color, int color_count, float scale, float alpha, int char_gap) {}
void Draw_FitPic (int x, int y, int fit_width, int fit_height, mpic_t *gl) {}

void R_Init (void) {}
void R_InitParticles (void) {}
void R_ParticleTrail (vec3_t start, vec3_t end, vec3_t *trail_origin, trail_type_t type) {}
void R_NewMap (qbool vid_restart) {}
void R_AddEfrags (entity_t *ent) {}
void R_TranslatePlayerSkin (int playernum) {}
void R_LavaSplash (vec3_t org) {}
void Classic_ParticleRailTrail (vec3_t start, vec3_t end, int color) {}
void R_RunParticleEffect (vec3_t org, vec3_t dir, int color, int count) {}
void R_ParticleExplosion (vec3_t org) {}
void R_BlobExplosion (vec3_t org) {}
void R_TeleportSplash (vec3_t org) {}
void R_RenderView (void) {}
qbool R_CharAvailable (wchar num) {return false;}
void R_MQW_NetGraph(int outgoing_sequence, int incoming_sequence, int *packet_latency,
                int lost, int minping, int avgping, int maxping, int devping,
				int posx, int posy, int width, int height, int revx, int revy) {}
