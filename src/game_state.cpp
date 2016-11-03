/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "game_state.h"
#include <typeinfo>

using state_type = game_state::state_type;
using transition_type = game_state::transition_type;

game_state::game_state(state_type initial_state) :
  state_(initial_state),
  transitions_{
    std::make_tuple(state_type::initializing, transition_type::next, state_type::probing),
    std::make_tuple(state_type::probing, transition_type::next, state_type::selecting),
    std::make_tuple(state_type::selecting, transition_type::select, state_type::waiting),
    std::make_tuple(state_type::selecting, transition_type::refresh, state_type::initializing),
    std::make_tuple(state_type::selecting, transition_type::select, state_type::waiting),
    std::make_tuple(state_type::waiting, transition_type::bail, state_type::initializing),
    std::make_tuple(state_type::waiting, transition_type::decline, state_type::initializing),
    std::make_tuple(state_type::waiting, transition_type::accept, state_type::countdown),
    std::make_tuple(state_type::countdown, transition_type::next, state_type::fighting),
    std::make_tuple(state_type::fighting, transition_type::next, state_type::celebrating),
    std::make_tuple(state_type::celebrating, transition_type::next, state_type::initializing),
  }
{
}

state_type game_state::state() {
  return state_;
}

state_type game_state::transition(transition_type transition) {
  for (const auto &transition_tuple : transitions_) {
    const auto &current_state = std::get<0>(transition_tuple);
    const auto &available_transition = std::get<1>(transition_tuple);
    const auto &resulting_state = std::get<2>(transition_tuple);
    if (current_state == state() && available_transition == transition) {
      state_ = resulting_state;
      return state_;
    }
  }
  throw std::runtime_error(std::string("given transition not supported for current state ") + typeid(state()).name());
}
