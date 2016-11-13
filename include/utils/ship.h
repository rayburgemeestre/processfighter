/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <chrono>

enum class ship_type
{
  top_ship,
  bottom_ship
};

class ship
{
public:
  ship(ship_type type = ship_type::bottom_ship);
  ship(float x, float y, ship_type type);

  const sf::Vector2f & position();
  void render(sf::RenderTarget &render_target);
  void set_position(float x, float y);

  void calculate();

  void left(bool value, std::chrono::time_point<std::chrono::high_resolution_clock> current);
  void right(bool value, std::chrono::time_point<std::chrono::high_resolution_clock> current);

private:

  sf::Vector2f position_;
  sf::Texture texture_;
  sf::Sprite sprite_;

  bool is_left_ = false;
  bool is_right_ = false;
  std::chrono::time_point<std::chrono::high_resolution_clock> begin_;
  double elapsed_ = 0.0f;
  std::chrono::time_point<std::chrono::high_resolution_clock> left_;
  std::chrono::time_point<std::chrono::high_resolution_clock> right_;
  std::chrono::time_point<std::chrono::high_resolution_clock> idle_;
  double elapsed_idle_ = 0.0f;
  sf::Vector2f velocity_;
};
