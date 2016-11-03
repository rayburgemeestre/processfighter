/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf {
  class RenderTarget;
}

class console
{
public:
  console(size_t lines = 54);

  void initialize();

  void draw(sf::RenderTarget &renderTarget);
  void log(const std::string & line);

private:
  size_t lines_;
  std::vector<std::string> console_lines_;
  sf::Font font_;
  sf::Text text_;
};

