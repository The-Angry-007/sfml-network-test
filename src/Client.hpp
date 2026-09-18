#pragma once

#include "State.hpp"
#include "PCH.hpp"
#include "Player.hpp"
#include "CSMessage.hpp"
class Client : public Kosmic::State
{
public:
float speed = 100.f;
bool connected;
Player player;
sf::TcpSocket socket; 
std::vector<Player> otherPlayers;
std::vector<std::unique_ptr<CSMessageRequest>> requests = {};

Client(sf::RenderTarget* renderTarget, Player player);
void DerivedUpdate();
void DerivedRender();
void ProcessPacket(sf::Packet& packet);
void SendPacket(sf::Packet& packet);
void LoadJoinData(sf::Packet& packet);
bool ConnectToServer(sf::IpAddress& ip, unsigned short port);
bool IsConnected()
{
    return connected;
}

void ReceivePackets();
};