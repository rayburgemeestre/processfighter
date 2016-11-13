/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <SFML/Graphics/CircleShape.hpp>
#include "utils/bullet.h"

extern int window_width;

bullet::bullet(sf::Vector2f position, sf::Vector2f velocity)
  : position_(position), velocity_(velocity), begin_(std::chrono::high_resolution_clock::now())
{
}

void bullet::calculate()
{
  auto current_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed_idle = current_time - begin_;
  while (elapsed_idle.count() - processed > 1) {
    position_.y -= 0.5f;
    while (position_.x < 0)
      position_.x += window_width;
    while (position_.x > window_width)
      position_.x -= window_width;

    // adding all velocity is a bit too much..
    position_.x += velocity_.x / 3.f;
    position_.y += velocity_.y / 3.f;

    processed += 1;
  }

}

void bullet::render(sf::RenderTarget &render_target)
{
  sf::CircleShape shape(5.f);
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(position_);
  shape.move(-2.5f, -2.5f);
  shape.move(velocity_);
  render_target.draw(shape);
}
