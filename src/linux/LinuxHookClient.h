#ifndef LINUXHOOKCLIENT_H_
#define LINUXHOOKCLIENT_H_

#include <QQueue>

#include "HookClient.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

/* Including  <X11/Xlib.h> gives an error with Qt. Undefining Bool fixes that. */
#undef Bool

class LinuxHookClient: public HookClient {
public:
	LinuxHookClient(Hook* hook, Display*, Window);

	void sendKeyPress(int);

	XEvent nextEvent();

	int pendingEvents() const;

private:
	Display* display_;
	Window window_;

	QQueue<XEvent> queue_;
};

#endif /* LINUXHOOKCLIENT_H_ */
