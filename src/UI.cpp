#include "headers/UI.h"
#include <iostream>

sf::Color UI::scheme[2] = {sf::Color(255, 255, 255, 20), sf::Color(255, 100, 100, 20)};
sf::Font UI::font;

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

void UI::drawTo(sf::RenderWindow& target)
{
	target.draw(save);
    target.draw(save.text);
	target.draw(load);
    target.draw(load.text);
	target.draw(playerBanner);
    target.draw(playerBanner.text);
}