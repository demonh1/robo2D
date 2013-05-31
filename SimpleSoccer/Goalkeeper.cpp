#include "GoalKeeper.h"
#include "misc/Cgdi.h"
#include "SteeringBehaviors.h"
#include "SoccerTeam.h"
#include "SoccerPitch.h"
#include "2D/transformations.h"
#include "GoalKeeperStates.h"
#include "Goal.h"
#include "function_templates.h"
#include "ParamLoader.h"




GoalKeeper::GoalKeeper(SoccerTeam*        home_team,
                       int                home_region,
                       State<GoalKeeper>* start_state,
                       Vector2D           heading,
                       Vector2D           velocity,
                       double              mass,
                       double              max_force,
                       double              max_speed,
                       double              max_turn_rate,
                       double              scale): PlayerBase(home_team,
                                                             home_region,
                                                             heading,
                                                             velocity,
                                                             mass,
                                                             max_force,
                                                             max_speed,
                                                             max_turn_rate,
                                                             scale,
                                                             PlayerBase::goal_keeper)
                                         
                                        
{   
   //set up the state machine
  m_pStateMachine = new StateMachine<GoalKeeper>(this);

  m_pStateMachine->SetCurrentState(start_state);
  m_pStateMachine->SetPreviousState(start_state);
  m_pStateMachine->SetGlobalState(GlobalKeeperState::Instance());

  m_pStateMachine->CurrentState()->Enter(this);        
}



//-------------------------- Update --------------------------------------

void GoalKeeper::Update()
{ 
  //run the logic for the current state
  m_pStateMachine->Update();

  //calculate the combined force from each steering behavior 
  Vector2D SteeringForce = m_pSteering->Calculate();



  //Acceleration = Force/Mass
  Vector2D Acceleration = SteeringForce / m_Mass;

  //update velocity
  m_Velocity += Acceleration;

  //make sure player does not exceed maximum velocity
  m_Velocity.Truncate(m_MaxSpeed);

  //update the position
  m_Position += m_Velocity;


  //enforce a non-penetration constraint if desired
  if(Prm.bNonPenetrationConstraint)
  {
    EnforceNonPenetrationContraint(this, AutoList<PlayerBase>::GetAllMembers());
  }

  //update the heading if the player has a non zero velocity
  if ( !m_Velocity.isZero())
  {    
    m_Heading = Vec2DNormalize(m_Velocity);

    m_Side = m_Heading.Perp();
  }

  //look-at vector always points toward the ball
  if (!Pitch()->GoalKeeperHasBall())
  {
	  m_vLookAt = Vec2DNormalize(Ball()->getPosition() - getPosition());
  }
}


bool GoalKeeper::BallWithinRangeForIntercept()const
{
  return (Vec2DDistanceSq(Team()->HomeGoal()->Center(), Ball()->getPosition()) <=
          Prm.GoalKeeperInterceptRangeSq);
}

bool GoalKeeper::TooFarFromGoalMouth()const
{
	return (Vec2DDistanceSq(getPosition(), GetRearInterposeTarget()) >
          Prm.GoalKeeperInterceptRangeSq);
}

Vector2D GoalKeeper::GetRearInterposeTarget()const
{
  double xPosTarget = Team()->HomeGoal()->Center().x;

  double yPosTarget = Pitch()->PlayingArea()->Center().y - 
	  Prm.GoalWidth*0.5 + (Ball()->getPosition().y*Prm.GoalWidth) /
                     Pitch()->PlayingArea()->Height();

  return Vector2D(xPosTarget, yPosTarget); 
}

//-------------------- HandleMessage -------------------------------------
//
//  routes any messages appropriately
//------------------------------------------------------------------------
bool GoalKeeper::HandleMessage(const Message& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

//--------------------------- Render -------------------------------------
void GoalKeeper::Render()                                         
{
  if (Team()->getColor() == SoccerTeam::blue) 
    gdi->BluePen();
  else 
    gdi->RedPen();
  
  m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
	  getPosition(),
                                       m_vLookAt,
                                       m_vLookAt.Perp(),
									   getScale());

  gdi->ClosedShape(m_vecPlayerVBTrans);
  
  //draw the head
  gdi->BrownBrush();
  gdi->Circle(getPosition(), 6);

  //draw the ID
  if (Prm.bIDs)
  {
    gdi->TextColor(0, 170, 0);;
    gdi->TextAtPos(getPosition().x-20, getPosition().y-20, ttos(getAgentId()));
  }

  //draw the state
  if (Prm.bStates)
  { 
    gdi->TextColor(0, 170, 0); 
    gdi->TransparentText(); 
    gdi->TextAtPos(m_Position.x, m_Position.y -20, std::string(m_pStateMachine->GetNameOfCurrentState()));
  }
}