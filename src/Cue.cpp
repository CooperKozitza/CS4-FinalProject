#include "headers/Cue.h"
#include <math.h>

Cue::Cue(Ball& cueBall) : cueBall(cueBall)
{

    stick.setSize(sf::Vector2f(300, 5));
    stick.setOrigin(sf::Vector2f(300, 2.5));
    stick.setFillColor(sf::Color(240, 200, 140));

    tip.setSize(sf::Vector2f(5, 5));
    tip.setOrigin(sf::Vector2f(5, 2.5));
    tip.setFillColor(sf::Color(40, 25, 0));
}

void Cue::release()
{
    released = true;
    if (power > 5) {
        cueBall.hit(angle, power / 4);
    }
}

void Cue::update(sf::Vector2i mousePosition)
{
    if (visible) {
        angle = atan2(cueBall.getPosition().y - mousePosition.y, cueBall.getPosition().x - mousePosition.x)  * 180 / M_PI;

        stick.setPosition(sf::Vector2f(cueBall.getPosition().x + cueBall.getRadius() - (power + cueBall.getRadius()) * cos(angle * M_PI / 180), cueBall.getPosition().y + cueBall.getRadius() - (power + cueBall.getRadius()) * sin(angle * M_PI / 180)));
        tip.setPosition(stick.getPosition());

        stick.setRotation(angle);
        tip.setRotation(stick.getRotation());

        if (released) {
            if (power > 0) power -= 5;
            else {
                power = 0;
                released = false;
            }
		}
    }
}

void Cue::drawTo(sf::RenderWindow& target)
{
    if (visible) {
        target.draw(stick);
        target.draw(tip);
    }
}