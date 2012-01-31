#include "LinuxHookClient.h"

LinuxHookClient::LinuxHookClient(Hook* hook, Display* display, Window window) :
		HookClient(hook), display_(display), window_(window) {
}

XKeyEvent createKeyEvent(Display* display, Window& win, Window& root, bool press, int keycode, int modifiers) {
	XKeyEvent event;
	event.display = display;
	event.window = win;
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

void LinuxHookClient::sendKeyPress(int keycode) {
	Window root = XDefaultRootWindow(display_);

	XKeyEvent event = createKeyEvent(display_, window_, root, true, keycode, 0);
	XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*) &event);

	event = createKeyEvent(display_, window_, root, false, keycode, 0);
	XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*) &event);
}
