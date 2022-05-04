#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <math.h>

class Table
{
    private:
        //private classes:
        class Ball : public sf::CircleShape
        {
            private:
                int id;

            public:
                float direction, speed;

                Ball() {setRadius(8);}

                void setId(int id) {this -> id = id;}

                /// \brief a method used for when two balls collide \param dir direction the hit ocuured (in degrees) \param mag the magnitude, or force, of the hit
                
                    void hit(float, float);

                /// \brief evaluates to true if the ball is colliding with the target ball \param target the ball to check for collision with
                
                    bool colliding(Ball);
        };

        class Pocket : public sf::CircleShape
        {
            public:
                Pocket();

        };
        
        // private members:
        Ball balls[16]; // array that stores all 15 of the balls and the cue ball ([0])
        Pocket pockets[6]; // array that stores all 6 of the pockets

        sf::RectangleShape field;
        sf::RectangleShape edge;

        const double frictionCoef = 0.1; // friction applied to moving objects on the feild

    public:
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

                /// \param ammount pulls back the cue to this ammount
                
                    void pullBack(float ammount) {power = ammount;}

                /// \brief releases the cue and hits the cue ball \param balll the ball to hit
                
                    void release();

                /// \brief called each frame; updates angle and position of cue \param angle the angle between cue-ball-cursor \param ball the ball to orbit the cue around
                
                    void update(sf::Vector2i);

                /// \brief if displayed, draws the cue to the target
                
                    void draw(sf::RenderWindow&);
        };

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
        
            bool inBounds(sf::Vector2f, sf::Vector2f);

        /// \brief returns a vector of the cue balls current position

            sf::Vector2f getCueBallPosition() {return balls[0].getPosition();}
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(40);
    
    Table table(sf::Vector2f(400, 200));
    table.init();

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
                table.cue.release();
            }
        }

        if (hold)
            table.cue.pullBack(sqrt(pow(clickPosition.x - sf::Mouse::getPosition(window).x, 2) + pow(clickPosition.y - sf::Mouse::getPosition(window).y, 2)));

        table.update(sf::Mouse::getPosition(window));

        window.clear();
        table.drawTo(window);
        window.display();
    }

    return 0;
}

void Table::Ball::hit(float dir, float mag)
{
    direction = dir;
    speed = mag;
}

bool Table::Ball::colliding(Ball target)
{
    float distance = sqrt(pow(target.getPosition().x - getPosition().x, 2) + pow(target.getPosition().y - getPosition().y, 2));
    if (distance <= 16)
        return true;
    else
        return false;
}

Table::Pocket::Pocket()
{
    setRadius(15);
    setFillColor(sf::Color(69, 39, 26));
} 

Table::Cue::Cue(Ball& cueBall) : cueBall(cueBall)
{

    stick.setSize(sf::Vector2f(300, 5));
    stick.setOrigin(sf::Vector2f(300, 2.5));
    stick.setFillColor(sf::Color(240, 200, 140));

    tip.setSize(sf::Vector2f(5, 5));
    tip.setOrigin(sf::Vector2f(5, 2.5));
    tip.setFillColor(sf::Color(40, 25, 0));
}

void Table::Cue::release()
{
    released = true;
    if (power > 10) {
        cueBall.hit(90 - angle, power);
    }
}

void Table::Cue::update(sf::Vector2i mousePosition)
{
    if (visible) {
        angle = atan2(cueBall.getPosition().y - mousePosition.y, cueBall.getPosition().x - mousePosition.x) * 180 / M_PI;

        stick.setPosition(sf::Vector2f(cueBall.getPosition().x + cueBall.getRadius() + (power + 8) * sin((270 - angle) * M_PI / 180), cueBall.getPosition().y  + cueBall.getRadius() + (power + 8) * cos((270 - angle) * M_PI / 180)));
        tip.setPosition(stick.getPosition());

        stick.setRotation(angle);
        tip.setRotation(stick.getRotation());

        if (released)
            if (power > 0) power -= 5;
            else {
                power = 0;
                released = false;
            }
    }
}

void Table::Cue::draw(sf::RenderWindow& target)
{
    if (visible) {
        target.draw(stick);
        target.draw(tip);
    }
}

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
    for (int i = 0; i < 16; i++) {
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
        sf::Vector2f(0, 0),
        sf::Vector2f(64, 0),
        sf::Vector2f(16, -8),
        sf::Vector2f(48, -8),
        sf::Vector2f(64, -16),
        sf::Vector2f(32, 16),
        sf::Vector2f(64, 32), 
        sf::Vector2f(32, 0),
        sf::Vector2f(48, -24),
        sf::Vector2f(64, 16),
        sf::Vector2f(16, 8),
        sf::Vector2f(64, -32),
        sf::Vector2f(48, 24), 
        sf::Vector2f(32, -16), 
        sf::Vector2f(48, 8)
    };

    for (int i = 1; i < 16; i++)
    {
        balls[i].setPosition(sf::Vector2f(field.getPosition().x + field.getSize().x / 4 * 3 + startingPositions[i - 1].x - balls[0].getRadius(), field.getPosition().y + field.getSize().y / 2 + startingPositions[i - 1].y - balls[0].getRadius()));
        balls[i].speed = 0;
    }
}

void Table::update(sf::Vector2i mousePosition)
{
    cue.update(mousePosition);
    for (int i = 0; i < 16; i++) {
        if (inBounds(balls[i].getPosition() + sf::Vector2f(balls[i].speed * sin(balls[i].direction * M_PI / 180), balls[i].speed * cos(balls[i].direction * M_PI / 180)), sf::Vector2f(balls[i].getRadius(), balls[i].getRadius())))
            balls[i].move(sf::Vector2f(balls[i].speed * sin(balls[i].direction * M_PI / 180), balls[i].speed * cos(balls[i].direction * M_PI / 180)));
        else {
            // wall collision physics
        }
        balls[i].speed -= balls[i].speed * frictionCoef;
        for (int j = 0; j < 16; j++)
            if (balls[i].colliding(balls[j])) {
              
            }
    }
}

void Table::drawTo(sf::RenderWindow& target) 
{
    target.draw(edge);
    target.draw(field);
    for (int i = 0; i < 6; i++)
        target.draw(pockets[i]);
    for (int i = 0; i < 16; i++)
        target.draw(balls[i]);
    cue.draw(target);
}

bool Table::inBounds(sf::Vector2f p, sf::Vector2f size = sf::Vector2f(0, 0))
{
    if (p. x > field.getPosition().x && p.y > field.getPosition().y && p.x + size.x < field.getPosition().x + field.getSize().x && p.y + size.y < field.getPosition().y + field.getSize().y)
        return true;
    else
        return false;
}