#pragma once

#include "GLCommon.h"

#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "cLightManager.h"
#include "cPhysics.h"

#include <PhysXWraper/cPhysXWraper.h>
#include <map>

extern cBasicFlyCamera* g_pFlyCamera;
extern cPhysics* g_pPhysicEngine;

extern cLightManager* g_pLightManager;
extern unsigned int g_selectedLightIndex;

// GLFW callback function signatures
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// Mouse entering and leaving the window
void cursor_enter_callback(GLFWwindow* window, int entered);
// Mouse button...
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// Mouse scroll wheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// Windows resize callback (for resizing FBO)
void window_size_callback(GLFWwindow* window, int width, int height);


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


// WE call these (not GLFW)
void handleKeyboardAsync(GLFWwindow* window);
void handleMouseAsync(GLFWwindow* window);

// Can call this from anything that has this header file
void DrawDebugSphere(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program);

extern bool g_bShowDebugSpheres;
extern bool g_bShowLightBulbs;

glm::vec3 g_rgb_from_HTML(unsigned int red, unsigned int green, unsigned int blue);

// HACK: Show "targeting LASER"
extern bool g_bShowLASERBeam;

std::string g_floatToString(float theFloat);
//glm::vec3 g_3StringsToVec3(std::string sX, std::string sY, std::string sZ);

float g_getRandomFloat(float a, float b);
glm::vec3 g_getRandom_vec3(glm::vec3 min, glm::vec3 max);
std::string g_getStringVec3(glm::vec3 theVec3);
// Returns NULL if NOT found
sMesh* g_pFindMeshByFriendlyName(std::string theNameToFind, bool bSearchChildrenToo = false);

bool isObjectACloserThanObjectB(sMesh* pA, sMesh* pB);

extern std::map<std::string, cPhysicsObjectTypes> map_cPhysicsObjectTypes;

extern float g_LODScale;
extern bool g_xray;