#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Table.h"
#include "UI.h"

class Game
{
	private:
		struct Player
		{
			bool team; // 0/false = solids, 1/true = stripes
			bool isTurn;
            int ballsRemaining{7};
		};

		Player player[2];
            bool turn = false;
        Table table;
        UI ui; 

	public:
        Cue& cue = table.cue;
		Game();
		void update(sf::Vector2i);
		void drawTo(sf::RenderWindow&);
};

#endif