#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "cPhysicsObjectTypes.h"
#include "PxPhysicsAPI.h"

class cPhysXWraper_Imp
{
public:
	cPhysXWraper_Imp();
	~cPhysXWraper_Imp();

	void initPhysics(bool interactive, std::string PVD_Host_Address);
	void cleanupPhysics(bool interactive);


	void update(void);

	void getSceneActors(std::vector<cPhysicsObjectTypes>& vecPhysActors);
	void updateActorVelocity(std::string name, float vX, float vY, float vZ);
	void updateActorTarget(std::string name, float vX, float vY, float vZ);
	glm::vec3 getActorPosition(std::string name);

	//void movePlayer(float vX, float vY, float vZ);
	glm::vec3 getPlayerPosition();

	void updateCameraPosition(float yaw, float pitch, glm::vec3& position, glm::vec3& target);
	bool isOnGround();

	// HACK: Remove his later
	void HACK_Snippet_ShootBall(void);
private:
	std::map<std::string, physx::PxRigidDynamic*> map_RigidDynamics;
};



