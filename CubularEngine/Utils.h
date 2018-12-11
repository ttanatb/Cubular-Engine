#pragma once

#ifdef _DEBUG
#define DEBUG_PRINT(a, ...) printf("%s: %d(): " a "\n", __FILE__, __LINE__, __VA_ARGS__);
#else
#define DEBUG_PRINT(a, ...)
#endif

#define C_MESH_TANK "tankMesh"
#define C_MAT_TANK "tankMat"

#define C_SCRIPT_DIR "assets/scripts/"
#define C_SCRIPT_EXT ".lua"

#define C_SCRIPT_SCENE_1 "scene"
#define C_SCRIPT_SCENE_TABLE "Scene"
#define C_SCRIPT_SCENE_OBJ_PREFIX "obj"
