#pragma once 

#include "glad/glad.h"

#ifdef SIMIENG_DLL_EXPORT
#define SIMIENG_API __declspec(dllexport)  // Building the DLL
#else
#define SIMIENG_API __declspec(dllimport)  // Using the DLL
#endif

namespace SimiEng {
	SIMIENG_API bool Init(GLADloadproc loadProc);
}
