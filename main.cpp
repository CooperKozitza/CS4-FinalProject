#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

class Table
{
    private:
        //private classes:
        class Ball : public sf::CircleShape
        {
            private:
                int id;
            public:
                Ball()
                {
                    setRadius(8);
                }

                /// \brief a method used for when two balls collide
                /// \param dir direction the hit ocuured (in degrees) \param mag the magnitude, or force, of the hit
                void hit(float dir, float mag);

                void setId(int id)
                {
                    this -> id = id;
                }
        };

        class Pocket : public sf::CircleShape
        {
            public:
                Pocket()
                {
                    setRadius(10);
                    setFillColor(sf::Color(69, 39, 26));
                } 
        };
        
        // private members:
        Ball balls[21]; // array that stores all 21 of the balls
        Pocket pockets[6]; // array that stores all 6 of the pockets

        sf::RectangleShape field;
        sf::RectangleShape edge;

        const double friction = 0.01; // ammount of friction applied to moving objects on the feild

    public:
        Table(sf::Vector2f);

        /// \brief called once, init() sets ball colors and ids
        void init();

        /// \brief sets table up for play (ie. places balls in triangle)
        void reset();

        /// \brief draws the table
        /// \param target window to draw to
        void drawTo(sf::RenderWindow& target);
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


Table::Table(sf::Vector2f size) 
{
    field.setSize(size);
    field.setPosition(sf::Vector2f(400 - size.x / 2, 300 - size.y / 2));
    field.setFillColor(sf::Color(130, 200, 130)); // light green

    edge.setSize(sf::Vector2f(size.x + 30, size.y + 30));
    edge.setPosition(sf::Vector2f(385 - size.x / 2, 285 - size.y / 2));
    edge.setFillColor(sf::Color(97, 54, 36)); // brown

    for (int i = 0; i < 6; i++) // places all 6 pockets in the correct positions in relation to the feild
        pockets[i].setPosition(sf::Vector2f(400 - size.x / 2 + size.x * ((i / 2) / 2.0) - 10, 300 - size.y / 2 + size.y * (i % 2) - 10));
}

void Table::init()
{
    sf::Color ballColors[8] = {sf::Color::Yellow, sf::Color::Blue, sf::Color::Red, sf::Color(75,0,130), sf::Color(255,69,0), sf::Color::Green, sf::Color(128,0,0), sf::Color::Black};
    for (int i = 0; i < 15; i++) {
        if (i < 8)
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
    for (int i = 0; i < 15; i++)
    {
        balls[i].setPosition(sf::Vector2f(field.getPosition().x + field.getSize().x / 4 * 3 + startingPositions[i].x - 8, field.getPosition().y + field.getSize().y / 2 + startingPositions[i].y - 8));
    }
}

void Table::drawTo(sf::RenderWindow& target) 
{
    target.draw(edge);
    target.draw(field);
    for (int i = 0; i < 6; i++)
        target.draw(pockets[i]);
    for (int i = 0; i < 15; i++)
        target.draw(balls[i]);
}