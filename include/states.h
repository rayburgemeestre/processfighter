/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <memory>

#include "game_state.h"
#include "state_interface.hpp"

class states
{
public:
  static std::unique_ptr<state_interface> factory(game_state::state_type state);
};
