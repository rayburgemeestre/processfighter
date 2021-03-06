/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "state_interface.hpp"

#include "states/selecting.h" // for struct probed_opponent_type
#include <deque>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class countdown : public state_interface
{
public:
  countdown(global_game_state &gs);

  void initialize() override;
  void handle(std::vector<std::unique_ptr<messages::message_interface>> msgs) override;
  void tick() override;
  void draw(sf::RenderTarget &renderTarget) override;

  void set_opponent(probed_opponent_type opponent);
  void set_challenger(bool is_challenger);

private:
  probed_opponent_type opponent_;
  bool is_challenger_;
  std::chrono::time_point<std::chrono::high_resolution_clock> heartbeat_;
  std::deque<double> lag_;

  sf::Font font_;
  sf::Text text_;
};
