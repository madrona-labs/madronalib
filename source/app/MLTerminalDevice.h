// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once


namespace ml
{

namespace terminalDevice
{

#ifdef ML_MAC
#include <unistd.h>
#include <termios.h>

void waitForConsoleKeyPress()
{
  if (isatty(STDIN_FILENO))
  {
    // Real terminal: disable canonical mode so any keypress works without Enter.
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  } 
  else
  {
    // IDE / piped stdin (e.g. Xcode): raw mode doesn't apply; wait for Enter.
    std::cin.get();
  }
}

#endif  // ML_MAC
#ifdef ML_WINDOWS
#include <conio.h>

void waitForConsoleKeyPress()
{
  // _getch() blocks until one keypress without requiring Enter, no echo.
  _getch();
}

#endif  // ML_WINDOWS

#if !defined(ML_MAC) && !defined(ML_WINDOWS)
void waitForConsoleKeyPress()
{
  std::cout << "Press Enter to continue...";
  std::cin.get();
}
#endif

}  // namespace terminalDevice
}  // namespace ml
