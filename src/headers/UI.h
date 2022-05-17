#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>

class UI
{
    private:
        static sf::Color scheme[2]; // stores two colors used for the color scheme of the ui
        static sf::Font font; // font of the ui
        class Button : public sf::RectangleShape // class for a basic button
        {
            public:
                bool clicked;
                void update(sf::Vector2i);
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
        void update(sf::Vector2i);
        void drawTo(sf::RenderWindow&);
};

#endif