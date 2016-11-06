#include "states/waiting.h"
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

waiting::waiting(global_game_state &gs)
  : state_interface(gs, game_state::state_type::waiting), menu_(window_width, window_height)
{
}

void waiting::initialize()
{
  messages::challenge c(global_game_state_);
  global_game_state_.send_socket().send(c.packet(), opponent_.ip, opponent_.port);

  menu_.add_item(std::string("Stop waiting..."), [&](){
    game_state state(state_);
    next_state_ = std::move(states::factory(state.transition(game_state::transition_type::bail), global_game_state_));
  });
  menu_.add_item("Exit", [](){
    std::exit(0);
  });

}

void waiting::set_opponent(probed_opponent_type opp)
{
  opponent_ = opp;
}

void waiting::handle(std::vector<std::unique_ptr<messages::message_interface>> msgs)
{
  for (auto &msg : msgs) {
    auto ca = dynamic_cast<messages::challenge_accepted *>(msg.get());
    if (ca) {
      std::cout << "Challenge was accepted!" << std::endl;
      game_state game(state_);
      auto new_state = states::factory(game.transition(game_state::transition_type::accept), global_game_state_);
      auto countdown_state = dynamic_cast<countdown *>(new_state.get());
      if (countdown_state) {
        countdown_state->set_opponent(opponent_);
        next_state_ = std::move(new_state);
      }
    }
  }
}

void waiting::tick()
{
}

void waiting::draw(sf::RenderTarget &renderTarget)
{
  menu_.render(renderTarget);
}
