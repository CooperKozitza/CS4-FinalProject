#include "headers/Game.h"

Game::Game() : table(sf::Vector2f(800, 600))
{

}

void Game::update(sf::Vector2i mousePosition) 
{
    table.update(mousePosition);
    ui.update(mousePosition);
}

void Game::drawTo(sf::RenderWindow& target)
{
    table.drawTo(target);
    ui.drawTo(target);
}