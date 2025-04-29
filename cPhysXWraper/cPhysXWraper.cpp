// PhysXWrap.cpp : Defines the functions for the static library.
//

#include "cPhysXWraper.h"

// For PIMPL pattern, this include is ONLY in the CPP file
// (not in the header)
#include "cPhysXWraper_Imp.h"

// Hey
cPhysXWraper::cPhysXWraper()
{
	this->m_pImp = new cPhysXWraper_Imp();

}

cPhysXWraper::~cPhysXWraper()
{
	delete this->m_pImp;
}

// HACK: Remove this later
void cPhysXWraper::HACK_Snippet_ShootBall(void)
{
	this->m_pImp->HACK_Snippet_ShootBall();
	return;
}

void cPhysXWraper::getSceneActors(std::vector<cPhysicsObjectTypes>& vecPhysActors)
{
	this->m_pImp->getSceneActors(vecPhysActors);
	return;
}

void cPhysXWraper::updateActorVelocity(std::string name, float vX, float vY, float vZ)
{
	this->m_pImp->updateActorVelocity(name, vX, vY, vZ);
	return;
}

void cPhysXWraper::updateActorTarget(std::string name, float vX, float vY, float vZ)
{
	this->m_pImp->updateActorTarget(name, vX, vY, vZ);
	return;
}

glm::vec3 cPhysXWraper::getActorPosition(std::string name)
{
	return this->m_pImp->getActorPosition(name);
}

//void cPhysXWraper::movePlayer(float vX, float vY, float vZ)
//{
//	this->m_pImp->movePlayer(vX, vY, vZ);
//	return;
//}

glm::vec3 cPhysXWraper::getPlayerPosition()
{
	return this->m_pImp->getPlayerPosition();
}

void cPhysXWraper::updateCameraPosition(float yaw, float pitch, glm::vec3& position, glm::vec3& target)
{
	this->m_pImp->updateCameraPosition(yaw, pitch, position, target);
	return;
}

bool cPhysXWraper::isOnGround()
{
	return this->m_pImp->isOnGround();
}

void cPhysXWraper::initPhysics(bool interactive, std::string PVD_Host_Address /*= "127.0.0.1"*/)
{
	this->m_pImp->initPhysics(interactive, PVD_Host_Address);


	return;
}

void cPhysXWraper::update(void)
{
	this->m_pImp->update();
	return;
}

void cPhysXWraper::cleanupPhysics(bool interactive)
{
	this->m_pImp->cleanupPhysics(interactive);

	return;
}

