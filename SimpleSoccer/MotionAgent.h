#ifndef MotionAgent_h
#define MotionAgent_h

#include "Agent.h"
#include "2d/C2DMatrix.h"



class MotionAgent : public Agent { 

public:

	MotionAgent(Vector2D position,
		double   radius,
		Vector2D velocity,
		double   maxSpeed,
		Vector2D heading,
		double   mass,
		Vector2D scale,
		double   turnRate,
		double   maxForce): Agent(Agent::getNextValidId()),
		m_Heading(heading),
		m_Velocity(velocity),
		m_Mass(mass),
		m_Side(m_Heading.Perp()),
		m_MaxSpeed(maxSpeed),
		m_MaxTurnRate(turnRate),
		m_MaxForce(maxForce)
	{
		m_Position = position;
		m_BoundingRadius = radius; 
		m_Scale = scale;
	}


	virtual ~MotionAgent(){}

	//accessors
	Vector2D  getVelocity()const{ return m_Velocity; }
	void      setVelocity(const Vector2D& NewVel){ m_Velocity = NewVel;}

	double    getMass()const{ return m_Mass; }

	Vector2D  getSide()const{ return m_Side;}

	double    getMaxSpeed()const{ return m_MaxSpeed; }                       
	void      setMaxSpeed(double new_speed){m_MaxSpeed = new_speed;}

	double    getMaxForce()const{return m_MaxForce;}
	void      setMaxForce(double mf){m_MaxForce = mf;}

	bool      isSpeedMaxedOut()const{ return m_MaxSpeed*m_MaxSpeed >= m_Velocity.LengthSq();}
	double    getSpeed()const{return m_Velocity.Length();}
	double    getSpeedSq()const{return m_Velocity.LengthSq();}

	Vector2D  getHeading()const{return m_Heading;}
	void      setHeading(Vector2D newHeading);
	bool      rotateHeadingToFacePosition(Vector2D target);

	double    getMaxTurnRate()const{return m_MaxTurnRate;}
	void      setMaxTurnRate(double val){m_MaxTurnRate = val;}

protected:

	Vector2D    m_Velocity;

	//a normalized vector pointing in the direction the entity is heading. 
	Vector2D    m_Heading;

	//a vector perpendicular to the heading vector
	Vector2D    m_Side; 

	double      m_Mass;
	double      m_MaxSpeed;
	double      m_MaxForce;       
	double      m_MaxTurnRate;


};


#endif
