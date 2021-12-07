#pragma once

/*
template<typename T> void _delete(T &* p) { if(p) {delete(p); p = nullptr;} }
*/

#define _DELETE(p)			{ if(p) {delete(p); p = nullptr;} }
#define _DELETE_ARRAY(p)	{ if(p) {delete[](p); p = nullptr;} }
#define _CLOSE(p)			{ if(p) {(p)->Close(); delete(p); p = nullptr;} }
#define _RELEASE(p)			{ if(p) {(p)->Release(); p = nullptr;} }

// the current version of the framework
#define VERSION_FRAMEWORK 1

