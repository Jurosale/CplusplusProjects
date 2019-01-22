#include "Paddle.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace sf;

Font font;

//sets up the initial values for all of paddle's attributes 
void Paddle::set_properties(float w, float l, float x, float y, Color c) {
	dim.x = w;
	dim.y = l;
	pos.x = x;
	pos.y = y;
	color = c;
	score = 0;
	lives = 3;
	rect.setSize(sf::Vector2f(dim.x, dim.y));
	rect.setPosition(pos.x, pos.y);
	rect.setFillColor(color);
	font.loadFromFile("BERNHC.ttf");
}

//This portion of code provided by Walter's example
//checks whether or not the paddle is colliding with the ball
bool Paddle::collided(float xpos, float ypos, float rad) {
	return pos.x + dim.x >= xpos &&
		pos.x < xpos + rad * 2 &&
		pos.y + dim.y >= ypos &&
		pos.y < ypos + rad * 2;
}

//constantly updates the paddle's position 
RectangleShape Paddle::update() {
	rect.setPosition(pos.x, pos.y);
	return rect;
}

//This part of the code provided by SFML tutorials
//displays the player's score on the top left
Text Paddle::displayScore(int currScore) {
	stringstream ss;
	ss << currScore;
	string str = ss.str();
	Text text;
	text.setFont(font);
	text.setCharacterSize(40);
	text.setColor(Color::Cyan);
	text.setStyle(Text::Bold);
	text.setString("Score: " + str);
	text.setPosition(0,0); 
	return text;
}

//displays the player's lives on the top right
Text Paddle::displayLives(int currLives, float width) {
	string str;
	if (currLives > 0) {
		stringstream ss;
		ss << currLives;
		str = ss.str();
	}
	else { str = "Game Over"; }
	Text text;
	text.setFont(font);
	text.setCharacterSize(40);
	text.setColor(Color::Magenta);
	text.setStyle(Text::Bold);
	if (currLives > 0) { 
		text.setPosition(width - 130.0f, 0.0f);
		text.setString("Lives: " + str); 
	}
	else { 
		text.setPosition(width - 190.0f, 0.0f);
		text.setString(str); 
	}
	return text;
}