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
#include "receiving_socket.hpp"

// TODO: put this in some sensible place
sf::UdpSocket main_socket;
sf::UdpSocket main_sending_socket;
console out;

int main()
{
  receiving_socket rs;
  sf::RenderWindow window(sf::VideoMode(800, 600), "Process Fighter");
  sf::CircleShape shape(10.f);
  shape.setFillColor(sf::Color::Green);

  game_state game;
  out.initialize();
  auto state = states::factory(game.state());
  state->initialize();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
    }

    if (state->next_state()) {
      state = std::move(state->next_state());
      state->initialize();
    }

    sf::Vector2f v1(0., 0.);
    sf::CircleShape shape(10.f);
    shape.setPosition(v1);

    window.clear();
    window.draw(shape);
    out.draw(window);
    state->draw(window);
    window.display();
  }
  return 0;
}

