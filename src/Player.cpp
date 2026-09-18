#include "Player.hpp"

Player::Player()
{
    this->color = sf::Color::White;
    size = 50.f;
    position = {0.f,0.f};
    UpdateRect();
}

void Player::UpdateRect()
{
    rect.setSize({size,size});
    rect.setFillColor(color);
    rect.setPosition(position);
}
void Player::SetColor(sf::Color color)
{
    this->color = color;
    UpdateRect();
}
void Player::SetPosition(sf::Vector2f position)
{
    this->position = position;
    UpdateRect();
}

void Player::SetSize(float size)
{
    this->size = size;
    UpdateRect();
}