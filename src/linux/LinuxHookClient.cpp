#include "LinuxHookClient.h"

XKeyEvent createKeyEvent(Display*, Window&, Window&, bool, int, int);

LinuxHookClient::LinuxHookClient(Hook* hook, Display* display, Window window) :
		HookClient(hook), display_(display), window_(window) {
}

void LinuxHookClient::sendKeyPress(int keycode) {
	Window root = XDefaultRootWindow(display_);
	XKeyEvent press = createKeyEvent(display_, window_, root, true, keycode, 0);
	XKeyEvent release = createKeyEvent(display_, window_, root, false, keycode, 0);

	XEvent event1;
	event1.type = KeyPress;
	event1.xkey = press;

	XEvent event2;
	event2.type = KeyRelease;
	event2.xkey = release;

	queue_.enqueue(event1);
	queue_.enqueue(event2);
}

XEvent LinuxHookClient::nextEvent() {
	return queue_.dequeue();
}

int LinuxHookClient::pendingEvents() const {
	return queue_.size();
}

XKeyEvent createKeyEvent(Display* display, Window& window, Window& root, bool press, int keycode, int modifiers) {
	XKeyEvent event;
	event.send_event = True;
	event.serial = XNextRequest(display);
	event.display = display;
	event.window = window;
	event.root = root;
	event.subwindow = None;
	event.time = CurrentTime;
	event.x = 1;
	event.y = 1;
	event.x_root = 1;
	event.y_root = 1;
	event.same_screen = True;
	event.keycode = XKeysymToKeycode(display, keycode);
	event.state = modifiers;
	event.type = press ? KeyPress : KeyRelease;
	return event;
}
