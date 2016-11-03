#include "states/selecting.h"

#include <iostream>

#include <SFML/Graphics.hpp>

selecting::selecting()
  : state_interface(game_state::state_type::selecting)
{
}

void selecting::initialize()
{
  std::cout << "initializing selecting.." << std::endl;
}

void selecting::add_opponent(probed_opponent_type type)
{
  std::cout << "adding opponent.." << std::endl;
  opponents_.push_back(type);
}

void selecting::draw(sf::RenderTarget &renderTarget)
{
}
