#ifndef FIELDPLAYERSTATES_H
#define FIELDPLAYERSTATES_H
//------------------------------------------------------------------------
//
//  Name: FieldPlayerStates.h
//
//
//------------------------------------------------------------------------

#include <string>

#include "FSM/State.h"
#include "Message.h"
#include "constants.h"


class FieldPlayer;
class SoccerPitch;


//------------------------------------------------------------------------
class GlobalPlayerState : public State<FieldPlayer>
{
private:
  
  GlobalPlayerState(){}

public:

  
  static GlobalPlayerState* Instance();

  void Enter(FieldPlayer* player){}

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Message&);
};

//------------------------------------------------------------------------
class ChaseBall : public State<FieldPlayer>
{
private:
  
  ChaseBall(){ }

public:


  static ChaseBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Message&){return false;}
};

//------------------------------------------------------------------------
class Dribble : public State<FieldPlayer>
{
private:
  
  Dribble(){}

public:

 
  static Dribble* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Message&){return false;}
};


//------------------------------------------------------------------------
class ReturnToHomeRegion: public State<FieldPlayer>
{
private:
  
  ReturnToHomeRegion(){}

public:

  static ReturnToHomeRegion* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Message&){return false;}
};

//------------------------------------------------------------------------
class Wait: public State<FieldPlayer>
{
private:
  
  Wait(){}

public:

  static Wait* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Message&){return false;}
};

//------------------------------------------------------------------------
class KickBall: public State<FieldPlayer>
{
private:
  
  KickBall(){}

public:

  static KickBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Message&){return false;}
};

//------------------------------------------------------------------------
class ReceiveBall: public State<FieldPlayer>
{
private:
  
  ReceiveBall(){}

public:

  static ReceiveBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Message&){return false;}
};


//------------------------------------------------------------------------
class SupportAttacker: public State<FieldPlayer>
{
private:
  
  SupportAttacker(){}

public:

  static SupportAttacker* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Message&){return false;}
};




  
#endif