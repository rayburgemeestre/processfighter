#include "states/waiting.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <sstream>
#include "utils/menu.h"
#include "states.h"

extern int window_width;
extern int window_height;

waiting::waiting(global_game_state &gs)
  : state_interface(gs, game_state::state_type::waiting)
{
}

void waiting::initialize()
{
  std::cout << "Should challenge opponent: " << opponent_.name << std::endl;
}

void waiting::set_opponent(probed_opponent_type opp)
{
  opponent_ = opp;
}

void waiting::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
{
}

void waiting::tick()
{
}

void waiting::draw(sf::RenderTarget &renderTarget)
{
  menu m(window_width, window_height);
  m.add_item(std::string("Stop waiting..."), [&](){
    game_state state(state_);
    next_state_ = std::move(states::factory(state.transition(game_state::transition_type::bail), global_game_state_));
  });
  m.add_item("Exit", [](){
    std::exit(0);
  });
  m.render(renderTarget);
}
