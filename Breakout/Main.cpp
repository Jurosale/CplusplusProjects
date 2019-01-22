#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Vector2f.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

using namespace std;
using namespace sf;

//Sets up the dimensions, positions and other important variables that we'll be using throughout the program
float widthWin = 800.0f;
float lengthWin = 600.0f;
float widthPad = 100.0f;
float lengthPad = 20.0f;
float radiusBall = 10.0f;
float VelBall = -150.0f;
int lvl = 0;
bool isSpawn;

//initalizes all the SFML tools we'll need for this project
RectangleShape rect;
CircleShape sphere;
Texture background;
SoundBuffer cheer;
Sound sCheer;
SoundBuffer paddle;
Sound sPaddle;
SoundBuffer destroy;
Sound sDestroy;
SoundBuffer damage;
Sound sDamage;

//Sets up the bricks we will be using
vector<unique_ptr<Brick>> bricks;
void create_bricks(int);

int main()
{
	RenderWindow window(VideoMode(widthWin, lengthWin), "Welcome to Pong!");

	//Sets up all the sound effects in the game
	cheer.loadFromFile("cheer2.wav");
	sCheer.setBuffer(cheer);
	paddle.loadFromFile("paddle.wav");
	sPaddle.setBuffer(paddle);
	destroy.loadFromFile("break.wav");
	sDestroy.setBuffer(destroy);
	damage.loadFromFile("damage.wav");
	sDamage.setBuffer(damage);

	//this part of code provided by SFML tutorials
	//displays the background image
	Sprite bg;
	background.loadFromFile("background.jpg");
	bg.setTexture(background);
	bg.setTextureRect(IntRect(0, 0, widthWin, lengthWin));
	bg.setColor(Color(255, 255, 255, 200));

	srand(time(NULL));
	Clock clock;

	//creats the paddle and ball and gives them their starting values
	Paddle player;
	Ball ball;
	player.set_properties(widthPad, lengthPad, (widthWin - widthPad) / 2, lengthWin - lengthPad, Color::Blue);
	ball.set_properties(radiusBall, 0.0f, 0.0f, 0.0f, 0.0f, Color::White);

	//the game loop
	while (window.isOpen()) {
	Event event;
	while (window.pollEvent(event)) { if (event.type == Event::Closed) { window.close(); } }

		//when player breaks all bricks, start a new game
		if (bricks.size() <= 0) {
			if (lvl > 0) { sCheer.play(); }
			isSpawn = true;
			VelBall -= 50.0f;
			lvl++;
			create_bricks(lvl);
		}

		//When ball falls out of bound, respawns back on player's paddle
		if (isSpawn && player.lives > 0) {
			ball.startRound(player.pos.x + (player.dim.x / 2) - ball.radius, player.pos.y - player.dim.y);
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				ball.initialAngle(widthWin, VelBall);
				isSpawn = false;
			}
		}

		float dt = clock.restart().asSeconds();

		//constantly updates and draws ball and paddle
		window.clear();
		window.draw(bg);
		window.draw(player.update());
		window.draw(ball.update(dt, widthWin, lengthWin, player.lives));

		//code partly provided by 11/14 lecture slides
		//Constantly updates all the bricks
		for (int i = 0; i < bricks.size(); i++) {
			window.draw(bricks[i]->draw(bricks[i]->hp));
			if (bricks[i]->collided(ball.pos.x, ball.pos.y, ball.radius)) {
				bricks[i]->hp--;
				ball.bounceStuff2(bricks[i]->pos.x, bricks[i]->pos.y, bricks[i]->dim.x, bricks[i]->dim.y);
				player.score += 10 * lvl;
				if (bricks[i]->hp <= 0) {
					if (bricks[i]->hp == -1) { ball.vel.y = ball.vel.y * 1.15; }
					bricks.erase(bricks.begin() + i);
					sDestroy.play();
					continue;
				}
				else { sDamage.play(); }
			}
		}

		//if ball passes below paddle, start a new round
		if (ball.pos.y > lengthWin) { 
			player.lives--;
			isSpawn = true;
		}
		
		//checks and updates the original ball for collisions with the paddle 
		bool collideP = player.collided(ball.pos.x, ball.pos.y, ball.radius);
		if (collideP && !isSpawn) { 
			ball.bounceStuff(player.pos.x, player.dim.x);
			sPaddle.play();
		}

		//displays the score and lives
		window.draw(player.displayScore(player.score));
		window.draw(player.displayLives(player.lives, widthWin));
		window.display();

		//updates the paddles movement based on player's input
		if (Keyboard::isKeyPressed(Keyboard::Left) && player.pos.x > 0.0f && player.lives > 0) { player.pos.x -= 500*dt; }
		if (Keyboard::isKeyPressed(Keyboard::Right) && (player.pos.x + player.dim.x) < widthWin && player.lives > 0) { player.pos.x += 500*dt; }
	}

	return 0;
}

//code partly provided by 11/14 lecture slides
//creates the bricks
void create_bricks(int level)
{
	float boundary = 0.0f;
	float dimX = 80.0f;
	float dimY = 40.0f;
	float posX;
	float posY = dimY * 2;
	sf::Color color;

	for (int i = 0; i < 24; ++i)
	{
		//Determines the probability of getting each type of brick
		int num;
		int ranNum = rand() % 20;
		if (ranNum < 8) { num = 0; }
		else if (ranNum < 14) { num = 1; }
		else if (ranNum < 18) { num = 2; }
		else if (ranNum < 19) { num = -2; }
		else { num = -1; }

		//assigns the bricks' positions and dimensions into a loop of 3 levels
		//Assigns level 1
		if (level % 3 == 1) {
			posX = dimX * (i + 1) - boundary;
			if (posX >= widthWin - dimX) {
				posX = dimX;
				boundary += (widthWin - dimX * 2);
				posY += dimY;
			}
		}
		//Assigns level 2
		else if (level % 3 == 2) {
			if (i <= 9) {
				posX = dimX * i;
				posY = dimY * i;
			}
			else if (i <= 19) {
				posX = dimX * (i-10);
				posY = dimY * (i-6);
				if (i == 19) { posY--; }

			}
			else {
				posX = dimX * (i-14);
				posY = dimY * (i-20);
			}
		}
		//Assigns level 3
		else {
			if (i <= 12) {
				posX = dimX * 3;
				posY = dimY * i;
			}
			else if (i == 13) {
				posX = dimX * 3;
				posY = (dimY * i) - 1;
			}
			else {
				posX = dimX * (i - 14);
				posY = dimY * 10;
			}
		}
		
		//puts the bricks in a list of bricks
		Brick* b = new Brick;
		b->Block(posX, posY, dimX, dimY, num + 1);
		bricks.push_back(unique_ptr<Brick>(b));
		
	}
}