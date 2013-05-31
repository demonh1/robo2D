
#include "AgentManager.h"
#include "Agent.h"

//--------------------------- Instance ----------------------------------------

AgentManager* AgentManager:: Instance() {
	static AgentManager instance;

  return &instance;
}

//-----------------------------getEntityFromId------------------------------------------------
Agent* AgentManager::getEntityFromId(int id)const
{
  //find the entity
  EntityMap::const_iterator ent = m_EntityMap.find(id);

  //assert that the entity is a member of the map
  assert ( (ent !=  m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

  return ent->second;
}

//-------------------------------------RegisterEntity----------------------------------------
void AgentManager::RegisterEntity(Agent* newEntity) {

	m_EntityMap.insert(std::make_pair(newEntity ->getAgentId(), newEntity));

}

//-------------------------------------removeEntity------------------------------------------

void AgentManager::removeEntity(Agent* entity) {

	m_EntityMap.erase(m_EntityMap.find(entity->getAgentId()));

}