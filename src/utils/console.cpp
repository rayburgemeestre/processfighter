/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "utils/console.h"

#include <SFML/Graphics.hpp>
#include "font.h"
#include <iostream>

console::console(size_t lines)
  : lines_(lines)
{
}

void console::initialize()
{
  if (!font_.loadFromMemory(Monaco_Linux_Powerline_ttf, Monaco_Linux_Powerline_ttf_len)) {
    std::cout << "Could not initialize font in memory.." << std::endl;
  }
  text_.setFont(font_); // font is a sf::Font
  text_.setString("");
  text_.setCharacterSize(14); // in pixels, not points!
  text_.setColor(sf::Color::Red);
  text_.setStyle(sf::Text::Bold);
}

void console::log(const std::string & line) {
  console_lines_.push_back(line);
  console_lines_ = std::vector<std::string>(
    console_lines_.end() - std::min(console_lines_.size(), lines_),
    console_lines_.end()
  );
}

void console::draw(sf::RenderTarget &renderTarget)
{
  text_.setPosition(0, 0);
  for (const auto &line : console_lines_) {
    text_.setString(line);
    text_.move(0, 20);
    renderTarget.draw(text_);
  }
}
