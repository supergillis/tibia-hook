## Introduction

Tibia Hook is a shared library that can be injected into the Tibia process. It can intercept and fake packets from client and server.
Currently there is support for injecting the hook into the Linux client. In theory, support for the Windows client can be added too.

## Compiling

### Requirements

* libqtcore library
* libqtgui library
* pthread library (for creating the Qt thread)

### Commands

	cd your-projects-directory/tibia-hook
	qmake && make all && make install

## Injecting in Tibia

The hook compiles into a shared library which you have to inject in the Tibia client. You will also need to inject the Qt Core and the Qt
Script library.

### Linux
First copy everything from the `bin` folder to the Tibia folder, or make links using `ln -s`. Then add this script to the Tibia folder too
and call it `run.sh`

	BIN=./Tibia

	LIB_HOOK=./libhook.so
	LIB_QT_CORE=/usr/lib/i386-linux-gnu/libQtCore.so
	LIB_QT_GUI=/usr/lib/i386-linux-gnu/libQtGui.so

	LD_PRELOAD=$LIB_QT_CORE:$LIB_QT_GUI:$LIB_HOOK ./Tibia

That's it.

### Windows

Not supported (yet).