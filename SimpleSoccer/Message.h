#ifndef Message_h
#define Message_h

#include <iostream>
#include <cmath>

class Message { 

public:

void* inform;
double timeStamp;

//constructor 
Message();

Message (double time, int sender, int iMsg, int receiver,  void* info = NULL);

 ~Message ();

// methods

// set-get

//inline double getTime () const { return timeStamp; }
inline int getSender () const { return _sender; }
inline int getReceiver () const { return _receiver; }
inline int getiMessage() const { return _iMsg; }


protected:

int _sender;
int _receiver;
int _iMsg;



};

const double sDelay = 0.025; // задержка

// operators overload

inline bool operator==(const Message& msg1, const Message& msg2)
{
  return ( fabs(msg1.timeStamp-msg2.timeStamp) < sDelay) &&
          (msg1.getSender() == msg2.getSender())        &&
          (msg1.getReceiver() == msg2.getReceiver())    &&
          (msg1.getiMessage() == msg2.getiMessage()     );
		  

}

inline bool operator<(const Message& msg1, const Message& msg2)
{
  if (msg1 == msg2)
  return false;
  
    return  (msg1.timeStamp < msg2.timeStamp);

}

// for debug
/*inline std::ostream& operator<<(std::ostream& os, const Message& msg)
{
  os << "Time: " << msg.getTime () << "  Sender: " << msg.getSender ()
   << "   Receiver: " << msg.getReceiver () << "   Message: " << msg.getMessage();

  return os;
}*/



#endif
