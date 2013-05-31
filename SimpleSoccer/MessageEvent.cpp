#include "MessageEvent.h"
#include "Agent.h"
#include "AgentManager.h"
#include "misc/FrameCounter.h"
#include "Debug/DebugConsole.h"

//uncomment below to send message info to the debug window
//#define SHOW_MESSAGING_INFO

//--------------------------- Instance ----------------------------------------

MessageEvent* MessageEvent::Instance()
{
  static MessageEvent instance; 
  
  return &instance;
}

//----------------------------- Dispatch ---------------------------------

void MessageEvent::discharge(Agent* receiver, const Message& msg) 
{
	if (!receiver -> HandleMessage(msg)) 
	{
		    #ifdef SHOW_MESSAGING_INFO
    debug_con << "Message not handled" << "";
    #endif
	}
}

//---------------------------- DispatchMessage---------------------------
//
//  given a message, a receiver, a sender and any time delay, this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------


void MessageEvent::dispatchMessage(double  delay,
                                    int     sender,
                                    int     receiver,
                                    int     iMsg,
									void*   extraInfo = NULL) 
{

	Agent* aReceiver = AgentMgr -> getEntityFromId(receiver);

  //make sure the receiver is valid
  if (aReceiver == NULL)
  {
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nWarning! No Receiver with Id  of " << receiver << " found" << "";
    #endif

    return;
  }

    //create the telegram
  Message message(0, sender, receiver, iMsg, extraInfo);

    //if there is no delay, route telegram immediately                       
  if (delay <= 0.0)                                                        
  {
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nTelegram dispatched at time: " << TickCounter->GetCurrentFrame()
         << " by " << sender << " for " << receiver 
         << ". Msg is " << msg << "";
    #endif

    //send the telegram to the recipient
	discharge(aReceiver, message);
  }
  
    else
  {
    double currentTime = TickCounter->GetCurrentFrame(); 
	message.timeStamp = currentTime + delay;
    //and put it in the queue
	pq.insert(message);

    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nDelayed message from " << sender << " recorded at time " 
            << TickCounter->GetCurrentFrame() << " for " << receiver
            << ". Message is " << msg << "";
    #endif
  }
}



void MessageEvent::dispatchDelayedMessages() 
{
	//first get current time
  double currentTime = TickCounter->GetCurrentFrame();
  while(!pq.empty() && (pq.begin()->timeStamp < currentTime) && (pq.begin() ->timeStamp > 0) )
  {
    const Message& msg = *pq.begin();

 //find the recipient
	Agent* aReceiver = AgentMgr ->getEntityFromId(msg.getReceiver());

    //send the telegram to the recipient
	discharge(aReceiver, msg);

	//remove it from the queue
  pq.erase(pq.begin());
  }
}