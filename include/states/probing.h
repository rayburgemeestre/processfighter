/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "state_interface.hpp"

struct opponent_type
{
  std::string name;
  std::string ip;
};

class probing : public state_interface
{
public:
  probing();

  void initialize();
  void draw(sf::RenderTarget &renderTarget);

  void probe_opponent(opponent_type type);

private:
  std::vector<opponent_type> opponents_;

};
