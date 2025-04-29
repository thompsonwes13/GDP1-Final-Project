// for non keyboard callback handling

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "sharedThings.h"   // The camera, etc.
#include "sMesh.h"

#include "cLightManager.h"

#include <fstream>
#include <sstream>
#include "../sharedThings.h"

#include "cTerrainPathChooser.h"

#include "cViperFlagConnector.h"

//#include "cParticleEmitter.h"
#include "cParticleEmitter_2.h"

#include <PhysXWraper/cPhysXWraper.h>
extern cPhysXWraper* g_pPhysX;// = NULL;


//extern cParticleEmitter* g_pParticles;
extern cParticleEmitter_2* g_pParticles;

//#include <Windows.h>

//extern cLightManager* g_pLightManager;

extern cTerrainPathChooser* g_pTerrainPathChooser;

extern cViperFlagConnector* g_pViperFlagConnector;

bool swingingForward = true;
bool swingingBackward = false;
bool jumping = false;

void kineWalkForward();
void kineWalkBackward();
void kineJump();
void kineReset();

struct sMouseState
{
    glm::ivec2 currentPositionXY = glm::ivec2(0, 0);
    glm::ivec2 lastPositionXY = glm::ivec2(0, 0);
    bool bIsLeftMouseButtonDown = false;
};

sMouseState g_MouseState;

float g_jumpHeight = 0.0f;

bool isShiftDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || 
        (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) )
    {
        return true;
    }
    return false;
}

bool isControlDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool isAltDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
    if ( isShiftDown(window) ) 
    {
        return false;
    }

    if ( isControlDown(window) ) 
    {
        return false;
    }

    if ( isAltDown(window) ) 
    {
        return false;
    }

    // Nothing is down
    return true;
}

void handleMouseAsync(GLFWwindow* window)
{

//    if (glfwRawMouseMotionSupported())
//        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

//    std::cout << "Async mouse location (x,y): " << xpos << ", " << ypos << std::endl;

    return;
}

void handleKeyboardAsync(GLFWwindow* window)
{
    //    const float CAMERA_MOVE_SPEED = 100.0f;
    const float CAMERA_MOVE_SPEED = 5.0f;
    //    const float CAMERA_MOVE_SPEED = 0.1f;
    const float CAMERA_TURN_SPEED = 0.1f;

    if (g_jumpHeight > 0)
    {
        g_jumpHeight -= 0.15f;
    }
    else if (g_jumpHeight < 0)
    {
        g_jumpHeight = 0;
    }

    if (isControlDown(window))
    {
        // Find the Light_Sphere
//        sMesh* pLightSphere = pFindMeshByFriendlyName("Light_Sphere");

        const float LIGHT_MOVE_SPEED = 0.1f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.z += LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.z -= LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.x -= LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.y += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.y -= LIGHT_MOVE_SPEED;
        }
        // Linear atten 1 & 2
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            // Increase linear (making it darker)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.y *= 0.99f;     // Down 1%
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            // Increase linear (making it darker)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.y *= 1.01f;     // Up 1%
        }
        // Quadratic atten 3 & 4
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            // Quadratic linear (making it darker)
            // This is WAY more sensitive than the linear
            //  ...so you might want to change it by 0.1% (0.999)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.z *= 0.99f;    // Down 1%
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            // Quadratic linear (making it darker)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.z *= 1.01f;     // Up 1%
        }

        // Angles of spot light
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.y -= 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.y += 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.z -= 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.z += 0.1f;
        }
        // HACK:Exit early
        return;
    }

    // Camera now moves when shift is down
    if (isShiftDown(window))
    {
        

        /*cPhysics::sPhysInfo* pViperPhys = ::g_pPhysicEngine->pFindAssociateMeshByFriendlyName("New_Viper_Player");

        const float VIPER_SPEED_CHANGE = 0.1f;

        // Exists? 
        if (pViperPhys)         // or != NULL
        {
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            {
                if (::g_pViperFlagConnector)
                {
                    ::g_pViperFlagConnector->ReleaseFlag();
                }
            }

            // CUT THE ENGINES!!!
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                pViperPhys->velocity = glm::vec3(0.0f);
            }
            // Go forward
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                // Accelerate
                pViperPhys->velocity.z += VIPER_SPEED_CHANGE;
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                // Accelerate
                pViperPhys->velocity.z -= VIPER_SPEED_CHANGE;
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                // Accelerate
                pViperPhys->velocity.x += VIPER_SPEED_CHANGE;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                pViperPhys->velocity.x -= VIPER_SPEED_CHANGE;
            }
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            {
                pViperPhys->velocity.y += VIPER_SPEED_CHANGE;
            }
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            {
                pViperPhys->velocity.y -= VIPER_SPEED_CHANGE;
            }


        }// if (pViperPhys)*/
        

    }//if (areAllModifiersUp(window)


    // move spiderman
    if (isAltDown(window))
    {
        sMesh* pSM = g_pFindMeshByFriendlyName("legospiderman_Hips");

        const float SPIDEY_MOVE_SPEED = 1.0f;

        if (pSM)
        {
            sMesh* pLeftArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_arm");
            if (pLeftArm)
            {
                if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
                {
                    pLeftArm->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
                }
                if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
                {
                    pLeftArm->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
                }

                sMesh* pLeftHand = pLeftArm->pFindChildMeshByFriendlyName("legospiderman_Left_hand");
                if (pLeftHand)
                {
                    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
                    {
                        pLeftHand->rotationEulerXYZ.x -= SPIDEY_MOVE_SPEED;
                    }
                    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
                    {
                        pLeftHand->rotationEulerXYZ.x += SPIDEY_MOVE_SPEED;
                    }
                }//if (pLeftArm)           
            }//if (pLeftArm)


            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                pSM->positionXYZ.x -= SPIDEY_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                pSM->positionXYZ.x += SPIDEY_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                pSM->positionXYZ.z -= SPIDEY_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                pSM->positionXYZ.z += SPIDEY_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            {
                pSM->positionXYZ.y -= SPIDEY_MOVE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            {
                pSM->positionXYZ.y += SPIDEY_MOVE_SPEED;
            }

        }//if (pSM)


    }//if (isAltDown(window))

    // This will control the player's character
    if (areAllModifiersUp(window))
    {
        float vX = 0;
        float vY = 0;
        float vZ = 0;

        glm::vec3 target = g_pFlyCamera->getTargetRelativeToCamera();
        glm::vec3 upVector = glm::vec3(0, 1, 0);

        bool buttonPressed = false;

        if (g_pPhysX->isOnGround())
        {
            if (jumping)
            {
                kineReset();
            }

            jumping = false;
        }

        glm::vec3 playerPositon = g_pPhysX->getActorPosition("Player");

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            vZ += glm::normalize(target).z * 0.1f;
            vX += glm::normalize(target).x * 0.1f;

            if (!jumping)
            {
                kineWalkForward();
            }

            buttonPressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            vZ += -glm::normalize(target).z * 0.1f;
            vX += -glm::normalize(target).x * 0.1f;

            if (!jumping)
            {
                kineWalkBackward();
            }

            buttonPressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            vX += -glm::normalize(glm::cross(target, upVector)).x * 0.1f;
            vZ += -glm::normalize(glm::cross(target, upVector)).z * 0.1f;

            if (!buttonPressed && !jumping)
            {
                kineWalkForward();
            }

            buttonPressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            vX += glm::normalize(glm::cross(target, upVector)).x * 0.1f;
            vZ += glm::normalize(glm::cross(target, upVector)).z * 0.1f;

            if (!buttonPressed && !jumping)
            {
                kineWalkForward();
            }

            buttonPressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            vY = 0.5;

            buttonPressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            vY = -0.1;

            buttonPressed = true;
        }

        if (!buttonPressed)
        {
            g_pPhysX->updateActorTarget("Player", playerPositon.x, playerPositon.y + g_jumpHeight, playerPositon.z);
            if (!jumping)
            {
                kineReset();
            }
        }
        else
        {
            g_pPhysX->updateActorTarget("Player", playerPositon.x + vX, playerPositon.y + vY + g_jumpHeight, playerPositon.z + vZ);
        }


        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        {
            if (g_LODScale > 0.2)
            {
                g_LODScale -= 0.05f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        {
            if (g_LODScale < 5)
            {
                g_LODScale += 0.05f;
            }
        }

    }
        
    return;
}

// *********************************************************
// Mouse callbacks:

// Set with glfwSetCursorPosCallback(window, cursor_position_callback);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
//    std::cout << "mouse x,y: " << xpos << ", " << ypos << std::endl;
    
    g_MouseState.currentPositionXY.x = (int)xpos;
    g_MouseState.currentPositionXY.y = (int)ypos;

    if ( g_MouseState.bIsLeftMouseButtonDown )
    {
        // If the Left mouse key is down, 
        //  figure out the differene between the current mouse postion and the last one

        glm::ivec2 deltaMousePosition = g_MouseState.lastPositionXY - g_MouseState.currentPositionXY;

        // Use those values to "turn" the camera
    
        // Move left or right? 
        if ( deltaMousePosition.x != 0 )
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( (float)deltaMousePosition.x );
        }
        // Mouse move Up or down? 
        if ( deltaMousePosition.y != 0 )
        {
            ::g_pFlyCamera->pitchUpDown((float)deltaMousePosition.y);
        }
    }//if ( g_MouseState.bIsLeftMouseButtonDown )


    // Update the last one
    g_MouseState.lastPositionXY = g_MouseState.currentPositionXY;

    //return;
}

// Set with glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered)
{
    //if ( entered )
    //{
    //    std::cout << "Mouse is over the window" << std::endl;
    //}
    //else
    //{
    //    std::cout << "Mouse left the window" << std::endl;
    //}
    return;
}


// Set with glfwSetMouseButtonCallback(window, mouse_button_callback);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        switch (action)
        {
        case GLFW_PRESS:
//            std::cout << "GLFW_PRESS" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = true;
            break;
        case GLFW_RELEASE:
//            std::cout << "GLFW_RELEASE" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = false;
            break;
        default:
            std::cout << "Mouse button state unknown" << std::endl;
            break;
        }
    }

    return;
}

// Set with glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // For most mice, the value is the DIFFERENCE between the last time this was called!
    // (i.e. it's not an absolute value)
    // It also goes up and down along the Y axis and is an integer
//    std::cout << "xoffset, yoffset: " << xoffset << ", " << yoffset << std::endl;

    const float MOUSE_WHEEL_RATIO = 0.1f;

    ::g_pFlyCamera->adjustMovementSpeed( float(yoffset) * MOUSE_WHEEL_RATIO );

    return;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float CAMERA_MOVE_SPEED = 0.1f;

    if (areAllModifiersUp(window))
    {
        float vX = 0;
        float vY = 0;
        float vZ = 0;

        glm::vec3 playerPositon = g_pPhysX->getActorPosition("Player");

        // PhysX example from the "hello world" snippet
        if (::g_pPhysX)
        {
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            {

                if (g_pPhysX->isOnGround())
                {
                    g_jumpHeight = 1.0f;
                    g_pPhysX->updateActorTarget("Player", playerPositon.x + vX, playerPositon.y + vY + g_jumpHeight, playerPositon.z + vZ);
                    kineJump();
                    jumping = true;
                }
            }
        }

        if (key == GLFW_KEY_X && action == GLFW_PRESS)
        {
            g_xray = !g_xray;
        }

    }//if (areAllModifiersUp(window))



    return;
}

void kineWalkForward()
{
    sMesh* pSM = g_pFindMeshByFriendlyName("legospiderman_Hips");

    const float SPIDEY_MOVE_SPEED = 2.5f;

    sMesh* pLeftArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_arm");
    sMesh* pRightArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_arm");
    sMesh* pLeftLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_leg");
    sMesh* pRightLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_leg");

    if (pLeftArm->rotationEulerXYZ.y < -60)
    {
        swingingForward = false;
        swingingBackward = true;
    }
    else if (pLeftArm->rotationEulerXYZ.y > 60)
    {
        swingingForward = true;
        swingingBackward = false;
    }

    if (swingingForward)
    {
        pLeftArm->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
        pRightArm->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
        pLeftLeg->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
        pRightLeg->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
    }
    else if (swingingBackward)
    {
        pLeftArm->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
        pRightArm->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
        pLeftLeg->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
        pRightLeg->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
    }

    return;
}

void kineWalkBackward()
{
    sMesh* pSM = g_pFindMeshByFriendlyName("legospiderman_Hips");

    const float SPIDEY_MOVE_SPEED = 2.5f;

    sMesh* pLeftArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_arm");
    sMesh* pRightArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_arm");
    sMesh* pLeftLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_leg");
    sMesh* pRightLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_leg");

    if (pLeftArm->rotationEulerXYZ.y < -60)
    {
        swingingForward = true;
        swingingBackward = false;
    }
    else if (pLeftArm->rotationEulerXYZ.y > 60)
    {
        swingingForward = false;
        swingingBackward = true;
    }

    if (swingingForward)
    {
        pLeftArm->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
        pRightArm->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
        pLeftLeg->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
        pRightLeg->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
    }
    else if (swingingBackward)
    {
        pLeftArm->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;
        pRightArm->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
        pLeftLeg->rotationEulerXYZ.y += SPIDEY_MOVE_SPEED;
        pRightLeg->rotationEulerXYZ.y -= SPIDEY_MOVE_SPEED;

    }
}

void kineJump()
{
    sMesh* pSM = g_pFindMeshByFriendlyName("legospiderman_Hips");

    sMesh* pLeftArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_arm");
    sMesh* pRightArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_arm");
    sMesh* pLeftLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_leg");
    sMesh* pRightLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_leg");

    pLeftArm->rotationEulerXYZ.y = -90;
    pRightArm->rotationEulerXYZ.y = -90;
    pLeftLeg->rotationEulerXYZ.y = 0;
    pRightLeg->rotationEulerXYZ.y = 0;
}

void kineReset()
{
    sMesh* pSM = g_pFindMeshByFriendlyName("legospiderman_Hips");

    sMesh* pLeftArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_arm");
    sMesh* pRightArm = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_arm");
    sMesh* pLeftLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Left_leg");
    sMesh* pRightLeg = pSM->pFindChildMeshByFriendlyName("legospiderman_Right_leg");

    pLeftArm->rotationEulerXYZ.y = 0;
    pRightArm->rotationEulerXYZ.y = 0;
    pLeftLeg->rotationEulerXYZ.y = 0;
    pRightLeg->rotationEulerXYZ.y = 0;
}