/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

extern sf::UdpSocket main_socket;
extern sf::UdpSocket main_sending_socket;
extern console out;

class receiving_socket
{
public:
  receiving_socket()
  {
    main_socket.setBlocking(true);
    if (main_socket.bind(7777) != sf::Socket::Done) {
      std::cout << "Could not bind to UDP port 7777" << std::endl;
      std::exit(1);
    }

    // non blocking udp socket gave me problems with Ubuntu, maybe it's the specific sfml version
    // as I don't feel like providing my own SFML with this simple game, I'll just wrap it in a std::thread
    //main_socket.setBlocking(false);

    receiving_ = std::thread([=]() {
      while (running) {
        size_t received;
        sf::IpAddress ipaddr;
        unsigned short port;

        sf::Packet packet;
        sf::Int32 id = 0;
        if (main_socket.receive(packet, ipaddr, port) == sf::Socket::Done) {
          if (!(packet >> id)) {
            out.log("Error reading packet..");
            continue;
          }
          std::stringstream ss;
          ss << "Got packet: " << id << " ";
          switch (id) {
            case 1000: {
              std::string s = "";
              if (!(packet >> s)) {
                out.log("DEBUG: Error reading packet.. ");
                continue;
              }
              ss << "s = " << s;
              out.log("DEBUG: responding to packet..");
              sf::Packet reply;
              sf::Int32 id = 2000;
              reply << id;
              main_sending_socket.send(reply, ipaddr, 7777);
              break;
            }
            case 1001: {
              float test1 = 0;
              float test2 = 0;
              std::string s2;
              if (!(packet >> test1 >> test2 >> s2)) {
                out.log("Error reading packet..");
                continue;
              }
              ss << "test1 = " << test1 << " test2 = " << test2 << " s2 = " << s2;
              break;
            }
          }
          out.log(ss.str());
        }
      }
    });
  }
  ~receiving_socket() {
    running = false;
  }

private:
  std::thread receiving_;
  volatile bool running = true;
};
