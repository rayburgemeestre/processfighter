/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "game_state.h"
#include <memory>

namespace sf {
  class RenderTarget;
}

class state_interface
{
public:

  state_interface(game_state::state_type initial_state = game_state::state_type::initializing)
    : state_(initial_state)
  {
  }

  virtual void initialize() = 0;
  virtual void draw(sf::RenderTarget &renderTarget) = 0;

  std::unique_ptr<state_interface> &next_state() {
    return next_state_;
  };

protected:
  std::unique_ptr<state_interface> next_state_;
  game_state::state_type state_;

};
