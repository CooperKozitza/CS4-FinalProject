#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "src/headers/Game.h"
#include <math.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(60);

    Game game;

    bool hold = false;
    sf::Vector2i clickPosition;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                hold = true;
                clickPosition = sf::Mouse::getPosition(window); 
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                hold = false;
                game.cue.release();
            }
        }

        if (hold)
            game.cue.pullBack(sqrt(pow(clickPosition.x - sf::Mouse::getPosition(window).x, 2) + pow(clickPosition.y - sf::Mouse::getPosition(window).y, 2)));

        game.update(sf::Mouse::getPosition(window));

        window.clear();
		game.drawTo(window);
        window.display();
    }

    return 0;
}