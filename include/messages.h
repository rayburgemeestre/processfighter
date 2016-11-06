/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "SFML/Network.hpp"
#include <chrono>

class global_game_state;

namespace messages
{
  enum class message_types
  {
    shutdown = 0,

    probe = 1000,
    probe_response,

    challenge = 2000,
    challenge_accepted,

    ping = 3000,
    pong,
  };

  class message_interface
  {
  public:
    message_interface(global_game_state & state, message_types type);

    sf::Packet &packet();
    bool parse(sf::Packet & packet_in, sf::IpAddress &addr);
    virtual bool parse_msg(sf::Packet & packet_in) = 0;

    sf::IpAddress ipaddr();
    sf::Int16 source_port();
    sf::Int64 requesting_id();

    global_game_state &global_state();

  protected:
    sf::Int16 source_port_ = 0;
    sf::Int64 requesting_id_ = 0;
    sf::Packet packet_;
    message_types type_;
    std::chrono::time_point<std::chrono::high_resolution_clock> time_;
    sf::IpAddress ipaddr_;

    global_game_state &global_state_;
  };

  class shutdown : public message_interface
  {
  public:
    shutdown(global_game_state & state);
    bool parse_msg(sf::Packet & packet_in) override;
  };

  class probe : public message_interface
  {
  public:
    probe(global_game_state & state);
    bool parse_msg(sf::Packet & packet_in) override;
  };

  class probe_response : public message_interface
  {
  public:
    probe_response(global_game_state & state);
    bool parse_msg(sf::Packet & packet_in) override;
    std::string name();
  private:
    std::string name_;
  };

  class challenge : public message_interface
  {
  public:
    challenge(global_game_state & state);
    bool parse_msg(sf::Packet & packet_in) override;
    std::string name();
  private:
    std::string name_;
  };

  class challenge_accepted : public message_interface
  {
  public:
    challenge_accepted(global_game_state & state);
    bool parse_msg(sf::Packet & packet_in) override;
  };

  class ping : public message_interface
  {
  public:
    ping(global_game_state & state, sf::Int64 payload);
    bool parse_msg(sf::Packet & packet_in) override;
    sf::Int64 payload();
  private:
    sf::Int64  payload_;
  };
  class pong : public message_interface
  {
  public:
    pong(global_game_state & state, sf::Int64 payload);
    bool parse_msg(sf::Packet & packet_in) override;
    sf::Int64 payload();
  private:
    sf::Int64 payload_;
  };
}
