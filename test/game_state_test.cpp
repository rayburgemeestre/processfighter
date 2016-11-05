/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "gtest/gtest.h"

#include "game_state.h"

TEST(GameTest, test_state_initial) {
  game_state g;
  ASSERT_EQ(g.state(), game_state::state_type::initializing);
}

TEST(GameTest, test_state_transition_next) {
  game_state g;
  g.transition(game_state::transition_type::next);
  ASSERT_EQ(g.state(), game_state::state_type::probing);
}

TEST(GameTest, test_state_transition_invalid_next) {
  game_state g;
  try {
    g.transition(game_state::transition_type::select);
    FAIL() << "expected runtime_error";
  }
  catch (std::runtime_error &e) {
    ASSERT_TRUE(std::string(e.what()).find("given transition not supported for current state") == 0);
  }
  catch (...) {
    FAIL() << "expected runtime_error";
  }
}

TEST(GameTest, test_state_transition_waiting) {
  game_state g;
  g.transition(game_state::transition_type::next); // probing
  g.transition(game_state::transition_type::next); // selecting
  g.transition(game_state::transition_type::select); // selecting
  ASSERT_EQ(g.state(), game_state::state_type::waiting);
}
