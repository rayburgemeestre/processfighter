/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"

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

private:

  sf::Vector2f position_;
  sf::Texture texture_;
  sf::Sprite sprite_;
};
