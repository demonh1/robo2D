#include "Agent.h"
#include <cstring>

int Agent::aNextValidId = 0;


//Agent:: Agent(){ };
Agent::Agent(std::string name, int id): aName(name),aId(id)  { }


void Agent:: setScale(Vector2D value) {

	m_BoundingRadius *= MaxOf(value.x, value.y)/MaxOf(m_Scale.x, m_Scale.y); 
	m_Scale = value;
}

void Agent:: setScale(double value) {

	m_BoundingRadius *= (value/MaxOf(m_Scale.x, m_Scale.y)); 
	m_Scale = Vector2D(value, value);
}   

Agent::Agent(int id): m_BoundingRadius(0.0),
	                 m_Scale(Vector2D(1.0,1.0)),
					 _type(default_type),
					 m_bTag(false) 
{
						 setAgentId(id);
}

