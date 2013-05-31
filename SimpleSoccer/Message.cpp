#include "Message.h"

Message::Message () { }


Message::Message (double time, int sender, int iMsg, int receiver, void* info):
timeStamp(time),
	_sender(sender),
	_iMsg(iMsg),
	_receiver(receiver), 
	inform(info) 
{ }

Message::~Message () { }