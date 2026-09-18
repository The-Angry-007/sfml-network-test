#pragma once
#include "PCH.hpp"

class Player
{
public:
sf::Color color;
sf::Vector2f position;
float size;
sf::RectangleShape rect;
Player();
void SetColor(sf::Color color);
void SetPosition(sf::Vector2f position);
void SetSize(float size);
void UpdateRect();
};