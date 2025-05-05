#include "CelluarAutomata.h"

double max(double a, double b) {
	if (a > b)return a;
	else return b;
}

double min(double a, double b) {
	if (a < b)return a;
	else return b;
}


CelluarAutomata::CelluarAutomata(int w, int h) {
	width = w;
	height = h;
	tau = 1;

	cells = new cell* [h];
	for (int i = 0; i < h; i++) {
		cells[i] = new cell[w];
	}
}

CelluarAutomata::~CelluarAutomata() {
	for (int i = 0; i < height; i++) {
		delete[] cells[i];
	}
	delete[] cells;
}

void CelluarAutomata::display_on_the_screen(sf::RenderWindow& w, sf::RenderWindow& w_x, sf::RenderWindow& w_y, int w_pixels, int h_pixels) {
	sf::Image canvas;
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Image canvas_x;
	sf::Image canvas_y;

	canvas.create(w_pixels, h_pixels);
	canvas_x.create(w_pixels, h_pixels);
	canvas_y.create(w_pixels, h_pixels);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (cells[i][j].density > 0) {
				int scaled_i = i * 3;
				int scaled_j = j * 3;
				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						if (scaled_i + k < 0 || scaled_i + k >= h_pixels || scaled_i + l < 0 || scaled_i + l >= w_pixels)continue;

						double m = (cells[i][j].density - 0.95) * 20;
						if (m > 1.0)m = 1.0;
						else if (m < 0.01)m = 0;
						canvas.setPixel(scaled_j + k, scaled_i + l, sf::Color(50 * m, 255 * m, 50 * m));

						double m_blue, m_red = 0.0;
						m_blue = (cells[i][j].velocity[0]) * 30;
						if (m_blue < 0.0)m_red = m_blue;
						if (m_blue > 1.0)m_blue = 1.0;
						if (m_red < -1.0)m_red = -1.0;
						canvas_x.setPixel(scaled_j + k, scaled_i + l, sf::Color(255 * fabs(min(0.0, m_red)), 0, 255 * max(0.0, m_blue)));

						m_red = 0.0;
						m_blue = (cells[i][j].velocity[1]) * 30;
						if (m_blue < 0.0)m_red = m_blue;
						if (m_blue > 1.0)m_blue = 1.0;
						if (m_red < -1.0)m_red = -1.0;
						canvas_y.setPixel(scaled_j + k, scaled_i + l, sf::Color(255 * fabs(min(0.0, m_red)), 0, 255 * max(0.0, m_blue)));
					}
				}
			}
			if (cells[i][j].is_a_wall == 1) {
				int scaled_i = i * 3;
				int scaled_j = j * 3;
				for (int k = -1; k < 2; k++) {
					if (scaled_j + k < 0 || scaled_j + k > h_pixels)continue;
					for (int l = -1; l < 2; l++) {
						if (scaled_i + l < 0 || scaled_i + l > w_pixels)continue;
						canvas.setPixel(scaled_j + k, scaled_i + l, sf::Color(100, 100, 100));
						canvas_x.setPixel(scaled_j + k, scaled_i + l, sf::Color(100, 100, 100));
						canvas_y.setPixel(scaled_j + k, scaled_i + l, sf::Color(100, 100, 100));
					}
				}
			}

		}
	}
	texture.loadFromImage(canvas);
	sprite.setTexture(texture);
	w.clear();
	w.draw(sprite);
	w.display();

	texture.loadFromImage(canvas_x);
	sprite.setTexture(texture);
	w_x.clear();
	w_x.draw(sprite);
	w_x.display();

	texture.loadFromImage(canvas_y);
	sprite.setTexture(texture);
	w_y.clear();
	w_y.draw(sprite);
	w_y.display();
}

double CelluarAutomata::weight(int direction) {
	if (direction == 0)return (4.0 / 9.0);
	else if (direction < 5)return (1.0 / 9.0);
	else return (1.0 / 36.0);
}

void CelluarAutomata::fill(int units, int up, int down, int left, int right) {
	double rho;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (cells[i][j].is_a_wall == 1)continue;

			if (i >= left && i <= right && j >= up && j <= down) {
				rho = cells[i][j].density = 1.0;
			}
			else {
				rho = cells[i][j].density = 0.96;
			}

			for (int k = 0; k < 9; k++) {
				cells[i][j].in[k] = weight(k) * rho;
			}

		}
	}
}

void CelluarAutomata::setup() {
	for (int i = 0; i < width; i++) {
		cells[0][i].solidify();
		cells[height - 1][i].solidify();
	}

	for (int i = 0; i < height; i++) {
		cells[i][0].solidify();
		cells[i][width-1].solidify();
		cells[i][width / 4].solidify();
	}
}

void CelluarAutomata::set_tau(double t) {
	if (t < 0.5) t = 0.5;
	tau = t;
}

void CelluarAutomata::execute_iteration() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {

			if (cells[i][j].is_a_wall == 1) {
				continue;
			}
				
			cell* ptr = &cells[i][j];
			double rho = ptr->density;
			
			ptr->velocity[0] = (ptr->in[1] + ptr->in[5] + ptr->in[8] - ptr->in[2] - ptr->in[7] - ptr->in[6])/rho;
			ptr->velocity[1] = (ptr->in[3] + ptr->in[5] + ptr->in[6] - ptr->in[4] - ptr->in[7] - ptr->in[8])/rho;

			double center = ((ptr->velocity[0] * ptr->velocity[0]) + (ptr->velocity[1] * ptr->velocity[1]));
			
			for (int k = 0; k < 9; k++) {
				double u = (dir[k][0] * ptr->velocity[0]) + (dir[k][1] * ptr->velocity[1]);
				ptr->eq[k] = weight(k) * rho * (1 + 3.0 * (u)+4.5 * (u * u) - 1.5 * center);
			}


			for (int k = 0; k < 9; k++) {
				ptr->out[k] = ptr->in[k] + (1.0 / tau) * (ptr->eq[k] - ptr->in[k]);
			}
			
		}
	}


	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			if (cells[i][j].is_a_wall == 1)
				continue;

			for (int k = 0; k < 9; k++) {
				cells[i][j].in[k] = 0.0;
			}

			cells[i][j].density = 0.0;

			for (int k = 0; k < 9; k++) {
				if (cells[i + automaton_dir[k][0]][j + automaton_dir[k][1]].is_a_wall == 1) {
					cells[i][j].in[k] += cells[i][j].out[contr[k][1]];
				}
				else {
					cells[i][j].in[k] += cells[i + automaton_dir[k][0]][j + automaton_dir[k][1]].out[k];
				}
			}
			
			for (int k = 0; k < 9; k++) {
				cells[i][j].density += cells[i][j].in[k];
			}
		}

	}

	
	
}

void CelluarAutomata::display() {
	int h_pixels = height * 3;
	int w_pixels = width * 3;
	sf::RenderWindow window(sf::VideoMode(w_pixels, h_pixels), "LBM - flow");
	sf::RenderWindow window_x(sf::VideoMode(w_pixels, h_pixels), "LBM - x");
	sf::RenderWindow window_y(sf::VideoMode(w_pixels, h_pixels), "LBM - y");
	display_on_the_screen(window, window_x, window_y, h_pixels, w_pixels);
	int iter = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				execute_iteration();
				display_on_the_screen(window, window_x, window_y, h_pixels, w_pixels);
				std::cout << ++iter << std::endl;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				for (int i = 80; i <= 120; i++) {
					cells[i][width / 4].remove();
					cells[i][width / 4].density = 1.0;
					cells[i][width / 4].in[0] = (4.0 / 9.0) * 1.0;
					for (int k = 1; k < 5; k++) {
						cells[i][width / 4].in[k] = (1.0 / 9.0) * 1.0;
					}
					for (int k = 5; k < 9; k++) {
						cells[i][width / 4  ].in[k] = (1.0 / 36.0) * 1.0;
					}

				}
		}

	}
}
