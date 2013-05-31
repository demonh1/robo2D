#ifndef msg_h
#define msg_h

namespace msg {

enum PlayerMessages {
  msgReceiveBall = 0,
  msgPassToMe,
  msgSupportAttacker,
  msgGoHome,
  msgWait
};

inline char* messageToString(int iMsg) {

switch (iMsg)
  {
  case  msgReceiveBall:
    
    return "ReceiveBall";

  case msgPassToMe:
    
    return "PassToMe";

  case msgSupportAttacker:

    return "SupportAttacker";

  case msgGoHome:

    return "GoHome";

  case msgWait:

    return "Wait";

  default:

    return "Invalid message";
  }

 } 

}
#endif