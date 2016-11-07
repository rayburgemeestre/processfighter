/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "state_interface.hpp"

struct opponent_type
{
  sf::Int64 requesting_id;
  std::string name;
  std::string ip;
  sf::Int16 port;
};

class probing : public state_interface
{
public:
  probing(global_game_state &gs);

  void initialize() override;
  void handle(std::vector<std::unique_ptr<messages::message_interface>> msgs) override;
  void tick() override;
  void draw(sf::RenderTarget &renderTarget) override;

  void probe_opponent(opponent_type type);
  void probed_opponent(opponent_type type);

private:
  std::vector<opponent_type> opponents_;
  std::vector<opponent_type> opponents_probed_;

};
