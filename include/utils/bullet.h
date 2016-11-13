/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <chrono>
#include <SFML/Graphics/RenderTarget.hpp>

class bullet
{
public:
  bullet(sf::Vector2f position, sf::Vector2f velocity);

  void calculate();
  void render(sf::RenderTarget &render_target);

private:
  sf::Vector2f position_;
  sf::Vector2f velocity_;
  std::chrono::time_point<std::chrono::high_resolution_clock> begin_;
  double processed;
};
