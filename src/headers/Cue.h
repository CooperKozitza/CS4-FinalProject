#ifndef CUE_H
#define CUE_H

#include <SFML/Graphics.hpp>
#include "Ball.h"

class Cue
{
    private:
        sf::RectangleShape stick;
        sf::RectangleShape tip;

        Ball& cueBall;

        float power, angle;
        bool visible = true, released = false;
    
    public:

        Cue(Ball&);

        void show() {visible = true;}
        void hide() {visible = false;}

        bool isVisible() {return visible;}

        /// \param ammount pulls back the cue to this ammount
        
            void pullBack(float ammount) {if (power < 30) power = ammount; else power = 100;}

        /// \brief releases the cue and hits the cue ball \param balll the ball to hit
        
            void release();

        /// \brief called each frame; updates angle and position of cue \param angle the angle between cue-ball-cursor \param ball the ball to orbit the cue around
        
            void update(sf::Vector2i);

        /// \brief if displayed, draws the cue to the target
        
            void drawTo(sf::RenderWindow&);
};

#endif