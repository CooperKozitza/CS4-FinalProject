#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
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
                
                    bool isCollidingWith(Ball&);\

                /// \brief gets the x and y velocities \returns sf::Vector2f

                    sf::Vector2f getVelocity();

                /// \brief sets the balls angle/speed based on x/y velocity \param vx the target x velocity \param vy the target y velocity

                    void setVelocity(float, float);
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

        const double frictionCoef = 0.95; // friction applied to moving objects on the feild

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
        
            bool inBounds(sf::Vector2f, sf::Vector2f = sf::Vector2f(0, 0));

        /// \brief returns a vector of the cue balls current position
            
            sf::Vector2f getCueBallPosition() {return balls[0].getPosition();}
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(60);
    
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

bool Table::Ball::isCollidingWith(Ball& target)
{
    float distance = fabs(pow(target.getPosition().x - getPosition().x, 2) + pow(target.getPosition().y - getPosition().y, 2));
    if (distance < pow(getRadius() + target.getRadius(), 2))
        return true;
    else
        return false;
}

sf::Vector2f Table::Ball::getVelocity() {return sf::Vector2f(speed * sin(direction * M_PI / 180), speed * cos(direction * M_PI / 180));}

void Table::Ball::setVelocity(float vx, float vy)
{
    speed = sqrtf((vx * vx) + (vy * vy));
    direction = atan2f(vy, vx) * 180 / M_PI;
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
    if (power > 5) {
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
    std::vector<std::pair<Ball&, Ball&>> collisionPairs;

    cue.update(mousePosition);
    for (int i = 0; i < 16; i++) {
        if (balls[i].speed > 0)
            for (int j = 1; j < balls[i].speed; j++) {
                if (inBounds(balls[i].getPosition() + sf::Vector2f(sin(balls[i].direction * M_PI / 180), cos(balls[i].direction * M_PI / 180)), sf::Vector2f(balls[i].getRadius(), balls[i].getRadius())))
                    balls[i].move(sf::Vector2f(sin(balls[i].direction * M_PI / 180), cos(balls[i].direction * M_PI / 180)));
                else { // ball + wall collision
                    if (balls[i].getPosition().x >= field.getPosition().x + 1 && balls[i].getPosition().x + balls[i].getRadius() + 1 <= field.getPosition().x + field.getSize().x)
                        balls[i].direction = (balls[i].direction * -1) + 180; // angle after hitting top or bottome wall
                    if (balls[i].getPosition().y >= field.getPosition().y + 1 && balls[i].getPosition().y + balls[i].getRadius() + 1 <= field.getPosition().y + field.getSize().y)
                        balls[i].direction *= -1; // angle after hitting left or right wall
                }
            }

            for (int j = 0; j < 16; j++) // ball + ball collision
                    if (balls[i].isCollidingWith(balls[j]) && j != i) { // finds collisions between balls[i] and every other ball besides itself
                        collisionPairs.push_back({balls[i], balls[j]});

                        // static resolution
                            float distance = sqrtf(pow(balls[i].getPosition().x - balls[j].getPosition().x, 2) + pow(balls[i].getPosition().y - balls[j].getPosition().y, 2));
                            float overlap = (distance - balls[i].getRadius() - balls[j].getRadius()) / 2;

                            // move balls so they are no longer colliding (makes them solid-bodies)
                            balls[i].move(sf::Vector2f(-(overlap * (balls[i].getPosition().x - balls[j].getPosition().x) / distance), -(overlap * (balls[i].getPosition().y - balls[j].getPosition().y) / distance)));
                            balls[j].move(sf::Vector2f(overlap * (balls[i].getPosition().x - balls[j].getPosition().x) / distance, overlap * (balls[i].getPosition().y - balls[j].getPosition().y) / distance));
            }
        balls[i].speed *= frictionCoef;
    }
    // dynamic resolution physics 
    for (int i = 0; i < collisionPairs.size(); i++) {
        float mass = 1;

        Ball& ball1 = collisionPairs[i].first;
        Ball& ball2 = collisionPairs[i].second;

        float distance = sqrtf(pow(ball1.getPosition().x - ball2.getPosition().x, 2) + pow(ball1.getPosition().y - ball2.getPosition().y, 2));

        // normal vector
        sf::Vector2f normal((ball1.getPosition().x - ball2.getPosition().x) / distance, (ball1.getPosition().y - ball2.getPosition().y) / distance);
        // tangent vector
        sf::Vector2f tangent(-normal.y, normal.x);

        float tanDotProduct1 = ball1.getVelocity().x * tangent.x + ball1.getVelocity().y * tangent.y;
        float tanDotProduct2 = ball2.getVelocity().x * tangent.x + ball2.getVelocity().y * tangent.y;

        float normalDotProduct1 = ball1.getVelocity().x * normal.x + ball1.getVelocity().y * normal.y;
        float normalDotProduct2 = ball2.getVelocity().x * normal.x + ball2.getVelocity().y * normal.y;

        float m1 = (normalDotProduct1 * (mass - mass) + 2.0f * mass * normalDotProduct2) / (mass + mass);
		float m2 = (normalDotProduct2 * (mass - mass) + 2.0f * mass * normalDotProduct1) / (mass + mass);

        ball1.setVelocity(tangent.x * tanDotProduct1 + normalDotProduct1 * m1, tangent.y * tanDotProduct1 + normalDotProduct1 * m1);
        ball2.setVelocity(tangent.x * tanDotProduct2 + normalDotProduct2 * m2, tangent.y * tanDotProduct2 + normalDotProduct2 * m2);
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

bool Table::inBounds(sf::Vector2f p, sf::Vector2f size)
{
    if (p. x >= field.getPosition().x && p.y > field.getPosition().y && p.x + size.x < field.getPosition().x + field.getSize().x && p.y + size.y < field.getPosition().y + field.getSize().y)
        return true;
    else
        return false;
}