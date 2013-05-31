#ifndef MessageEvent_h
#define MessageEvent_h

#include <set>
#include <string>
#include "Message.h"

class Agent;


#define Dispatcher MessageEvent::Instance()

//to make code easier to read
const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    SENDER_ID_IRRELEVANT = -1;

class MessageEvent { 

	private:
       std::set<Message> pq;

	   MessageEvent(){}

  MessageEvent(const MessageEvent&);
  MessageEvent& operator=(const MessageEvent&);
  void discharge(Agent* receiver, const Message& msg);

public:
	static MessageEvent* Instance();

	//send a message to another agent. 
  void dispatchMessage(double  delay,
                   int     sender,
                   int     receiver,
                   int     iMsg,
                   void*  extraInfo);

  void dispatchDelayedMessages();


};


#endif