// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Helpers for hosting madronalib tasks in a console application.

#pragma once

namespace ml
{

class AudioTask;

// Install SIGINT / SIGTERM handlers and block until one of them arrives.
void waitForQuitSignal();

// Start the task's audio device, print stream info, block until a quit
// signal arrives, then stop the device. Returns a process exit code.
int runConsoleApp(AudioTask& task);

}  // namespace ml
