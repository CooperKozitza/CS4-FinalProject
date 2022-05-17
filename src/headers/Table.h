#ifndef TABLE_H
#define TABLE_H

#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Cue.h"
#include "Pocket.h"

class Table
{
    private:
        //private classes:
        
        // private members:
        Ball balls[16]; // array that stores all 15 of the balls and the cue ball ([0])
        Pocket pockets[6]; // array that stores all 6 of the pockets

        sf::RectangleShape field;
        sf::RectangleShape edge;

        const double frictionCoef = 0.99; // friction applied to moving objects on the feild

    public:
        Cue cue;

        Table(sf::Vector2f);

        /// \brief called once, init() sets ball colors and ids
        
            void init();

        /// \brief sets table up for play (ie. places balls in triangle)
        
            void reset();

        /// \brief called every frame; updates position of all balls \param mousePosition the current postion of the mouse
        
            void update(sf::Vector2i);

        /// \brief draws the table \param target window to draw to
        
            void drawTo(sf::RenderWindow&);

        /// \brief evaluates to true if the provided vector is within the playfeild \param p the point to check \param size optional - for checking if a area is in bounds
        
            bool inBounds(sf::Vector2f, sf::Vector2f = sf::Vector2f(0, 0));
};

#endif