#pragma once

#include <vector>

//GL and GLM
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

//lua binding
#define SOL_CHECK_ARGUMENTS 1
#include <sol/sol.hpp>
#include <cassert>

//additional debugging and memory leak detection
//Memory leak detection
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <iostream>
#include <crtdbg.h> 

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#include <iostream>
#else
#include <stdexcpt.h>
#include <stdexcept>
#endif


#include "Utils.h"
#include "Configs.h"
