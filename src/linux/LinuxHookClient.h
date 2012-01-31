#ifndef LINUXHOOKCLIENT_H_
#define LINUXHOOKCLIENT_H_

#include "HookClient.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

/* Including  <X11/Xlib.h> gives an error with Qt. Undefining Bool fixes that. */
#undef Bool

class LinuxHookClient: public HookClient {
public:
	LinuxHookClient(Hook* hook, Display*, Window);

	void sendKeyPress(int);

private:
	Display* display_;
	Window window_;
};

#endif /* LINUXHOOKCLIENT_H_ */
