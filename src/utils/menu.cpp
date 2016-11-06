/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <iostream>
#include <algorithm>
#include <sstream>
#include "utils/menu.h"
#include "font.h"

#include <SFML/Graphics.hpp>
#include <global_game_state.h>

menu::menu(size_t window_width, size_t window_height)
  : window_width_(window_width), window_height_(window_height)
{
}

void menu::add_item(std::string menu_text, on_click_function on_click)
{
  menu_items.push_back(std::make_pair(menu_text, on_click));
}

void menu::render(sf::RenderTarget & render_target)
{
  auto total_rects = static_cast<int>(menu_items.size());
  auto num_cols = std::max(1, total_rects / 10);
  auto num_rects = (total_rects / num_cols) + 0.5;
  auto dyn_height = 450 / num_rects;
  auto rect_margin_size = dyn_height * 0.2;

  auto dyn_width = 700 / num_cols;
  auto rect_margin_width = dyn_width * 0.2;

  auto rect_size = sf::Vector2f(dyn_width * 0.8, dyn_height * 0.8);
  auto total_height = (num_rects * rect_size.y) + ((num_rects - 1) * rect_margin_size);
  auto total_width = (num_cols * rect_size.x) + ((num_cols - 1) * rect_margin_width);
  auto start_x = (window_width_ / 2) - (total_width / 2.0);
  auto start_y = (window_height_ / 2) - (total_height / 2.0) + 50.;
  size_t index = 0;
  for (int i=0; i<num_rects; i++) {
    sf::RectangleShape rect(rect_size);
    for (int j=0; j<num_cols; j++) {
      if (total_rects-- == 0)
        break;
      rect.setPosition(start_x, start_y);
      rect.setOutlineColor(sf::Color::Red);

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i position = sf::Mouse::getPosition(static_cast<sf::RenderWindow &>(render_target));
        if (rect.getGlobalBounds().contains(position.x, position.y)) {
          function_selected = menu_items[index].second;
        }
      } else if (function_selected) {
        sf::Vector2i position = sf::Mouse::getPosition(static_cast<sf::RenderWindow &>(render_target));
        if (rect.getGlobalBounds().contains(position.x, position.y)) {
          function_selected();
          function_selected = nullptr;
        }
      }

      render_target.draw(rect);
      {
        if (!font_.loadFromMemory(Monaco_Linux_Powerline_ttf, Monaco_Linux_Powerline_ttf_len)) {
          std::cout << "Could not initialize font in memory.." << std::endl;
          std::exit(1);
        }
        text_.setFont(font_); // font is a sf::Font
        text_.setString(menu_items[index].first);
        text_.setCharacterSize(14); // in pixels, not points!
        text_.setColor(sf::Color::Red);
        text_.setStyle(sf::Text::Bold);
        text_.setPosition(start_x, start_y);
        sf::FloatRect textRect = text_.getLocalBounds();
        text_.setPosition(start_x + (rect.getSize().x / 2.), start_y);
        if (textRect.width < rect.getSize().x)
          text_.move(-1 * (textRect.width / 2.), 0);
        else
          text_.move(-1 * (rect.getSize().x / 2.), 0);

        index++;
        render_target.draw(text_);
      }
      start_x += rect.getSize().x + rect_margin_width;
    }
    start_y += rect.getSize().y + rect_margin_size;
    // reset start_x again
    start_x = (window_width_ / 2) - (total_width / 2.0);
  }
}
