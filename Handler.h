#ifndef HANDLER_H_
#define HANDLER_H_

#include "Message.h"

class Handler {
public:
	virtual void handleOutgoingMessage(const Message&) = 0;
	virtual void handleIncomingMessage(const Message&) = 0;
};

#endif /* HANDLER_H_ */
