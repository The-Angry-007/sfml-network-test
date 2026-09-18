#pragma once

#include "State.hpp"
#include "PCH.hpp"
#include "Player.hpp"
class Client : public Kosmic::State
{
public:
Player player;
sf::TcpSocket socket;
std::vector<Player> otherPlayers;

Client(sf::RenderTarget* renderTarget, Player player);
void DerivedUpdate();
void DerivedRender();
};