#include "Client.hpp"

Client::Client(sf::RenderTarget* target, Player player)
{
    this->renderTarget = target;
    this->player = player;
    otherPlayers = {};
}

void Client::DerivedUpdate()
{
    if (inputState.Down(sf::Keyboard::Key::A))
    {
        //...
    }
}

void Client::DerivedRender()
{
    renderTarget->draw(player.rect);
    for (auto& p : otherPlayers)
    {
        renderTarget->draw(p.rect);
    }
}