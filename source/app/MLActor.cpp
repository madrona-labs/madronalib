// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2022 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLActor.h"

using namespace ml;

// Define static member for optional logging callback
ActorLogCallback Actor::logCallback_{nullptr};

Actor* ActorRegistry::getActor(Path actorName) { return actors_[actorName]; }

void ActorRegistry::doRegister(Path actorName, Actor* a)
{
  actors_[actorName] = a;
  a->registeredName_ = actorName;  // Store name for logging identification
}

void ActorRegistry::doRemove(Actor* actorToRemove)
{
  // get exclusive access to the Tree
  std::unique_lock<std::mutex> lock(listMutex_);

  // remove the Actor
  for (auto it = actors_.begin(); it != actors_.end(); ++it)
  {
    Actor* pa = *it;
    if (pa == actorToRemove)
    {
      auto p = it.getCurrentPath();
      actors_[p] = nullptr;
    }
  }
}

void ActorRegistry::dump() { actors_.dump(); }
