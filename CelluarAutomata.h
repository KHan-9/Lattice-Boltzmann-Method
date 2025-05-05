#pragma once
#include"Cell.h"

class CelluarAutomata {
	int width, height;		//both variables describe amount of cells, not size in pixels!
	cell** cells;
	double tau;
	int dir[9][2] = { {0,0}, {1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1} };
	int automaton_dir[9][2] = { {0,0}, {0,1}, {0,-1}, {-1,0}, {1,0}, {-1,1}, {-1,-1}, {1,-1}, {1,1} };
	int contr[9][2] = { {0,0}, {1,2}, {2,1}, {3,4}, {4,3}, {5,7}, {6,8}, {7,5}, {8,6} };
	void display_on_the_screen(sf::RenderWindow& w, sf::RenderWindow& w_x, sf::RenderWindow& w_y, int w_pixels, int h_pixels);

public:
	CelluarAutomata(int, int);
	~CelluarAutomata();
	void display();
	void fill(int units, int up, int down, int left, int right);
	void setup();
	void execute_iteration();
	void set_tau(double t);
	double weight(int direction);
};

