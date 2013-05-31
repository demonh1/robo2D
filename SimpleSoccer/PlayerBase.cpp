#include "PlayerBase.h"
#include "SteeringBehaviors.h"
#include "2D/Transformations.h"
#include "2D/Geometry.h"
#include "misc/Cgdi.h"
#include "2D/C2DMatrix.h"
#include "Game/Region.h"
#include "ParamLoader.h"

#include "SoccerTeam.h"
#include "ParamLoader.h"
#include "Goal.h"
#include "SoccerBall.h"
#include "SoccerPitch.h"
#include "Debug/DebugConsole.h"

#include "msg.h"
#include "MessageEvent.h"



PlayerBase::PlayerBase(SoccerTeam* home_team,
                       int   home_region,
                       Vector2D  heading,
                       Vector2D velocity,
                       double    mass,
                       double    max_force,
                       double    max_speed,
                       double    max_turn_rate,
                       double    scale,
                       player_role role):    

MotionAgent(home_team->getPitch()->GetRegionFromIndex(home_region)->Center(),
                 scale*10.0,
                 velocity,
                 max_speed,
                 heading,
                 mass,
                 Vector2D(scale,scale),
                 max_turn_rate,
                 max_force),
   m_pTeam(home_team),
   m_dDistSqToBall(MaxFloat),
   m_iHomeRegion(home_region),
   m_iDefaultRegion(home_region),
   m_PlayerRole(role)
{
  
  //setup the vertex buffers and calculate the bounding radius
  const int NumPlayerVerts = 4;
  const Vector2D player[NumPlayerVerts] = {Vector2D(-3, 8),
                                            Vector2D(3,10),
                                            Vector2D(3,-10),
                                            Vector2D(-3,-8)};

  for (int vtx = 0; vtx < NumPlayerVerts; ++vtx)
  {
    m_vecPlayerVB.push_back(player[vtx]);

    //set the bounding radius to the length of the 
    //greatest extent
    if (abs(player[vtx].x) > m_BoundingRadius)
    {
      m_BoundingRadius = abs(player[vtx].x);
    }

    if (abs(player[vtx].y) > m_BoundingRadius)
    {
      m_BoundingRadius = abs(player[vtx].y);
    }
  }

  //set up the steering behavior class
  m_pSteering = new SteeringBehaviors(this,
                                      m_pTeam->getPitch(),
                                      Ball());  
  
  //a player's start target is its start position (because it's just waiting)
  m_pSteering->setTarget(home_team->getPitch()->GetRegionFromIndex(home_region)->Center());
}


PlayerBase::~PlayerBase()
{
  delete m_pSteering;
}



//----------------------------- TrackBall --------------------------------
//
//  sets the player's heading to point at the ball
//------------------------------------------------------------------------
void PlayerBase::TrackBall()
{
  rotateHeadingToFacePosition(Ball()->getPosition());  
}

//----------------------------- TrackTarget --------------------------------
//
//  sets the player's heading to point at the current target
//------------------------------------------------------------------------
void PlayerBase::TrackTarget()
{
	setHeading(Vec2DNormalize(Steering()->getTarget() - getPosition()));
}


//------------------------------------------------------------------------
//
//binary predicates for std::sort (see CanPassForward/Backward)
//------------------------------------------------------------------------
bool  SortByDistanceToOpponentsGoal(const PlayerBase*const p1,
                                    const PlayerBase*const p2)
{
  return (p1->DistToOppGoal() < p2->DistToOppGoal());
}

bool  SortByReversedDistanceToOpponentsGoal(const PlayerBase*const p1,
                                            const PlayerBase*const p2)
{
  return (p1->DistToOppGoal() > p2->DistToOppGoal());
}


//------------------------- WithinFieldOfView ---------------------------
//
//  returns true if subject is within field of view of this player
//-----------------------------------------------------------------------
bool PlayerBase::PositionInFrontOfPlayer(Vector2D position)const
{
  Vector2D ToSubject = position - getPosition();

  if (ToSubject.Dot(getHeading()) > 0) 
    
    return true;

  else

    return false;
}

//------------------------- IsThreatened ---------------------------------
//
//  returns true if there is an opponent within this player's 
//  comfort zone
//------------------------------------------------------------------------
bool PlayerBase::isThreatened()const
{
  //check against all opponents to make sure non are within this
  //player's comfort zone
  std::vector<PlayerBase*>::const_iterator curOpp;  
  curOpp = Team()->getOpponents()->Members().begin();
 
  for (curOpp; curOpp != Team()->getOpponents()->Members().end(); ++curOpp)
  {
    //calculate distance to the player. if dist is less than our
    //comfort zone, and the opponent is infront of the player, return true
    if (PositionInFrontOfPlayer((*curOpp)->getPosition()) &&
       (Vec2DDistanceSq(getPosition(), (*curOpp)->getPosition()) < Prm.PlayerComfortZoneSq))
    {        
      return true;
    }
   
  }// next opp

  return false;
}

//----------------------------- FindSupport -----------------------------------
//
//  determines the player who is closest to the SupportSpot and messages him
//  to tell him to change state to SupportAttacker
//-----------------------------------------------------------------------------
void PlayerBase::FindSupport()const
{    
  //if there is no support we need to find a suitable player.
  if (Team()->getSupportingPlayer() == NULL)
  {
    PlayerBase* BestSupportPly = Team()->DetermineBestSupportingAttacker();

    Team()->setSupportingPlayer(BestSupportPly);

   /* Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            getAgentId(),
                            Team()->SupportingPlayer()->getAgentId(),
                            Msg_SupportAttacker,
                            NULL);*/
	Dispatcher ->dispatchMessage( SEND_MSG_IMMEDIATELY,
		                          getAgentId(),
								  Team()->getSupportingPlayer()->getAgentId(),
								  msg::msgSupportAttacker, NULL);

  }
    
  PlayerBase* BestSupportPly = Team()->DetermineBestSupportingAttacker();
    
  //if the best player available to support the attacker changes, update
  //the pointers and send messages to the relevant players to update their
  //states
  if (BestSupportPly && (BestSupportPly != Team()->getSupportingPlayer()))
  {
    
    if (Team()->getSupportingPlayer())
    {
     /* Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                              getAgentId(),
                              Team()->SupportingPlayer()->getAgentId(),
                              Msg_GoHome,
                              NULL);*/
		Dispatcher ->dispatchMessage(SEND_MSG_IMMEDIATELY,
                              getAgentId(),
                              Team()->getSupportingPlayer()->getAgentId(),
							  msg::msgGoHome, NULL);

    }
    
    
    
    Team()->setSupportingPlayer(BestSupportPly);

    /*Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            getAgentId(),
                            Team()->SupportingPlayer()->getAgentId(),
                            Msg_SupportAttacker,
                            NULL);*/
	Dispatcher ->dispatchMessage(SEND_MSG_IMMEDIATELY,
                            getAgentId(),
                            Team()->getSupportingPlayer()->getAgentId(),
							msg::msgSupportAttacker, NULL);
  }
}


  //calculate distance to opponent's goal. Used frequently by the passing//methods
double PlayerBase::DistToOppGoal()const
{
	return fabs(getPosition().x - Team()->OpponentsGoal()->Center().x);
}

double PlayerBase::DistToHomeGoal()const
{
	return fabs(getPosition().x - Team()->HomeGoal()->Center().x);
}

bool PlayerBase::isControllingPlayer()const
{return Team()->getControllingPlayer()==this;}

bool PlayerBase::BallWithinKeeperRange()const
{
	return (Vec2DDistanceSq(getPosition(), Ball()->getPosition()) < Prm.KeeperInBallRangeSq);
}

bool PlayerBase::BallWithinReceivingRange()const
{
	return (Vec2DDistanceSq(getPosition(), Ball()->getPosition()) < Prm.BallWithinReceivingRangeSq);
}

bool PlayerBase::BallWithinKickingRange()const
{
	return (Vec2DDistanceSq(Ball()->getPosition(), getPosition()) < Prm.PlayerKickingDistanceSq);
}


bool PlayerBase::InHomeRegion()const
{
  if (m_PlayerRole == goal_keeper)
  {
	  return Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(getPosition(), Region::normal);
  }
  else
  {
	  return Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(getPosition(), Region::halfsize);
  }
}

bool PlayerBase::AtTarget()const
{
	return (Vec2DDistanceSq(getPosition(), Steering()->getTarget()) < Prm.PlayerInTargetRangeSq);
}

bool PlayerBase::isClosestTeamMemberToBall()const
{
  return Team()->getPlayerClosestToBall() == this;
}

bool PlayerBase::isClosestPlayerOnPitchToBall()const
{
  return isClosestTeamMemberToBall() && 
         (DistSqToBall() < Team()->getOpponents()->getClosestDistToBallSq());
}

bool PlayerBase::InHotRegion()const
{
	return fabs(getPosition().y - Team()->OpponentsGoal()->Center().y ) <
         Pitch()->PlayingArea()->Length()/3.0;
}

bool PlayerBase::isAheadOfAttacker()const
{
	return fabs(getPosition().x - Team()->OpponentsGoal()->Center().x) <
	  fabs(Team()->getControllingPlayer()->getPosition().x - Team()->OpponentsGoal()->Center().x);
}

SoccerBall* const PlayerBase::Ball()const
{
  return Team()->getPitch()->Ball();
}

SoccerPitch* const PlayerBase::Pitch()const
{
  return Team()->getPitch();
}

const Region* const PlayerBase::HomeRegion()const
{
  return Pitch()->GetRegionFromIndex(m_iHomeRegion);
}


