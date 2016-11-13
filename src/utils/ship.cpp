/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "utils/ship.h"
#include "ship_png.h"
#include "ship2_png.h"

#include <iostream>
#include <SFML/Graphics.hpp>

extern int window_width;
extern int window_height;

ship::ship(ship_type type)
  : ship(0.f, 0.f, type)
{

}

ship::ship(float x, float y, ship_type type)
  : position_(x, y), is_left_(false),
    begin_(std::chrono::high_resolution_clock::now()),
    idle_(std::chrono::high_resolution_clock::now())
{
  if (type == ship_type::top_ship) {
    if (!texture_.loadFromMemory(ship_png, ship_png_len, sf::IntRect(0, 0, 256, 256))) {
      std::cerr << "Error initializing ship sprite.." << std::endl;
      std::exit(1);
    }
  }
  else if (type == ship_type::bottom_ship) {
    if (!texture_.loadFromMemory(ship2_png, ship2_png_len, sf::IntRect(0, 0, 256, 256))) {
      std::cerr << "Error initializing ship sprite.." << std::endl;
      std::exit(1);
    }
  }
  texture_.setSmooth(true);
  sprite_.setScale(0.5f, 0.5f);
  sprite_.setTexture(texture_);
}

void ship::set_position(float x, float y)
{
  position_.x = x;
  position_.y = y;
}

const sf::Vector2f & ship::position()
{
  return position_;
}

void ship::render(sf::RenderTarget &render_target)
{
  auto ship_width = -256 / 2.f;
  auto ship_height = -256 / 2.f;
  auto enemy_x = position_.x;
  auto enemy_y = position_.y;

  auto current_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = current_time - begin_;
  while (elapsed.count() - elapsed_ > 1) {
    enemy_x += velocity_.x;
    elapsed_ += 1;
  }

  while (enemy_x < 0) enemy_x += window_width;
  while (enemy_x > window_width) enemy_x -= window_width;
  while (enemy_y < 0) enemy_y += window_width;
  while (enemy_y > window_width) enemy_y -= window_width;

  sprite_.setPosition(enemy_x, enemy_y);

//  sf::CircleShape shape(10.f);
//  shape.setPosition(enemy_x, enemy_y);
//  shape.move(-5.f, -5.f);
//  render_target.draw(shape);

  sprite_.move(ship_width / 2.f, ship_height / 2.f);
  render_target.draw(sprite_);

  // simple trick to make it move inside the screen from left/right etc.
  sprite_.move(window_width, 0.f);
  render_target.draw(sprite_);
  sprite_.move(-2 * window_width, 0.f);
  render_target.draw(sprite_);

  position_.x = enemy_x;
  position_.y = enemy_y;
}

void ship::calculate()
{
  auto current_time = std::chrono::high_resolution_clock::now();
  auto set_idle = [=](){
    idle_ = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_idle = current_time - idle_;
    elapsed_idle_ = elapsed_idle.count();
  };
  if (is_left_) {
    std::chrono::duration<double, std::milli> elapsed_left = current_time - left_;
    velocity_.x = -1 * std::min(static_cast<float>(elapsed_left.count() / 1000.f), 1.f);
    set_idle();
    return;
  }
  if (is_right_) {
    std::chrono::duration<double, std::milli> elapsed_right = current_time - right_;
    velocity_.x = std::min(static_cast<float>(elapsed_right.count() / 1000.f), 1.f);
    set_idle();
    return;
  }

  std::chrono::duration<double, std::milli> elapsed_idle = current_time - idle_;
  while (elapsed_idle.count() - elapsed_idle_ > 50) {
    velocity_.x /= 2;
    elapsed_idle_ += 50;
  }
}

void ship::left(bool value, std::chrono::time_point<std::chrono::high_resolution_clock> current)
{
  if (!is_left_ && value) {
    left_ = current;
  }
  is_left_ = value;
}
void ship::right(bool value, std::chrono::time_point<std::chrono::high_resolution_clock> current)
{
  if (!is_right_ && value) {
    right_ = current;
  }
  is_right_ = value;
}
