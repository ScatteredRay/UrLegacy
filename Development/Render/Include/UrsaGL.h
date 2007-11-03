#ifndef _URSAGL_H_
#define _URSAGL_H_

#include <gl/gl.h>
#include "glext.h"

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;

void initUrsaGL();

#endif //_URSAGL_H_
