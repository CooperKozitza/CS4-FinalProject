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

                /// \brief a method used for when two balls collide
                /// \param dir direction the hit ocuured (in degrees) \param mag the magnitude, or force, of the hit
                void hit(float, float);

                /// \brief evaluates to true if the ball is colliding with the target ball
                /// \param target the ball to check for collision with
                bool colliding(Ball);
        };

        class Pocket : public sf::CircleShape
        {
            public:
                Pocket()
                {
                    setRadius(15);
                    setFillColor(sf::Color(69, 39, 26));
                } 
        };
        
        // private members:
        Ball balls[16]; // array that stores all 21 of the balls
        Pocket pockets[6]; // array that stores all 6 of the pockets

        sf::RectangleShape field;
        sf::RectangleShape edge;

        const double frictionCoef = 0.01; // friction applied to moving objects on the feild

    public:
        Table(sf::Vector2f);

        /// \brief called once, init() sets ball colors and ids
        void init();

        /// \brief sets table up for play (ie. places balls in triangle)
        void reset();

        /// \brief called every frame; updates position of all balls
        void update();

        /// \brief draws the table
        /// \param target window to draw to
        void drawTo(sf::RenderWindow&);

        /// \brief evaluates to true if the provided vector is within the playfeild
        /// \param p the point to check
        bool inBounds(sf::Vector2f);
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    Table table(sf::Vector2f(400, 200));
    table.init();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

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

Table::Table(sf::Vector2f size) 
{
    field.setSize(size);
    field.setPosition(sf::Vector2f(400 - size.x / 2, 300 - size.y / 2));
    field.setFillColor(sf::Color(130, 200, 130)); // light green

    edge.setSize(sf::Vector2f(size.x + 30, size.y + 30));
    edge.setPosition(sf::Vector2f(385 - size.x / 2, 285 - size.y / 2));
    edge.setFillColor(sf::Color(97, 54, 36)); // brown

    for (int i = 0; i < 6; i++) // places all 6 pockets in the correct positions in relation to the feild
        pockets[i].setPosition(sf::Vector2f(400 - size.x / 2 + size.x * ((i / 2) / 2.0) - pockets[i].getRadius(), 300 - size.y / 2 + size.y * (i % 2) - pockets[i].getRadius()));
}

void Table::init()
{
    sf::Color ballColors[9] = {sf::Color::White, sf::Color::Yellow, sf::Color::Blue, sf::Color::Red, sf::Color(75,0,130), sf::Color(255,69,0), sf::Color::Green, sf::Color(128,0,0), sf::Color::Black};
    for (int i = 0; i < 16; i++) {
        if (i < 9)
            balls[i].setFillColor(ballColors[i]);
        else {
            balls[i].setOutlineColor(ballColors[i - 9]);
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

void Table::update()
{
    for (int i = 0; i < 16; i++) {
        if (inBounds(balls[i].getPosition() + sf::Vector2f(balls[i].speed * sin(balls[i].direction * M_PI / 180), balls[i].speed * cos(balls[i].direction * M_PI / 180))))
            balls[i].move(sf::Vector2f(balls[i].speed * sin(balls[i].direction * M_PI / 180), balls[i].speed * cos(balls[i].direction * M_PI / 180)));
        else {
            // wall collision physics
        }
        balls[i].speed *= frictionCoef;
        for (int j = 0; j < 16; j++)
            if (balls[i].colliding(balls[j])) {
                // collision physics
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
}

bool Table::inBounds(sf::Vector2f p)
{
    if (p. x > field.getPosition().x && p.y > field.getPosition().y && p.x < field.getPosition().x + field.getSize().x && p.y < field.getPosition().y + field.getSize().y)
        return true;
    else
        return false;
}