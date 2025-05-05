#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<time.h>


class cell {
public:
	double in[9];
	double out[9];
	double eq[9];
	double density;
	double velocity[2];		// 0 -> X	1 -> Y
	int is_a_wall;

	cell() {
		for (int i = 0; i < 9; i++) {
			in[i] = out[i] = eq[i] =  0.0;
		}
		density = 0.0;
		velocity[0] = velocity[1] = 0.0;
		is_a_wall = 0;
	}

	void solidify() {
		is_a_wall = 1;
	}

	void remove() {
		is_a_wall = 0;
	}
};
