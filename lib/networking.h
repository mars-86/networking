#ifndef __LIB_NETWORKING_INCLUDED_H__
#define __LIB_NETWORKING_INCLUDED_H__

#include "sys/common.h"

#ifdef _WIN32
#if defined __MINGW32__ || defined __MINGW64__
#include "sys/win32/mingw/proto.h"
#else

#endif // __MINGW32__ || __MINGW64__
#else
 #include "sys/linux/proto.h"
#endif // _WIN32

#endif // __LIB_NETWORKING_INCLUDED_H__
