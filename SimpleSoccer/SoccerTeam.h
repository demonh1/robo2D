#ifndef SOCCERTEAM_H
#define SOCCERTEAM_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   SoccerTeam.h
//
//  Desc:   class to define a team of soccer playing agents. 
//            A SoccerTeam is implemented as FSM and has states for
//          attacking, defending, and KickOff.
//
//
//------------------------------------------------------------------------

#include <vector>

#include "Game/Region.h"
#include "SupportSpotCalculator.h"
#include "FSM/StateMachine.h"

class Goal;
class PlayerBase;
class FieldPlayer;
class SoccerPitch;
class GoalKeeper;
class SupportSpotCalculator;




                
class SoccerTeam 
{
public:
  
  enum team_color {blue, red};

private:

  StateMachine<SoccerTeam>*  m_pStateMachine;

  team_color                m_Color;

  std::vector<PlayerBase*>  m_Players;

  SoccerPitch*              m_pPitch;

  Goal*                     m_pOpponentsGoal;
  Goal*                     m_pHomeGoal;
  
  SoccerTeam*               m_pOpponents;
   
  //pointers to 'key' players
  PlayerBase*               m_pControllingPlayer;
  PlayerBase*               m_pSupportingPlayer;
  PlayerBase*               m_pReceivingPlayer;
  PlayerBase*               m_pPlayerClosestToBall;

  double                     m_dDistSqToBallOfClosestPlayer;

  //players use this to determine strategic positions on the playing field
  SupportSpotCalculator*    m_pSupportSpotCalc;

  void CreatePlayers();

  //called each frame. Sets m_pClosestPlayerToBall to point to the player
  //closest to the ball. 
  void CalculateClosestPlayerToBall();


public:

  SoccerTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~SoccerTeam();

  //
  void        Render()const;
  void        Update();

  //calling this changes the state of all field players to that of 
  //ReturnToHomeRegion. Mainly used when a goal keeper has
  //possession
  void        ReturnAllFieldPlayersToHome()const;

  //returns true if player has a clean shot at the goal and sets ShotTarget
  //to a normalized vector pointing in the direction the shot should be
  //made. Else returns false and sets heading to a zero vector
  bool        CanShoot(Vector2D  BallPos,
                       double     power, 
                       Vector2D& ShotTarget = Vector2D())const;

  //The best pass is considered to be the pass that cannot be intercepted 
  //by an opponent and that is as far forward of the receiver as possible  
  //If a pass is found, the receiver's address is returned in the 
  //reference, 'receiver' and the position the pass will be made to is 
  //returned in the  reference 'PassTarget'
  bool        FindPass(const PlayerBase*const passer,
                      PlayerBase*&           receiver,
                      Vector2D&              PassTarget,
                      double                  power,
                      double                  MinPassingDistance)const;

  //Three potential passes are calculated. One directly toward the receiver's
  //current position and two that are the tangents from the ball position
  //to the circle of radius 'range' from the receiver.
  //These passes are then tested to see if they can be intercepted by an
  //opponent and to make sure they terminate within the playing area. If
  //all the passes are invalidated the function returns false. Otherwise
  //the function returns the pass that takes the ball closest to the 
  //opponent's goal area.
  bool        getBestPassToReceiver(const PlayerBase* const passer,
                                    const PlayerBase* const receiver,
                                    Vector2D& PassTarget,
                                    const double power)const;

  //test if a pass from positions 'from' to 'target' kicked with force 
  //'PassingForce'can be intercepted by an opposing player
  bool        isPassSafeFromOpponent(Vector2D    from,
                                     Vector2D    target,
                                     const PlayerBase* const receiver,
                                     const PlayerBase* const opp,
                                     double       PassingForce)const;

  //tests a pass from position 'from' to position 'target' against each member
  //of the opposing team. Returns true if the pass can be made without
  //getting intercepted
  bool        isPassSafeFromAllOpponents(Vector2D from,
                                         Vector2D target,
                                         const PlayerBase* const receiver,
                                         double     PassingForce)const;

  //returns true if there is an opponent within radius of position
  bool        isOpponentWithinRadius(Vector2D pos, double rad);

  //this tests to see if a pass is possible between the requester and
  //the controlling player. If it is possible a message is sent to the
  //controlling player to pass the ball asap.
  void        RequestPass(FieldPlayer* requester)const;

  //calculates the best supporting position and finds the most appropriate
  //attacker to travel to the spot
  PlayerBase* DetermineBestSupportingAttacker();
  

  const std::vector<PlayerBase*>& Members()const{return m_Players;}  

  StateMachine<SoccerTeam>* getFSM()const{return m_pStateMachine;}
  
  Goal*const           HomeGoal()const{return m_pHomeGoal;}
  Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}

  SoccerPitch*const  getPitch()const{return m_pPitch;}           

  SoccerTeam*const     getOpponents()const{return m_pOpponents;}
  void                 setOpponents(SoccerTeam* opps){m_pOpponents = opps;}

  team_color           getColor()const{return m_Color;}

  void                 setPlayerClosestToBall(PlayerBase* plyr){m_pPlayerClosestToBall=plyr;}
  PlayerBase*          getPlayerClosestToBall()const{return m_pPlayerClosestToBall;}
  
  double               getClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

  Vector2D             getSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

  PlayerBase*          getSupportingPlayer()const{return m_pSupportingPlayer;}
  void                 setSupportingPlayer(PlayerBase* plyr){m_pSupportingPlayer = plyr;}

  PlayerBase*          getReceiver()const{return m_pReceivingPlayer;}
  void                 setReceiver(PlayerBase* plyr){m_pReceivingPlayer = plyr;}

  PlayerBase*          getControllingPlayer()const{return m_pControllingPlayer;}
  void                 setControllingPlayer(PlayerBase* player)
  {
    m_pControllingPlayer = player;

    //rub it in the opponents faces!
    getOpponents()->LostControl();
  }


  bool  inControl()const{ return (m_pControllingPlayer)? true : false;  }
  void  LostControl(){ m_pControllingPlayer = NULL; }

  PlayerBase*  getPlayerFromID(int id)const;
  

  void setPlayerHomeRegion(int plyr, int region)const;

  void DetermineBestSupportingPosition()const{
	  m_pSupportSpotCalc->DetermineBestSupportingPosition();
  }

  void UpdateTargetsOfWaitingPlayers()const;

  //returns false if any of the team are not located within their home region
  bool AllPlayersAtHome()const;

  std::string getName()const {
	  if (m_Color == blue) return "Blue"; 
	  return "Red";
  }

};

#endif