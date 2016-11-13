/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include "game_state.h"
#include "states.h"
#include "utils/console.h"
#include "global_game_state.h"

// TODO: perhaps make this configurable
int window_width = 800, window_height = 600;

class processfighter_game : public global_game_state
{
public:
  processfighter_game(std::string name) : global_game_state(name)
  {
  }

  void run()
  {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Process Fighter");
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);

    game_state game;
    console_out_.initialize();
    auto state = states::factory(game.state(), *this);
    state->initialize();

    enemy_ship().set_position(window_width / 2.f, (256 / 4.f));
    my_ship().set_position(window_width / 2.f, window_height - (256 / 4.f));

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          window.close();
      }

      if (state->next_state()) {
        state = std::move(state->next_state());
        state->initialize();

        std::stringstream ss;
        ss << "Switching to state: " << static_cast<int>(state->type());
        console_out_.log(ss.str());
      }
      state->handle(rs.messages());
      state->tick();

      window.clear();

      sf::CircleShape shape(10.f);
      sf::Vector2f v1(0., 0.);
      shape.setPosition(v1);
      window.draw(shape);
      console_out_.draw(window);
      state->draw(window);

      enemy_ship().render(window);

      my_ship().calculate(*this);
      for (auto &bullet : bullets()) {
        bullet.calculate();
        bullet.render(window);
      }

      my_ship().render(window);

      window.display();
    }
  }
};

int main(int argc, char *argv[])
{
  std::string name = "anonymous";
  if (argc > 1) name = std::string(argv[1]);
  processfighter_game game(name);
  game.run();

  std::exit(0);
}

