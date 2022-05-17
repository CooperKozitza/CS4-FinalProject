#include "headers/Ball.h"
#include <math.h>

void Ball::hit(float dir, float mag)
{
    direction = dir;
    speed = mag;
}

bool Ball::isCollidingWith(Ball& target)
{
    float distanceSquared = fabs(pow(target.getPosition().x - getPosition().x, 2) + pow(target.getPosition().y - getPosition().y, 2));
    if (distanceSquared < pow(getRadius() + target.getRadius(), 2))
        return true;
    else
        return false;
}

sf::Vector2f Ball::getVelocity() {return sf::Vector2f(speed * cos(direction * M_PI / 180), speed * sin(direction * M_PI / 180));}

void Ball::setVelocity(float vx, float vy)
{
    speed = sqrtf((vy * vy) + (vx * vx));
    direction = atan2f(vy, vx) * 180 / M_PI;
}