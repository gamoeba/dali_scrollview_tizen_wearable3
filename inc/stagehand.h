#ifndef _STAGEHAND_H_
#define _STAGEHAND_H_

#define DEBUG_TAG "Stagehand"
// Note: STAGEHAND_PATH should end with path separator /
#define STAGEHAND_PATH "/opt/usr/home/owner/data/stagehand/"
#define STAGEHAND_ZIP "stagehand.zip"
#define STAGEHAND_PORT 27000

/**
 * This header file is included to define _EXPORT_.
 */
#include <stdbool.h>
#include <tizen.h>
#include <dlfcn.h>
#include <dlog/dlog.h>

typedef void* (*fn_prototype)(const char*, int);
class StageHand{
public:
	StageHand(){
		dlog_print(DLOG_INFO, DEBUG_TAG, "entry");
#ifdef __i386__
		dlog_print(DLOG_INFO, DEBUG_TAG, "load i386 library");
		void* hLibrary = dlopen(STAGEHAND_PATH"libstagehand_i386.so", RTLD_NOW | RTLD_GLOBAL);
#else
		dlog_print(DLOG_INFO, DEBUG_TAG, "load arm library");
		void* hLibrary = dlopen(STAGEHAND_PATH"libstagehand_armel.so", RTLD_NOW | RTLD_GLOBAL);
#endif
		dlog_print(DLOG_INFO, DEBUG_TAG, "library handle %08x", hLibrary);
		if (hLibrary){
			fn_prototype startstagehand;
			 *(void**)(&startstagehand) = dlsym(hLibrary,"startstagehand1");
			dlog_print(DLOG_INFO, DEBUG_TAG, "symbol address %08x", startstagehand);

			(void)startstagehand(STAGEHAND_PATH STAGEHAND_ZIP, STAGEHAND_PORT);
		} else {
			dlog_print(DLOG_WARN, DEBUG_TAG, "library not found, cannot start debug server");
		}
	}
};

#endif // _STAGEHAND_H_

