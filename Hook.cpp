#include <stdio.h>
#include <string.h>

#include "Hook.h"
#include "HookMessageEvent.h"
#include "Main.h"
#include "Handler.h"
#include "ScriptHandler.h"

Hook* Hook::_instance = NULL;

static int _argc = 0;

Hook::Hook() :
		QCoreApplication(_argc, NULL), _socket(-1), _display(NULL), _loggedIn(true), _pendingLogin(false), _protocol(0) {
	_handler = new ScriptHandler();
}

Hook::~Hook() {
	delete _handler;
}

const int Hook::socket() const {
	return _socket;
}

void Hook::setSocket(int socket) {
	qDebug() << "set socket";
	_socket = socket;
}

const Display* Hook::display() const {
	return _display;
}

void Hook::setDisplay(Display* display) {
	qDebug() << "set display";
	_display = display;
}

const Window Hook::window() const {
	return _window;
}

void Hook::setWindow(Window window) {
	qDebug() << "set window";
	_window = window;
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

void Hook::sendKeyPress(int keycode) {
	Window root = XDefaultRootWindow(_display);
	XKeyEvent event = createKeyEvent(_display, _window, root, true, keycode, 0);
	XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*) &event);

	event = createKeyEvent(_display, _window, root, false, keycode, 0);
	XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*) &event);
}

ssize_t Hook::read(quint8* buffer, ssize_t length) {
	return __read(_socket, buffer, length);
}

ssize_t Hook::write(const quint8* buffer, ssize_t length) {
	return __write(_socket, buffer, length);
}

ssize_t Hook::write(const EncryptedMessage* message) {
	return write(message->rawData(), message->rawLength());
}

ssize_t Hook::write(const DecryptedMessage* message) {
	EncryptedMessage encrypted(message);
	return encrypted.isValid() ? write(&encrypted) : 0;
}

ssize_t Hook::hookOutgoingMessage(const quint8* buffer, ssize_t length) {
	if (_loggedIn) {
		EncryptedMessage message(buffer, length);
		if (message.isValid()) {
			QCoreApplication::postEvent(_handler, new HookMessageEvent(HookMessageEvent::Outgoing, &message), Qt::HighEventPriority);
			return length;
		}
	}
	else {
		DecryptedMessage message(buffer, length);
		ReadOnlyPacket packet(message);
		quint8 protocol = packet.readU8();
		quint16 os = packet.readU16();
		quint16 client = packet.readU16();
		if (protocol == 0x0A) {
			_protocol = protocol;
			_pendingLogin = true;
		}
	}
	return write(buffer, length);
}

ssize_t Hook::hookIncomingMessage(quint8* buffer, ssize_t length) {
	ssize_t result = read(buffer, length);
	return result;
}
