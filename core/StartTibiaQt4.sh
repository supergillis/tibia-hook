#!/bin/bash

# This script starts Tibia with a set of provided libraries.
# (the file libc6/README explains where those libraries came from)

# Please try calling this script instead of calling ./Tibia
# in case of problems while starting Tibia.
# (a typical error this script might help with is the dreaded
# "Floating point exception" right after starting Tibia)

BIN=./Tibia

LIB_HOOK=./libhook.so
LIB_QT_CORE=/usr/lib/i386-linux-gnu/libQtCore.so
LIB_QT_GUI=/usr/lib/i386-linux-gnu/libQtGui.so

LD_PRELOAD=$LIB_QT_CORE:$LIB_QT_GUI:$LIB_HOOK ./Tibia
