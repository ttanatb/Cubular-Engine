#pragma once

#include <vector>

//GL, Glew, GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

//GlM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

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
