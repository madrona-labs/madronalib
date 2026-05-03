// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once


namespace ml
{

namespace terminalDevice
{

// TODO move to TerminalDevice

#ifdef ML_MAC
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

void waitForConsoleKeyPress()
{
  char ch{EOF};
  struct termios oldt, newt;
  int oldf;
  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
  while (ch == EOF)
  {
    ch = getchar();
    std::this_thread::sleep_for(10ms);
  }
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
}

#endif  // ML_MAC
#ifdef ML_WINDOWS
#include <conio.h>

char keyPressedAsync()
{
  for (int i = 0x07; i < 256; ++i)
  {
    if (GetAsyncKeyState(i) & 0x8000)
    {
      return i;
    }
  }
  return false;
}

void waitForConsoleKeyPress()
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(hConsole, &cursorInfo);
  cursorInfo.bVisible = false;
  SetConsoleCursorInfo(hConsole, &cursorInfo);
  
  while (true)
  {
    if (_kbhit())
    {
      int ch = _getch();
      break;
    }
    std::this_thread::sleep_for(10ms);
  }
  
  cursorInfo.bVisible = true;
  SetConsoleCursorInfo(hConsole, &cursorInfo);
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
