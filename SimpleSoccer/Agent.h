#ifndef Agent_h
#define Agent_h

//------------------------------------------------------------------------
//      
//  Desc:   A interface class defining an agent 
//  Author: Alexandr Arkhipov <demon7987@gmail.com>
//
//------------------------------------------------------------------------

#include <string>
#include "2d/Vector2D.h" 

class Message;

class Agent {

public:

	enum {default_type = -1};

//Agent();
Agent(int id);
Agent(const std::string name, int id);
//Agent(int id);
 virtual ~Agent() { };

inline void setName(std::string name) { name = aName; }
std::string getName() const { return aName; }

inline void setAgentId(int id) {   aId = id; aNextValidId = aId+1; }

int getAgentId() const { return aId; }

////////////////////////////////////
  
virtual void Update() = 0;
virtual void Render() { };

virtual bool HandleMessage(const Message& msg){return false;}
  

////////////////////////

static void  resetNextValidId(){ aNextValidId = 0; }
static int   getNextValidId() { return aNextValidId; }

// Methods for determining the position of the robot
Vector2D getPosition()const { return m_Position; }
void   setPosition(Vector2D newPosition){ m_Position = newPosition; }

double getBoundingRadius()const{ return m_BoundingRadius; }
void setBoundingRadius (double radius){m_BoundingRadius = radius;}

Vector2D getScale( ) const { return m_Scale; }

void setScale(Vector2D value); 
void setScale(double value);

void         Tag(){m_bTag = true;}
void         UnTag(){m_bTag = false;}

private:

int aId;
std::string aName;

static int aNextValidId; // next valid id for update

int _type;
 bool  m_bTag;

 


protected:

// location in the environment
Vector2D m_Position;
Vector2D m_Scale;



  //the magnitude of this object's bounding radius
double    m_BoundingRadius;
   
};
#endif
