/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <string>
#include <vector>
#include <functional>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"

namespace sf {
  class RenderTarget;
}

using on_click_function = std::function<void()>;
using menu_items_type = std::vector<std::pair<std::string, on_click_function>>;

class menu
{
public:

  menu(size_t window_width, size_t window_height);
  void add_item(std::string menu_text, on_click_function on_click);
  void render(sf::RenderTarget &render_target);

private:
  menu_items_type menu_items;

  sf::Font font_;
  sf::Text text_;

  size_t window_width_;
  size_t window_height_;
  std::function<void()> function_selected;
};
