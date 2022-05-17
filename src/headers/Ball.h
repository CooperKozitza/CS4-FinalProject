#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball : public sf::CircleShape
{
    private:
        int id;

    public:
        float direction, speed;

        sf::RectangleShape deleteme;

        Ball() {setRadius(8); deleteme.setSize(sf::Vector2f(10, 3)); deleteme.setOrigin(sf::Vector2f(0, 1.5)); deleteme.setFillColor(sf::Color::Red);}

        void setId(int id) {this -> id = id;}

        /// \brief a method used for when two balls collide \param dir direction the hit ocuured (in degrees) \param mag the magnitude, or force, of the hit
        
            void hit(float, float);

        /// \brief evaluates to true if the ball is colliding with the target ball \param target the ball to check for collision with
        
            bool isCollidingWith(Ball&);\

        /// \brief gets the x and y velocities \returns sf::Vector2f

            sf::Vector2f getVelocity();

        /// \brief sets the balls angle/speed based on x/y velocity \param vx the target x velocity \param vy the target y velocity

            void setVelocity(float, float);
};

#endif