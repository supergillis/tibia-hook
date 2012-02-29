# Introduction

Tibia Hook is a shared library that can be injected into the Tibia process. It can intercept and fake packets from client and server.
Currently there is support for injecting the hook into the Linux client. In theory, support for the Windows client can be added too.

# Design

The hook is written with Qt. This is because Qt can make cross-platform applications and because it has great support for multithreaded
applications.

When the hook gets injected, a new thread is created. Qt runs in this thread. So we actually have two threads then: the Tibia thread, and
the injected Qt thread.

The hook has three important elements. The `DetourManager` class, the `Hook` class and the `Handler` interface.

## DetourManager

The `DetourManager` class installs the detours, or function hooks. So when Tibia calls the function to send its buffer to the server, we
can actually intercept it and send something different, or we can send nothing at all.

It contains only static functions and static variables. You might ask why I didn't use the singleton pattern here. Well, this is because
we cannot detour a Tibia function to a member function.

The hooked functions get called in Tibia's thread, but the `Hook` and the `Handler` run in the Qt thread. How do we make this thread-safe?
I solved this by looking for a loop in the Tibia client. When we then find a function that gets called every time in this loop, and we
hook it, then we can do stuff in this function, and thus in Tibia's thread. So when we want to send a packet from the Qt thread, we put
this packet on `DetourManager`'s queue and then when the function in the loop gets called, we can send this queued packet in Tibia's
thread.

I'll explain the above with an example. Tibia has a function `send`. We hook this function with a new function, `onSend`. So from now on,
when Tibia calls `send` it will actually call `onSend`. In the `onSend` function we read out the buffer that Tibia is trying to send and
we put a `DataEvent`, that contains the buffer, on the Qt thread. Then we exit the `onSend` function, without actually sending something.
The `Hook`, running in the Qt thread then does all kinds of nasty things with this buffer. If it wants this buffer to be sent to the client
for real, then it puts it on `DetourManager`'s queue. Now remember the loop function. It runs in the Tibia thread. When this function runs,
we check if there is something on the queue. If this is the case, we replace Tibia's buffer with the buffer on the queue and call the real
`send` function. That's how it'sdone. I hope it was clear...

## Hook

The `Hook` class runs in the Qt thread. It delegates client and server messages to an instance of the `Handler` class. The handler decides
if it blocks a packet or not. If it decides that a packet shouldn't be blocked, then the hook puts the packet on `DetourManager`'s queue.

Currently only client packets can be blocked.

## Handler

The `Handler` class is actually an interface. It has no functionality, only pure virtual functions, namely `bool
receiveFromClient(const QByteArray&)` and `void receiveFromServer(const QByteArray&)`.

`receiveFromClient` should return `false` if the packet should be blocked.

# Compiling

There is something I need to confess first. I really suck in writing Makefiles. That's why I've written some stuff in Ruby to compile this
project. It's probably a bit of overkill, but I couldn't have written it faster with a Makefile!

## Requirements

* Ruby (see above)
* libqtcore library
* libqtscript library
* pthread library (for creating the Qt thread)

## Commands

	cd your-projects-directory/tibia-hook
	make all

# Injecting in Tibia

The hook compiles into a shared library which you have to inject in the Tibia client. You will also need to inject the Qt Core and the Qt
Script library.

## Linux
First copy everything from the `bin` folder to the Tibia folder, or make links using `ln -s`. Then add this script to the Tibia folder too
and call it `run.sh`

	BIN=./Tibia
	LIB_HOOK=./hook.so
	LIB_QT_CORE=/usr/lib/i386-linux-gnu/libQtCore.so
	LIB_QT_SCRIPT=/usr/lib/i386-linux-gnu/libQtScript.so

	LD_PRELOAD=$LIB_QT_CORE:$LIB_QT_SCRIPT:$LIB_HOOK ./Tibia

That's it.

## Windows

Not supported (yet).

# Scripting

Scripting is made possible with the `ScriptHandler`. It delegates everything to scripts written in Javascript.

## Core

## Plugins
