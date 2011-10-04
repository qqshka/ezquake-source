/*
   Copyright (C) 1997-2001 Id Software, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 */

// qcommon.h -- definitions common between client and server, but not game.dll

#ifndef __QCOMMON_H
#define __QCOMMON_H

#include "../gameshared/q_arch.h"
#include "../gameshared/q_math.h"
#include "../gameshared/q_shared.h"
#include "../gameshared/q_cvar.h"
//#include "../gameshared/q_dynvar.h"
#include "../gameshared/q_comref.h"
#include "../gameshared/q_collision.h"

#include "qfiles.h"
//#include "cmodel.h"
#include "version.h"

//#define	PARANOID			// speed sapping error checking

#define	DEFAULT_BASEGAME    "basewsw"

//============================================================================

struct mempool_s;

struct snapshot_s;

struct ginfo_s;
struct client_s;
struct cmodel_state_s;
struct client_entities_s;
struct fatvis_s;

//============================================================================

// some hax, because we want to save the file and line where the copy was called
// from, not the file and line from ZoneCopyString function
char *_ZoneCopyString( const char *in, const char *filename, int fileline );
#define ZoneCopyString( in ) _ZoneCopyString( in, __FILE__, __LINE__ )

/* patch.h */
void Patch_GetFlatness( float maxflat, const float *points, int comp, const int *patch_cp, int *flat );
void Patch_Evaluate( const vec_t *p, int *numcp, const int *tess, vec_t *dest, int comp );

/*
==============================================================

BSP FORMATS

==============================================================
*/

typedef void ( *modelLoader_t )( void *param0, void *param1, void *param2, void *param3 );

#define BSP_NONE		0
#define BSP_RAVEN		1
#define BSP_NOAREAS		2

typedef struct
{
	const char * const header;
	const int * const versions;
	const int lightmapWidth;
	const int lightmapHeight;
	const int flags;
	const int entityLumpNum;
} bspFormatDesc_t;

typedef struct
{
	const char * const header;
	const int headerLen;
	const bspFormatDesc_t * const bspFormats;
	const int maxLods;
	const modelLoader_t loader;
} modelFormatDescr_t;

extern const bspFormatDesc_t q1BSPFormats[];
extern const bspFormatDesc_t q2BSPFormats[];
extern const bspFormatDesc_t q3BSPFormats[];

const bspFormatDesc_t *Com_FindBSPFormat( const bspFormatDesc_t *formats, const char *header, int version );
const modelFormatDescr_t *Com_FindFormatDescriptor( const modelFormatDescr_t *formats, const qbyte *buf, const bspFormatDesc_t **bspFormat );

/*
   ==============================================================

   CVAR

   ==============================================================
 */

#include "cvar.h"

/*
   ==========================================================

   DYNVAR

   ==========================================================
 */

//#include "dynvar.h"

/*
==============================================================

FILESYSTEM

==============================================================
*/

#define FS_NOTIFT_NEWPAKS	0x01

//void	    FS_Init( void );
//int			FS_Rescan( void );
//void	    FS_Frame( void );
//void	    FS_Shutdown( void );

const char *FS_GameDirectory( void );
//const char *FS_BaseGameDirectory( void );
//qboolean    FS_SetGameDirectory( const char *dir, qboolean force );
//int			FS_GetGameDirectoryList( char *buf, size_t bufsize );
//int			FS_GetExplicitPurePakList( char ***paknames );

// handling of absolute filenames
// only to be used if necessary (library not supporting custom file handling functions etc.)
const char *FS_WriteDirectory( void );
void	    FS_CreateAbsolutePath( const char *path );
//const char *FS_AbsoluteNameForFile( const char *filename );
//const char *FS_AbsoluteNameForBaseFile( const char *filename );

// // game and base files
// file streaming
int	    FS_FOpenFile( const char *filename, int *filenum, int mode );
//int	    FS_FOpenBaseFile( const char *filename, int *filenum, int mode );
int		FS_FOpenAbsoluteFile( const char *filename, int *filenum, int mode );
void	FS_FCloseFile( int file );

int	    FS_Read( void *buffer, size_t len, int file );
int	    FS_Print( int file, const char *msg );
//int	    FS_Printf( int file, const char *format, ... );
int	    FS_Write( const void *buffer, size_t len, int file );
int	    FS_Tell( int file );
int	    FS_Seek( int file, int offset, int whence );
int	    FS_Eof( int file );
//int	    FS_Flush( int file );

// file loading
int	    FS_LoadFileExt( const char *path, void **buffer, void *stack, size_t stackSize, const char *filename, int fileline );
//int	    FS_LoadBaseFileExt( const char *path, void **buffer, void *stack, size_t stackSize, const char *filename, int fileline );
void	FS_FreeFile( void *buffer );
//void	FS_FreeBaseFile( void *buffer );
#define FS_LoadFile(path,buffer,stack,stacksize) FS_LoadFileExt(path,buffer,stack,stacksize,__FILE__,__LINE__)
//#define FS_LoadBaseFile(path,buffer,stack,stacksize) FS_LoadBaseFileExt(path,buffer,stack,stacksize,__FILE__,__LINE__)
//
//int		FS_GetNotifications( void );
//int		FS_RemoveNotifications( int bitmask );

// util functions
//qboolean    FS_CopyFile( const char *src, const char *dst );
//qboolean    FS_CopyBaseFile( const char *src, const char *dst );
//qboolean    FS_MoveFile( const char *src, const char *dst );
//qboolean    FS_MoveBaseFile( const char *src, const char *dst );
//qboolean    FS_RemoveFile( const char *filename );
//qboolean    FS_RemoveBaseFile( const char *filename );
//qboolean    FS_RemoveAbsoluteFile( const char *filename );
//qboolean    FS_RemoveDirectory( const char *dirname );
//qboolean    FS_RemoveBaseDirectory( const char *dirname );
//qboolean    FS_RemoveAbsoluteDirectory( const char *dirname );
//unsigned    FS_ChecksumAbsoluteFile( const char *filename );
//unsigned    FS_ChecksumBaseFile( const char *filename );
//qboolean	FS_CheckPakExtension( const char *filename );

// // only for game files
const char *FS_FirstExtension( const char *filename, const char *extensions[], int num_extensions );
//const char *FS_PakNameForFile( const char *filename );
//qboolean    FS_IsPureFile( const char *pakname );
//const char *FS_FileManifest( const char *filename );
//const char *FS_BaseNameForFile( const char *filename );
//
int	    FS_GetFileList( const char *dir, const char *extension, char *buf, size_t bufsize, int start, int end );
int	    FS_GetFileListExt( const char *dir, const char *extension, char *buf, size_t *bufsize, int start, int end );

// // only for base files
//qboolean    FS_IsPakValid( const char *filename, unsigned *checksum );
//qboolean    FS_AddPurePak( unsigned checksum );
//void	    FS_RemovePurePaks( void );


/*
   ==============================================================

   MISC

   ==============================================================
 */


#define	ERR_FATAL	0       // exit the entire game with a popup window
#define	ERR_DROP	1       // print to console and disconnect from game

#define MAX_PRINTMSG	3072

void	    Com_BeginRedirect( int target, char *buffer, int buffersize, void ( *flush ), const void *extra );
void	    Com_EndRedirect( void );
void	    Com_Printf( const char *format, ... );
void	    Com_DPrintf( const char *format, ... );
void	    Com_Error( int code, const char *format, ... );
void	    Com_Quit( void );

int	    Com_ClientState( void );        // this should have just been a cvar...
void	    Com_SetClientState( int state );

qboolean    Com_DemoPlaying( void );
void	    Com_SetDemoPlaying( qboolean state );

int	    Com_ServerState( void );        // this should have just been a cvar...
void	    Com_SetServerState( int state );

unsigned    Com_BlockChecksum( void *buffer, size_t length );
qbyte	    COM_BlockSequenceCRCByte( qbyte *base, size_t length, int sequence );

void	    Com_PageInMemory( qbyte *buffer, int size );

unsigned int Com_HashKey( const char *name, int hashsize );

unsigned int Com_DaysSince1900( void );

extern cvar_t *developer;
extern cvar_t *dedicated;
extern cvar_t *host_speeds;
extern cvar_t *log_stats;
extern cvar_t *versioncvar;
extern cvar_t *revisioncvar;

extern int log_stats_file;

// host_speeds times
extern unsigned int time_before_game;
extern unsigned int time_after_game;
extern unsigned int time_before_ref;
extern unsigned int time_after_ref;


//#define MEMCLUMPING
//#define MEMTRASH

#define POOLNAMESIZE 128

#ifdef MEMCLUMPING

// give malloc padding so we can't waste most of a page at the end
#define MEMCLUMPSIZE ( 65536 - 1536 )

// smallest unit we care about is this many bytes
#define MEMUNIT 8
#define MEMBITS ( MEMCLUMPSIZE / MEMUNIT )
#define MEMBITINTS ( MEMBITS / 32 )
#define MEMCLUMP_SENTINEL 0xABADCAFE

#endif

#define MEMHEADER_SENTINEL1 0xDEADF00D
#define MEMHEADER_SENTINEL2 0xDF

typedef struct memheader_s
{
        // next and previous memheaders in chain belonging to pool
        struct memheader_s *next;
        struct memheader_s *prev;

        // pool this memheader belongs to
        struct mempool_s *pool;

#ifdef MEMCLUMPING
        // clump this memheader lives in, NULL if not in a clump
        struct memclump_s *clump;
#endif

        // size of the memory after the header (excluding header and sentinel2)
        size_t size;

        // file name and line where Mem_Alloc was called
        const char *filename;
        int fileline;

        // should always be MEMHEADER_SENTINEL1
        unsigned int sentinel1;
        // immediately followed by data, which is followed by a MEMHEADER_SENTINEL2 byte
} memheader_t;

#ifdef MEMCLUMPING

typedef struct memclump_s
{
        // contents of the clump
        qbyte block[MEMCLUMPSIZE];

        // should always be MEMCLUMP_SENTINEL
        unsigned int sentinel1;

        // if a bit is on, it means that the MEMUNIT bytes it represents are
        // allocated, otherwise free
        int bits[MEMBITINTS];

        // should always be MEMCLUMP_SENTINEL
        unsigned int sentinel2;

        // if this drops to 0, the clump is freed
        int blocksinuse;

        // largest block of memory available (this is reset to an optimistic
        // number when anything is freed, and updated when alloc fails the clump)
        int largestavailable;

        // next clump in the chain
        struct memclump_s *chain;
} memclump_t;

#endif

#define MEMPOOL_TEMPORARY	1
#define MEMPOOL_GAMEPROGS	2
#define MEMPOOL_USERINTERFACE	4
#define MEMPOOL_CLIENTGAME	8
#define MEMPOOL_SOUND		16
#define MEMPOOL_DB        32
#define MEMPOOL_ANGELSCRIPT		64

typedef struct mempool_s
{
        // should always be MEMHEADER_SENTINEL1
        unsigned int sentinel1;

        // chain of individual memory allocations
        struct memheader_s *chain;

#ifdef MEMCLUMPING
        // chain of clumps (if any)
        struct memclump_s *clumpchain;
#endif

        // temporary, etc
        int flags;

        // total memory allocated in this pool (inside memheaders)
        int totalsize;

        // total memory allocated in this pool (actual malloc total)
        int realsize;

        // updated each time the pool is displayed by memlist, shows change from previous time (unless pool was freed)
        int lastchecksize;

        // name of the pool
        char name[POOLNAMESIZE];

        // linked into global mempool list or parent's children list
        struct mempool_s *next;

        struct mempool_s *parent;
        struct mempool_s *child;

        // file name and line where Mem_AllocPool was called
        const char *filename;

        int fileline;

        // should always be MEMHEADER_SENTINEL1
        unsigned int sentinel2;
} mempool_t;

void Memory_Init( void );
void Memory_InitCommands( void );
void Memory_Shutdown( void );
void Memory_ShutdownCommands( void );

void *_Mem_AllocExt( mempool_t *pool, size_t size, int z, int musthave, int canthave, const char *filename, int fileline );
void *_Mem_Alloc( mempool_t *pool, size_t size, int musthave, int canthave, const char *filename, int fileline );
void *_Mem_Realloc( void *data, size_t size, const char *filename, int fileline );
void _Mem_Free( void *data, int musthave, int canthave, const char *filename, int fileline );
mempool_t *_Mem_AllocPool( mempool_t *parent, const char *name, int flags, const char *filename, int fileline );
mempool_t *_Mem_AllocTempPool( const char *name, const char *filename, int fileline );
void _Mem_FreePool( mempool_t **pool, int musthave, int canthave, const char *filename, int fileline );
void _Mem_EmptyPool( mempool_t *pool, int musthave, int canthave, const char *filename, int fileline );

void _Mem_CheckSentinels( void *data, const char *filename, int fileline );
void _Mem_CheckSentinelsGlobal( const char *filename, int fileline );

#define Mem_AllocExt( pool, size, z ) _Mem_AllocExt( pool, size, z, 0, 0, __FILE__, __LINE__ )
#define Mem_Alloc( pool, size ) _Mem_Alloc( pool, size, 0, 0, __FILE__, __LINE__ )
#define Mem_Realloc( data, size ) _Mem_Realloc( data, size, __FILE__, __LINE__ )
#define Mem_Free( mem ) _Mem_Free( mem, 0, 0, __FILE__, __LINE__ )
#define Mem_AllocPool( parent, name ) _Mem_AllocPool( parent, name, 0, __FILE__, __LINE__ )
#define Mem_AllocTempPool( name ) _Mem_AllocTempPool( name, __FILE__, __LINE__ )
#define Mem_FreePool( pool ) _Mem_FreePool( pool, 0, 0, __FILE__, __LINE__ )
#define Mem_EmptyPool( pool ) _Mem_EmptyPool( pool, 0, 0, __FILE__, __LINE__ )

#define Mem_CheckSentinels( data ) _Mem_CheckSentinels( data, __FILE__, __LINE__ )
#define Mem_CheckSentinelsGlobal() _Mem_CheckSentinelsGlobal( __FILE__, __LINE__ )

// used for temporary allocations
extern mempool_t *tempMemPool;
extern mempool_t *zoneMemPool;

#define Mem_ZoneMallocExt( size, z ) Mem_AllocExt( zoneMemPool, size, z )
#define Mem_ZoneMalloc( size ) Mem_Alloc( zoneMemPool, size )
#define Mem_ZoneFree( data ) Mem_Free( data )

#define Mem_TempMallocExt( size, z ) Mem_AllocExt( tempMemPool, size, z )
#define Mem_TempMalloc( size ) Mem_Alloc( tempMemPool, size )
#define Mem_TempFree( data ) Mem_Free( data )

void *Q_malloc( size_t size );
void *Q_realloc( void *buf, size_t newsize );
void Q_free( void *buf );

/*
==============================================================

CPU FEATURES

==============================================================
*/

#define QCPU_HAS_RDTSC		0x00000001
#define QCPU_HAS_MMX		0x00000002
#define QCPU_HAS_MMXEXT		0x00000004
#define QCPU_HAS_3DNOW		0x00000010
#define QCPU_HAS_3DNOWEXT	0x00000020
#define QCPU_HAS_SSE		0x00000040
#define QCPU_HAS_SSE2		0x00000080

unsigned int COM_CPUFeatures( void );

/*
   ==============================================================

   NON-PORTABLE SYSTEM SERVICES

   ==============================================================
 */

unsigned int	Sys_Milliseconds( void );

//===========================================================================

/*

   Command execution takes a null terminated string, breaks it into tokens,
   then searches for a command or variable that matches the first token.

 */

typedef void ( *xcommand_t )( void );
typedef char ** ( *xcompletionf_t )( const char *partial );

void	    Cmd_AddCommand( const char *cmd_name, xcommand_t function );
void	    Cmd_RemoveCommand( const char *cmd_name );

int			Cmd_Argc( void );
char		*Cmd_Argv( int arg );
char		*Cmd_Args( void );
void	    Cmd_TokenizeString( const char *text );

#endif // __QCOMMON_H
