/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <vector>
#include <tuple>

class game_state
{
public:
  enum class state_type
  {
    initializing,
    probing,
    selecting,
    waiting,
    countdown,
    fighting,
    celebrating, // you 'won'
    // there is no 'lost', you fight until your system dies
  };
  enum class transition_type
  {
    next,

    refresh,
    select,

    bail,
    decline,
    accept,
  };

  game_state(state_type initial_state = state_type::initializing);
  state_type state();
  state_type transition(transition_type transition);

private:
  state_type state_;
  std::vector<std::tuple<state_type, transition_type, state_type>> transitions_;

};

