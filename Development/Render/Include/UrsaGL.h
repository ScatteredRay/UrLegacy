#ifndef _URSAGL_H_
#define _URSAGL_H_

#include "Core.h"
#include <gl/gl.h>
#include "glext.h"

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;

void initUrsaGL();

#endif //_URSAGL_H_
