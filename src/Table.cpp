#include "headers/Table.h"
#include <math.h>
#include <iostream>

Table::Table(sf::Vector2f size) : cue(balls[0])
{
    field.setSize(size);
    field.setPosition(sf::Vector2f(400 - size.x / 2, 300 - size.y / 2));
    field.setFillColor(sf::Color(130, 200, 130)); // light green

    edge.setSize(sf::Vector2f(size.x + 20, size.y + 20));
    edge.setPosition(sf::Vector2f(390 - size.x / 2, 290 - size.y / 2));
    edge.setFillColor(sf::Color(97, 54, 36)); // brown

    for (int i = 0; i < 6; i++) // places all 6 pockets in the correct positions in relation to the feild
        pockets[i].setPosition(sf::Vector2f(395 - size.x / 2 + (size.x + 10) * ((i / 2) / 2.0) - pockets[i].getRadius(), 295 - size.y / 2 + (size.y + 10) * (i % 2) - pockets[i].getRadius()));
}

void Table::init()
{
    sf::Color ballColors[9] = {sf::Color::White, sf::Color::Yellow, sf::Color::Blue, sf::Color::Red, sf::Color(75,0,130), sf::Color(255,69,0), sf::Color::Green, sf::Color(128,0,0), sf::Color::Black};
    for (int i = 0; i < 2 + 0; i++) {
        if (i < 9)
            balls[i].setFillColor(ballColors[i]);
        else {
            balls[i].setOutlineColor(ballColors[i - 8]);
            balls[i].setOutlineThickness(-5);
        }
        balls[i].setId(i);
    }
    reset();
}

void Table::reset()
{
    balls[0].setPosition(sf::Vector2f(field.getPosition().x + field.getSize().x / 4 - balls[0].getRadius(), field.getPosition().y + field.getSize().y / 2 - balls[0].getRadius()));
    balls[0].speed = 0;

    sf::Vector2f startingPositions[15] =
    {
        sf::Vector2f(0, 0), sf::Vector2f(64, 0), sf::Vector2f(16, -8),
        sf::Vector2f(48, -8), sf::Vector2f(64, -16), sf::Vector2f(32, 16),
        sf::Vector2f(64, 32), sf::Vector2f(32, 0), sf::Vector2f(48, -24),
        sf::Vector2f(64, 16), sf::Vector2f(16, 8), sf::Vector2f(64, -32),
        sf::Vector2f(48, 24), sf::Vector2f(32, -16), sf::Vector2f(48, 8)
    };

    for (int i = 1; i < 2 + 0; i++)
    {
        balls[i].setPosition(sf::Vector2f(field.getPosition().x + field.getSize().x / 4 * 3 + startingPositions[i - 1].x - balls[0].getRadius(), field.getPosition().y + field.getSize().y / 2 + startingPositions[i - 1].y - balls[0].getRadius()));
        balls[i].speed = 0;
    }
}

void Table::update(sf::Vector2i mousePosition)
{
    if (cue.isVisible())
        cue.update(mousePosition);

    for (int i = 0; i < 2 + 0; i++) { // parse thru entire ball array
		if (balls[i].speed > 30) balls[i].speed = 20;
        for (int j = 1; j < balls[i].speed; j++) { // apply speed in direction while speed is greater than zero and the ball is in bounds
            balls[i].move(cos(balls[i].direction * M_PI / 180), sin(balls[i].direction * M_PI / 180));
            if (!inBounds(balls[i].getPosition(), sf::Vector2f(2 * balls[i].getRadius(), 2 * balls[i].getRadius()))) break;
            for (int k = 0; k < 2 + 0; k++) if (balls[i].isCollidingWith(balls[k])) break;
        }
        if (!inBounds(balls[i].getPosition(), sf::Vector2f(2 * balls[i].getRadius(), 2 * balls[i].getRadius()))) { // ball + wall collision
            std::cout << "ball " << i << " hit a wall" << std::endl;
            if (balls[i].getPosition().x <= field.getPosition().x) {
                while (balls[i].getPosition().x < field.getPosition().x) // resolve tunnelling
                    balls[i].move(1, 0);
                balls[i].direction = (balls[i].direction * -1) + 180; // angle after hitting right wall
            }
            else if (balls[i].getPosition().x + 2 * balls[i].getRadius() >= field.getPosition().x + field.getSize().x) {
                while (balls[i].getPosition().x + 2 * balls[i].getRadius() > field.getPosition().x + field.getSize().x) // resolve tunnelling
                    balls[i].move(-1, 0);
                balls[i].direction = (balls[i].direction * -1) + 180; // angle after hitting left wall
            }
            if (balls[i].getPosition().y <= field.getPosition().y) {
                while (balls[i].getPosition().y < field.getPosition().y) // resolve tunnelling
                    balls[i].move(0, 1);
                balls[i].direction *= -1; // angle after hitting top wall
            }
            else if (balls[i].getPosition().y + 2 * balls[i].getRadius() >= field.getPosition().y + field.getSize().y) {
                while (balls[i].getPosition().y + 2 * balls[i].getRadius() > field.getPosition().y + field.getSize().y) // resolve tunnelling
                    balls[i].move(0, -1);
                balls[i].direction *= -1; // angle after hitting bottom wall
            }
        }
        for (int j = 0; j < 2 + 0; j++) // check for collisions between target ball and every other ball, ik its not efficient :)
            if (balls[i].isCollidingWith(balls[j]) && j != i) {
                std::cout << "ball " << i << " collided with ball " << j << " at " << balls[i].speed << " px/f:\n";
                float distance = sqrtf(pow(balls[i].getPosition().x - balls[j].getPosition().x, 2) + pow(balls[i].getPosition().y - balls[j].getPosition().y, 2));
                float overlap = (distance - balls[i].getRadius() - balls[j].getRadius()) / 2;

                // move balls so they are no longer colliding (static resolution)
                balls[i].move(sf::Vector2f(-(overlap * (balls[i].getPosition().x - balls[j].getPosition().x) / distance), -(overlap * (balls[i].getPosition().y - balls[j].getPosition().y) / distance)));
                balls[j].move(sf::Vector2f(overlap * (balls[i].getPosition().x - balls[j].getPosition().x) / distance, overlap * (balls[i].getPosition().y - balls[j].getPosition().y) / distance));

                // dynamic resolution
                sf::Vector2f normal((balls[j].getPosition().x - balls[i].getPosition().x) / distance, (balls[j].getPosition().y - balls[i].getPosition().y) / distance);
                sf::Vector2f tangent(-(balls[j].getPosition().y - balls[i].getPosition().y) / distance, (balls[j].getPosition().x - balls[i].getPosition().x) / distance);

                
                std::pair<float, float> normalDotProduct = {balls[i].getVelocity().x * normal.x + balls[i].getVelocity().y * normal.y, balls[j].getVelocity().x * normal.x + balls[j].getVelocity().y * normal.y};
                std::pair<float, float> tangentDotProduct = {balls[i].getVelocity().x * tangent.x + balls[i].getVelocity().y * tangent.y, balls[j].getVelocity().x * tangent.x + balls[j].getVelocity().y * tangent.y};

                balls[i].setVelocity(tangent.x * tangentDotProduct.first + normal.x * normalDotProduct.second, tangent.y * tangentDotProduct.first + normal.y * normalDotProduct.second);
                balls[j].setVelocity(tangent.x * tangentDotProduct.second + normal.x * normalDotProduct.first, tangent.y * tangentDotProduct.second + normal.y * normalDotProduct.first);

                std::cout << "-  ball " << i << " post-speed: " << balls[i].speed << "\n" << "- ball " << j << " post-speed: " << balls[j].speed << "\n";
            }
        balls[i].speed *= frictionCoef; // apply friction

        // delete this:
        balls[i].deleteme.setPosition(balls[i].getPosition() + sf::Vector2f(balls[i].getRadius(), balls[i].getRadius()));
        balls[i].deleteme.setRotation(balls[i].direction);
    }
    
}

void Table::drawTo(sf::RenderWindow& target) 
{
    target.draw(edge);
    target.draw(field);
    for (int i = 0; i < 6; i++)
        target.draw(pockets[i]);
    for (int i = 0; i < 2 + 0; i++)
        target.draw(balls[i]), target.draw(balls[i].deleteme);
    cue.drawTo(target);
}

bool Table::inBounds(sf::Vector2f p, sf::Vector2f size)
{
    if (p.x >= field.getPosition().x && p.y >= field.getPosition().y && p.x + size.x <= field.getPosition().x + field.getSize().x && p.y + size.y <= field.getPosition().y + field.getSize().y)
        return true;
    else
        return false;
}