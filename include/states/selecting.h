/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "state_interface.hpp"

struct probed_opponent_type
{
  std::string name;
  std::string ip;
  enum class opponent_state {
    idle,
    in_game
  } opponent_state;
};

class selecting : public state_interface
{
public:
  selecting();

  void initialize();
  void draw(sf::RenderTarget &renderTarget);

  void add_opponent(probed_opponent_type opponent);

private:
  std::vector<probed_opponent_type> opponents_;

};
