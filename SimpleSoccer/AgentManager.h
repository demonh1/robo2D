#ifndef AgentManager_h
#define AgentManager_h

#include <map>
#include <cassert>


class Agent;

//provide easy access
#define AgentMgr AgentManager::Instance()



class AgentManager
{
private:

  typedef std::map<int, Agent*> EntityMap;

private:

  //to facilitate quick lookup the entities are stored in a std::map, in which
  //pointers to entities are cross referenced by their identifying number
  EntityMap m_EntityMap;

AgentManager(){}

  //copy ctor and assignment should be private
  AgentManager(const AgentManager&);
  AgentManager& operator=(const AgentManager&);

public:

  static AgentManager* Instance();

  //this method stores a pointer to the entity in the std::vector
  //m_Entities at the index position indicated by the entity's ID
  //(makes for faster access)
  void  RegisterEntity(Agent* newEntity);

  //returns a pointer to the entity with the ID given as a parameter
  Agent* getEntityFromId(int id)const;

  //this method removes the entity from the list
  void  removeEntity(Agent* entity);

  //clears all entities from the entity map
  void  reset(){m_EntityMap.clear();}
};







#endif