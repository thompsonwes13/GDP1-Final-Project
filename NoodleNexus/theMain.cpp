//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "GLCommon.h"


//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <iostream>     // "input output" stream
#include <fstream>      // "file" stream
#include <sstream>      // "string" stream ("string builder" in Java c#, etc.)
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

//void ReadPlyModelFromFile(std::string plyFileName);
#include "PlyFileLoaders.h"
#include "Basic_Shader_Manager/cShaderManager.h"
#include "sMesh.h"
#include "cVAOManager/cVAOManager.h"
#include "sharedThings.h"       // Fly camera
#include "cPhysics.h"
#include "cLightManager.h"
#include <windows.h>    // Includes ALL of windows... MessageBox
#include "cLightHelper/cLightHelper.h"
//
#include "cBasicTextureManager/cBasicTextureManager.h"

#include "cLowPassFilter.h"

#include "cTerrainPathChooser.h"

// Frame Buffer Object (FBO)
//#include "cFBO/cFBO_RGB_depth.h"
// Deferred render FBO
#include "cFBO/cFBO_deferred.h"

// Shadow map FBO
#include "cFBO/cFBO_Depth_Only.h"

#include "cViperFlagConnector.h"

#include "PhysXWraper/cPhysXWraper.h"

//#include "cParticleEmitter.h"
#include "cParticleEmitter_2.h"

double deltaTime;

bool g_useThreads = true;

// Deferred rendering Geometry "G" buffer
cFBO_deferred* g_pFBO_G_Buffer = NULL;

std::vector<sMesh*> g_vecMeshesToDraw;

cPhysics* g_pPhysicEngine = NULL;
cPhysXWraper* g_pPhysX = NULL;

// This loads the 3D models for drawing, etc.
cVAOManager* g_pMeshManager = NULL;

cBasicTextureManager* g_pTextures = NULL;

cTerrainPathChooser* g_pTerrainPathChooser = NULL;

extern cViperFlagConnector* g_pViperFlagConnector;

cParticleEmitter_2* g_pParticles = NULL;

unsigned int g_numLODTrisDrawnThisFrame = 0;

struct sParticleThreadData
{
    int bottomIndex;
    int topIndex;

    // Lets us know the thread is still working
    // (can't take any more work)
    bool bThreadIsBusy = false;
    // This controls when the thread wakes up
    bool bThreadIsAwake = true;
    DWORD threadSleepTime = 50;
    // This will "kill" the thread (i.e. make it exit)
    bool bKillThread = false;

    int random_seed;
};

std::vector<sParticleThreadData*> g_vecThreadData;

CRITICAL_SECTION cs_particleThread;

// our thread function, with this specific signature
DWORD WINAPI threadFunction_MoveParticles(LPVOID lpParameter)
{
    // Cast it back to the type we know it is...
    sParticleThreadData* pThreadData = (sParticleThreadData*)lpParameter;

    int spawnCount = 0;

    while (!pThreadData->bKillThread)
    {
        if (pThreadData->bThreadIsAwake)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() + (pThreadData->random_seed * 0.01);
            std::minstd_rand0 generator(seed);
            //unsigned int direction = generator() % 4;

            // Every tick, create X number of particles and add to the vector
            //if (spawnCount >= 15)
            //{
            //    g_pParticles->m_CreateParticles(g_pParticles->GetNumberOfParticlesPerFrame(), seed);
            //    spawnCount = 0;
            //}

            for (unsigned int index = pThreadData->bottomIndex; index != pThreadData->topIndex; index++)
            {
                cParticle* pTestParticle = &(g_pParticles->m_pParticles[index]);

                float discard = 0;
                std::ostringstream discardStream;

                for (int i = 0; i < 3; i++)
                {
                    discardStream << discard;
                    discard = std::exp(25);
                    discard = std::pow(12, 53);
                    discard = std::log(26);
                    discard = std::atan2(16, 26);
                    //discard = std::tan(67);
                    //discard = std::fmod(15, 200);
                    //discard = std::sqrt(660);
                    discardStream << discard;
                }

                if (pTestParticle->timeTillDeath > 0.0f)
                {
                    pTestParticle->timeTillDeath -= (float)deltaTime;

                    // Euler integration
                    pTestParticle->velocity += pTestParticle->accel * (float)deltaTime;

                    pTestParticle->position += pTestParticle->velocity * (float)deltaTime;
                }
            }//for (unsigned int index

            pThreadData->bThreadIsAwake = false;
        }
        else
        {
            // Wait
            Sleep(pThreadData->threadSleepTime);
            pThreadData->bThreadIsAwake = true;
            spawnCount++;
        }
    }

    return 0;
}

void CreateThreads(int num_threads);

//cLightManager* g_pLightManager = NULL;

void AddModelsToScene(cVAOManager* pMeshManager, GLuint shaderProgram);

//void DrawMesh(sMesh* pCurMesh, GLuint program, bool SetTexturesFromMeshInfo = true);
// Now we pass the original (parent) matrix.
// We can also pass this matrix instead of the position, orientation, etc.
void DrawMesh(sMesh* pCurMesh, glm::mat4 matModel, GLuint program, bool SetTexturesFromMeshInfo = true);

//glm::vec3 cameraEye = glm::vec3(0.0, 0.0, 4.0f);

void AddToPhysXMap(std::string name, unsigned int index);

void RenderScene(
    GLuint program,
    glm::mat4 matProjection,
    glm::mat4 matView,
    float ratio,
    glm::vec3 eyeLocation);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

bool isControlDown(GLFWwindow* window);

// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float g_getRandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

glm::vec3 g_getRandom_vec3(glm::vec3 min, glm::vec3 max)
{
    return glm::vec3(
        ::g_getRandomFloat(min.x, max.x),
        ::g_getRandomFloat(min.y, max.y),
        ::g_getRandomFloat(min.z, max.z));
}

std::string g_getStringVec3(glm::vec3 theVec3)
{
    std::stringstream ssVec;
    ssVec << "(" << theVec3.x << ", " << theVec3.y << ", " << theVec3.z << ")";
    return ssVec.str();
}

sMesh* g_pFindChildMeshByFriendlyName(sMesh* pParent, std::string theNameToFind)
{
    for (unsigned int index = 0; index != pParent->vec_pChildMeshes.size(); index++)
    {
        if (pParent->vec_pChildMeshes[index]->uniqueFriendlyName == theNameToFind)
        {
            return pParent->vec_pChildMeshes[index];
        }
        sMesh* pChildFound = g_pFindChildMeshByFriendlyName(pParent->vec_pChildMeshes[index], theNameToFind);
        if (pChildFound)
        {
            return pChildFound;
        }
    }
    return NULL;
}

// Returns NULL if NOT found
sMesh* g_pFindMeshByFriendlyName(std::string theNameToFind, bool bSearchChildrenToo)
{
    for (unsigned int index = 0; index != ::g_vecMeshesToDraw.size(); index++)
    {
        if (::g_vecMeshesToDraw[index]->uniqueFriendlyName == theNameToFind)
        {
            return ::g_vecMeshesToDraw[index];
        }
        sMesh* pChildFound = g_pFindChildMeshByFriendlyName(::g_vecMeshesToDraw[index], theNameToFind);
        if (pChildFound)
        {
            return pChildFound;
        }
    }
    // Didn't find it
    return NULL;
}

void SaveMeshes();
void LoadMeshes();
void SaveLights();
void LoadLights();
void SetCameraPosition();
void ModelsToVAO(const GLuint program);
void DrawMeshes();
void LoadTextures();


int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Spidey Gets the Scooby Snacks", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Callback for keyboard, but for "typing"
    // Like it captures the press and release and repeat
    glfwSetKeyCallback(window, key_callback);

    // 
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowFocusCallback(window, cursor_enter_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // Resets the FBO when we change window size
    // https://www.glfw.org/docs/3.3/window_guide.html#window_events
    glfwSetWindowSizeCallback(window, window_size_callback);



    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);


// **********************************************************
//    ____  _               _           
//   / ___|| |__   __ _  __| | ___ _ __ 
//   \___ \| '_ \ / _` |/ _` |/ _ \ '__|
//    ___) | | | | (_| | (_| |  __/ |   
//   |____/|_| |_|\__,_|\__,_|\___|_|   
//                                      

    cShaderManager* pShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "assets/shaders/vertex01.glsl";

    cShaderManager::cShader geometryShader;
    geometryShader.fileName = "assets/shaders/geom_pass_through.glsl";
//    geometryShader.fileName = "assets/shaders/geom_split_triangle.glsl";
//    geometryShader.fileName = "assets/shaders/geom_DrawNormal_lines.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "assets/shaders/fragment01.glsl";

    //if ( ! pShaderManager->createProgramFromFile("shader01",
    //                                             vertexShader, fragmentShader))
    if ( ! pShaderManager->createProgramFromFile("shader01",
                                                 vertexShader, geometryShader, fragmentShader))
    {
        std::cout << "Error shader: " << pShaderManager->getLastError() << std::endl;
    }
    else
    {
        std::cout << "Shader built OK" << std::endl;
    }

    GLuint program = pShaderManager->getIDFromFriendlyName("shader01");

    glUseProgram(program);

    
    cShaderManager::cShaderProgram* pShaderProgram
        = pShaderManager->pGetShaderProgramFromFriendlyName("shader01");

// **********************************************************


    std::string strUniformSummary = pShaderProgram->getActiveUniformSummary();

//    cVAOManager* pMeshManager = new cVAOManager();
    ::g_pMeshManager = new cVAOManager();


    // Traversing the path
    ::g_pTerrainPathChooser = new cTerrainPathChooser(::g_pMeshManager);
    // Set the terrain, etc. 
    // HACK:
    ::g_pTerrainPathChooser->setTerrainMesh(
        "assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply", 
        glm::vec3(0.0f, -175.0f, 0.0f));        // Offset of mesh
    //
    ::g_pTerrainPathChooser->startXYZ = glm::vec3(-500.0f, -75.0f, -500.0f);
    ::g_pTerrainPathChooser->destinationXYZ = glm::vec3(+500.0f, -75.0f, +500.0f);

    ::g_pPhysicEngine = new cPhysics();
    // For triangle meshes, let the physics object "know" about the VAO manager
    ::g_pPhysicEngine->setVAOManager(::g_pMeshManager);


    // Start up the PhysX middleware...
    ::g_pPhysX = new cPhysXWraper();
    ::g_pPhysX->initPhysics(true);

    AddToPhysXMap("Player", 0);

    ModelsToVAO(program);
    AddModelsToScene(::g_pMeshManager, program);
     
    ::g_pFlyCamera = new cBasicFlyCamera();

    SetCameraPosition();

    glUseProgram(program);

    // Enable depth buffering (z buffering)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnable.xhtml
    glEnable(GL_DEPTH_TEST);

    cLowPassFilter frameTimeFilter;         // clamped to 60 Hz or something
    cLowPassFilter frameTimeFilterACTUAL;
//    frameTimeFilter.setNumSamples(30000);

    double currentFrameTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();


    // Set up the lights
    ::g_pLightManager = new cLightManager();
    // Called only once
    ::g_pLightManager->loadUniformLocations(program);

    // Set up one of the lights in the scene
    ::g_pLightManager->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up one of the lights in the scene
    ::g_pLightManager->theLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    ::g_pLightManager->theLights[1].direction = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_pLightManager->theLights[1].param1.y = 26.2f;   //  y = inner angle
    ::g_pLightManager->theLights[1].param1.z = 35.4f;  //  z = outer angle

    // Set up one of the lights in the scene
    ::g_pLightManager->theLights[2].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up one of the lights in the scene
    ::g_pLightManager->theLights[3].diffuse = glm::vec4(150.0f / 255.0f, 40.0f / 255.0f, 0.0f / 255.0f, 1.0f);
    ::g_pLightManager->theLights[3].specular = glm::vec4(200.0f / 255.0f, 100.0f / 255.0f, 50.0f / 255.0f, 1.0f);

    // Set up one of the lights in the scene
    ::g_pLightManager->theLights[4].diffuse = glm::vec4(150.0f / 255.0f, 40.0f / 255.0f, 0.0f / 255.0f, 1.0f);
    ::g_pLightManager->theLights[4].specular = glm::vec4(200.0f / 255.0f, 100.0f / 255.0f, 50.0f / 255.0f, 1.0f);

    LoadLights();

    // Add a shadowmap texture
    ::g_pLightManager->theLights[1].pShadowMap = new cFBO_Depth_Only();
    // We'll pick a decent range here
    std::string FBOShadowErrorString = "";
    if (::g_pLightManager->theLights[1].pShadowMap->init(2048, 2048, FBOShadowErrorString))
    {
        //std::cout << "Light #1 shadowmap created OK" << std::endl;
    }
    else
    {
        std::cout << "Can't create light shadowmap because: " << FBOShadowErrorString << std::endl;
    }

    ::g_pTextures = new cBasicTextureManager();

    //::g_pTextures->SetBasePath("assets/textures");

    std::cout << "Loading textures...";
    LoadTextures();


    GLint iMaxCombinedTextureInmageUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &iMaxCombinedTextureInmageUnits);
    //std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << iMaxCombinedTextureInmageUnits << std::endl;

    // data returns one value, the maximum number of components of the inputs read by the fragment shader, 
    // which must be at least 128.
    GLint iMaxFragmentInputComponents = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &iMaxFragmentInputComponents);
    //std::cout << "GL_MAX_FRAGMENT_INPUT_COMPONENTS = " << iMaxFragmentInputComponents << std::endl;
    

    // data returns one value, the maximum number of individual floating - point, integer, or boolean values 
    // that can be held in uniform variable storage for a fragment shader.The value must be at least 1024. 
    GLint iMaxFragmentUniformComponents = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &iMaxFragmentUniformComponents);
    //std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS = " << iMaxFragmentUniformComponents << std::endl;
        

    //  Turn on the blend operation
    glEnable(GL_BLEND);
    // Do alpha channel transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Is the default (cull back facing polygons)
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // HACK:
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;



    // Deferred rendering Geometry "G" buffer
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);

    ::g_pFBO_G_Buffer = new cFBO_deferred();
    std::string FBOinitError;
    if ( ! ::g_pFBO_G_Buffer->init(screen_width, screen_height, FBOinitError) )
    {
        std::cout << "ERROR: Can't init deferred FBO buffer because: "
            << FBOinitError << std::endl;
    }
    else
    {
        std::cout << "Deferred FBO init() OK." << std::endl;
    }
 
    ::g_pParticles = new cParticleEmitter_2();
    ::g_pParticles->SetMaximumNumberOfParticles(10'000);

    ::g_pParticles->SetSourceLocation(glm::vec3(13.75f, 0.4f, 0.1f));
    ::g_pParticles->SetInitalVelocity(
            glm::vec3(-2.0f, 3.0f, -2.0f),      // Min
            glm::vec3(2.0f, 6.0f, 2.0f));       // Max
    ::g_pParticles->SetNumberOfParticlesPerFrame(1);
    ::g_pParticles->TurnOn();

    if (MessageBox(NULL, L"Would you like to run threads for the particles?", L"Thread?", MB_YESNO) == IDYES)
    {
        g_useThreads = true;
    }
    else
    {
        g_useThreads = false;
    }

    if (g_useThreads)
    {
        CreateThreads(10);
    }

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        ratio = width / (float)height;

        // Clear the LOD debug info
        ::g_numLODTrisDrawnThisFrame = 0;

        // Calculate elapsed time
        // We'll enhance this
        currentFrameTime = glfwGetTime();
        double tempDeltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // save the ACTUAL frame time (even if it sucks)
        frameTimeFilterACTUAL.addSample(tempDeltaTime);

        // Set a limit on the maximum frame time
        const double MAX_FRAME_TIME = 1.0 / 60.0;   // 60Hz (16 ms)
        if (tempDeltaTime > MAX_FRAME_TIME)
        {
            tempDeltaTime = MAX_FRAME_TIME;
        }

        // Add this sample to the low pass filer ("averager")
        frameTimeFilter.addSample(tempDeltaTime);
        // 
        deltaTime = frameTimeFilter.getAverage();

        // Physic update and test 
        // (The phsyics "engine" is NOT updating the Verlet (soft bodies) 
        //  in this method...)
        ::g_pPhysicEngine->StepTick(deltaTime);


        // THREADED NOW
        //         
        //  Update this from THE LAST FRAME
        // 
        // Update the meshes in the VAO to match any soft bodies
        // MUST keep this on the OpenGL thread that "has context"
        // (i.e. the one that we created the GL window on)
        // (i.e. we are NOT going to move this to another thread)
        ::g_pPhysicEngine->updateSoftBodyMeshes(program);


        // Update the mesh information from the LAST frame:
        // (i.e. from the thread we kicked off last frame)

        // Move the flag to where the viper is
        if (::g_pViperFlagConnector)
        {
            ::g_pViperFlagConnector->UpdateFlagLocation();
        }


        // This might be better inside the StepTick(),
        //  but I'm leaving it here for clarification
        //  or if you DON'T want any soft bodies
        ::g_pPhysicEngine->updateSoftBodies(deltaTime);

        glm::vec3 camPosition;// = g_pFlyCamera->getEyeLocation();
        glm::vec3 target;

        // Update PhysX...
        ::g_pPhysX->update();

        sMesh* SpiderMan = g_pFindMeshByFriendlyName("legospiderman_Hips");

        // Rotate mesh to fit camera
        float angle = atan2(glm::normalize(g_pFlyCamera->getTargetRelativeToCamera()).x, glm::normalize(g_pFlyCamera->getTargetRelativeToCamera()).z);
        SpiderMan->rotationEulerXYZ.z = glm::degrees(angle) - 90;
        
        // Update camera to orbit player
        g_pPhysX->updateCameraPosition(g_pFlyCamera->getYaw(), g_pFlyCamera->getPitch(), camPosition, target);

        g_pFlyCamera->setEyeLocation(camPosition);
        g_pFlyCamera->setTarget(target);

        if (g_pParticles->bIsOn() && ::g_pParticles->GetNumberOfLiveParticles() < 75)
        {
            g_pParticles->m_CreateParticles(g_pParticles->GetNumberOfParticlesPerFrame());
        }

        if (!g_useThreads)
        {
            ::g_pParticles->Update(deltaTime);
        }

        sMesh* Scooby = g_pFindMeshByFriendlyName("Scooby");

        if (Scooby)
        {
            if (Scooby->rotationEulerXYZ.y < 360)
            {
                Scooby->rotationEulerXYZ.y += 1.0f;
            }
            else
            {
                Scooby->rotationEulerXYZ.y = 0;
            }

            if (glm::distance(Scooby->positionXYZ, SpiderMan->positionXYZ) < 2.0f)
            {
                Scooby->bIsVisible = false;
            }
        }

        //glm::vec3 playerPosition = g_pPhysX->getPlayerPosition();
        glm::vec3 playerPosition = g_pPhysX->getActorPosition("Player");

        SpiderMan->positionXYZ = playerPosition;
        SpiderMan->positionXYZ.y -= 1.0f;

        glm::mat4 matProjection = glm::mat4(1.0f);
        glm::mat4 matView = glm::mat4(1.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        // Pass #0 --> Render the G buffer
        //          output to the FBO deferred object
        // 
        // (Pass #1 --> optional effect pass, "2nd pass effect")
        //
        // Pass #2 --> Render the Deferred lighting pass 
        //          output to the screen onto a full screen quad
        //

        // uniform int renderPassNumber;
        GLint renderPassNumber_UL = glGetUniformLocation(program, "renderPassNumber");
        // Pass "0" (regular forward rendering)
        // 


        // **************************************************
        //     ____           ____         __  __                                 
        //    / ___|         | __ ) _   _ / _|/ _| ___ _ __   _ __   __ _ ___ ___ 
        //   | |  _   _____  |  _ \| | | | |_| |_ / _ \ '__| | '_ \ / _` / __/ __|
        //   | |_| | |_____| | |_) | |_| |  _|  _|  __/ |    | |_) | (_| \__ \__ \
        //    \____|         |____/ \__,_|_| |_|  \___|_|    | .__/ \__,_|___/___/
        //                                                   |_|                  
        // 
        //        // Pass "1" for deferred G buffer pass
        //        glUniform1i(renderPassNumber_UL, 1);
        //
        //        // Point the output to the G buffer...
        //        glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO_G_Buffer->ID);
        //
        //        // Clear the buffers on the FBO
        //        // (remember that glClear() only works on the regular screen buffer)
        //        ::g_pFBO_G_Buffer->clearBuffers(true, true);

        // Shadow map pass
        cFBO_Depth_Only* pShadowMap = ::g_pLightManager->theLights[1].pShadowMap;
        if (pShadowMap)
        {
            // Point output to the shadow map
            glBindFramebuffer(GL_FRAMEBUFFER, pShadowMap->ID);
            // Get the viewport from the FBO not the actual screen
            glViewport(0, 0, pShadowMap->width, pShadowMap->height);
            pShadowMap->clearDepthBuffer();

            //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //// These will ONLY work on the default framebuffer
            //glViewport(0, 0, width, height);
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::vec3 lightLocation = glm::vec3(::g_pLightManager->theLights[1].position);

            sMesh* pBarrel = ::g_pFindMeshByFriendlyName("Barrel");

            // Calculate the view from the light to the barrel
            // (Spot is looking at the barrel)
            matView = glm::lookAt(
                lightLocation,
                pBarrel->positionXYZ,
                upVector);

            GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
            glUniform4f(eyeLocation_UL,
                lightLocation.x,
                lightLocation.y,
                lightLocation.z, 1.0f);

            // Warehouse is: 167 x 60 x 72
            //
            // So greatest possible distance INSIDE warehouse is about 170.
            // We'll pick a far plane of 200

            matProjection = glm::perspective(0.6f,
                ratio,
                0.1f,       // Very close to the light
                200.0f);    // Mostly inside the warehouse


            RenderScene(program, matProjection, matView, ratio, lightLocation);


        }//if (pShadowMap)


        // Point framebuffer back to the screen...
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // These will ONLY work on the default framebuffer
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pass "0" for regular forward rendering
        glUniform1i(renderPassNumber_UL, 0);

        matView = glm::lookAt(
            ::g_pFlyCamera->getEyeLocation(),
            ::g_pFlyCamera->getTargetLocation(),
            upVector);

        GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
        glUniform4f(eyeLocation_UL, 
            ::g_pFlyCamera->getEyeLocation().x,
            ::g_pFlyCamera->getEyeLocation().y, 
            ::g_pFlyCamera->getEyeLocation().z, 1.0f);

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        matProjection = glm::perspective(0.6f,
            ratio,
            1.0f,
            50'000.0f);

        RenderScene(program, matProjection, matView, ratio, ::g_pFlyCamera->getEyeLocation());
 
        // **************************************************


        // **************************************************
        //    ____        __                        _   _ _       _     _   _                                   
        //   |  _ \  ___ / _| ___ _ __ _ __ ___  __| | | (_) __ _| |__ | |_(_)_ __   __ _   _ __   __ _ ___ ___ 
        //   | | | |/ _ \ |_ / _ \ '__| '__/ _ \/ _` | | | |/ _` | '_ \| __| | '_ \ / _` | | '_ \ / _` / __/ __|
        //   | |_| |  __/  _|  __/ |  | | |  __/ (_| | | | | (_| | | | | |_| | | | | (_| | | |_) | (_| \__ \__ \
        //   |____/ \___|_|  \___|_|  |_|  \___|\__,_| |_|_|\__, |_| |_|\__|_|_| |_|\__, | | .__/ \__,_|___/___/
        //                                                  |___/                   |___/  |_|                  

        // Point the output to the regular framebuffer (the screen)
        //        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //        // These will ONLY work on the default framebuffer
        //        glViewport(0, 0, width, height);
        //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //        // Pass "3" for deferred lighting pass
        //        glUniform1i(renderPassNumber_UL, 3);
        //
        //
        //        // We can render any object
        ////        sMesh* pFSQ = g_pFindMeshByFriendlyName("New_Viper_Player");
        //        sMesh* pFSQ = g_pFindMeshByFriendlyName("Full_Screen_Quad");
        //
        //
        //        // We are setting the camera (view) and projection matrix 
        //        //  specifically for this shot of the FSQ
        //
        //        // In our case, the quad is 2x2 in size, centred at the origin
        //        //  facing along the +ve z axis.
        //        // It goes from -1.0 to 1.0 on the x and y axes
        //
        //        pFSQ->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
        //        pFSQ->bIsVisible = true;
        //       // pFSQ->rotationEulerXYZ.y += 0.1f;
        //        // 
        //        // 
        //        // Set the camera 
        //        //
        //        // ...We could make the quad bigger or move closer
        //        //
        //        // The key is we want to the full screen quad to be "too" big,
        //        //  like it's completely filling the creen and going off the edges
        //        // (that way whatever resolution or window size, we'll be OK)
        //        //
        //        matView = glm::lookAt(
        //            glm::vec3(0.0f, 0.0f, +1.0f),  // +1 units along the z
        //            glm::vec3(0.0f, 0.0f, 0.0f),    // Looking at the origin 
        //            glm::vec3(0.0f, 1.0f, 0.0f));   // "up" is +ve Y
        //
        //        GLint matView_UL = glGetUniformLocation(program, "matView");
        //        glUniformMatrix4fv(matView_UL, 1, GL_FALSE, (const GLfloat*)&matView);
        //
        //        // glm::ortho(
        //        
        //        // Watch the near and far plane as we are REALLY close to the quad...
        //        matProjection = glm::perspective(0.6f,
        //            ratio,
        //            0.1f, 2.0f);       // FSQ is 10 units from the camera 
        //                                // (and it's a flat object)
        //
        //        GLint matProjection_UL = glGetUniformLocation(program, "matProjection");
        //        glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, (const GLfloat*)&matProjection);
        //
        //
        //
        //
        //        // Connect the FBO deferred textures to the textures in our final salighting pass
        //        //    uniform sampler2D vertexWorldLocationXYZ_texture;
        //        //    uniform sampler2D vertexNormalXYZ_texture;
        //        //    uniform sampler2D vertexDiffuseRGB_texture;
        //        //    uniform sampler2D vertexSpecularRGA_P_texture;
        //        //
        //        // Note here I'm picking texture unit numbers for not particular 
        //        //  reason. i.e. they don't have to be zero, or match the FBO, or 
        //        //  even in any order. They are independent of all that
        //        {
        //            glActiveTexture(GL_TEXTURE0 + 13);
        //            glBindTexture(GL_TEXTURE_2D, g_pFBO_G_Buffer->vertexWorldLocationXYZ);
        //            GLint vertexWorldLocationXYZ_texture_UL 
        //                = glGetUniformLocation(program, "vertexWorldLocationXYZ_texture");
        //            glUniform1i(vertexWorldLocationXYZ_texture_UL, 13);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
        //        }
        //        {
        //            glActiveTexture(GL_TEXTURE0 + 14);
        //            glBindTexture(GL_TEXTURE_2D, g_pFBO_G_Buffer->vertexNormalXYZ);
        //            GLint vertexNormalXYZ_texture_UL
        //                = glGetUniformLocation(program, "vertexNormalXYZ_texture");
        //            glUniform1i(vertexNormalXYZ_texture_UL, 14);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
        //        }
        //        {
        //            glActiveTexture(GL_TEXTURE0 + 15);
        //            glBindTexture(GL_TEXTURE_2D, g_pFBO_G_Buffer->vertexDiffuseRGB);
        //            GLint vertexDiffuseRGB_texture_UL
        //                = glGetUniformLocation(program, "vertexDiffuseRGB_texture");
        //            glUniform1i(vertexDiffuseRGB_texture_UL, 15);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
        //        }
        //        {
        //            glActiveTexture(GL_TEXTURE0 + 16);
        //            glBindTexture(GL_TEXTURE_2D, g_pFBO_G_Buffer->vertexSpecularRGA_P);
        //            GLint vertexSpecularRGA_P_texture_UL
        //                = glGetUniformLocation(program, "vertexSpecularRGA_P_texture");
        //            glUniform1i(vertexSpecularRGA_P_texture_UL, 16);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
        //        }
        //
        //        // Also pass the current screen size
        //        // uniform vec2 screenSize_width_height
        //        int screen_width, height_height;
        //        glfwGetFramebufferSize(window, &screen_width, &height_height);
        //
        //        GLint screenSize_width_height_UL
        //            = glGetUniformLocation(program, "screenSize_width_height");
        //
        //        glUniform2f(screenSize_width_height_UL, 
        //                     (GLfloat)screen_width, 
        //                     (GLfloat)height_height);
        //
        //
        //        glm::mat4 matModel = glm::mat4(1.0f);   // Identity
        //        DrawMesh(pFSQ, matModel, program, false);
        //
        //        // Hide the quad from rendering anywhere else
        //        pFSQ->bIsVisible = false;
        //
        //// **************************************************

        
        // Load any outstanding models async...
        ::g_pMeshManager->LoadAsynModels(program);

        // Handle async IO stuff
        handleKeyboardAsync(window);
        handleMouseAsync(window);

        glfwSwapBuffers(window);
        glfwPollEvents();


        //std::cout << "Camera: "
        std::stringstream ssTitle;
        ssTitle << "Get the Scooby Snacks!" << " | ";

        // Show frame time
        double ACTUAL_deltaTime = frameTimeFilterACTUAL.getAverage();
        if (ACTUAL_deltaTime < 1000)
        {
            ssTitle << " FPS: " << (int)(1.0 / ACTUAL_deltaTime);
        }

        glfwSetWindowTitle(window, ssTitle.str().c_str());


    }// End of the draw loop


    // Delete everything
    delete ::g_pFlyCamera;
    delete ::g_pPhysicEngine;
    ::g_pFBO_G_Buffer->shutdown();
    delete ::g_pFBO_G_Buffer;

    ::g_pPhysX->cleanupPhysics(true);
    delete ::g_pPhysX;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void SaveMeshes()
{
    // Save state to file

    if (MessageBox(NULL, L"Would you like to save the mesh locations?", L"SAVE", MB_YESNO) == IDYES)
    {
        std::ofstream meshInfo("meshInfo.txt");

        for (int i = 0; i < g_vecMeshesToDraw.size(); i++)
        {
            if (::g_vecMeshesToDraw[i] == g_pFindMeshByFriendlyName("Light_Sphere"))
            {
            }
            else if (::g_vecMeshesToDraw[i] == g_pFindMeshByFriendlyName("Debug_Sphere"))
            {
            }
            else
            {
                meshInfo << ::g_vecMeshesToDraw[i]->uniqueFriendlyName << "\n";

                meshInfo << "Position - ";
                meshInfo << "x: " << ::g_vecMeshesToDraw[i]->positionXYZ.x << " ";
                meshInfo << "y: " << ::g_vecMeshesToDraw[i]->positionXYZ.y << " ";
                meshInfo << "z: " << ::g_vecMeshesToDraw[i]->positionXYZ.z << "\n";

                meshInfo << "Rotation - ";
                meshInfo << "x: " << ::g_vecMeshesToDraw[i]->rotationEulerXYZ.x << " ";
                meshInfo << "y: " << ::g_vecMeshesToDraw[i]->rotationEulerXYZ.y << " ";
                meshInfo << "z: " << ::g_vecMeshesToDraw[i]->rotationEulerXYZ.z << "\n";

                meshInfo << "Scale - ";
                meshInfo << ::g_vecMeshesToDraw[i]->uniformScale << "\n\n";
            }
        }

        meshInfo.close();

        std::cout << "Saved Meshes!" << std::endl;
    }
    else
    {
        std::cout << "Cancelled Save" << std::endl;
    }

    return;
}

void LoadMeshes()
{
    // Load state to file
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox

    if (MessageBox(NULL, L"Would you like to load the mesh locations?", L"LOAD", MB_YESNO) == IDYES)
    {
        std::ifstream meshInfo("meshInfo.txt");

        for (int i = 0; i < g_vecMeshesToDraw.size(); i++)
        {
            std::string name;

            float xPos;
            float yPos;
            float zPos;

            float xRot;
            float yRot;
            float zRot;

            float scale;

            std::string discard;

            if (::g_vecMeshesToDraw[i] == g_pFindMeshByFriendlyName("Light_Sphere"))
            {
            }
            else if (::g_vecMeshesToDraw[i] == g_pFindMeshByFriendlyName("Debug_Sphere"))
            {
            }
            else
            {
                meshInfo >> name    // Mesh Name
                    >> discard    // "Position"
                    >> discard    // "-"
                    >> discard    // "x:"
                    >> xPos       // x val
                    >> discard    // "y:"
                    >> yPos       // y val
                    >> discard    // "z:"
                    >> zPos       // z val

                    >> discard    // "Rotation"
                    >> discard    // "-"
                    >> discard    // "x:"
                    >> xRot       // x val
                    >> discard    // "y:"
                    >> yRot       // y val
                    >> discard    // "z:"
                    >> zRot       // z val

                    >> discard    // "Scale"
                    >> discard    // "-"
                    >> scale;     // scale
                if (name != "SkySphere")
                {
                    ::g_vecMeshesToDraw[i]->positionXYZ.x = xPos;
                    ::g_vecMeshesToDraw[i]->positionXYZ.y = yPos;
                    ::g_vecMeshesToDraw[i]->positionXYZ.z = zPos;
                }

                ::g_vecMeshesToDraw[i]->rotationEulerXYZ.x = xRot;
                ::g_vecMeshesToDraw[i]->rotationEulerXYZ.y = yRot;
                ::g_vecMeshesToDraw[i]->rotationEulerXYZ.z = zRot;

                ::g_vecMeshesToDraw[i]->uniformScale = scale;
            }
        }

        std::cout << "Loaded Meshes!" << std::endl;
    }
    else
    {
        std::cout << "Cancelled Load" << std::endl;
    }

    return;
}

void SaveLights()
{
    if (MessageBox(NULL, L"Would you like to save the light locations?", L"SAVE", MB_YESNO) == IDYES)
    {
        std::ofstream lightInfo("lightInfo.txt");
        std::string lightType;
        std::string on_off;

        for (int i = 0; i < cLightManager::NUMBEROFLIGHTS; i++)
        {
            if (::g_pLightManager->theLights[i].param1.x == 0.0)
            {
                lightType = "Point";
            }
            else if (::g_pLightManager->theLights[i].param1.x == 1.0)
            {
                lightType = "Spot";
            }
            else if (::g_pLightManager->theLights[i].param1.x == 2.0)
            {
                lightType = "Directional";
            }

            if (::g_pLightManager->theLights[i].param2.x == 0.0)
            {
                on_off = "OFF";
            }
            else if (::g_pLightManager->theLights[i].param2.x == 1.0)
            {
                on_off = "ON";
            }

            lightInfo << "Light " << i + 1 << " " << lightType << " " << on_off << "\n";
            lightInfo << "x: " << ::g_pLightManager->theLights[i].position.x << " ";
            lightInfo << "y: " << ::g_pLightManager->theLights[i].position.y << " ";
            lightInfo << "z: " << ::g_pLightManager->theLights[i].position.z << "\n";
            lightInfo << "Linear: " << ::g_pLightManager->theLights[i].atten.y << " ";
            lightInfo << "Quadratic: " << ::g_pLightManager->theLights[i].atten.z << "\n\n";
        }

        lightInfo.close();

        std::cout << "Saved Lights!" << std::endl;
    }
    else
    {
        std::cout << "Cancelled Save" << std::endl;
    }

    return;
}

void LoadLights()
{
    //if (MessageBox(NULL, L"Would you like to load the light locations?", L"LOAD", MB_YESNO) == IDYES)
    {
        std::ifstream lightInfo("lightInfo.txt");

        for (int i = 0; i < cLightManager::NUMBEROFLIGHTS; i++)
        {
            float x;
            float y;
            float z;
            float linear;
            float quadratic;
            std::string lightType;
            std::string on_off;
            std::string discard;

            lightInfo >> discard    // "Light"
                >> discard    // Light #
                >> lightType
                >> on_off
                >> discard    // "x:"
                >> x          // x val
                >> discard    // "y:"
                >> y          // y val
                >> discard    // "z:"
                >> z          // z val
                >> discard    // "Linear:"
                >> linear     // Linear value
                >> discard    // "Quardratic
                >> quadratic;  // Quadratic value

            ::g_pLightManager->theLights[i].position.x = x;
            ::g_pLightManager->theLights[i].position.y = y;
            ::g_pLightManager->theLights[i].position.z = z;
            ::g_pLightManager->theLights[i].atten.y = linear;
            ::g_pLightManager->theLights[i].atten.z = quadratic;

            if (lightType == "Point")
            {
                ::g_pLightManager->theLights[i].param1.x = 0.0f;
            }
            else if (lightType == "Spot")
            {
                ::g_pLightManager->theLights[i].param1.x = 1.0f;
            }
            else if (lightType == "Directional")
            {
                ::g_pLightManager->theLights[i].param1.x = 2.0f;
            }

            if (on_off == "OFF")
            {
                ::g_pLightManager->theLights[i].param2.x = 0.0f;
            }
            else if (on_off == "ON")
            {
                ::g_pLightManager->theLights[i].param2.x = 1.0f;
            }
        }

        std::cout << "Loaded Lights!" << std::endl;
    }
    //else
    //{
    //    std::cout << "Cancelled Load" << std::endl;
    //}

    return;
}

void SetCameraPosition()
{
    std::ifstream cameraInfo("cameraInfo.txt");

    float x;
    float y;
    float z;
    float turnLR;
    float turnUD;
    std::string discard;

    cameraInfo >> discard // "Camera"
        >> discard        // "x:"
        >> x              // x value
        >> discard        // "y:"
        >> y              // y value
        >> discard        // "z:"
        >> z              // z value
        >> discard        // "Rotation-Left-Right:"
        >> turnLR         // Left Right value
        >> discard        // "Rotation-Up-Down:"
        >> turnUD;        // Up Down value

    ::g_pFlyCamera->setEyeLocation(x, y, z);
    ::g_pFlyCamera->rotateLeftRight_Yaw_NoScaling(glm::radians(turnLR));
    ::g_pFlyCamera->pitchUpDown(turnUD);

    std::cout << "Camera Position Loaded!" << std::endl;

    return;
}

void ModelsToVAO(const GLuint program)
{
    std::ifstream loadMeshes("loadMeshes.txt");

    std::string path;

    while (!loadMeshes.eof())
    {
        loadMeshes >> path;      // Path value

        sModelDrawInfo mesh;
        ::g_pMeshManager->LoadModelIntoVAO(path, mesh, program);
        //std::cout << mesh.numberOfVertices << " vertices loaded" << std::endl;
    }
}

void DrawMeshes()
{
    std::ifstream drawMeshes("drawMeshes.txt");

    std::string discard;
    std::string name;
    std::string path;
    std::string isWireFrame;
    float transparency;
    std::string texture;
    float texScale;

    bool wireframe;

    float r;
    float g;
    float b;

    while (!drawMeshes.eof())
    {
        drawMeshes >> discard // "Name:"
            >> name           // Name value
            >> discard        // "Path:"
            >> path           // Path value
            >> discard        // "isWireframe"
            >> isWireFrame    // isWireFrame value
            >> discard        // "Colour:"
            >> r              // r value
            >> g              // g value
            >> b              // b value
            >> discard        // "Transparency:"
            >> transparency   // transparency value
            >> discard        // "Texture:"
            >> texture        // texture value
            >> texScale;      // texture scale

        if (drawMeshes.eof())
        {
            break;
        }

        if (isWireFrame == "true")
        {
            wireframe = true;
        }
        else
        {
            wireframe = false;
        }

        sMesh* pMesh = new sMesh();
        pMesh->modelFileName = path;
        pMesh->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
        pMesh->bIsWireframe = wireframe;
        pMesh->uniformScale = 0.1f;
        pMesh->objectColourRGBA = glm::vec4((r / 255.0f), (g / 255.0f), (b / 255.0f), 1.0f);
        pMesh->uniqueFriendlyName = name;
        pMesh->alphaTransparency = transparency;
        pMesh->textures[0] = texture;
        //pMesh->textureScale = 1 / texScale;
        //pMesh->bModulateTexture = false;

        ::g_vecMeshesToDraw.push_back(pMesh);
    }
}

void LoadTextures()
{
    ::g_pTextures->SetBasePath("assets/textures");
    ::g_pTextures->Create2DTextureFromBMPFile("UV_Test_750x750.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Grey_Brick_Wall_Texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("wall_texture_01.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("wall_texture_02.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("wood_floor_texture_01.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("ceiling_texture_01.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("chandelier_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("window_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("window_cutout.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("door_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("wall_light_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("light_switch_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("cabinet_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("grass_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("low_poly_tree_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("dirt_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("vase_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("basketball_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("scratches_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("money_texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("tv_texture.bmp");

    ::g_pTextures->Create2DTextureFromBMPFile("bad_bunny_1920x1080.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("dua-lipa-promo.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Puzzle_parts.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Non-uniform concrete wall 0512-3-1024x1024.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("UV_Test_750x750.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("shape-element-splattered-texture-stroke_1194-8223.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Grey_Brick_Wall_Texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("dirty-metal-texture_1048-4784.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("bad_bunny_1920x1080_24bit_black_and_white.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("red-hot-fire-flames-1024x1024.bmp");
    //
    ::g_pTextures->Create2DTextureFromBMPFile("SurprisedChildFace.bmp");
    // 
    ::g_pTextures->Create2DTextureFromBMPFile("Canadian_Flag_Texture.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Chinese_Flag_Texture.bmp");
    //
    ::g_pTextures->Create2DTextureFromBMPFile("solid_black.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("solid_white.bmp");
    //
    ::g_pTextures->Create2DTextureFromBMPFile("SpidermanUV_square.bmp");

    // Barrel (normal mapped)
    ::g_pTextures->SetBasePath("assets/models/Wooden barrel (has normal map)/Barrel_textures/barrel");
    ::g_pTextures->Create2DTextureFromBMPFile("barrel_BaseColor - Inverted_Y.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("barrel_Normal - Inverted_Y.bmp");

    // Stool (normal mapped)
    ::g_pTextures->SetBasePath("assets/models/Stool");
    ::g_pTextures->Create2DTextureFromBMPFile("Stool_AlbedoSmoothness.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Stool_Normal.bmp");

    // House (normal mapped)
    ::g_pTextures->SetBasePath("assets/models/House/Texture");
    ::g_pTextures->Create2DTextureFromBMPFile("House_UV2_BaseColor.1002_Flipped.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("House_UV2_Normal.1002_Flipped.bmp");

    ::g_pTextures->Create2DTextureFromBMPFile("House_UV4_BaseColor.1004_Flipped.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("House_UV4_Normal.1004_Flipped.bmp");

    ::g_pTextures->Create2DTextureFromBMPFile("House_UV3_BaseColor.1003_Flipped.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("House_UV3_Normal.1003_Flipped.bmp");

    ::g_pTextures->Create2DTextureFromBMPFile("House_glass_BaseColor.1005_Flipped.bmp");

    ::g_pTextures->Create2DTextureFromBMPFile("House_UV1_BaseColor.1001_Flipped.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("House_UV1_Normal.1001_Flipped.bmp");

    // Forest
    ::g_pTextures->SetBasePath("assets/models/Forest/Textures");
    ::g_pTextures->Create2DTextureFromBMPFile("Grass.bmp");

    // Ground
    ::g_pTextures->SetBasePath("assets/models/Ground");
    ::g_pTextures->Create2DTextureFromBMPFile("brown_mud_dry_diff_8k.bmp");

    // Table
    ::g_pTextures->SetBasePath("assets/models/FBX_Wooden_assets/Textures_wooden_asset/Table/Textures/2K/Clean");
    ::g_pTextures->Create2DTextureFromBMPFile("Table_Clean_2K_Albedo.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Table_Clean_2K_Normal.bmp");

    // Chair
    ::g_pTextures->SetBasePath("assets/models/FBX_Wooden_assets/Textures_wooden_asset/Chair/Textures/2K/Clean");
    ::g_pTextures->Create2DTextureFromBMPFile("Chair_Clean_2K_Albedo.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Chair_Clean_2K_Normal.bmp");

    // Fireplace
    ::g_pTextures->SetBasePath("assets/models/Fireplace/firePlaceSet_textures");
    ::g_pTextures->Create2DTextureFromBMPFile("fireplace_diffuse.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("fireplace_normal.bmp");

    // Chandelier
    ::g_pTextures->SetBasePath("assets/models/Chandelier/beneviento-house-chandelier/textures");
    ::g_pTextures->Create2DTextureFromBMPFile("chandelier_low_defaultMat_BaseColor.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("chandelier_low_defaultMat_Normal.bmp");

    // Scooby
    ::g_pTextures->SetBasePath("assets/models/Scooby");
    ::g_pTextures->Create2DTextureFromBMPFile("scooby.bmp");

    // Chandelier
    ::g_pTextures->SetBasePath("assets/models/Candle/Candle_Textures_2K");
    ::g_pTextures->Create2DTextureFromBMPFile("Candle_LP_Candle_BaseColor.bmp");
    ::g_pTextures->Create2DTextureFromBMPFile("Candle_LP_Candle_Normal.bmp");

    // Load the dark sky skybox
    std::string errorString;
    ::g_pTextures->SetBasePath("assets/textures/CubeMaps");
    if (::g_pTextures->CreateCubeTextureFromBMPFiles("DarkSky",
        "DarkStormyLeft2048.bmp",
        "DarkStormyRight2048.bmp",
        "DarkStormyDown2048.bmp",
        "DarkStormyUp2048.bmp",
        "DarkStormyFront2048.bmp",
        "DarkStormyBack2048.bmp", true, errorString))
    {
        std::cout << "Loaded dark sky skybox" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Didn't load dark sky skybox because: " << errorString << std::endl;
    }

    // Load an "interior" skybox
    //::g_pTextures->SetBasePath("assets/textures/skyboxes");
    //if (::g_pTextures->CreateCubeTextureFromBMPFiles("Interior",
    //    "wall_texture_011.bmp",
    //    "wall_texture_011.bmp",
    //    "ceiling_texture_01.bmp",
    //    "wood_floor_texture_01.bmp",
    //    "wall_texture_011.bmp",
    //    "wall_texture_011.bmp", true, errorString))
    //{
    //    std::cout << "Loaded interior skybox" << std::endl;
    //}
    //else
    //{
    //    std::cout << "ERROR: Didn't load interior skybox because: " << errorString << std::endl;
    //}

    return;
}

void AddToPhysXMap(std::string name, unsigned int index)
{
    std::vector<cPhysicsObjectTypes> vecPhysActors;
    ::g_pPhysX->getSceneActors(vecPhysActors);

    map_cPhysicsObjectTypes.insert({ name, vecPhysActors[index] });

    return;
}

void CreateThreads(int num_threads)
{
    // Init the critical section
    InitializeCriticalSection(&cs_particleThread);

    // 15,000 / 100 = 150
    int particlesPerThread = g_pParticles->GetMaximumNumberOfParticles() / num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        sParticleThreadData* pThreadData = new sParticleThreadData();

        // Create thread, but asleep
        pThreadData->bKillThread = false;
        pThreadData->bThreadIsAwake = true;

        pThreadData->threadSleepTime = 10;

        pThreadData->random_seed = i;
        
        // 0 to 149
        // 150 to 299
        pThreadData->bottomIndex = i * particlesPerThread; // i = 0: 0, i = 1: 150, i = 2: 300
        pThreadData->topIndex = pThreadData->bottomIndex + particlesPerThread - 1; // i = 0: 149, i = 1: 299, i = 2: 449

        g_vecThreadData.push_back(pThreadData);

        // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
        HANDLE hVerletUpdateThread
            = CreateThread(
                NULL,                           // LPSECURITY_ATTRIBUTES lpThreadAttributes
                0,                              // SIZE_T dwStackSize,
                threadFunction_MoveParticles,   // LPTHREAD_START_ROUTINE lpStartAddress,
                (void*)pThreadData,             //__drv_aliasesMem LPVOID lpParameter,
                0,                              // DWORD dwCreationFlags,
                NULL);                          // LPDWORD lpThreadId
    }
}