/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "messages.h"
#include "SFML/Network.hpp"
#include "global_game_state.h"

namespace messages {

  message_interface::message_interface(global_game_state &state, message_types type) :
    type_(type), time_(std::chrono::high_resolution_clock::now()), global_state_(state)
  {
    sf::Int32 id = static_cast<sf::Int32>(type_);
    sf::Int16 port = state.my_udp_port();
    sf::Int64 us = state.unique_id();
    packet_ << id << port << us;
  }

  sf::Packet & message_interface::packet()
  {
    return packet_;
  }
  sf::IpAddress message_interface::ipaddr()
  {
    return ipaddr_;
  }
  sf::Int16 message_interface::source_port()
  {
    return source_port_;
  }
  sf::Int64 message_interface::requesting_id()
  {
    return requesting_id_;
  }

  bool message_interface::parse(sf::Packet & packet_in, sf::IpAddress &addr)
  {
    ipaddr_ = addr;
    if (!(packet_in >> source_port_ >> requesting_id_)) {
      return false;
    }
    return parse_msg(packet_in);
  }

  global_game_state & message_interface::global_state()
  {
    return global_state_;
  }

  shutdown::shutdown(global_game_state &state) : message_interface(state, message_types::shutdown)
  {
  }
  bool shutdown::parse_msg(sf::Packet &packet_in)
  {
    return true;
  }

  probe::probe(global_game_state &state) : message_interface(state, message_types::probe)
  {
  }
  bool probe::parse_msg(sf::Packet &packet_in)
  {
    return true;
  }

  probe_response::probe_response(global_game_state &state) : message_interface(state, message_types::probe_response)
  {
    packet_ << global_state_.name();
  }
  bool probe_response::parse_msg(sf::Packet &packet_in)
  {
    if (!(packet_in >> name_))
      return false;

    return true;
  }
  std::string probe_response::name()
  {
    return name_;
  }


}
