#pragma warning (disable:4786)
#ifndef FIELDPLAYER_H
#define FIELDPLAYER_H

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

#include "FieldPlayerStates.h"
#include "2D/Vector2D.h"
#include "FSM/StateMachine.h"
#include "PlayerBase.h"
#include "FSM/StateMachine.h"
#include "time/Regulator.h"

class CSteeringBehavior;
class SoccerTeam;
class SoccerPitch;
class Goal;
struct Telegram;


class FieldPlayer : public PlayerBase
{
private:

   //an instance of the state machine class
  StateMachine<FieldPlayer>*  m_pStateMachine;
  
  //limits the number of kicks a player may take per second
  Regulator*                  m_pKickLimiter;

  
public:

  FieldPlayer(SoccerTeam*    home_team,
             int        home_region,
             State<FieldPlayer>* start_state,
             Vector2D  heading,
             Vector2D      velocity,
             double         mass,
             double         max_force,
             double         max_speed,
             double         max_turn_rate,
             double         scale,
             player_role    role);   
  
  ~FieldPlayer();

  //call this to update the player's position and orientation
  void        Update();   

  void        Render();

  bool        HandleMessage(const Message& msg);

  StateMachine<FieldPlayer>* getFSM()const{return m_pStateMachine;}

  bool        isReadyForNextKick()const{return m_pKickLimiter->isReady();}

         
};




#endif