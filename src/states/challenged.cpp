#include "states/challenged.h"
#include "states/countdown.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <sstream>
#include "utils/menu.h"
#include "states.h"
#include "messages.h"
#include "global_game_state.h"

extern int window_width;
extern int window_height;

challenged::challenged(global_game_state &gs)
  : state_interface(gs, game_state::state_type::challenged), menu_(window_width, window_height)
{
}

void challenged::initialize()
{
  menu_.add_item(std::string("Fight this guy"), [&](){
    game_state state(state_);
    auto new_state = states::factory(state.transition(game_state::transition_type::accept), global_game_state_);
    auto countdown_state = dynamic_cast<countdown *>(new_state.get());
    if (countdown_state) {
      countdown_state->set_opponent(opponent_);
      next_state_ = std::move(new_state);
    }
  });
  menu_.add_item("Run away like a coward", [&](){
    game_state state(state_);
    next_state_ = std::move(states::factory(state.transition(game_state::transition_type::bail), global_game_state_));
  });
  menu_.add_item("Exit", [](){
    std::exit(0);
  });
}

void challenged::set_opponent(probed_opponent_type opp)
{
  opponent_ = opp;
}

void challenged::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
{
}

void challenged::tick()
{
}

void challenged::draw(sf::RenderTarget &renderTarget)
{
  menu_.render(renderTarget);
}
