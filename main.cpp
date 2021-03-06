#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "src/headers/Game.h"
#include <math.h>
#include <string.h>
#include <sstream>
#include <fstream>

class Table
{
    private:
        //private classes:
        class Ball : public sf::CircleShape
        {
            private:
                int id;

            public:
                float direction{0}, speed{0};

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

        class Pocket : public sf::CircleShape
        {
            public:
                Pocket();

        };
        
        // private members:
        sf::RectangleShape field;
        sf::RectangleShape edge;

        const double frictionCoef = 0.99; // friction applied to moving objects on the feild

    public:
        Ball balls[16]; // array that stores all 15 of the balls and the cue ball ([0])
        Pocket pockets[6]; // array that stores all 6 of the pockets
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

class UI
{
	private:
		static sf::Color scheme[2]; // stores two colors used for the color scheme of the ui
		static sf::Font font; // font of the ui
		class Button : public sf::RectangleShape // class for a basic button
		{
            public:
                bool clicked;
                void update(bool, sf::Vector2i);
		};
		class SaveButton : public Button // saves current game to a file (extention problem)
		{
            public:
                SaveButton(sf::Vector2f);
                sf::Text text;
		};
		class LoadButton : public Button // loads the game from a file (extention problem)
		{
            public:
                LoadButton(sf::Vector2f);
                sf::Text text;
		};

		class PlayerBanner : public sf::RectangleShape // indicates whos turn it is
		{
            public:
                PlayerBanner(sf::Vector2f);
                sf::Text text;
		};
	public:

		SaveButton save;
		LoadButton load;

		PlayerBanner playerBanner;
		UI();
		void update(bool, sf::Vector2i, bool);
		void drawTo(sf::RenderWindow&);
};

class Game
{
	private:
		struct Player
		{
			bool team; // 0/false = solids, 1/true = stripes
			bool isTurn;
            int ballsRemaining;
		};
		Player player[2];
        Table table;
        UI ui;
	public:
        Table::Cue& cue = table.cue;
        bool turn = 0;

		Game();

        void save();
        void load();

		void update(bool, sf::Vector2i);
		void drawTo(sf::RenderWindow&);
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(60);

    Game game;

    bool mouseDown = false;
    sf::Vector2i clickPosition;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                mouseDown = true;
                clickPosition = sf::Mouse::getPosition(window); 
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                mouseDown = false;
                game.cue.release();
                game.turn = !game.turn;
            }
        }
        if (mouseDown)
            game.cue.pullBack(sqrt(pow(clickPosition.x - sf::Mouse::getPosition(window).x, 2) + pow(clickPosition.y - sf::Mouse::getPosition(window).y, 2)));

        game.update(mouseDown, sf::Mouse::getPosition(window));

        window.clear();
		game.drawTo(window);
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
    float distanceSquared = fabs(pow(target.getPosition().x - getPosition().x, 2) + pow(target.getPosition().y - getPosition().y, 2));
    if (distanceSquared < pow(getRadius() + target.getRadius(), 2))
        return true;
    else
        return false;
}

sf::Vector2f Table::Ball::getVelocity() {return sf::Vector2f(speed * cos(direction * M_PI / 180), speed * sin(direction * M_PI / 180));}

void Table::Ball::setVelocity(float vx, float vy)
{
    speed = sqrtf((vy * vy) + (vx * vx));
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
        cueBall.hit(angle, power / 4);
    }
}

void Table::Cue::update(sf::Vector2i mousePosition)
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

void Table::Cue::drawTo(sf::RenderWindow& target)
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
        sf::Vector2f(0, 0), sf::Vector2f(64, 0), sf::Vector2f(16, -8),
        sf::Vector2f(48, -8), sf::Vector2f(64, -16), sf::Vector2f(32, 16),
        sf::Vector2f(64, 32), sf::Vector2f(32, 0), sf::Vector2f(48, -24),
        sf::Vector2f(64, 16), sf::Vector2f(16, 8), sf::Vector2f(64, -32),
        sf::Vector2f(48, 24), sf::Vector2f(32, -16), sf::Vector2f(48, 8)
    };

    for (int i = 1; i < 16; i++)
    {
        balls[i].setPosition(sf::Vector2f(field.getPosition().x + field.getSize().x / 4 * 3 + startingPositions[i - 1].x - balls[0].getRadius(), field.getPosition().y + field.getSize().y / 2 + startingPositions[i - 1].y - balls[0].getRadius()));
        balls[i].speed = 0;
    }
}

void Table::update(sf::Vector2i mousePosition)
{
    if (cue.isVisible())
        cue.update(mousePosition);

    for (int i = 0; i < 16; i++) { // parse thru entire ball array
		if (balls[i].speed > 30) balls[i].speed = 20;
        else if (balls[i].speed < 1) balls[i].speed = 0;
        for (int j = 1; j < balls[i].speed; j++) { // apply speed in direction while speed is greater than zero and the ball is in bounds
            balls[i].move(cos(balls[i].direction * M_PI / 180), sin(balls[i].direction * M_PI / 180));
            if (!inBounds(balls[i].getPosition(), sf::Vector2f(2 * balls[i].getRadius(), 2 * balls[i].getRadius()))) break;
            for (int k = 0; k < 16; k++) if (balls[i].isCollidingWith(balls[k])) break;
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
        for (int j = 0; j < 16; j++) // check for collisions between target ball and every other ball, ik its not efficient :)
            if (balls[i].isCollidingWith(balls[j]) && j != i) {
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
    for (int i = 0; i < 16; i++)
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

sf::Color UI::scheme[2] = {sf::Color(255, 255, 255, 20), sf::Color(255, 100, 100, 20)};
sf::Font UI::font;

void UI::Button::update(bool mouseDown, sf::Vector2i mousePosition)
{
    if (mousePosition.x > getPosition().x && mousePosition.y > getPosition().y && mousePosition.x < getPosition().x + getSize().x && mousePosition.y < getPosition().y + getSize().y) {
        setFillColor(scheme[1]);
        setOutlineColor(scheme[1] + sf::Color(0, 0, 0, 50));
        if (mouseDown) {
            clicked = true;
            setFillColor(scheme[0]);
            setOutlineColor(scheme[0] + sf::Color(0, 0, 0, 50));
        }
    }
    else {
        clicked = false;
        setFillColor(scheme[0]);
        setOutlineColor(scheme[0] + sf::Color(0, 0, 0, 50));
    }
}

UI::SaveButton::SaveButton(sf::Vector2f size) : text("save", font) {
		setSize(size);

        text.setScale(0.5, 0.5);

		setOutlineThickness(5);
		setOutlineColor(scheme[0] + sf::Color(0, 0, 0, 50));

		setFillColor(scheme[0]);
}

UI::LoadButton::LoadButton(sf::Vector2f size) : text("load", font)
{
		setSize(size);

        text.setScale(0.5, 0.5);

		setOutlineThickness(5);
		setOutlineThickness(5);
		setOutlineColor(scheme[0] + sf::Color(0, 0, 0, 50));

		setFillColor(scheme[0]);
}

UI::PlayerBanner::PlayerBanner(sf::Vector2f size) : text("Player: ", font)
{
		setSize(size);

        text.setScale(0.5, 0.5);

		setOutlineThickness(5);
		setOutlineThickness(5);
		setOutlineColor(scheme[0] + sf::Color(0, 0, 0, 50));

		setFillColor(scheme[0]);
}

UI::UI() : 
	save(sf::Vector2f(60, 20)), load(sf::Vector2f(60, 20)), 
	playerBanner(sf::Vector2f(150, 20))
{
	if (!font.loadFromFile("RobotoMono-Bold.ttf"))
        std::cout << "error loading font: ./RobotoMono-Bold.ttf" << std::endl;

	save.setPosition(sf::Vector2f(15, 570));
    save.text.setPosition(sf::Vector2f(20, 570));

	load.setPosition(sf::Vector2f(90, 570));
    load.text.setPosition(sf::Vector2f(95, 570));

	playerBanner.setPosition(sf::Vector2f(325, 15));
    playerBanner.text.setPosition(sf::Vector2f(330, 15));

};

void UI::update(bool mouseDown, sf::Vector2i mousePosition, bool turn)
{
    save.update(mouseDown, mousePosition);
    load.update(mouseDown, mousePosition);
    playerBanner.text.setString("Player: " + std::to_string(int(turn) + 1));
}

void UI::drawTo(sf::RenderWindow& target)
{
	target.draw(save);
    target.draw(save.text);
	target.draw(load);
    target.draw(load.text);
	target.draw(playerBanner);
    target.draw(playerBanner.text);
}

Game::Game() : table(sf::Vector2f(400, 200))
{
    table.init();
}

void Game::save()
{
    std::fstream saveFile("saves/save.txt", std::ios::out);
    for (int i = 0; i < 16; i++) {
        saveFile << table.balls[i].getPosition().x << "," << table.balls[i].getPosition().y << "," << table.balls[i].speed << "," << table.balls[i].direction << "\n";
    }
}

void Game::load()
{
    std::fstream saveFile("saves/save.txt", std::ios::in);
    if (saveFile.is_open())
        for (int i = 0; i < 16; i++) {
            std::stringstream ball;
            std::string temp;
            sf::Vector2f newPos;

            std::getline(saveFile, temp);
            ball << temp;

            std::getline(ball, temp, ',');
            newPos.x = std::stof(temp);
            std::getline(ball, temp, ',');
            newPos.y = std::stof(temp);

            table.balls[i].setPosition(newPos);

            std::getline(ball, temp, ',');
            table.balls[i].speed = std::stof(temp);
            std::getline(ball, temp);
            table.balls[i].direction = std::stof(temp);
        }
}

void Game::update(bool mouseDown, sf::Vector2i mousePosition)
{
    table.update(mousePosition);
    ui.update(mouseDown, mousePosition, turn);

    if (ui.save.clicked) save();
    if (ui.load.clicked) load();
}

void Game::drawTo(sf::RenderWindow& target)
{
    table.drawTo(target);
    ui.drawTo(target);
}