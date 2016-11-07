/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "state_interface.hpp"

class initializing : public state_interface
{
public:
  initializing(global_game_state &gs);

  void initialize() override;
  void handle(std::vector<std::unique_ptr<messages::message_interface>> msgs) override;
  void tick() override;
  void draw(sf::RenderTarget &renderTarget) override;
};
