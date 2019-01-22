#include "Ball.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <math.h>
#define PI 3.14159265

using namespace sf;

SoundBuffer wall;
Sound sWall;
SoundBuffer life;
Sound sLife;

//sets up the initial values for all of ball's attributes 
void Ball::set_properties(float r, float x, float y, float dx, float dy, Color c) {
	radius = r;
	pos.x = x;
	pos.y = y;
	color = c;
	vel.x = dx;
	vel.y = dy;
	sphere.setRadius(radius);
	sphere.setPosition(pos.x, pos.y);
	sphere.setFillColor(color);
	wall.loadFromFile("wallBounce.wav");
	sWall.setBuffer(wall);
	life.loadFromFile("rip.wav");
	sLife.setBuffer(life);
}

//This portion of code provided by Walter's example
//constantly updates the ball's position and checks for boundary collision
CircleShape Ball::update(float dt, float width, float length, int currlives) {
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	if (pos.y + radius * 2 > length && vel.y > 0){
		pos.y = length + 100;
		pos.x = width / 2;
		if (currlives > 0) { sLife.play(); }
	}
	if (pos.y < 0 && vel.y < 0) {
		vel.y = -vel.y;
		sWall.play();
	}
	if (pos.x + radius * 2 > width && vel.x > 0) {
		vel.x = -vel.x;
		sWall.play();
	}
	if (pos.x < 0 && vel.x < 0) {
		vel.x = -vel.x;
		sWall.play();
	}
	sphere.setPosition(pos.x, pos.y);
	return sphere;
}

//puts the ball back on top of the paddle 
void Ball::startRound(float xpos, float ypos) {
	pos.x = xpos;
	pos.y = ypos;
	vel.x = 0.0f;
	vel.y = 0.0f;
}

//checks the point on the paddle where the ball collides with it and adjusts its velocities accordingly
void Ball::bounceStuff(float xpos, float width) {
	vel.y = -vel.y;
	float midpoint = width / 2;
	float point = (pos.x + radius) - xpos;
	float value = (point - midpoint) / midpoint;
	float angle = sinf(value*PI / 2);
	vel.x = 400.0f * angle;
}

//makes ball bounce off of bricks
void Ball::bounceStuff2(float xpos, float ypos, float width, float length) {
	if (abs((pos.x + radius) - (xpos + width/2)) > (width/2+radius)*.7) { vel.x = -vel.x; }
	if (abs((pos.y + radius) - (ypos + length/2)) > (length/2+radius)*.4) { vel.y = -vel.y; }
}

//checks where the ball was when it was released and adjusts its velocities accordingly
void Ball::initialAngle(float width, float currSpeed) {
	vel.y = currSpeed;
	float midpoint = width / 2;
	float point = pos.x + radius;
	float value = (point - midpoint) / midpoint;
	float angle = sinf(value*PI / 2);
	vel.x = 300.0f * angle;
}