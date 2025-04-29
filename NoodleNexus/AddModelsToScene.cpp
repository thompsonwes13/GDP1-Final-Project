#include "sharedThings.h"
#include <iostream>
#include <sstream>
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cViperFlagConnector.h"
#include <glm/gtc/matrix_transform.hpp> 

#include "cAssimpHelper/cAssimpHelper.h"

extern std::vector<sMesh*> g_vecMeshesToDraw;
extern cPhysics* g_pPhysicEngine;
extern cVAOManager* g_pMeshManager;
extern cBasicTextureManager* g_pTextures;
cViperFlagConnector* g_pViperFlagConnector = NULL;
AH::cFileLoader* g_pAssimp = NULL;

bool Convert_AssimpHelperMesh_to_sModelDrawInfo(AH::cMesh* pAHMesh, sModelDrawInfo& DrawInfo);

void AddModelsToScene(cVAOManager* pMeshManager, GLuint program)
{
    ::g_pAssimp = new AH::cFileLoader();

    AH::cFileLoader::sPostProcessFlags postProcessingNormalMaps;
    postProcessingNormalMaps.bProcess_CalcTangentSpace = true;
    postProcessingNormalMaps.bProcess_FlipUVs = true;
    postProcessingNormalMaps.bProcess_Triangulate = true;


    ::g_pAssimp->SetBasePath("assets/models/Ingenuity (helicopter, has normal maps)/Ingenuity/OBJ (blender export)");
    ::g_pAssimp->Load3DModelFile("Ingenuity (blender exort).obj", postProcessingNormalMaps);

    // Barrel
    ::g_pAssimp->SetBasePath("assets/models/Wooden barrel (has normal map)");
    if (::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj", postProcessingNormalMaps))
    {
        sMesh* pBarrel = new sMesh();
        AH::cScene* pSceneWithBarrel = ::g_pAssimp->pGetScenePointer("Barrel_OBJ.obj");
        if (pSceneWithBarrel)
        {
            // Find "barrel"
            AH::cMesh* pBarrel = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithBarrel->vecMeshes)
            {
                if (pCurMesh->name == "barrel")
                {
                    pBarrel = pCurMesh;
                    break;
                }
            }
            if (pBarrel)
            {
                sModelDrawInfo barrelDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pBarrel, barrelDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    barrelDrawInfo.meshName = "Barrel_OBJ.obj";

                    barrelDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(barrelDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pBarrel)
        }//if (pSceneWithBarrel)

        pBarrel->modelFileName = "Barrel_OBJ.obj";
        pBarrel->uniqueFriendlyName = "Barrel";
        pBarrel->textures[0] = "barrel_BaseColor - Inverted_Y.bmp";
        pBarrel->blendRatio[0] = 1.0f;
        pBarrel->normalMap = "barrel_Normal - Inverted_Y.bmp";

        pBarrel->uniformScale = 10.0f;
        pBarrel->positionXYZ = glm::vec3(-50.0f, -50.0f, 0.0f);
        pBarrel->vecLODInfos.push_back(sMesh::sLODInfo("Barrel_OBJ.obj", 5, 69451));

        if (::g_pAssimp->Load3DModelFile("Barrel_half_OBJ.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithBarrel = ::g_pAssimp->pGetScenePointer("Barrel_half_OBJ.obj");
            if (pSceneWithBarrel)
            {
                // Find "barrel"
                AH::cMesh* pBarrel = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithBarrel->vecMeshes)
                {
                    if (pCurMesh->name == "barrel")
                    {
                        pBarrel = pCurMesh;
                        break;
                    }
                }
                if (pBarrel)
                {
                    sModelDrawInfo barrelDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pBarrel, barrelDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        barrelDrawInfo.meshName = "Barrel_half_OBJ.obj";

                        barrelDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(barrelDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pBarrel)
            }//if (pSceneWithBarrel)

            sMesh* pHalfBarrel = new sMesh();
            pHalfBarrel->modelFileName = "Barrel_half_OBJ.obj";
            pHalfBarrel->uniqueFriendlyName = "Barrel_half";
            pHalfBarrel->textures[0] = "barrel_BaseColor - Inverted_Y.bmp";
            pHalfBarrel->blendRatio[0] = 1.0f;
            
            pHalfBarrel->normalMap = "barrel_Normal - Inverted_Y.bmp";

            pHalfBarrel->uniformScale = 10.0f;
            pHalfBarrel->positionXYZ = glm::vec3(-50.0f, -50.0f, 0.0f);
            pBarrel->vecLODInfos.push_back(sMesh::sLODInfo("Barrel_half_OBJ.obj", 10, 69451));

        }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"

        if (::g_pAssimp->Load3DModelFile("Barrel_quarter_OBJ.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithBarrel = ::g_pAssimp->pGetScenePointer("Barrel_quarter_OBJ.obj");
            if (pSceneWithBarrel)
            {
                // Find "barrel"
                AH::cMesh* pBarrel = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithBarrel->vecMeshes)
                {
                    if (pCurMesh->name == "barrel")
                    {
                        pBarrel = pCurMesh;
                        break;
                    }
                }
                if (pBarrel)
                {
                    sModelDrawInfo barrelDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pBarrel, barrelDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        barrelDrawInfo.meshName = "Barrel_quarter_OBJ.obj";

                        barrelDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(barrelDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pBarrel)
            }//if (pSceneWithBarrel)

            sMesh* pHalfBarrel = new sMesh();
            pHalfBarrel->modelFileName = "Barrel_quarter_OBJ.obj";
            pHalfBarrel->uniqueFriendlyName = "Barrel_quarter";
            pHalfBarrel->textures[0] = "barrel_BaseColor - Inverted_Y.bmp";
            pHalfBarrel->blendRatio[0] = 1.0f;

            pHalfBarrel->normalMap = "barrel_Normal - Inverted_Y.bmp";

            pHalfBarrel->uniformScale = 10.0f;
            pHalfBarrel->positionXYZ = glm::vec3(-50.0f, -50.0f, 0.0f);
            pBarrel->vecLODInfos.push_back(sMesh::sLODInfo("Barrel_quarter_OBJ.obj", FLT_MAX, 69451));

        }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"
        ::g_vecMeshesToDraw.push_back(pBarrel);
    }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"

    
    // Ground
    ::g_pAssimp->SetBasePath("assets/models/Ground");
    if (::g_pAssimp->Load3DModelFile("Ground.obj", postProcessingNormalMaps))
    {
        AH::cScene* pSceneWithForest = ::g_pAssimp->pGetScenePointer("Ground.obj");
        if (pSceneWithForest)
        {
            // Find "Ground"
            AH::cMesh* pGroundMesh = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithForest->vecMeshes)
            {
                if (pCurMesh->name == "Ground")
                {
                    pGroundMesh = pCurMesh;
                    break;
                }
            }
            if (pGroundMesh)
            {
                sModelDrawInfo houseDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pGroundMesh, houseDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    houseDrawInfo.meshName = "Ground_OBJ.obj";

                    houseDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(houseDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pStool)
        }//if (pSceneWithStool)

        sMesh* pGround = new sMesh();
        pGround->modelFileName = "Ground_OBJ.obj";
        pGround->uniqueFriendlyName = "Ground";
        pGround->textures[0] = "brown_mud_dry_diff_8k.bmp";
        pGround->blendRatio[0] = 1.0f;
        //pGround->bDoNotLight = true;
        //
        //pHouseWalls->normalMap = "House_UV2_Normal.1002_Flipped.bmp";

        pGround->uniformScale = 10.0f;
        pGround->positionXYZ = glm::vec3(50.0f, -180.0f, 0.0f);
        ::g_vecMeshesToDraw.push_back(pGround);
    }

    // Wooden assets
    ::g_pAssimp->SetBasePath("assets/models/FBX_Wooden_assets");
    if (::g_pAssimp->Load3DModelFile("Wooden_objects.obj", postProcessingNormalMaps))
    {
        sMesh* pTable = new sMesh();
        sMesh* pChair = new sMesh();
        {
            AH::cScene* pSceneWithWoodenObjects = ::g_pAssimp->pGetScenePointer("Wooden_objects.obj");
            if (pSceneWithWoodenObjects)
            {
                // Find "barrel"
                AH::cMesh* pTable = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "table_3")
                    {
                        pTable = pCurMesh;
                        break;
                    }
                }
                if (pTable)
                {
                    sModelDrawInfo tableDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pTable, tableDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        tableDrawInfo.meshName = "Table.obj";

                        tableDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(tableDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            pTable->modelFileName = "Table.obj";
            pTable->uniqueFriendlyName = "Table";
            pTable->textures[0] = "Table_Clean_2K_Albedo.bmp";
            pTable->blendRatio[0] = 1.0f;
            pTable->normalMap = "Table_Clean_2K_Normal.bmp";

            pTable->uniformScale = 5.0f;
            pTable->positionXYZ = glm::vec3(-21.0f, 0.0f, 4.5f);
            pTable->rotationEulerXYZ.y = 90.0f;
            pTable->vecLODInfos.push_back(sMesh::sLODInfo("Table.obj", 30, 69451));

            if (pSceneWithWoodenObjects)
            {
                // Find "barrel"
                AH::cMesh* pChair = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "chair_3")
                    {
                        pChair = pCurMesh;
                        break;
                    }
                }
                if (pChair)
                {
                    sModelDrawInfo chairDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pChair, chairDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        chairDrawInfo.meshName = "Chair.obj";

                        chairDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(chairDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            pChair->modelFileName = "Chair.obj";
            pChair->uniqueFriendlyName = "Chair";
            pChair->textures[0] = "Chair_Clean_2K_Albedo.bmp";
            pChair->blendRatio[0] = 1.0f;
            pChair->normalMap = "Chair_Clean_2K_Normal.bmp";

            pChair->uniformScale = 5.0f;
            pChair->positionXYZ = glm::vec3(-21.0f, 0.0f, 4.5f);
            pChair->rotationEulerXYZ.y = 90.0f;
            pChair->vecLODInfos.push_back(sMesh::sLODInfo("Chair.obj", 30, 69451));
        }

        if (::g_pAssimp->Load3DModelFile("Wooden_half_objects.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithWoodenObjects = ::g_pAssimp->pGetScenePointer("Wooden_half_objects.obj");
            if (pSceneWithWoodenObjects)
            {
                // Find "barrel"
                AH::cMesh* pTable = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "table_3")
                    {
                        pTable = pCurMesh;
                        break;
                    }
                }
                if (pTable)
                {
                    sModelDrawInfo tableDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pTable, tableDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        tableDrawInfo.meshName = "HalfTable.obj";

                        tableDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(tableDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pHalfTable = new sMesh();
            pHalfTable->modelFileName = "HalfTable.obj";
            pHalfTable->uniqueFriendlyName = "HalfTable";
            pHalfTable->textures[0] = "Table_Clean_2K_Albedo.bmp";
            pHalfTable->blendRatio[0] = 1.0f;
            pHalfTable->normalMap = "Table_Clean_2K_Normal.bmp";

            pHalfTable->uniformScale = 5.0f;
            pHalfTable->positionXYZ = glm::vec3(-21.0f, 0.0f, 4.5f);
            pHalfTable->rotationEulerXYZ.y = 90.0f;
            pTable->vecLODInfos.push_back(sMesh::sLODInfo("HalfTable.obj", 50, 69451));

            if (pSceneWithWoodenObjects)
            {
                // Find "barrel"
                AH::cMesh* pChair = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "chair_3")
                    {
                        pChair = pCurMesh;
                        break;
                    }
                }
                if (pChair)
                {
                    sModelDrawInfo chairDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pChair, chairDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        chairDrawInfo.meshName = "HalfChair.obj";

                        chairDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(chairDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pHalfChair = new sMesh();
            pHalfChair->modelFileName = "HalfChair.obj";
            pHalfChair->uniqueFriendlyName = "HalfChair";
            pHalfChair->textures[0] = "Chair_Clean_2K_Albedo.bmp";
            pHalfChair->blendRatio[0] = 1.0f;
            pHalfChair->normalMap = "Chair_Clean_2K_Normal.bmp";

            pHalfChair->uniformScale = 5.0f;
            pHalfChair->positionXYZ = glm::vec3(-21.0f, 0.0f, 4.5f);
            pHalfChair->rotationEulerXYZ.y = 90.0f;
            pChair->vecLODInfos.push_back(sMesh::sLODInfo("HalfChair.obj", 50, 69451));

        }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"

        if (::g_pAssimp->Load3DModelFile("Wooden_quarter_objects.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithWoodenObjects = ::g_pAssimp->pGetScenePointer("Wooden_quarter_objects.obj");
            if (pSceneWithWoodenObjects)
            {
                // Find "barrel"
                AH::cMesh* pTable = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "table_3")
                    {
                        pTable = pCurMesh;
                        break;
                    }
                }
                if (pTable)
                {
                    sModelDrawInfo tableDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pTable, tableDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        tableDrawInfo.meshName = "QuarterTable.obj";

                        tableDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(tableDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pQuarterTable = new sMesh();
            pQuarterTable->modelFileName = "QuarterTable.obj";
            pQuarterTable->uniqueFriendlyName = "QuarterTable";
            pQuarterTable->textures[0] = "Table_Clean_2K_Albedo.bmp";
            pQuarterTable->blendRatio[0] = 1.0f;
            pQuarterTable->normalMap = "Table_Clean_2K_Normal.bmp";

            pQuarterTable->uniformScale = 5.0f;
            pQuarterTable->positionXYZ = glm::vec3(-21.0f, 0.0f, 4.5f);
            pQuarterTable->rotationEulerXYZ.y = 90.0f;
            pTable->vecLODInfos.push_back(sMesh::sLODInfo("QuarterTable.obj", FLT_MAX, 69451));

            if (pSceneWithWoodenObjects)
            {
                // Find "barrel"
                AH::cMesh* pChair = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "chair_3")
                    {
                        pChair = pCurMesh;
                        break;
                    }
                }
                if (pChair)
                {
                    sModelDrawInfo chairDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pChair, chairDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        chairDrawInfo.meshName = "QuarterChair.obj";

                        chairDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(chairDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pQuarterChair = new sMesh();
            pQuarterChair->modelFileName = "QuarterChair.obj";
            pQuarterChair->uniqueFriendlyName = "QuarterChair";
            pQuarterChair->textures[0] = "Chair_Clean_2K_Albedo.bmp";
            pQuarterChair->blendRatio[0] = 1.0f;
            pQuarterChair->normalMap = "Chair_Clean_2K_Normal.bmp";

            pQuarterChair->uniformScale = 5.0f;
            pQuarterChair->positionXYZ = glm::vec3(-21.0f, 0.0f, 4.5f);
            pQuarterChair->rotationEulerXYZ.y = 90.0f;
            pChair->vecLODInfos.push_back(sMesh::sLODInfo("QuarterChair.obj", FLT_MAX, 69451));

        }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"

        ::g_vecMeshesToDraw.push_back(pTable);
        ::g_vecMeshesToDraw.push_back(pChair);
    }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"

    // Fireplace
    ::g_pAssimp->SetBasePath("assets/models/Fireplace");
    if (::g_pAssimp->Load3DModelFile("fireplace.obj", postProcessingNormalMaps))
    {
        sMesh* pFireplace = new sMesh();
        sMesh* pFirewood = new sMesh();

        {
            AH::cScene* pSceneWithWoodenObjects = ::g_pAssimp->pGetScenePointer("fireplace.obj");
            if (pSceneWithWoodenObjects)
            {
                // Find "fireplace_noFireWood"
                AH::cMesh* pFireplace = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "fireplace_noFireWood")
                    {
                        pFireplace = pCurMesh;
                        break;
                    }
                }
                if (pFireplace)
                {
                    sModelDrawInfo fireplaceDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pFireplace, fireplaceDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        fireplaceDrawInfo.meshName = "Fireplace.obj";

                        fireplaceDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(fireplaceDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            pFireplace->modelFileName = "Fireplace.obj";
            pFireplace->uniqueFriendlyName = "Fireplace";
            pFireplace->textures[0] = "fireplace_diffuse.bmp";
            pFireplace->blendRatio[0] = 1.0f;
            pFireplace->normalMap = "fireplace_normal.bmp";

            pFireplace->uniformScale = 3.0f;
            pFireplace->positionXYZ = glm::vec3(13.5f, 0.35f, 0.0f);
            pFireplace->rotationEulerXYZ.y = 270.0f;
            pFireplace->vecLODInfos.push_back(sMesh::sLODInfo("Fireplace.obj", 15, 69451));
            //::g_vecMeshesToDraw.push_back(pFireplace);

            if (pSceneWithWoodenObjects)
            {
                // Find "firewood_separate"
                AH::cMesh* pFirewood = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "firewood_separate")
                    {
                        pFirewood = pCurMesh;
                        break;
                    }
                }
                if (pFirewood)
                {
                    sModelDrawInfo firewoodDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pFirewood, firewoodDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        firewoodDrawInfo.meshName = "Firewood.obj";

                        firewoodDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(firewoodDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pStool)
            }//if (pSceneWithStool)

            pFirewood->modelFileName = "Firewood.obj";
            pFirewood->uniqueFriendlyName = "Firewood";
            pFirewood->textures[0] = "fireplace_diffuse.bmp";
            pFirewood->blendRatio[0] = 1.0f;
            //
            pFirewood->normalMap = "fireplace_normal.bmp";
            pFirewood->vecLODInfos.push_back(sMesh::sLODInfo("Firewood.obj", 15, 69451));

            // Add as child mesh
            pFireplace->vec_pChildMeshes.push_back(pFirewood);
        }

        if (::g_pAssimp->Load3DModelFile("fireplace_half.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithWoodenObjects = ::g_pAssimp->pGetScenePointer("fireplace_half.obj");
            if (pSceneWithWoodenObjects)
            {
                // Find "fireplace_noFireWood"
                AH::cMesh* pFireplace = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "fireplace_noFireWood")
                    {
                        pFireplace = pCurMesh;
                        break;
                    }
                }
                if (pFireplace)
                {
                    sModelDrawInfo fireplaceDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pFireplace, fireplaceDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        fireplaceDrawInfo.meshName = "HalfFireplace.obj";

                        fireplaceDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(fireplaceDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pHalfFireplace = new sMesh();
            pHalfFireplace->modelFileName = "HalfFireplace.obj";
            pHalfFireplace->uniqueFriendlyName = "HalfFireplace";
            pHalfFireplace->textures[0] = "fireplace_diffuse.bmp";
            pHalfFireplace->blendRatio[0] = 1.0f;
            pHalfFireplace->normalMap = "fireplace_normal.bmp";

            pHalfFireplace->uniformScale = 3.0f;
            pHalfFireplace->positionXYZ = glm::vec3(13.5f, 0.35f, 0.0f);
            pHalfFireplace->rotationEulerXYZ.y = 270.0f;
            pFireplace->vecLODInfos.push_back(sMesh::sLODInfo("HalfFireplace.obj", 20, 69451));
            //::g_vecMeshesToDraw.push_back(pFireplace);

            if (pSceneWithWoodenObjects)
            {
                // Find "firewood_separate"
                AH::cMesh* pFirewood = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "firewood_separate")
                    {
                        pFirewood = pCurMesh;
                        break;
                    }
                }
                if (pFirewood)
                {
                    sModelDrawInfo firewoodDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pFirewood, firewoodDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        firewoodDrawInfo.meshName = "HalfFirewood.obj";

                        firewoodDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(firewoodDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pStool)
            }//if (pSceneWithStool)

            sMesh* pHalfFirewood = new sMesh();
            pHalfFirewood->modelFileName = "HalfFirewood.obj";
            pHalfFirewood->uniqueFriendlyName = "HalfFirewood";
            pHalfFirewood->textures[0] = "fireplace_diffuse.bmp";
            pHalfFirewood->blendRatio[0] = 1.0f;
            //
            pHalfFirewood->normalMap = "fireplace_normal.bmp";
            pFirewood->vecLODInfos.push_back(sMesh::sLODInfo("HalfFirewood.obj", 20, 69451));

            // Add as child mesh
            pHalfFireplace->vec_pChildMeshes.push_back(pFirewood);

        }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"

        if (::g_pAssimp->Load3DModelFile("fireplace_quarter.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithWoodenObjects = ::g_pAssimp->pGetScenePointer("fireplace_quarter.obj");
            if (pSceneWithWoodenObjects)
            {
                // Find "fireplace_noFireWood"
                AH::cMesh* pFireplace = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithWoodenObjects->vecMeshes)
                {
                    if (pCurMesh->name == "fireplace_noFireWood")
                    {
                        pFireplace = pCurMesh;
                        break;
                    }
                }
                if (pFireplace)
                {
                    sModelDrawInfo fireplaceDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pFireplace, fireplaceDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        fireplaceDrawInfo.meshName = "QuarterFireplace.obj";

                        fireplaceDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(fireplaceDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pQuarterFireplace = new sMesh();
            pQuarterFireplace->modelFileName = "QuarterFireplace.obj";
            pQuarterFireplace->uniqueFriendlyName = "QuarterFireplace";
            pQuarterFireplace->textures[0] = "fireplace_diffuse.bmp";
            pQuarterFireplace->blendRatio[0] = 1.0f;
            pQuarterFireplace->normalMap = "fireplace_normal.bmp";

            pQuarterFireplace->uniformScale = 3.0f;
            pQuarterFireplace->positionXYZ = glm::vec3(13.5f, 0.35f, 0.0f);
            pQuarterFireplace->rotationEulerXYZ.y = 270.0f;
            pFireplace->vecLODInfos.push_back(sMesh::sLODInfo("QuarterFireplace.obj", FLT_MAX, 69451));

        }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"
        g_vecMeshesToDraw.push_back(pFireplace);
        g_vecMeshesToDraw.push_back(pFirewood);
    }// if (!::g_pAssimp->Load3DModelFile("Barrel_OBJ.obj"

    // Chandelier
    ::g_pAssimp->SetBasePath("assets/models/Chandelier");
    if (::g_pAssimp->Load3DModelFile("chandelier.obj", postProcessingNormalMaps))
    {
        sMesh* pChandelier = new sMesh();
        {
            AH::cScene* pSceneWithChandelier = ::g_pAssimp->pGetScenePointer("chandelier.obj");
            if (pSceneWithChandelier)
            {
                // Find "BezierCurve"
                AH::cMesh* pChandelier = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithChandelier->vecMeshes)
                {
                    if (pCurMesh->name == "BezierCurve")
                    {
                        pChandelier = pCurMesh;
                        break;
                    }
                }
                if (pChandelier)
                {
                    sModelDrawInfo chandelierDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pChandelier, chandelierDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        chandelierDrawInfo.meshName = "Chandelier.obj";

                        chandelierDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(chandelierDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            pChandelier->modelFileName = "Chandelier.obj";
            pChandelier->uniqueFriendlyName = "Chandelier";
            pChandelier->textures[0] = "chandelier_low_defaultMat_BaseColor.bmp";
            pChandelier->blendRatio[0] = 1.0f;
            pChandelier->normalMap = "chandelier_low_defaultMat_Normal.bmp";

            pChandelier->uniformScale = 0.5f;
            pChandelier->positionXYZ = glm::vec3(0.0f, 8.0f, 0.0f);
            pChandelier->vecLODInfos.push_back(sMesh::sLODInfo("Chandelier.obj", 12, 69451));
        }

        if (::g_pAssimp->Load3DModelFile("chandelier_half.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithChandelier = ::g_pAssimp->pGetScenePointer("chandelier_half.obj");
            if (pSceneWithChandelier)
            {
                // Find "BezierCurve"
                AH::cMesh* pChandelier = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithChandelier->vecMeshes)
                {
                    if (pCurMesh->name == "BezierCurve")
                    {
                        pChandelier = pCurMesh;
                        break;
                    }
                }
                if (pChandelier)
                {
                    sModelDrawInfo chandelierDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pChandelier, chandelierDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        chandelierDrawInfo.meshName = "HalfChandelier.obj";

                        chandelierDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(chandelierDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pHalfChandelier = new sMesh();
            pHalfChandelier->modelFileName = "HalfChandelier.obj";
            pHalfChandelier->uniqueFriendlyName = "HalfChandelier";
            pHalfChandelier->textures[0] = "chandelier_low_defaultMat_BaseColor.bmp";
            pHalfChandelier->blendRatio[0] = 1.0f;
            pHalfChandelier->normalMap = "chandelier_low_defaultMat_Normal.bmp";

            pHalfChandelier->uniformScale = 0.5f;
            pHalfChandelier->positionXYZ = glm::vec3(0.0f, 8.0f, 0.0f);

            pChandelier->vecLODInfos.push_back(sMesh::sLODInfo("HalfChandelier.obj", 15, 69451));
            //::g_vecMeshesToDraw.push_back(pChandelier);
        }

        if (::g_pAssimp->Load3DModelFile("chandelier_quarter.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithChandelier = ::g_pAssimp->pGetScenePointer("chandelier_quarter.obj");
            if (pSceneWithChandelier)
            {
                // Find "BezierCurve"
                AH::cMesh* pChandelier = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithChandelier->vecMeshes)
                {
                    if (pCurMesh->name == "BezierCurve")
                    {
                        pChandelier = pCurMesh;
                        break;
                    }
                }
                if (pChandelier)
                {
                    sModelDrawInfo chandelierDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pChandelier, chandelierDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        chandelierDrawInfo.meshName = "QuarterChandelier.obj";

                        chandelierDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(chandelierDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pQuarterChandelier = new sMesh();
            pQuarterChandelier->modelFileName = "QuarterChandelier.obj";
            pQuarterChandelier->uniqueFriendlyName = "QuarterChandelier";
            pQuarterChandelier->textures[0] = "chandelier_low_defaultMat_BaseColor.bmp";
            pQuarterChandelier->blendRatio[0] = 1.0f;
            pQuarterChandelier->normalMap = "chandelier_low_defaultMat_Normal.bmp";

            pQuarterChandelier->uniformScale = 0.5f;
            pQuarterChandelier->positionXYZ = glm::vec3(0.0f, 8.0f, 0.0f);

            pChandelier->vecLODInfos.push_back(sMesh::sLODInfo("QuarterChandelier.obj", FLT_MAX, 69451));
            ::g_vecMeshesToDraw.push_back(pChandelier);
        }
    }

    // Scooby
    ::g_pAssimp->SetBasePath("assets/models/Scooby");
    if (::g_pAssimp->Load3DModelFile("scooby.obj", postProcessingNormalMaps))
    {
        AH::cScene* pSceneWithScooby = ::g_pAssimp->pGetScenePointer("scooby.obj");
        if (pSceneWithScooby)
        {
            // Find "BezierCurve"
            AH::cMesh* pScooby = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithScooby->vecMeshes)
            {
                if (pCurMesh->name == "Cube.001")
                {
                    pScooby = pCurMesh;
                    break;
                }
            }
            if (pScooby)
            {
                sModelDrawInfo scoobyDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pScooby, scoobyDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    scoobyDrawInfo.meshName = "Scooby.obj";

                    scoobyDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(scoobyDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pTable)
        }//if (pSceneWithBarrel)

        sMesh* pScooby = new sMesh();
        pScooby->modelFileName = "Scooby.obj";
        pScooby->uniqueFriendlyName = "Scooby";
        pScooby->textures[0] = "scooby.bmp";
        pScooby->blendRatio[0] = 1.0f;

        pScooby->uniformScale = 0.5f;
        pScooby->positionXYZ = glm::vec3(-11.75f, 5.5f, 4.5f);
        ::g_vecMeshesToDraw.push_back(pScooby);
    }

    // Candle
    ::g_pAssimp->SetBasePath("assets/models/Candle");
    if (::g_pAssimp->Load3DModelFile("Candle_Stick_OBJ.obj", postProcessingNormalMaps))
    {
        AH::cScene* pSceneWithCandle = ::g_pAssimp->pGetScenePointer("Candle_Stick_OBJ.obj");
        if (pSceneWithCandle)
        {
            // Find "group2"
            AH::cMesh* pCandle = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithCandle->vecMeshes)
            {
                if (pCurMesh->name == "group2")
                {
                    pCandle = pCurMesh;
                    break;
                }
            }
            if (pCandle)
            {
                sModelDrawInfo candleDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pCandle, candleDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    candleDrawInfo.meshName = "Candle.obj";

                    candleDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(candleDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pTable)
        }//if (pSceneWithBarrel)

        sMesh* pCandle = new sMesh();
        pCandle->modelFileName = "Candle.obj";
        pCandle->uniqueFriendlyName = "Candle";
        pCandle->textures[0] = "Candle_LP_Candle_BaseColor.bmp";
        pCandle->blendRatio[0] = 1.0f;
        pCandle->normalMap = "Candle_LP_Candle_Normal.bmp";

        pCandle->uniformScale = 0.025f;
        pCandle->positionXYZ = glm::vec3(-10.5f, 4.1f, 7.0f);
        pCandle->vecLODInfos.push_back(sMesh::sLODInfo("Candle.obj", 10, 69451)); // Up to 25 units from the camera

        if (::g_pAssimp->Load3DModelFile("Candle_Stick_OBJ_half.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithHalfCandle = ::g_pAssimp->pGetScenePointer("Candle_Stick_OBJ_half.obj");
            if (pSceneWithHalfCandle)
            {
                // Find "group2"
                AH::cMesh* pHalfCandle = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithHalfCandle->vecMeshes)
                {
                    if (pCurMesh->name == "Candle_Stick_OBJ")
                    {
                        pHalfCandle = pCurMesh;
                        break;
                    }
                }
                if (pHalfCandle)
                {
                    sModelDrawInfo halfCandleDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pHalfCandle, halfCandleDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        halfCandleDrawInfo.meshName = "HalfCandle.obj";

                        halfCandleDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(halfCandleDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pHalfCandle = new sMesh();
            pHalfCandle->modelFileName = "HalfCandle.obj";
            pHalfCandle->uniqueFriendlyName = "HalfCandle";
            pHalfCandle->textures[0] = "Candle_LP_Candle_BaseColor.bmp";
            pHalfCandle->blendRatio[0] = 1.0f;
            pHalfCandle->normalMap = "Candle_LP_Candle_Normal.bmp";

            pHalfCandle->uniformScale = 0.025f;
            pHalfCandle->positionXYZ = glm::vec3(-10.5f, 4.1f, 7.0f);
            pCandle->vecLODInfos.push_back(sMesh::sLODInfo("HalfCandle.obj", 20, 69451)); // Up to 25 units from the camera
        }

        if (::g_pAssimp->Load3DModelFile("Candle_Stick_OBJ_quarter.obj", postProcessingNormalMaps))
        {
            AH::cScene* pSceneWithQuarterCandle = ::g_pAssimp->pGetScenePointer("Candle_Stick_OBJ_quarter.obj");
            if (pSceneWithQuarterCandle)
            {
                // Find "group2"
                AH::cMesh* pQuarterCandle = NULL;
                for (AH::cMesh* pCurMesh : pSceneWithQuarterCandle->vecMeshes)
                {
                    if (pCurMesh->name == "Candle_Stick_OBJ")
                    {
                        pQuarterCandle = pCurMesh;
                        break;
                    }
                }
                if (pQuarterCandle)
                {
                    sModelDrawInfo quarterCandleDrawInfo;
                    if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pQuarterCandle, quarterCandleDrawInfo))
                    {
                        // Change the name to the file name (it's "barrel" in the OBJ model
                        quarterCandleDrawInfo.meshName = "QuarterCandle.obj";

                        quarterCandleDrawInfo.calculateExtents();

                        ::g_pMeshManager->AddModelToVAO(quarterCandleDrawInfo, program);
                    }
                    else
                    {
                        std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                    }
                }//if (pTable)
            }//if (pSceneWithBarrel)

            sMesh* pQuarterCandle = new sMesh();
            pQuarterCandle->modelFileName = "QuarterCandle.obj";
            pQuarterCandle->uniqueFriendlyName = "QuarterCandle";
            pQuarterCandle->textures[0] = "Candle_LP_Candle_BaseColor.bmp";
            pQuarterCandle->blendRatio[0] = 1.0f;
            pQuarterCandle->normalMap = "Candle_LP_Candle_Normal.bmp";

            pQuarterCandle->uniformScale = 0.025f;
            pQuarterCandle->positionXYZ = glm::vec3(-10.5f, 4.1f, 7.0f);
            pCandle->vecLODInfos.push_back(sMesh::sLODInfo("QuarterCandle.obj", FLT_MAX, 69451)); // Up to 25 units from the camera
            ::g_vecMeshesToDraw.push_back(pCandle);
        }

        sMesh* pFlameSphere = new sMesh();
        pFlameSphere->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv_64_faces.ply";
        pFlameSphere->positionXYZ = glm::vec3(-10.49f, 5.75f, 7.0f);
        pFlameSphere->bIsWireframe = false;
        pFlameSphere->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        pFlameSphere->uniqueFriendlyName = "FlameSphere";
        pFlameSphere->uniformScale = 0.02f;
        pFlameSphere->bDoNotLight = false;
        pFlameSphere->textures[0] = "red-hot-fire-flames-1024x1024.bmp";
        ::g_vecMeshesToDraw.push_back(pFlameSphere);
    }

    sModelDrawInfo lightDrawInfo;
    if (::g_pMeshManager->LoadModelIntoVAO("assets/models/Lightbulb/lightbulb (unit size, inverted normals).ply", lightDrawInfo, program))
    {
        //std::cout << "Loaded " << lightDrawInfo.meshName << " OK" << std::endl;

        sMesh* pLightBulb = new sMesh();
        pLightBulb->modelFileName = "assets/models/Lightbulb/lightbulb (unit size, inverted normals).ply";
        pLightBulb->uniqueFriendlyName = "LightBulb";
        pLightBulb->textures[0] = "solid_white.bmp";
        pLightBulb->blendRatio[0] = 1.0f;
        pLightBulb->bOverrideObjectColour = true;
        pLightBulb->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        pLightBulb->bIsWireframe = true;
        pLightBulb->uniformScale = 3.0f;
        pLightBulb->bDoNotLight = true;
        pLightBulb->bIsVisible = false;
        ::g_vecMeshesToDraw.push_back(pLightBulb);
    }

    std::vector< std::pair<std::string, bool> > vecLODBunnies;
    vecLODBunnies.push_back(std::pair<std::string, bool>("bun_zipper_LOD_res1_n_uv (69,451 tris).ply", false));
    vecLODBunnies.push_back(std::pair<std::string, bool>("bun_zipper_LOD_res2_n_uv (16,301 tris).ply", false));
    vecLODBunnies.push_back(std::pair<std::string, bool>("bun_zipper_LOD_res3_n_uv (3,851 tris).ply", false));
    vecLODBunnies.push_back(std::pair<std::string, bool>("bun_zipper_LOD_res4_n_uv (948 tris).ply", false));

    if (!::g_pMeshManager->LoadModelsIntoVAO("assets/models/", vecLODBunnies, program))
    {
        //std::cout << "Some of the LOD bunnies didn't load" << std::endl;
    }
    else
    {
        //std::cout << "LOD bunnies loaded OK" << std::endl;
    }


    // Spiderman: This model has parts all relative to 
    //  the origin in model space
    std::vector< std::pair<std::string, bool> > vecSpiderManMeshes;
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_body_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_head_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_Hips_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_Left_arm_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_Left_hand_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_Left_leg_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_Right_arm_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_Right_hand_xyz_n_uv.ply", false));
    vecSpiderManMeshes.push_back(std::pair<std::string, bool>("legospiderman_Right_leg_xyz_n_uv.ply", false));


    if ( ! ::g_pMeshManager->LoadModelsIntoVAO("assets/models/LEGO_Spiderman/", vecSpiderManMeshes, program) )
    {
        std::cout << "Some spiderman meshes didn't load:" << std::endl;
        for (std::pair<std::string, bool> filePair : vecSpiderManMeshes)
        {
            if (filePair.second == false)
            {
                std::cout << "\t" << filePair.first << std::endl;
            }
        }
    }
    else
    {
        //std::cout << "Spiderman meshes loaded OK" << std::endl;
    }


    // Add a mesh with all the child meshes of spiderman


    {// LEGO Spiderman mesh
        sMesh* p_legospiderman_Hips = new sMesh();
        p_legospiderman_Hips->modelFileName = "legospiderman_Hips_xyz_n_uv.ply";
        p_legospiderman_Hips->uniqueFriendlyName = "legospiderman_Hips";
        p_legospiderman_Hips->textures[0] = "SpidermanUV_square.bmp";
        p_legospiderman_Hips->blendRatio[0] = 1.0f;
        p_legospiderman_Hips->uniformScale = 0.5f;
//
        p_legospiderman_Hips->positionXYZ = glm::vec3(-55.0f, -30.0f, 0.0f);

        p_legospiderman_Hips->rotationEulerXYZ.x = -90.0f;
//        p_legospiderman_Hips->rotationEulerXYZ.y = +90.0f;
        p_legospiderman_Hips->rotationEulerXYZ.z = +90.0f;
        ::g_vecMeshesToDraw.push_back(p_legospiderman_Hips);


        sMesh* legospiderman_body = new sMesh();
        legospiderman_body->modelFileName = "legospiderman_body_xyz_n_uv.ply";
        legospiderman_body->uniqueFriendlyName = "legospiderman_body";
        legospiderman_body->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_body->blendRatio[0] = 1.0f;
        
        // Connect the body to the hips as a "child" mesh
        p_legospiderman_Hips->vec_pChildMeshes.push_back(legospiderman_body);

        sMesh* legospiderman_head = new sMesh();
        legospiderman_head->modelFileName = "legospiderman_head_xyz_n_uv.ply";
        legospiderman_head->uniqueFriendlyName = "legospiderman_head";
        legospiderman_head->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_head->blendRatio[0] = 1.0f;
        
        p_legospiderman_Hips->vec_pChildMeshes.push_back(legospiderman_head);

        sMesh* legospiderman_Left_arm = new sMesh();
        legospiderman_Left_arm->modelFileName = "legospiderman_Left_arm_xyz_n_uv.ply";
        legospiderman_Left_arm->uniqueFriendlyName = "legospiderman_Left_arm";
        legospiderman_Left_arm->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_Left_arm->blendRatio[0] = 1.0f;
        //
        // (0, 0, -2.55)
        //        
        legospiderman_Left_arm->matPreParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.55f));

        //legospiderman_Left_arm->rotationEulerXYZ.y = -45.0f;

        // Rotate the arm (at the shoulder)
        legospiderman_Left_arm->matPostParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 2.55f));

        p_legospiderman_Hips->vec_pChildMeshes.push_back(legospiderman_Left_arm);

        sMesh* legospiderman_Left_hand = new sMesh();
        legospiderman_Left_hand->modelFileName = "legospiderman_Left_hand_xyz_n_uv.ply";
        legospiderman_Left_hand->uniqueFriendlyName = "legospiderman_Left_hand";
        legospiderman_Left_hand->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_Left_hand->blendRatio[0] = 1.0f;
        //
        // To move this to the origin, I do this:
        //  Translate (-0.15, -0.9, -2.25)
        //  Rotate -5 degrees around Z
        //  Rotate -27 degrees around Y
        //
        legospiderman_Left_hand->matPreParentRelative = glm::mat4(1.0f);
        //  Rotate -27 degrees around Y
        legospiderman_Left_hand->matPreParentRelative 
            = glm::rotate(legospiderman_Left_hand->matPreParentRelative, 
                          glm::radians(-27.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //  Rotate -5 degrees around Z
        legospiderman_Left_hand->matPreParentRelative 
            = glm::rotate(legospiderman_Left_hand->matPreParentRelative, 
                          glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //  Translate (-0.15, -0.9, -2.25)
        legospiderman_Left_hand->matPreParentRelative
            = glm::translate(legospiderman_Left_hand->matPreParentRelative, glm::vec3(-0.15f, -0.9f, -2.25f));

        // To move it back, we do the reverse
        //  Rotate 27 degrees around Y
        //  Rotate 5 degrees around Z
        //  Translate (0.15, 0.9, 2.25)
        //
        // ...but it's mathematically in the reverse
        legospiderman_Left_hand->matPostParentRelative = glm::mat4(1.0f);
        //  Translate (0.15, 0.9, 2.25)
        legospiderman_Left_hand->matPostParentRelative
            = glm::translate(legospiderman_Left_hand->matPostParentRelative, glm::vec3(0.15f, 0.9f, 2.25f));
        //  Rotate 5 degrees around Z
        legospiderman_Left_hand->matPostParentRelative
            = glm::rotate(legospiderman_Left_hand->matPostParentRelative,
                          glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //  Rotate 27 degrees around Y
        legospiderman_Left_hand->matPostParentRelative
            = glm::rotate(legospiderman_Left_hand->matPostParentRelative,
                          glm::radians(27.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//        p_legospiderman_Hips->vec_pChildMeshes.push_back(legospiderman_Left_hand);
        legospiderman_Left_arm->vec_pChildMeshes.push_back(legospiderman_Left_hand);

        // Add a mesh that's at the "tip" of Spiderman's hand
        // Using meshlab, that's (1.0, 0.5, 0.0) from
        //  the base of the hand mesh
        sMesh* pMeshAtHandTip = new sMesh();
        // I'm using the sphere, but it doesn't have to be anything
        pMeshAtHandTip->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
        pMeshAtHandTip->bIsWireframe = true;
        pMeshAtHandTip->bIsVisible = false;
        pMeshAtHandTip->uniformScale = 0.5f;
        pMeshAtHandTip->uniqueFriendlyName = "Forward Kinematic tip of hand";
        pMeshAtHandTip->textures[0] = "SpidermanUV_square.bmp";
        pMeshAtHandTip->blendRatio[0] = 1.0f;
        //

        // Translate: (0.0, -1.0, -2.0)
        pMeshAtHandTip->matPreParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -2.0f));
        // The reverse of that...
        // ...which we don't really need because we aren't altering it
        pMeshAtHandTip->matPostParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 2.0f));

        // note it's added to his HAND, not arm
        legospiderman_Left_hand->vec_pChildMeshes.push_back(pMeshAtHandTip);

        sMesh* legospiderman_Right_arm = new sMesh();
        legospiderman_Right_arm->modelFileName = "legospiderman_Right_arm_xyz_n_uv.ply";
        legospiderman_Right_arm->uniqueFriendlyName = "legospiderman_Right_arm";
        legospiderman_Right_arm->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_Right_arm->blendRatio[0] = 1.0f;
        p_legospiderman_Hips->vec_pChildMeshes.push_back(legospiderman_Right_arm);

        legospiderman_Right_arm->matPreParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -2.55f));

        //legospiderman_Left_arm->rotationEulerXYZ.y = -45.0f;

        // Rotate the arm (at the shoulder)
        legospiderman_Right_arm->matPostParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 2.55f));

        sMesh* legospiderman_Right_hand = new sMesh();
        legospiderman_Right_hand->modelFileName = "legospiderman_Right_hand_xyz_n_uv.ply";
        legospiderman_Right_hand->uniqueFriendlyName = "legospiderman_Right_hand";
        legospiderman_Right_hand->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_Right_hand->blendRatio[0] = 1.0f;
        legospiderman_Right_arm->vec_pChildMeshes.push_back(legospiderman_Right_hand);

        sMesh* legospiderman_Left_leg = new sMesh();
        legospiderman_Left_leg->modelFileName = "legospiderman_Left_leg_xyz_n_uv.ply";
        legospiderman_Left_leg->uniqueFriendlyName = "legospiderman_Left_leg";
        legospiderman_Left_leg->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_Left_leg->blendRatio[0] = 1.0f;

        // Translate: (0.0, -1.0, -2.0)
        legospiderman_Left_leg->matPreParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.1f));
        // The reverse of that...
        // ...which we don't really need because we aren't altering it
        legospiderman_Left_leg->matPostParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.1f));

        p_legospiderman_Hips->vec_pChildMeshes.push_back(legospiderman_Left_leg);

        sMesh* legospiderman_Right_leg = new sMesh();
        legospiderman_Right_leg->modelFileName = "legospiderman_Right_leg_xyz_n_uv.ply";
        legospiderman_Right_leg->uniqueFriendlyName = "legospiderman_Right_leg";
        legospiderman_Right_leg->textures[0] = "SpidermanUV_square.bmp";
        legospiderman_Right_leg->blendRatio[0] = 1.0f;

        // Translate: (0.0, -1.0, -2.0)
        legospiderman_Right_leg->matPreParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.1f));
        // The reverse of that...
        // ...which we don't really need because we aren't altering it
        legospiderman_Right_leg->matPostParentRelative
            = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.1f));

        p_legospiderman_Hips->vec_pChildMeshes.push_back(legospiderman_Right_leg);

    }

    // House
    ::g_pAssimp->SetBasePath("assets/models/House");
    if (::g_pAssimp->Load3DModelFile("house_all_OBJ.obj", postProcessingNormalMaps))
    {
        AH::cScene* pSceneWithHouse = ::g_pAssimp->pGetScenePointer("house_all_OBJ.obj");
        if (pSceneWithHouse)
        {
            // Find "Walls"
            AH::cMesh* pHouseWall = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithHouse->vecMeshes)
            {
                if (pCurMesh->name == "Walls")
                {
                    pHouseWall = pCurMesh;
                    break;
                }
            }
            if (pHouseWall)
            {
                sModelDrawInfo houseDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pHouseWall, houseDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    houseDrawInfo.meshName = "house_walls_OBJ.obj";

                    houseDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(houseDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pStool)
        }//if (pSceneWithStool)

        sMesh* pHouseWalls = new sMesh();
        pHouseWalls->modelFileName = "house_walls_OBJ.obj";
        pHouseWalls->uniqueFriendlyName = "HouseWalls";
        pHouseWalls->textures[0] = "House_UV2_BaseColor.1002_Flipped.bmp";
        pHouseWalls->blendRatio[0] = 1.0f;
        //
        pHouseWalls->normalMap = "House_UV2_Normal.1002_Flipped.bmp";

        pHouseWalls->uniformScale = 10.0f;
        pHouseWalls->positionXYZ = glm::vec3(0.0f, -3.25f, 0.0f);
        ::g_vecMeshesToDraw.push_back(pHouseWalls);

        if (pSceneWithHouse)
        {
            // Find "roof"
            AH::cMesh* pHouseRoof = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithHouse->vecMeshes)
            {
                if (pCurMesh->name == "roof")
                {
                    pHouseRoof = pCurMesh;
                    break;
                }
            }
            if (pHouseRoof)
            {
                sModelDrawInfo houseDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pHouseRoof, houseDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    houseDrawInfo.meshName = "house_roof_OBJ.obj";

                    houseDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(houseDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pStool)
        }//if (pSceneWithStool)

        sMesh* pHouseRoof = new sMesh();
        pHouseRoof->modelFileName = "house_roof_OBJ.obj";
        pHouseRoof->uniqueFriendlyName = "HouseRoof";
        pHouseRoof->textures[0] = "House_UV4_BaseColor.1004_Flipped.bmp";
        pHouseRoof->blendRatio[0] = 1.0f;
        //
        pHouseRoof->normalMap = "House_UV4_Normal.1004_Flipped.bmp";

        //pHouseRoof->uniformScale = 10.0f;
        //pHouseRoof->positionXYZ = glm::vec3(50.0f, -30.0f, 0.0f);

        // Add as child mesh
        pHouseWalls->vec_pChildMeshes.push_back(pHouseRoof);

        if (pSceneWithHouse)
        {
            // Find "roof"
            AH::cMesh* pHouseFloor = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithHouse->vecMeshes)
            {
                if (pCurMesh->name == "floor")
                {
                    pHouseFloor = pCurMesh;
                    break;
                }
            }
            if (pHouseFloor)
            {
                sModelDrawInfo houseDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pHouseFloor, houseDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    houseDrawInfo.meshName = "house_floor_OBJ.obj";

                    houseDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(houseDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pStool)
        }//if (pSceneWithStool)

        sMesh* pHouseFloor = new sMesh();
        pHouseFloor->modelFileName = "house_floor_OBJ.obj";
        pHouseFloor->uniqueFriendlyName = "HouseFloor";
        pHouseFloor->textures[0] = "House_UV3_BaseColor.1003_Flipped.bmp";
        pHouseFloor->blendRatio[0] = 1.0f;
        //
        pHouseFloor->normalMap = "House_UV3_Normal.1003_Flipped.bmp";

        //pHouseFloor->uniformScale = 10.0f;
        //pHouseFloor->positionXYZ = glm::vec3(50.0f, -30.0f, 0.0f);

        // Add as child mesh
        pHouseWalls->vec_pChildMeshes.push_back(pHouseFloor);

        if (pSceneWithHouse)
        {
            // Find "Glass"
            AH::cMesh* pHouseGlass = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithHouse->vecMeshes)
            {
                if (pCurMesh->name == "Glass")
                {
                    pHouseGlass = pCurMesh;
                    break;
                }
            }
            if (pHouseGlass)
            {
                sModelDrawInfo houseDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pHouseGlass, houseDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    houseDrawInfo.meshName = "house_glass_OBJ.obj";

                    houseDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(houseDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pStool)
        }//if (pSceneWithStool)

        sMesh* pHouseGlass = new sMesh();
        pHouseGlass->modelFileName = "house_glass_OBJ.obj";
        pHouseGlass->uniqueFriendlyName = "HouseGlass";
        pHouseGlass->textures[0] = "House_glass_BaseColor.1005_Flipped.bmp";
        pHouseGlass->blendRatio[0] = 1.0f;
        //
        //pHouseGlass->normalMap = "House_UV3_Normal.1003_Flipped.bmp";
        pHouseGlass->alphaTransparency = 0.35f;
        //pHouseGlass->uniformScale = 10.0f;
        //pHouseGlass->positionXYZ = glm::vec3(50.0f, -30.0f, 0.0f);

        // Add as child mesh
        pHouseWalls->vec_pChildMeshes.push_back(pHouseGlass);

        if (pSceneWithHouse)
        {
            // Find "Windowdoorframe"
            AH::cMesh* pHouseFrames = NULL;
            for (AH::cMesh* pCurMesh : pSceneWithHouse->vecMeshes)
            {
                if (pCurMesh->name == "Windowdoorframe")
                {
                    pHouseFrames = pCurMesh;
                    break;
                }
            }
            if (pHouseFrames)
            {
                sModelDrawInfo houseDrawInfo;
                if (Convert_AssimpHelperMesh_to_sModelDrawInfo(pHouseFrames, houseDrawInfo))
                {
                    // Change the name to the file name (it's "barrel" in the OBJ model
                    houseDrawInfo.meshName = "house_frames_OBJ.obj";

                    houseDrawInfo.calculateExtents();

                    ::g_pMeshManager->AddModelToVAO(houseDrawInfo, program);
                }
                else
                {
                    std::cout << "Error: Could not convert assimp model to VAO model" << std::endl;
                }
            }//if (pStool)
        }//if (pSceneWithStool)

        sMesh* pHouseFrames = new sMesh();
        pHouseFrames->modelFileName = "house_frames_OBJ.obj";
        pHouseFrames->uniqueFriendlyName = "HouseFrames";
        pHouseFrames->textures[0] = "House_UV1_BaseColor.1001_Flipped.bmp";
        pHouseFrames->blendRatio[0] = 1.0f;
        //
        pHouseFrames->normalMap = "House_UV1_Normal.1001_Flipped.bmp";

        //pHouseFrames->uniformScale = 10.0f;
        //pHouseFrames->positionXYZ = glm::vec3(50.0f, -30.0f, 0.0f);

        // Add as child mesh
        pHouseWalls->vec_pChildMeshes.push_back(pHouseFrames);
    }


    // Deferred rendering "full screen quad" (FSQ) mesh
    {
        //sModelDrawInfo FSQMesh;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/2x2_Quad_for_FSQuad_xyz_n_uv.ply",
        //    FSQMesh, program);
        //std::cout << FSQMesh.numberOfVertices << " vertices loaded" << std::endl;
        //
        sMesh* pFSQMesh = new sMesh();
        pFSQMesh->modelFileName = "assets/models/2x2_Quad_for_FSQuad_xyz_n_uv.ply";
        pFSQMesh->uniqueFriendlyName = "Full_Screen_Quad";
        pFSQMesh->bIsVisible = false;
        // We're saving this into the vector of meshes, but
        //  we could not do this and keep the pointer to directly call
  
        ::g_vecMeshesToDraw.push_back(pFSQMesh);
    }

    //{
    //    sModelDrawInfo sphereMesh;
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_xyz_N_uv.ply",
    //        sphereMesh, program);
    //    std::cout << sphereMesh.meshName << ": " << sphereMesh.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo smoothSphereMesh;
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Smooth_Sphere.ply",
    //        smoothSphereMesh, program);
    //    std::cout << smoothSphereMesh.meshName << ": " << smoothSphereMesh.numberOfVertices << " vertices loaded" << std::endl;
    //}

    {// PhysX "Hello world" snippet example object
        //  Sphere of radius 3:   PhysX_Spere_radius_of_3.0f_xyz_n_uv.ply
        //  Cube of 1 / 2 extent 2 : PhysX_Cube_half-extent_of_2.0f_xyz_n_uv.ply

        //sModelDrawInfo PhysXSphereRaidus3;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/PhysX_Spere_radius_of_3.0f_xyz_n_uv.ply",
        //    PhysXSphereRaidus3, program);
        //std::cout << PhysXSphereRaidus3.numberOfVertices << " vertices loaded" << std::endl;
        
        sMesh* pPhysXSphere = new sMesh();
        pPhysXSphere->modelFileName = "assets/models/PhysX_Spere_radius_of_3.0f_xyz_n_uv.ply";
        pPhysXSphere->uniqueFriendlyName = "pPhysXSphere";
        pPhysXSphere->textures[0] = "UV_Test_750x750.bmp";
        pPhysXSphere->blendRatio[0] = 1.0f;
        pPhysXSphere->bIsVisible = false;
        ::g_vecMeshesToDraw.push_back(pPhysXSphere);

        //sModelDrawInfo PhysXCubeHalfExtent2;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/PhysX_Cube_half-extent_of_2.0f_xyz_n_uv.ply",
        //    PhysXCubeHalfExtent2, program);
        //std::cout << PhysXCubeHalfExtent2.numberOfVertices << " vertices loaded" << std::endl;
        {
            sMesh* pPhysXCube = new sMesh();
            pPhysXCube->modelFileName = "assets/models/PhysX_Cube_half-extent_of_2.0f_xyz_n_uv.ply";
            pPhysXCube->uniqueFriendlyName = "pPhysXCube";
            pPhysXCube->textures[0] = "Canadian_Flag_Texture.bmp";
            pPhysXCube->blendRatio[0] = 1.0f;
            pPhysXCube->bIsVisible = false;
            ::g_vecMeshesToDraw.push_back(pPhysXCube);
        }
        
        {
            sMesh* pPhysXCube = new sMesh();
            pPhysXCube->modelFileName = "assets/models/PhysX_Cube_half-extent_of_1.0f_xyz_n_uv.ply";
            pPhysXCube->uniqueFriendlyName = "pPhysXCube1";
            pPhysXCube->textures[0] = "Canadian_Flag_Texture.bmp";
            pPhysXCube->blendRatio[0] = 1.0f;
            pPhysXCube->bIsVisible = false;
            ::g_vecMeshesToDraw.push_back(pPhysXCube);
        }

    }// PhysX "Hello world" snippet example object




    // Off screen quad
    //sModelDrawInfo flat10x10Mesh;
    //::g_pMeshManager->LoadModelIntoVAO("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply",
    //    flat10x10Mesh, program);
    //std::cout << flat10x10Mesh.numberOfVertices << " vertices loaded" << std::endl;
    
    //sMesh* pOffScreenViewMesh = new sMesh();
    //pOffScreenViewMesh->modelFileName = "assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply";
    //pOffScreenViewMesh->uniqueFriendlyName = "WareHouseView";
    //pOffScreenViewMesh->positionXYZ = glm::vec3(0.0f, -250.0f, 100.0f);
    //pOffScreenViewMesh->rotationEulerXYZ = glm::vec3(0.0f);
    //pOffScreenViewMesh->textures[0] = "dua-lipa-promo.bmp";
    //pOffScreenViewMesh->blendRatio[0] = 1.0f;
    //pOffScreenViewMesh->uniformScale = 10.0f;
    //pOffScreenViewMesh->bDoNotLight = true;
    //::g_vecMeshesToDraw.push_back(pOffScreenViewMesh);


    // Load a few soft bodies (two flags and a bunny)
    {
        //sModelDrawInfo softBodyFlagMesh;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply",
        //    softBodyFlagMesh, program);
        //std::cout << softBodyFlagMesh.numberOfVertices << " vertices loaded" << std::endl;

        //sModelDrawInfo softBodyFlagMesh1;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/30x30_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply",
        //    softBodyFlagMesh1, program);
        //std::cout << softBodyFlagMesh1.numberOfVertices << " vertices loaded" << std::endl;

        //sModelDrawInfo softBodyFlagMesh2;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_82,944_faces.ply",
        //    softBodyFlagMesh2, program);
        //std::cout << softBodyFlagMesh2.numberOfVertices << " vertices loaded" << std::endl;

        //sModelDrawInfo softBodyTube;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/tube_xyz_n_uv.ply",
        //    softBodyTube, program);
        //std::cout << softBodyTube.numberOfVertices << " vertices loaded" << std::endl;

        //sModelDrawInfo softBodyFlagMesh3;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_497,656_faces.ply",
        //    softBodyFlagMesh3, program);
        //std::cout << softBodyFlagMesh3.numberOfVertices << " vertices loaded" << std::endl;

        //sModelDrawInfo bun_zipper_res4_larger_Mesh;
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res4_larger_for_soft_body.ply",
        //    bun_zipper_res4_larger_Mesh, program);
        //std::cout << bun_zipper_res4_larger_Mesh.meshName << " : " << bun_zipper_res4_larger_Mesh.numberOfVertices << " vertices loaded" << std::endl;

       // sModelDrawInfo softBodyBunnyMeshDrawInfo;
       // ::g_pMeshManager->FindDrawInfoByModelName("assets/models/bun_zipper_res4_larger_for_soft_body.ply", softBodyBunnyMeshDrawInfo);
       // ::g_pMeshManager->CloneMeshToDynamicVAO("SoftBodyBunnyMesh_01", softBodyBunnyMeshDrawInfo, program);
       // 
       //glm::mat4 matModel = glm::mat4(1.0f);
       //std::string error;
       //cSoftBodyVerlet* pSB_Bunny = ::g_pPhysicEngine->createSoftBodyFromMesh("SoftBodyBunnyMesh_01", matModel, error);

       //// Add a force (gravity) to this bunny
       //pSB_Bunny->acceleration = glm::vec3(0.0f, -1.0f, 0.0f);

       //pSB_Bunny->CreateRandomBracing(1000, 5.0f);
       
//        // DEBUG
////        {
//            sMesh* pSoftBodyBunny = new sMesh();
//            pSoftBodyBunny->modelFileName = "SoftBodyBunnyMesh_01";
//            pSoftBodyBunny->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
//            pSoftBodyBunny->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//            pSoftBodyBunny->bOverrideObjectColour = true;
//            pSoftBodyBunny->bIsWireframe = true;
//            //            pCanadianFlag->rotationEulerXYZ = glm::vec3(0.0f);
//            //            pCanadianFlag->rotationEulerXYZ.y = 180.0f;
//            //pSoftBodyBunny->textures[0] = "Canadian_Flag_Texture.bmp";
//            //pSoftBodyBunny->blendRatio[0] = 1.0f;
//            //pSoftBodyBunny->uniformScale = 1.0f;
//            //pSoftBodyBunny->bDoNotLight = true;
//            ::g_vecMeshesToDraw.push_back(pSoftBodyBunny);
////
////
////
////
//            sModelDrawInfo softBodyCanadianFlagMesh;
////            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply", softBodyCanadianFlagMesh);
//            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/30x30_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply", softBodyCanadianFlagMesh);
////            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_82,944_faces.ply", softBodyCanadianFlagMesh);
////            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_497,656_faces.ply", softBodyCanadianFlagMesh);
////            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/tube_xyz_n_uv.ply", softBodyCanadianFlagMesh);
//            ::g_pMeshManager->CloneMeshToDynamicVAO("CanadaFlag_SoftBodyMesh", softBodyCanadianFlagMesh, program);
//
//
//            sMesh* pCanadianFlag = new sMesh();
//            //            pCanadianFlag->modelFileName = "Canadian_Flag_Mesh";
//            pCanadianFlag->modelFileName = "CanadaFlag_SoftBodyMesh";
//            pCanadianFlag->uniqueFriendlyName = "Canadian_Flag";
//
////            pCanadianFlag->positionXYZ = glm::vec3(0.0f, +30.0f, 0.0f);
////            pCanadianFlag->rotationEulerXYZ = glm::vec3(0.0f, 0.0f, -90.0f);
//
////            pCanadianFlag->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
////            pCanadianFlag->bOverrideObjectColour = true;
//            pCanadianFlag->bIsWireframe = true;
//            //            pCanadianFlag->rotationEulerXYZ = glm::vec3(0.0f);
//            //            pCanadianFlag->rotationEulerXYZ.y = 180.0f;
//            pCanadianFlag->textures[0] = "Canadian_Flag_Texture.bmp";
//            pCanadianFlag->blendRatio[0] = 1.0f;
//            pCanadianFlag->uniformScale = 1.0f;
//            pCanadianFlag->bDoNotLight = true;
////            pCanadianFlag->bIsVisible = false;
//            ::g_vecMeshesToDraw.push_back(pCanadianFlag);
//
//
//            glm::mat4 matModelCF = glm::mat4(1.0f);
//    //        matModelCF = pCanadianFlag->calcMatModel();
//
//            cSoftBodyVerlet* pSB_CanadianFlag = ::g_pPhysicEngine->createSoftBodyFromMesh("CanadaFlag_SoftBodyMesh", matModelCF, error);
//            pSB_CanadianFlag->acceleration = glm::vec3(0.0f, -3.0f, 0.0f);
//
//            // ********************************************************
//            // START OF: Viper with flag set up
//
//            ::g_pViperFlagConnector = new cViperFlagConnector();
//
//            sMesh* pViperPlayer = g_pFindMeshByFriendlyName("New_Viper_Player");
//            ::g_pViperFlagConnector->setViperModel(pViperPlayer);
//
//            ::g_pViperFlagConnector->setFlagSoftBody(pSB_CanadianFlag);
//
//            ::g_pViperFlagConnector->ConnectViperToFlag();
//
//            // END OF: Viper with flag set up
//            // ********************************************************
//
//
//
//           //  Add the sphere that they soft bodies are hitting. 
//           //  NOTE: The collision is HARD CODED (in the ApplyCollision() method of cSoftBodyVerlet.cpp).
//           //  This object is here to make the collision more visible:
//            
//            glm::vec3 sphereCentre = glm::vec3(-1.0f, -30.0f, 1.0f);
//            float sphereRadius = 15.0f;
//            
//            sMesh* pSoftBodyCollisionSphere = new sMesh();
//            pSoftBodyCollisionSphere->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
//            pSoftBodyCollisionSphere->positionXYZ = sphereCentre;
//            pSoftBodyCollisionSphere->uniformScale = sphereRadius;
//            pSoftBodyCollisionSphere->textures[0] = "Grey_Brick_Wall_Texture.bmp";
//            pSoftBodyCollisionSphere->blendRatio[0] = 1.0f;
//            ::g_vecMeshesToDraw.push_back(pSoftBodyCollisionSphere);
//
//            sMesh* pSoftBodyCollisionSphereWireFrame = new sMesh();
//            pSoftBodyCollisionSphereWireFrame->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
//            pSoftBodyCollisionSphereWireFrame->positionXYZ = pSoftBodyCollisionSphere->positionXYZ;
//            pSoftBodyCollisionSphereWireFrame->bIsWireframe = true;
//            pSoftBodyCollisionSphereWireFrame->textures[0] = "solid_black.bmp";
//            pSoftBodyCollisionSphereWireFrame->blendRatio[0] = 1.0f;
//            pSoftBodyCollisionSphereWireFrame->bDoNotLight = true;
//            pSoftBodyCollisionSphereWireFrame->uniformScale = pSoftBodyCollisionSphere->uniformScale * 1.001f;
//            ::g_vecMeshesToDraw.push_back(pSoftBodyCollisionSphereWireFrame);
//
//            //sModelDrawInfo softBodyCanadianFlagMesh;
//            //::g_pMeshManager->FindDrawInfoByModelName("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply", softBodyCanadianFlagMesh);
//            //::g_pMeshManager->CloneMeshToDynamicVAO("ChineseFlag_SoftBodyMesh", softBodyBunnyMeshDrawInfo, program);
//            //sMesh* pChineseFlag = new sMesh();
//            //pChineseFlag->modelFileName = "ChineseFlag_SoftBodyMesh";
//            //pChineseFlag->positionXYZ = glm::vec3(50.0f, 0.0f, 200.0f);
//            //pChineseFlag->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
//            //pChineseFlag->bOverrideObjectColour = true;
//            //pChineseFlag->bIsWireframe = true;
//            //pChineseFlag->rotationEulerXYZ = glm::vec3(0.0f);
//            //pChineseFlag->rotationEulerXYZ.y = 180.0f;
//            //pChineseFlag->textures[0] = "Chinese_Flag_Texture.bmp";
//            //pChineseFlag->blendRatio[0] = 1.0f;
//            //pChineseFlag->uniformScale = 5.0f;
//            //::g_vecMeshesToDraw.push_back(pChineseFlag);
////
////
////        }
//
//        // Add a "ground" for the bunny to hit
//        sMesh* pPlaneForSoftBodiesToHit = new sMesh();
//        pPlaneForSoftBodiesToHit->modelFileName = "assets/models/Flat_Plane_xyz_N_uv.ply";
//        pPlaneForSoftBodiesToHit->positionXYZ = glm::vec3(0.0f, -50.5f, 0.0f);
//        pPlaneForSoftBodiesToHit->textures[0] = "Grey_Brick_Wall_Texture.bmp";
//        pPlaneForSoftBodiesToHit->blendRatio[0] = 1.0f;
//        pPlaneForSoftBodiesToHit->alphaTransparency = 0.8f;
////        pPlaneForSoftBodiesToHit->bIsVisible = false;
//        ::g_vecMeshesToDraw.push_back(pPlaneForSoftBodiesToHit);
//
//
    
    }// ENDOF: Load a few soft bodies (two flags and a bunny)

    //{
    //    sModelDrawInfo cubeMinXYZ_at_OriginInfo;
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Cube_MinXYZ_at_Origin_xyz_n_uv.ply",
    //        cubeMinXYZ_at_OriginInfo, program);
    //    std::cout << cubeMinXYZ_at_OriginInfo.meshName << ": " << cubeMinXYZ_at_OriginInfo.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo warehouseModel;
    //    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Warehouse_xyz_n.ply",
    //    ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Warehouse_xyz_n_uv (wall removed).ply",
    //        warehouseModel, program);
    //    std::cout << warehouseModel.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo tankModel;
    //    //    pMeshManager->LoadModelIntoVAO("assets/models/Low_Poly_Tank_Model_3D_model.ply", 
    //    pMeshManager->LoadModelIntoVAO("assets/models/Low_Poly_Tank_Model_3D_model_xyz_n_uv.ply",
    //        tankModel, program);
    //    std::cout << tankModel.meshName << " : " << tankModel.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo terrainModel;
    //    ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply",
    //        terrainModel, program);
    //    std::cout << terrainModel.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo bunnyModel;
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res2_10x_size_xyz_N_uv.ply",
    //        bunnyModel, program);
    //    std::cout << bunnyModel.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo platPlaneDrawInfo;
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Flat_Plane_xyz_N_uv.ply",
    //        platPlaneDrawInfo, program);
    //    std::cout << platPlaneDrawInfo.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo sphereShadowMesh;
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_Flat_Shadow_xyz_N_uv.ply",
    //        sphereShadowMesh, program);
    //    std::cout << sphereShadowMesh.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo newViperModelInfo;
    //    ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Viper_MkVII_xyz_n_uv.ply",
    //        newViperModelInfo, program);
    //    std::cout << newViperModelInfo.numberOfVertices << " vertices loaded" << std::endl;
    //}

    //{
    //    sModelDrawInfo cheeseMesh;
    //    ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Cheese_xyz_n_uv.ply",
    //        cheeseMesh, program);
    //    std::cout << cheeseMesh.numberOfVertices << " vertices loaded" << std::endl;
    //}

//    // Add a bunch of bunny rabbits
//    float boxLimit = 500.0f;
//    float boxStep = 50.0f;
//    unsigned int ID_count = 0;
//    for (float x = -boxLimit; x <= boxLimit; x += boxStep)
//    {
//        for (float z = -(2.0f * boxLimit); z <= boxLimit; z += boxStep)
//        {
//            sMesh* pBunny = new sMesh();
//            //            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_only.ply";
//            //            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_N_only.ply";
//            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_N_uv.ply";
//            pBunny->positionXYZ = glm::vec3(x, -35.0f, z);
//            pBunny->uniformScale = 2.0f;
//            pBunny->objectColourRGBA
//                = glm::vec4(::g_getRandomFloat(0.0f, 1.0f),
//                    ::g_getRandomFloat(0.0f, 1.0f),
//                    ::g_getRandomFloat(0.0f, 1.0f),
//                    1.0f);
//            // Set some transparency
//            pBunny->alphaTransparency = ::g_getRandomFloat(0.25f, 1.0f);
//            //            pBunny->alphaTransparency = 0.0f;
//            std::stringstream ssName;
//            ssName << "Bunny_" << ID_count;
//            pBunny->uniqueFriendlyName = ssName.str();
//            ID_count++;
//
//            ::g_vecMeshesToDraw.push_back(pBunny);
//        }
//    }//for (float x = -boxLimit...


    //// Place a bunny somewhere else in the scene
    //sMesh* pBunny_15 = ::g_pFindMeshByFriendlyName("Bunny_15");
    //if (pBunny_15)
    //{
    //    pBunny_15->positionXYZ = glm::vec3(-50.0f, 15.0f, 30.0f);
    //    pBunny_15->rotationEulerXYZ.x = glm::radians(180.0f);
    //    pBunny_15->uniformScale = 10.0f;
    //}
    //// Place a bunny somewhere else in the scene
    //sMesh* pBunny_27 = ::g_pFindMeshByFriendlyName("Bunny_27");
    //if (pBunny_27)
    //{
    //    pBunny_27->positionXYZ = glm::vec3(75.0f, 10.0f, -45.0f);
    //    pBunny_27->rotationEulerXYZ.x = glm::radians(180.0f);
    //    pBunny_27->uniformScale = 10.0f;
    //}


    {
        sMesh* pSkySphere = new sMesh();
//        pSkySphere->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
        pSkySphere->modelFileName = "assets/models/Smooth_Sphere.ply";
        pSkySphere->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
        pSkySphere->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        //       pSkySphere->bIsWireframe = true;
        pSkySphere->bOverrideObjectColour = true;
//        pSkySphere->uniformScale = 25.0f;
        pSkySphere->uniqueFriendlyName = "SkySphere";
        pSkySphere->textures[0] = "bad_bunny_1920x1080.bmp";
        pSkySphere->blendRatio[0] = 1.0f;
        pSkySphere->bIsVisible = false;
        ::g_vecMeshesToDraw.push_back(pSkySphere);
    }



    {
        //    ____                _            __                   _     
        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
        //                                                                
        sMesh* pWarehouse = new sMesh();
        //        pWarehouse->modelFileName = "assets/models/Warehouse_xyz_n.ply";
        pWarehouse->modelFileName = "assets/models/Warehouse_xyz_n_uv (wall removed).ply";
        pWarehouse->positionXYZ = glm::vec3(-50.0f, -48.0f, 0.0f);
        pWarehouse->rotationEulerXYZ.y = +90.0f;
        pWarehouse->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        //pWarehouse->bIsWireframe = true;
        pWarehouse->bOverrideObjectColour = true;
        pWarehouse->uniqueFriendlyName = "Warehouse";
        //
        pWarehouse->textures[0] = "bad_bunny_1920x1080.bmp";

        ::g_vecMeshesToDraw.push_back(pWarehouse);

        //    ____  _               _                  _     _           _   
        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
        //                |___/                           |__/               
        ::g_pPhysicEngine->addTriangleMesh(
            "assets/models/Warehouse_xyz_n_uv (wall removed).ply",
            pWarehouse->positionXYZ,
            pWarehouse->rotationEulerXYZ,
            pWarehouse->uniformScale);

    }

    /*
    {
        sMesh* pTerrain = new sMesh();
        pTerrain->modelFileName = "assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply";
        pTerrain->positionXYZ = glm::vec3(0.0f, -175.0f, 0.0f);
        pTerrain->uniqueFriendlyName = "Terrain";
        pTerrain->textures[0] = "Grey_Brick_Wall_Texture.bmp";
        pTerrain->blendRatio[0] = 1.0f;
        ::g_vecMeshesToDraw.push_back(pTerrain);



//        sMesh* pTerrainWireFrame = new sMesh();
//        pTerrainWireFrame->modelFileName = "assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply";
//        pTerrainWireFrame->positionXYZ = glm::vec3(0.0f, -173.8f, 0.0f);
//        pTerrainWireFrame->bDoNotLight = true;
//        pTerrainWireFrame->bOverrideObjectColour = true;
//        pTerrainWireFrame->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//        pTerrainWireFrame->uniqueFriendlyName = "Terrain_Wireframe";
//        pTerrainWireFrame->rotationEulerXYZ.y = 90.0f;
//        pTerrainWireFrame->bIsWireframe = true;
////        pTerrainWireFrame->textures[0] = "Grey_Brick_Wall_Texture.bmp";
////        pTerrainWireFrame->blendRatio[0] = 1.0f;
//        ::g_vecMeshesToDraw.push_back(pTerrainWireFrame);
    }
    */

    /* {
        sMesh* pFlatPlane = new sMesh();
        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N_uv.ply";
        pFlatPlane->positionXYZ = glm::vec3(0.0f, -5.5f, 0.0f);
        pFlatPlane->rotationEulerXYZ.y = 90.0f;
        pFlatPlane->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        pFlatPlane->uniqueFriendlyName = "Ground";
        //
        pFlatPlane->textures[0] = "dua-lipa-promo.bmp";     // 1.0
        pFlatPlane->textures[1] = "Puzzle_parts.bmp";       // 0.0
        pFlatPlane->textures[2] = "shape-element-splattered-texture-stroke_1194-8223.bmp";
        pFlatPlane->textures[3] = "Grey_Brick_Wall_Texture.bmp";

        //        pFlatPlane->alphaTransparency = 0.5f;

        pFlatPlane->blendRatio[0] = 0.0f;
        pFlatPlane->blendRatio[1] = 1.0f;

        pFlatPlane->bIsVisible = false;

        //
        //        pFlatPlane->bIsWireframe = true;
        //        ::g_myMeshes[::g_NumberOfMeshesToDraw] = pFlatPlane;
        //        ::g_NumberOfMeshesToDraw++;
        ::g_vecMeshesToDraw.push_back(pFlatPlane);


        // Add the "ground" to the physcs
        cPhysics::sAABB* pAABBGround = new cPhysics::sAABB();
        pAABBGround->centreXYZ = pFlatPlane->positionXYZ;
        sModelDrawInfo planeMeshInfo;
        ::g_pMeshManager->FindDrawInfoByModelName(pFlatPlane->modelFileName, planeMeshInfo);

        // Manually enter the AABB info:
        pAABBGround->centreXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
        // How far from the centre the XYZ min and max are
        // This information is from the mesh we loaded
        // WARNING: We need to be careful about the scale
        pAABBGround->minXYZ.x = -100.0f;
        pAABBGround->maxXYZ.x = 100.0f;

        pAABBGround->minXYZ.z = -100.0f;
        pAABBGround->maxXYZ.z = 100.0f;

        pAABBGround->minXYZ.y = -1.0f;
        pAABBGround->maxXYZ.y = 1.0f;

        // Copy the physics object position from the initial mesh position
        pAABBGround->pPhysicInfo->position = pFlatPlane->positionXYZ;

        // Don't move this ground (skip integration step)
        pAABBGround->pPhysicInfo->bDoesntMove = true;

        pAABBGround->pPhysicInfo->pAssociatedDrawingMeshInstance = pFlatPlane;

        ::g_pPhysicEngine->vecAABBs.push_back(pAABBGround);
    }*/
    //    {
    //        sMesh* pFlatPlane = new sMesh();
    ////        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz.ply";
    ////        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N.ply";
    //        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N_uv.ply";
    //        pFlatPlane->positionXYZ = glm::vec3(0.0f, -5.0f, 0.0f);
    //        pFlatPlane->bIsWireframe = true;
    //        pFlatPlane->uniformScale = 1.01f;
    //        pFlatPlane->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //
    //        ::g_vecMeshesToDraw.push_back(pFlatPlane);
    //    }


    /*
    {

        //    ____                _            __                   _     
        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
        //                                                                
        sMesh* pSphereMesh = new sMesh();
        //        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz.ply";
        //        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
        pSphereMesh->positionXYZ = glm::vec3(-15.0f, -3.0f, -20.0f);
        //pSphereMesh->bIsWireframe = true;
        pSphereMesh->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        pSphereMesh->uniqueFriendlyName = "Ball";

        //::g_myMeshes[::g_NumberOfMeshesToDraw] = pSphere;
        //::g_NumberOfMeshesToDraw++;
        ::g_vecMeshesToDraw.push_back(pSphereMesh);

        {
            sMesh* pSphereShadowMesh = new sMesh();
            //            pSphereShadowMesh->modelFileName = "assets/models/Sphere_radius_1_Flat_Shadow_xyz_N.ply";
            pSphereShadowMesh->modelFileName = "assets/models/Sphere_radius_1_Flat_Shadow_xyz_N_uv.ply";
            pSphereShadowMesh->positionXYZ = pSphereMesh->positionXYZ;
            pSphereShadowMesh->positionXYZ.y = -3.95f;  // JUST above the ground
            pSphereShadowMesh->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            pSphereShadowMesh->uniqueFriendlyName = "Ball_Shadow";
            ::g_vecMeshesToDraw.push_back(pSphereShadowMesh);
        }


        //    ____  _               _                  _     _           _   
        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
        //                |___/                           |__/               
        // Add sphere
        cPhysics::sSphere* pSphereInfo = new cPhysics::sSphere();

        pSphereInfo->centre = glm::vec3(0.0f);  // Sphere's centre (i.e. an offset from the position)

        pSphereInfo->pPhysicInfo->position = pSphereMesh->positionXYZ;
        // HACK: We know this is 1.0 because...?
        // We could also have pulled that information from the mesh info
        pSphereInfo->radius = 1.0f;

        pSphereInfo->pPhysicInfo->velocity.y = 7.5f;

        // Set some x velocity
        pSphereInfo->pPhysicInfo->velocity.x = 1.0f;


        pSphereInfo->pPhysicInfo->acceleration.y = -3.0f;

        // Associate this drawing mesh to this physics object
        pSphereInfo->pPhysicInfo->pAssociatedDrawingMeshInstance = pSphereMesh;

        ::g_pPhysicEngine->vecSpheres.push_back(pSphereInfo);
    }
    */

//    for (unsigned int ballCount = 0; ballCount != 10; ballCount++)
//    {
//        //    ____                _            __                   _     
//        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
//        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
//        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
//        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
//        //                                                                
//        sMesh* pSphereMesh = new sMesh();
//        //        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz.ply";
////        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
//        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
//        pSphereMesh->positionXYZ.x = ::g_getRandomFloat(-30.0f, 30.0f);
//        pSphereMesh->positionXYZ.z = ::g_getRandomFloat(-30.0f, 30.0f);
//        pSphereMesh->positionXYZ.y = ::g_getRandomFloat(0.0f, 40.0f);
//        //pSphereMesh->bIsWireframe = true;
//        pSphereMesh->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//        pSphereMesh->objectColourRGBA.r = ::g_getRandomFloat(0.0f, 1.0f);
//        pSphereMesh->objectColourRGBA.g = ::g_getRandomFloat(0.0f, 1.0f);
//        pSphereMesh->objectColourRGBA.b = ::g_getRandomFloat(0.0f, 1.0f);
//        std::stringstream ssBallName;
//        ssBallName << "Bouncy_" << ballCount;
//        pSphereMesh->uniqueFriendlyName = ssBallName.str();
//
//        //
//        pSphereMesh->textures[0] = "Non-uniform concrete wall 0512-3-1024x1024.bmp";
//
//        ::g_vecMeshesToDraw.push_back(pSphereMesh);
//
//        //    ____  _               _                  _     _           _   
//        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
//        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
//        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
//        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
//        //                |___/                           |__/               
//        // Add sphere
//        cPhysics::sSphere* pSphereInfo = new cPhysics::sSphere();
//        pSphereInfo->centre = glm::vec3(0.0f);  // Sphere's centre (i.e. an offset from the position)
//        pSphereInfo->pPhysicInfo->position = pSphereMesh->positionXYZ;
//        pSphereInfo->radius = 1.0f;
//        pSphereInfo->pPhysicInfo->velocity.y = ::g_getRandomFloat(2.0f, 10.0f);
//        pSphereInfo->pPhysicInfo->velocity.x = ::g_getRandomFloat(-5.0f, 5.0f);
//        pSphereInfo->pPhysicInfo->velocity.z = ::g_getRandomFloat(-5.0f, 5.0f);
//        pSphereInfo->pPhysicInfo->acceleration.y = -3.0f;
//        pSphereInfo->pPhysicInfo->pAssociatedDrawingMeshInstance = pSphereMesh;
//        ::g_pPhysicEngine->vecSpheres.push_back(pSphereInfo);
//    }//for ( unsigned int ballCount


    return;
}

bool Convert_AssimpHelperMesh_to_sModelDrawInfo(AH::cMesh* pAHMesh, sModelDrawInfo& DrawInfo)
{
    DrawInfo.meshName = pAHMesh->name;

    DrawInfo.numberOfTriangles = (unsigned int)pAHMesh->vecFaces.size();
    DrawInfo.numberOfVertices = (unsigned int)pAHMesh->vecPositions.size();
    DrawInfo.numberOfIndices = (unsigned int)pAHMesh->vecIndices.size();

    DrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV_TanBi[DrawInfo.numberOfVertices];
    for (unsigned int vecIndex = 0; vecIndex != pAHMesh->vecPositions.size(); vecIndex++)
    {
        DrawInfo.pVertices[vecIndex].x = pAHMesh->vecPositions[vecIndex].x;
        DrawInfo.pVertices[vecIndex].y = pAHMesh->vecPositions[vecIndex].y;
        DrawInfo.pVertices[vecIndex].z = pAHMesh->vecPositions[vecIndex].z;

        if ( ! pAHMesh->vecColourSetsRGBA.empty() )
        {
            DrawInfo.pVertices[vecIndex].r = pAHMesh->vecColourSetsRGBA[0][vecIndex].r;
            DrawInfo.pVertices[vecIndex].g = pAHMesh->vecColourSetsRGBA[0][vecIndex].g;
            DrawInfo.pVertices[vecIndex].b = pAHMesh->vecColourSetsRGBA[0][vecIndex].b;
        }
        else
        {
            DrawInfo.pVertices[vecIndex].r = 0.0f;
            DrawInfo.pVertices[vecIndex].g = 0.0f;
            DrawInfo.pVertices[vecIndex].b = 0.0f;
        }

        if (!pAHMesh->vecNormals.empty())
        {
            DrawInfo.pVertices[vecIndex].nx = pAHMesh->vecNormals[vecIndex].x;
            DrawInfo.pVertices[vecIndex].ny = pAHMesh->vecNormals[vecIndex].y;
            DrawInfo.pVertices[vecIndex].nz = pAHMesh->vecNormals[vecIndex].z;
        }
        else
        {
            DrawInfo.pVertices[vecIndex].nx = 0.0f;
            DrawInfo.pVertices[vecIndex].ny = 0.0f;
            DrawInfo.pVertices[vecIndex].nz = 0.0f;
        }

        if (!pAHMesh->vecTextureChannels.empty())
        {
            DrawInfo.pVertices[vecIndex].u = pAHMesh->vecTextureChannels[0].vecUVs[vecIndex].x;
            DrawInfo.pVertices[vecIndex].v = pAHMesh->vecTextureChannels[0].vecUVs[vecIndex].y;
        }
        else
        {
            DrawInfo.pVertices[vecIndex].u = 0.0f;
            DrawInfo.pVertices[vecIndex].v = 0.0f;
        }

        if (!pAHMesh->vecTangents.empty())
        {
            DrawInfo.pVertices[vecIndex].tx = pAHMesh->vecTangents[vecIndex].x;
            DrawInfo.pVertices[vecIndex].ty = pAHMesh->vecTangents[vecIndex].y;
            DrawInfo.pVertices[vecIndex].tz = pAHMesh->vecTangents[vecIndex].z;
        }
        else
        {
            DrawInfo.pVertices[vecIndex].tx = 0.0f;
            DrawInfo.pVertices[vecIndex].ty = 0.0f;
            DrawInfo.pVertices[vecIndex].tz = 0.0f;
        }

        if (!pAHMesh->vecBiTangents.empty())
        {
            DrawInfo.pVertices[vecIndex].bx = pAHMesh->vecBiTangents[vecIndex].x;
            DrawInfo.pVertices[vecIndex].by = pAHMesh->vecBiTangents[vecIndex].y;
            DrawInfo.pVertices[vecIndex].bz = pAHMesh->vecBiTangents[vecIndex].z;
        }
        else
        {
            DrawInfo.pVertices[vecIndex].bx = 0.0f;
            DrawInfo.pVertices[vecIndex].by = 0.0f;
            DrawInfo.pVertices[vecIndex].bz = 0.0f;
        }

    }// for (unsigned int vecIndex


    DrawInfo.pIndices = new unsigned int[DrawInfo.numberOfIndices];
    for (unsigned int index = 0; index != pAHMesh->vecIndices.size(); index++)
    {
        DrawInfo.pIndices[index] = pAHMesh->vecIndices[index];
    }


    return true;
}