#include "Brick.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

//initializes each of the brick's positions, sizes, lives and textures
void Brick::Block(float xpos, float ypos, float xdim, float ydim, int lives) {
	pos.x = xpos;
	pos.y = ypos;
	dim.x = xdim;
	dim.y = ydim;
	hp = lives;
	rect.setSize(sf::Vector2f(dim.x, dim.y));
	rect.setPosition(pos.x, pos.y);
}

//checks whether or not the brick is colliding with the ball
bool Brick::collided(float xpos, float ypos, float rad) {
	return pos.x + dim.x >= xpos &&
		pos.x < xpos + rad * 2 &&
		pos.y + dim.y >= ypos &&
		pos.y < ypos + rad * 2;
}

//draws the brick and the respective color to indicate type of brick
RectangleShape Brick::draw(int lives) {
	Color color;
	switch (lives) {
	case -1:
		color = Color::Transparent;
		break;
	case 0:
		color = Color::Black;
		break;
	case 1:
		color = Color::Red;
		break;
	case 2:
		color = Color::Yellow;
		break;
	case 3:
		color = Color::Green;
		break;
	}
	rect.setFillColor(color);
	return rect;
}