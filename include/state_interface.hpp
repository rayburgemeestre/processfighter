/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "game_state.h"
#include "messages.h"
#include <memory>
#include <chrono>

namespace sf {
  class RenderTarget;
}

class global_game_state;

class state_interface
{
public:

  state_interface(global_game_state &gs, game_state::state_type initial_state = game_state::state_type::initializing)
    : global_game_state_(gs), state_(initial_state), begin_(std::chrono::high_resolution_clock::now())
  {
  }

  virtual void initialize() = 0;
  virtual void handle(std::vector<std::unique_ptr<messages::message_interface>> msgs) = 0;
  virtual void tick() = 0;
  virtual void draw(sf::RenderTarget &renderTarget) = 0;

  std::unique_ptr<state_interface> &next_state() {
    return next_state_;
  };

  game_state::state_type type() {
    return state_;
  }

protected:
  global_game_state &global_game_state_;
  std::unique_ptr<state_interface> next_state_;
  game_state::state_type state_;
  std::chrono::time_point<std::chrono::high_resolution_clock> begin_;

};
