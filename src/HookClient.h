#ifndef HOOKCLIENT_H_
#define HOOKCLIENT_H_

class Hook;
class HookClient {
public:
	HookClient(Hook*);

	virtual void sendKeyPress(int) = 0;

	virtual int pendingEvents() const = 0;

private:
	Hook* hook_;
};

#endif /* HOOKCLIENT_H_ */
