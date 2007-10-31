#ifndef _NAMES_H_
#define _NAMES_H_
#include "Name.h"

#define DECLARE_NAME(nam) static Name Name_##nam(#nam)

DECLARE_NAME(Texture);
DECLARE_NAME(Model);
DECLARE_NAME(Path);

#endif //_NAME_H_
