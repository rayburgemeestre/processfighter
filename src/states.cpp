/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "states.h"
#include "states/initializing.h"
#include "states/probing.h"
#include "states/selecting.h"
#include "states/waiting.h"
#include "states/challenged.h"
#include "states/countdown.h"

std::unique_ptr<state_interface> states::factory(game_state::state_type state, global_game_state &gs)
{
  switch (state) {
    case game_state::state_type::initializing:
      return std::make_unique<initializing>(gs);
    case game_state::state_type::probing:
      return std::make_unique<probing>(gs);
    case game_state::state_type::selecting:
      return std::make_unique<selecting>(gs);
    case game_state::state_type::waiting:
      return std::make_unique<waiting>(gs);
    case game_state::state_type::challenged:
      return std::make_unique<challenged>(gs);
    case game_state::state_type::countdown:
      return std::make_unique<countdown>(gs);
    default:
      break;
  }
  return nullptr;
}
