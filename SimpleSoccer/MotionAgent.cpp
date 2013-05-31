#include "MotionAgent.h"



bool MotionAgent::rotateHeadingToFacePosition(Vector2D target)
{
  Vector2D toTarget = Vec2DNormalize(target - m_Position);

  double dot = m_Heading.Dot(toTarget);

  //some compilers lose acurracy so the value is clamped to ensure it remains valid for the acos
  Clamp(dot, -1, 1);

  //first determine the angle between the heading vector and the target
  double angle = acos(dot);

  //return true if the player is facing the target
  if (angle < 0.00001) return true;

  //clamp the amount to turn to the max turn rate
  if (angle > m_MaxTurnRate) angle = m_MaxTurnRate;
  
	C2DMatrix rotationMatrix;
  
	rotationMatrix.Rotate(angle * m_Heading.Sign(toTarget));	
  rotationMatrix.TransformVector2Ds(m_Heading);
  rotationMatrix.TransformVector2Ds(m_Velocity);

  m_Side = m_Heading.Perp();

  return false;
}


void MotionAgent::setHeading(Vector2D newHeading)
{
 // assert( (new_heading.lengthSq() - 1.0) < 0.00001);
  
  m_Heading = newHeading;

  //the side vector must always be perpendicular to the heading
  m_Side = m_Heading.Perp();
}


