/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "state_interface.hpp"

struct probed_opponent_type
{
  enum class probed_opponent_state {
    idle,
    in_game
  };
  size_t id;
  std::string name;
  std::string ip;
  probed_opponent_state opponent_state;
};

class selecting : public state_interface
{
public:
  selecting(global_game_state &gs);

  void initialize();
  void handle(std::vector<std::unique_ptr<messages::message_interface>> msgs) override;
  void tick() override;
  void draw(sf::RenderTarget &renderTarget) override;

  void add_opponent(probed_opponent_type opponent);

private:
  std::vector<probed_opponent_type> opponents_;

};
