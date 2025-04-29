#include "cPhysXWraper_Imp.h"

//cPhysXWraper_Imp::cPhysXWraper_Imp()
//{
//}
//
//cPhysXWraper_Imp::~cPhysXWraper_Imp()
//{
//}
//
//
//void cPhysXWraper_Imp::update(void)
//{
//
//	return;
//}
//
//void cPhysXWraper_Imp::initPhysics(bool interactive, std::string PVD_Host_Address)
//{
//
//	return;
//}
//
//
//void cPhysXWraper_Imp::getSceneActors(std::vector<cPhysicsObjectTypes>& vecPhysActors)
//{
//	return;
//}
//
//void cPhysXWraper_Imp::cleanupPhysics(bool interactive)
//{
//
//	printf("SnippetHelloWorld done.\n");
//}

// Note that my PhyX is NOT in the project 'include' folder
// C:\PhysX (git)\PhysX\physx\include

// https://nvidia-omniverse.github.io/PhysX/physx/5.3.1/docs/BuildingWithPhysX.html

#include <vector>

using namespace physx;

static PxDefaultAllocator		gAllocator;
static PxDefaultErrorCallback	gErrorCallback;
static PxFoundation* gFoundation = NULL;
static PxPhysics* gPhysics = NULL;
static PxDefaultCpuDispatcher* gDispatcher = NULL;
static PxScene* gScene = NULL;
static PxMaterial* gMaterial = NULL;
static PxPvd* gPvd = NULL;
static PxControllerManager* manager = NULL;


// These are taken from the SnippedHelloWorld example.
// We are only using these now to make sure that the PhysX stuff is up and running.
// 
// i.e. we will be deleting these once we are sure that PhysX is working OK
static PxReal stackZ = 50.0f;
void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry,
	const PxVec3& velocity /*= PxVec3(0)*/);



cPhysXWraper_Imp::cPhysXWraper_Imp()
{



}

cPhysXWraper_Imp::~cPhysXWraper_Imp()
{



}


void cPhysXWraper_Imp::update(void)
{
	// From renderCallback() in SnippetHelloWorld

	//stepPhysics(true);
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);

	// Don't need this - it's for the SnippedHelloWorld GLUT rendering
	//		Snippets::startRender(sCamera);

	PxScene* scene;
	PxGetPhysics().getScenes(&scene, 1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		//			Snippets::renderActors(&actors[0], static_cast<PxU32>(actors.size()), true);
	}

	// Don't need this - it's for the SnippedHelloWorld GLUT rendering
	//		Snippets::finishRender();

	return;
}

void cPhysXWraper_Imp::initPhysics(bool interactive, std::string PVD_Host_Address)
{
	// Taken from the SnippetHelloWorld.cpp file:

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	// This is connecting to the visual debugger...
	gPvd = PxCreatePvd(*gFoundation);
	//	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_Host_Address.c_str(), 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	// This creates the 'world' that all the physics things are in
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	float halfExtent = 1.0f;

// **** 
	// Character
	{
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
		PxTransform localTm(PxVec3(3.0f, 2.0f, -1.0f));
		PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(localTm));
		body->setLinearVelocity(PxVec3(0, 0, 0));
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		//PxRigidBody::addTorque
		body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		gScene->addActor(*body);
		body->setName("Player");
		//PxTransform position = body->getGlobalPose();
		//PxQuat rotation(PxPi / 4, PxVec3(0, 1, 0));
		//PxQuat newRotation = rotation * position.q;
		//body->setKinematicTarget(PxTransform(position.p, newRotation));

		map_RigidDynamics.insert({ "Player", body });

		float radius = 0.5f;
		float halfHeight = 5.0f;
	}
// ****

	{
		// Create wall
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent * 15, halfExtent * 5, halfExtent), *gMaterial);
		PxTransform localTm(PxVec3(0.0f, 5.0f, -10.0f));
		PxRigidStatic* body = gPhysics->createRigidStatic(PxTransform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
	}

	{
		// Create wall
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent * 15, halfExtent * 5, halfExtent), *gMaterial);
		PxTransform localTm(PxVec3(0.0f, 5.0f, 10.0f));
		PxRigidStatic* body = gPhysics->createRigidStatic(PxTransform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
	}

	{
		// Create wall
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent * 5, halfExtent * 15), *gMaterial);
		PxTransform localTm(PxVec3(17.5f, 5.0f, 0.0f));
		PxRigidStatic* body = gPhysics->createRigidStatic(PxTransform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
	}

	{
		// Create wall
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent * 5, halfExtent * 15), *gMaterial);
		PxTransform localTm(PxVec3(-16.5f, 5.0f, 0.0f));
		PxRigidStatic* body = gPhysics->createRigidStatic(PxTransform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
	}

	// Dynamic box
	{
		PxMaterial* boxMaterial = gPhysics->createMaterial(0.8f, 0.8f, 0.1f);
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent - 0.1, halfExtent - 0.1, halfExtent - 0.1), *boxMaterial);
		PxTransform localTm(PxVec3(-6.0f, 2.0f, -1.0f));
		PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(localTm));
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 2000.0f);
		body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
		body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
		body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
		//body->setMass(1'000.0f);
		//body->setMassSpaceInertiaTensor(PxVec3(0.5f, 0.5f, 0.5f));
		body->setName("Chair");
		gScene->addActor(*body);

		map_RigidDynamics.insert({ "Chair", body });
	}

	// Static box (table)
	{
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(2, 2.5f, 2.75), *gMaterial);
		PxTransform localTm(PxVec3(-12.1f, 1.1f, 4.5f));
		PxRigidStatic* body = gPhysics->createRigidStatic(PxTransform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
	}

	// Static box (fireplace)
	{
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(1.5f, 5.0f, 1.25), *gMaterial);
		PxTransform localTm(PxVec3(14.5f, 1.0f, 0.0f));
		PxRigidStatic* body = gPhysics->createRigidStatic(PxTransform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
	}

	// Dynamic capsule
	{
		PxMaterial* capsuleMaterial = gPhysics->createMaterial(0.75f, 0.75f, 0.2f);
		PxShape* shape = gPhysics->createShape(PxCapsuleGeometry(0.3f, 1.0f), *capsuleMaterial);
		PxTransform localTm(PxVec3(5.0f, 1.5f, 2.0f));
		PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(localTm));
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 2000.0f);
		body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
		//body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
		body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
		//body->setMass(1'000.0f);
		//body->setMassSpaceInertiaTensor(PxVec3(0.5f, 0.5f, 0.5f));
		body->setName("Barrel");
		body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		gScene->addActor(*body);

		map_RigidDynamics.insert({ "Barrel", body });
	}

// ********************************************************
// This block of code is from the hello world PhyX example.

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

// ********************************************************

	return;
}

// HACK: Remove this later
void cPhysXWraper_Imp::HACK_Snippet_ShootBall(void)
{
	// In the original snippet example, it had this code:
	//void keyPress(unsigned char key, const PxTransform & camera)
	//{
	//	switch (toupper(key))
	//	{
	//	case 'B':	createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);						break;
	//	case ' ':	createDynamic(camera, PxSphereGeometry(3.0f), camera.rotate(PxVec3(0, 0, -1)) * 200);	break;
	//	}
	//}
	createDynamic(PxTransform(PxVec3(0, 40, 200)), PxSphereGeometry(10), PxVec3(0, -50, -100));

	return;
}

void cPhysXWraper_Imp::getSceneActors(std::vector<cPhysicsObjectTypes>& vecPhysActors)
{
	vecPhysActors.clear();

	// Taken from the Snippet::HelloWorld
	PxScene* scene;
	PxGetPhysics().getScenes(&scene, 1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		//		Snippets::renderActors(&actors[0], static_cast<PxU32>(actors.size()), true);

					// This number is taken from the SnippetHelloWorld
				//	and is used in the render call. 
		const unsigned int MAX_NUM_ACTOR_SHAPES = 128;

		PxShape* shapes[MAX_NUM_ACTOR_SHAPES];

		unsigned int numActors = static_cast<unsigned int>(actors.size());

		for (PxU32 i = 0; i < numActors; i++)
		{
			const PxU32 nbShapes = actors[i]->getNbShapes();
			PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
			actors[i]->getShapes(shapes, nbShapes);

			//			bool sleeping;
			//			if (changeColorForSleepingActors)
			//				sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;
			//			else
			//				sleeping = false;

			for (PxU32 j = 0; j < nbShapes; j++)
			{
				const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
				const PxGeometry& geom = shapes[j]->getGeometry();


				//				if (sleeping)
				//				{
				//					const PxVec3 darkColor = color * 0.25f;
				//					glColor4f(darkColor.x, darkColor.y, darkColor.z, 1.0f);
				//				}
				//				else
				//					glColor4f(color.x, color.y, color.z, 1.0f);

				//				renderGeometry(geom);

				switch (geom.getType())
				{
				case PxGeometryType::eBOX:
				{
					const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
					//						glScalef(boxGeom.halfExtents.x, boxGeom.halfExtents.y, boxGeom.halfExtents.y);
					//						glutSolidCube(2);

											// 		BOX,		// Params: "half-extents": x,y,z 
					cPhysicsObjectTypes box;
					box.shapeType = cPhysicsObjectTypes::BOX;
					box.map_ParamToValue["half-extents"] = glm::vec4(boxGeom.halfExtents.x,
						boxGeom.halfExtents.y,
						boxGeom.halfExtents.y,
						1.0f);
					box.uniqueFriendlyName = "Box" + std::to_string(i);

					PxRigidActor* playerActor = map_RigidDynamics.find(("Player"))->second;
					if (playerActor->getName() == actors[i]->getName())
					{
						box.uniqueFriendlyName = "Player";
					}

					PxRigidActor* chairActor = map_RigidDynamics.find(("Chair"))->second;
					if (chairActor->getName() == actors[i]->getName())
					{
						box.uniqueFriendlyName = "Chair";
					}

					const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));

					// They are using glu in the Snippets::HelloWorld
					//	glMultMatrixf(&shapePose.column0.x);

					// From https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.20.pdf
					// Section: 5.4.2 Vector and Matrix Constructors
					// (Page 78)
					// 
					//	mat4(float, float, float, float, // first column
					//	     float, float, float, float, // second column
					//	     float, float, float, float, // third column
					//	     float, float, float, float); // fourth column

					box.matModel = glm::mat4(shapePose.column0.x, shapePose.column0.y, shapePose.column0.z, shapePose.column0.w,
											 shapePose.column1.x, shapePose.column1.y, shapePose.column1.z, shapePose.column1.w,
											 shapePose.column2.x, shapePose.column2.y, shapePose.column2.z, shapePose.column2.w,
											 shapePose.column3.x, shapePose.column3.y, shapePose.column3.z, shapePose.column3.w);

					// Can get other stuff, too
					// shapes[j]->getDensityForFluid(...)
					//shapes[j]->getMaterials(
					
					//PxRigidActor* actor = shapes[j]->getActor();

					vecPhysActors.push_back(box);

				}
				break;

				case PxGeometryType::eSPHERE:
				{
					const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);
					//						glutSolidSphere(GLdouble(sphereGeom.radius), 10, 10);

											// SPHERE,		// Params: "radius": x

					cPhysicsObjectTypes sphere;
					sphere.shapeType = cPhysicsObjectTypes::SPHERE;
					sphere.map_ParamToValue["radius"] = glm::vec4(sphereGeom.radius, 0.0f, 0.0f, 1.0f);

					sphere.matModel = glm::mat4(shapePose.column0.x, shapePose.column0.y, shapePose.column0.z, shapePose.column0.w,
												shapePose.column1.x, shapePose.column1.y, shapePose.column1.z, shapePose.column1.w,
												shapePose.column2.x, shapePose.column2.y, shapePose.column2.z, shapePose.column2.w,
												shapePose.column3.x, shapePose.column3.y, shapePose.column3.z, shapePose.column3.w);


					// Can get other stuff, too
					// shapes[j]->getDensityForFluid(...)
					//shapes[j]->getMaterials(

					vecPhysActors.push_back(sphere);

				}
				break;

				case PxGeometryType::eCAPSULE:
				{
					const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geom);

					cPhysicsObjectTypes capsule;
					capsule.shapeType = cPhysicsObjectTypes::CAPSULE;
					capsule.map_ParamToValue["radius"] = glm::vec4(capsuleGeom.radius, 0.0f, 0.0f, 1.0f);
					capsule.map_ParamToValue["half-height"] = glm::vec4(capsuleGeom.halfHeight, 0.0f, 0.0f, 1.0f);

					capsule.matModel = glm::mat4(shapePose.column0.x, shapePose.column0.y, shapePose.column0.z, shapePose.column0.w,
												 shapePose.column1.x, shapePose.column1.y, shapePose.column1.z, shapePose.column1.w,
												 shapePose.column2.x, shapePose.column2.y, shapePose.column2.z, shapePose.column2.w,
												 shapePose.column3.x, shapePose.column3.y, shapePose.column3.z, shapePose.column3.w);

					vecPhysActors.push_back(capsule);
				}
				break;
				}

			}//for (PxU32 j = 0; j < nbShapes;
		}//for (PxU32 i = 0; i < numActors; i++)

	}//if (nbActors)

	return;
}

void cPhysXWraper_Imp::cleanupPhysics(bool interactive)
{
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);

	//printf("SnippetHelloWorld done.\n");
}

// These are taken from the SnippedHelloWorld example.
// We are only using these now to make sure that the PhysX stuff is up and running.
void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}

PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry,
	const PxVec3& velocity = PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}
//
//PxRigidActor* getActor(void)
//{
//	PxScene* scene;
//	PxGetPhysics().getScenes(&scene, 1);
//	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
//	if (nbActors)
//	{
//		std::vector<PxRigidActor*> actors(nbActors);
//		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
//		
//		const unsigned int MAX_NUM_ACTOR_SHAPES = 128;
//
//		PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
//		
//		unsigned int numActors = static_cast<unsigned int>(actors.size());
//
//		for (PxU32 i = 0; i < numActors; i++)
//		{
//			const PxU32 nbShapes = actors[i]->getNbShapes();
//			PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
//			actors[i]->getShapes(shapes, nbShapes);
//
//			for (PxU32 j = 0; j < nbShapes; j++)
//			{
//				const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
//				const PxGeometry& geom = shapes[j]->getGeometry();
//
//				switch (geom.getType())
//				{
//				case PxGeometryType::eBOX:
//				{
//					const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
//
//					// 		BOX,		// Params: "half-extents": x,y,z 
//					cPhysicsObjectTypes box;
//					box.shapeType = cPhysicsObjectTypes::BOX;
//					box.map_ParamToValue["half-extents"] = glm::vec4(boxGeom.halfExtents.x,
//						boxGeom.halfExtents.y,
//						boxGeom.halfExtents.y,
//						1.0f);
//
//					const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
//
//					// They are using glu in the Snippets::HelloWorld
//					//	glMultMatrixf(&shapePose.column0.x);
//
//					// From https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.20.pdf
//					// Section: 5.4.2 Vector and Matrix Constructors
//					// (Page 78)
//					// 
//					//	mat4(float, float, float, float, // first column
//					//	     float, float, float, float, // second column
//					//	     float, float, float, float, // third column
//					//	     float, float, float, float); // fourth column
//
//					box.matModel = glm::mat4(shapePose.column0.x, shapePose.column0.y, shapePose.column0.z, shapePose.column0.w,
//						shapePose.column1.x, shapePose.column1.y, shapePose.column1.z, shapePose.column1.w,
//						shapePose.column2.x, shapePose.column2.y, shapePose.column2.z, shapePose.column2.w,
//						shapePose.column3.x, shapePose.column3.y, shapePose.column3.z, shapePose.column3.w);
//
//					// Can get other stuff, too
//					// shapes[j]->getDensityForFluid(...)
//					//shapes[j]->getMaterials(
//
//					return shapes[j]->getActor();
//
//				}
//				break;
//
//				case PxGeometryType::eSPHERE:
//				{
//					const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);
//					//						glutSolidSphere(GLdouble(sphereGeom.radius), 10, 10);
//
//											// SPHERE,		// Params: "radius": x
//
//					cPhysicsObjectTypes sphere;
//					sphere.shapeType = cPhysicsObjectTypes::SPHERE;
//					sphere.map_ParamToValue["radius"] = glm::vec4(sphereGeom.radius, 0.0f, 0.0f, 1.0f);
//
//					sphere.matModel = glm::mat4(shapePose.column0.x, shapePose.column0.y, shapePose.column0.z, shapePose.column0.w,
//						shapePose.column1.x, shapePose.column1.y, shapePose.column1.z, shapePose.column1.w,
//						shapePose.column2.x, shapePose.column2.y, shapePose.column2.z, shapePose.column2.w,
//						shapePose.column3.x, shapePose.column3.y, shapePose.column3.z, shapePose.column3.w);
//
//
//					// Can get other stuff, too
//					// shapes[j]->getDensityForFluid(...)
//					//shapes[j]->getMaterials(
//
//					return shapes[j]->getActor();
//
//					//vecPhysActors.push_back(sphere);
//
//				}
//				break;
//
//				}
//
//			}//for (PxU32 j = 0; j < nbShapes;
//		}//for (PxU32 i = 0; i < numActors; i++)
//
//	}//if (nbActors)
//}

void cPhysXWraper_Imp::updateActorVelocity(std::string name, float vX, float vY, float vZ)
{
	PxRigidDynamic* actor = map_RigidDynamics.find(("Player"))->second;

	actor->setLinearVelocity(PxVec3(vX, vY, vZ));
	//actor->setKinematicTarget(PxTransform(vX, vY, vZ));
	return;
}

void cPhysXWraper_Imp::updateActorTarget(std::string name, float vX, float vY, float vZ)
{
	PxRigidDynamic* actor = map_RigidDynamics.find(("Player"))->second;

	float boxDistance = 1.25f;

	// ****************************** STATIC ACTORS ****************************** //

	// Downward sweep
	{
		PxVec3 downwardDir(0, -1, 0);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.y -= boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.5f, 0.05f, 0.5f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			downwardDir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting the ground" << std::endl;
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			vY -= 0.0981f;
		}
	}

	// Left sweep
	{
		PxVec3 dir(1, 0, 0);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.x += boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.05f, 0.05f, 0.05f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eSTATIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting left" << std::endl;
			vX = actor->getGlobalPose().p.x - 0.1f;
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	// Right sweep
	{
		PxVec3 dir(-1, 0, 0);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.x -= boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.05f, 0.05f, 0.05f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eSTATIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting right" << std::endl;
			vX = actor->getGlobalPose().p.x + 0.1f;
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	// Forward sweep
	{
		PxVec3 dir(0, 0, 1);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.z += boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.05f, 0.05f, 0.05f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eSTATIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting forward" << std::endl;
			vZ = actor->getGlobalPose().p.z - 0.1f;
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	// Backward sweep
	{
		PxVec3 dir(0, 0, -1);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.z -= boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.05f, 0.05f, 0.05f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eSTATIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting Backward" << std::endl;
			vZ = actor->getGlobalPose().p.z + 0.1f;
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	// ****************************** DYNAMIC ACTORS ****************************** //

	// Left sweep
	{
		PxVec3 dir(1, 0, 0);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.x += boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.15f, 0.05f, 0.75f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eDYNAMIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting left" << std::endl;
			vX = actor->getGlobalPose().p.x + ((vX - actor->getGlobalPose().p.x) * 0.5);
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	// Right sweep
	{
		PxVec3 dir(-1, 0, 0);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.x -= boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.15f, 0.05f, 0.75f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eDYNAMIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting right" << std::endl;
			vX = actor->getGlobalPose().p.x + ((vX - actor->getGlobalPose().p.x) * 0.5);
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	// Forward sweep
	{
		PxVec3 dir(0, 0, 1);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.z += boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.75f, 0.15f, 0.15f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eDYNAMIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting forward" << std::endl;
			vZ = actor->getGlobalPose().p.z + ((vZ - actor->getGlobalPose().p.z) * 0.5);
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	// Backward sweep
	{
		PxVec3 dir(0, 0, -1);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.z -= boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.75f, 0.05f, 0.15f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			dir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eDYNAMIC)
		);

		if (didHit) {
			// On ground, reset vertical velocity
			//vY += 9.81f;
			//std::cout << "Hitting Backward" << std::endl;
			vZ = actor->getGlobalPose().p.z + ((vZ - actor->getGlobalPose().p.z) * 0.5);
		}
		else {
			// Apply gravity if in air
			//velocity += gravity * deltaTime;
			//vY -= 0.0981f;
		}
	}

	//PxTransform currentPose = actor->getGlobalPose();
	//PxQuat rotation(0.05f, PxVec3(0, 1, 0));
	//PxQuat newRotation = rotation * currentPose.q;

	//actor->setKinematicTarget(PxTransform(PxVec3(vX, vY, vZ), newRotation));

	actor->setKinematicTarget(PxTransform(vX, vY, vZ));
	
	return;
}

glm::vec3 cPhysXWraper_Imp::getActorPosition(std::string name)
{
	PxRigidDynamic* actor = map_RigidDynamics.find(("Player"))->second;

	PxTransform t = actor->getGlobalPose();

	glm::vec3 glmT = glm::vec3(t.p.x, t.p.y, t.p.z);

	return glmT;
}

//void cPhysXWraper_Imp::movePlayer(float vX, float vY, float vZ)
//{
//	PxController* player = manager->getController(0);
//
//	PxVec3 position = PxVec3(vX, vY - 9.81f, vZ);
//
//	player->move(position, 0.1f, 1/60.0f, NULL);
//
//	PxOverlapBufferN<64> hitBuffer;
//	PxBoxGeometry boxGeometry(PxVec3(10.5f, 10.5f, 10.5f));  // Box size, adjust as needed
//
//	PxExtendedVec3 playerPosition = player->getPosition(); //PxExtendedVec3(position.x, position.y, position.z);
//	PxVec3 positionFix(static_cast<float>(playerPosition.x),
//					   static_cast<float>(playerPosition.y),
//					   static_cast<float>(playerPosition.z));
//	
//	PxTransform pose(positionFix);
//
//	std::cout << pose.p.x << " " << pose.p.y << " " << pose.p.z << std::endl;
//	std::cout << player->getPosition().x << " " << player->getPosition().y << " " << player->getPosition().z << std::endl;
//	std::cout << PxBounds3::poseExtent(pose, PxVec3(10.5f, 10.5f, 10.5f)).getCenter().x << " "
//				<< PxBounds3::poseExtent(pose, PxVec3(10.5f, 10.5f, 10.5f)).getCenter().y << " "
//				<< PxBounds3::poseExtent(pose, PxVec3(10.5f, 10.5f, 10.5f)).getCenter().z << std::endl;
//
//	PxQueryFilterData filterData;
//	filterData.flags |= PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::eANY_HIT;
//
//	gScene->overlap(boxGeometry, pose, hitBuffer, filterData);
//	std::cout << "Overlap hits: " << hitBuffer.getNbAnyHits() << std::endl;
//
//	PxU32 nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
//	std::vector<PxRigidActor*> actors(nbActors);
//	gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
//
//	PxBounds3 myQueryBounds = PxBounds3::poseExtent(pose, boxGeometry.halfExtents);
//
//	for (PxU32 i = 0; i < nbActors; ++i)
//	{
//		PxActor* actor = actors[i];
//		PxRigidDynamic* dynamicActor = actor->is<PxRigidDynamic>();
//		if (dynamicActor)
//		{
//			dynamicActor->wakeUp();
//		}
//		// Get position
//		PxVec3 pos = static_cast<PxRigidActor*>(actor)->getGlobalPose().p;
//
//		// Check manually if it's inside your overlap bounds
//		if (myQueryBounds.contains(pos))
//		{
//			std::cout << "This actor should be inside the overlap area!" << std::endl;
//		}
//	}
//
//	// Loop through any collisions detected
//	for (int i = 0; i < hitBuffer.getNbAnyHits(); ++i)
//	{
//		PxActor* actor = hitBuffer.getAnyHit(i).actor;
//		PxRigidDynamic* dynamicActor = actor->is<PxRigidDynamic>();
//
//		if (dynamicActor && !dynamicActor->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC))
//		{
//			// Apply a small force to the dynamic actor (push)
//			PxVec3 pushDirection = dynamicActor->getGlobalPose().p - positionFix;
//			pushDirection.normalize();
//
//			dynamicActor->wakeUp();
//			// Apply a force or impulse (make sure you use a reasonable value)
//			dynamicActor->addForce(pushDirection * 100000.0f, PxForceMode::eIMPULSE);  // Adjust force as necessary
//
//			const char* type = actor ? actor->getConcreteTypeName() : "NULL";
//			std::cout << "Overlap with: " << type << std::endl;
//		}
//	}
//
//	return;
//}

glm::vec3 cPhysXWraper_Imp::getPlayerPosition()
{
	PxController* player = manager->getController(0);

	PxExtendedVec3 t = player->getPosition();

	glm::vec3 glmT = glm::vec3(t.x, t.y, t.z);

	return glmT;
}

void cPhysXWraper_Imp::updateCameraPosition(float yaw, float pitch, glm::vec3& position, glm::vec3& target)
{
	PxRigidDynamic* actor = map_RigidDynamics.find(("Player"))->second;

	float radius = 7.5f;
	//float yaw = yaw;
	//float pitch = pitch;
	const float minPitch = 0.1f;
	const float maxPitch = 1.5f;

	// Clamp to max or min
	if (pitch > maxPitch)
	{
		pitch = maxPitch;
	}
	else if (pitch < minPitch)
	{
		pitch = minPitch;
	}

	// Convert spherical coordinates to Cartesian
	float x = radius * cosf(pitch) * sinf(yaw);
	float y = radius * sinf(pitch) + 0.5f;
	float z = radius * cosf(pitch) * cosf(yaw);

	glm::vec3 playerPosition = glm::vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z);
	glm::vec3 cameraOffset(x, -y, z);

	position = playerPosition - cameraOffset;
	target = playerPosition;
	//glm::vec3 up(0.0f, 1.0f, 0.0f);

	//view = glm::lookAt(glm::vec3(position.x, position.y, position.z),
	//				   glm::vec3(target.x, target.y, target.z),
	//				   glm::vec3(up.x, up.y, up.z));

	return;
}

bool cPhysXWraper_Imp::isOnGround()
{
	PxRigidDynamic* actor = map_RigidDynamics.find(("Player"))->second;

	float boxDistance = 1.25f;

	// Downward sweep
	{
		PxVec3 downwardDir(0, -1, 0);
		float maxDistance = 0.2f; // how far below to check for ground

		PxTransform currentPose = actor->getGlobalPose();
		currentPose.p.y -= boxDistance;
		PxBoxGeometry boxGeom(PxVec3(0.15f, 0.05f, 0.15f)); // half extents of your actor

		PxSweepBuffer hit;
		bool didHit = gScene->sweep(
			boxGeom,
			currentPose,
			downwardDir,
			maxDistance,
			hit,
			PxHitFlag::eDEFAULT,
			PxQueryFilterData(PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC)
		);

		if (didHit)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}