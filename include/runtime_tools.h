#ifndef RUNTIME_TOOLS_H
#define RUNTIME_TOOLS_H

#include <env_build.h>

long getEpochFromRTC();

int getRotar();

void print_Clock(bool mode);

void display_edit_mode(LinkedList<char> &inputList, bool mode);

long set_timer_time(LinkedList<char> &inputList);

void ringBell();

#endif