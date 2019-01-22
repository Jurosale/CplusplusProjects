#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#define PI 3.14159265

//Sets up the dimensions and positions that we'll be using throughout the program
float widthWin = 800.0f;
float lengthWin = 600.0f;
float widthPad = 20.0f;
float lengthPad = 100.0f;
float ypos1 = (lengthWin / 2) - (lengthPad/2);
float ypos2 = (lengthWin / 2) - (lengthPad / 2);
float radiusBall = 10.0f;
float xposBstart = (widthWin / 2);
float yposBstart = (lengthWin / 2);

//sets up flags and other in game elements to trigger events
int winner = 0;
int players = 0;
bool isSpawn = true;
bool checkForPlayers = true;
bool twoballs = true;
bool collidedObs;
bool collidedObs2;
time_t timer;
double delay;

//initalizes all the SFML tools we'll need for this project
sf::RectangleShape rect;
sf::CircleShape sphere;
sf::Font font;
sf::Texture background;
sf::SoundBuffer cheer;
sf::Sound sound;

//sets up the classes for the paddles and ball
struct Vector2f {
	float x;
	float y;
};

class Paddle {
public:
	sf::Color color;
	Vector2f dim;
	Vector2f pos;
	Vector2f vel;
	int score;
	void set_properties(float, float, float, float, sf::Color);
	sf::RectangleShape update();
	bool collided(float,float, float);
	sf::Text displayScore(int, int);
	sf::RectangleShape obs_update(float);
};

class Ball {
public:
	sf::Color color;
	float radius;
	Vector2f pos;
	Vector2f vel;
	int point;
	void set_properties(float, float, float, float, float, sf::Color);
	sf::CircleShape update(float);
	double startRound();
	void bounceStuff(float, float, float, float);
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(widthWin, lengthWin), "Welcome to Pong!");
	
	font.loadFromFile("BERNHC.ttf");
	background.loadFromFile("background.jpg");
	cheer.loadFromFile("cheer2.wav");

	//this part of code provided by SFML tutorials
	//displays the background image
	sf::Sprite bg;
	bg.setTexture(background);
	bg.setTextureRect(sf::IntRect(0, 0, widthWin, lengthWin));
	bg.setColor(sf::Color(255, 255, 255, 200));

	sound.setBuffer(cheer);

	srand(time(NULL));
	sf::Clock clock;

	//creats the paddles, obstacle, and ball and gives them their starting values
	Paddle player1, player2, obstacle;
	Ball ball, ball2;
	player1.set_properties(widthPad, lengthPad, widthPad, ypos1, sf::Color::Red);
	player2.set_properties(widthPad, lengthPad, widthWin - widthPad*2, ypos2, sf::Color::Blue);
	obstacle.set_properties(widthPad, lengthPad, widthWin/2, 0, sf::Color::Black);
	ball.set_properties(radiusBall, xposBstart, yposBstart, 0.0f, 0.0f, sf::Color::White);
	player1.vel.y = 500.0f;
	player2.vel.y = 500.0f;
	obstacle.vel.y = 10.0f;

	//this part of code provided by SFML tutorials
	//Displays the inital text that tells the player to pick 1 or 2 players
	sf::Text result;
	result.setFont(font);
	result.setCharacterSize(70);
	result.setColor(sf::Color::Yellow);
	result.setStyle(sf::Text::Bold);
	result.setPosition(xposBstart/4 , yposBstart/2);
	result.setString("Press '1' for 1 player,\n    '2' for 2 players ");
	window.clear();
	window.draw(result);
	window.display();

	//the game loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) { if (event.type == sf::Event::Closed) { window.close(); } }
		//continues to play as long as no winner has been declared yet
		if (winner == 0 && players != 0) {
	
			//When someone has scores a point, the ball resets back to the center and the 2nd ball (if active) disappears
			if (isSpawn) {
				timer = time(NULL);
				delay = ball.startRound();
				ball2.pos.x = xposBstart;
				ball2.pos.y = -yposBstart;
				ball2.vel.x = 0;
				ball2.vel.y = 0;
				twoballs = true;
				isSpawn = false;
			}

			float dt = clock.restart().asSeconds();

			window.clear();
			window.draw(bg);
			window.draw(player1.update());
			window.draw(player2.update());
			window.draw(obstacle.obs_update(dt));
			
			//this part of the code ensures the obstacle doesn't move through the ball as it's respawning back to the center
			if (difftime(time(NULL), timer) < delay && (collidedObs == true)) { 
				if (obstacle.vel.y > 0) { obstacle.pos.y += obstacle.dim.y; }
				else { obstacle.pos.y -= obstacle.dim.y; }
			}
			//the ball will start moving once it's delay timer is up
			else if (difftime(time(NULL), timer) > delay) { window.draw(ball.update(dt)); }
			else {
				window.draw(ball.update(0));
			}

			//if 15 seconds have passed, a 2nd ball spawns out of the 1st ball
			if (difftime(time(NULL), timer) > 15.0f && twoballs) { 
				float uselessFloat = ball2.startRound();
				ball2.set_properties(radiusBall, ball.pos.x, ball.pos.y, ball2.vel.x, ball2.vel.y, sf::Color::White);
				twoballs = false;
			}

			//checks and updates the 2nd ball for collisions between the paddles and obstacle 
			if (difftime(time(NULL), timer) > 15.0f) { 
				window.draw(ball2.update(dt));
				bool collideP1b2 = player1.collided(ball2.pos.x, ball2.pos.y, ball2.radius);
				bool collideP2b2 = player2.collided(ball2.pos.x, ball2.pos.y, ball2.radius);
				collidedObs2 = obstacle.collided(ball2.pos.x, ball2.pos.y, ball2.radius);
				if (collideP1b2) { ball2.bounceStuff(player1.pos.x, player1.pos.y, player1.dim.x, player1.dim.y); }
				else if (collideP2b2) { ball2.bounceStuff(player2.pos.x, player2.pos.y, player2.dim.x, player2.dim.y); }
				else if (collidedObs2) { ball2.bounceStuff(obstacle.pos.x, obstacle.pos.y, obstacle.dim.x, obstacle.dim.y); }
			}

			//checks and updates the original ball for collisions between the paddles and obstacle 
			bool collideP1 = player1.collided(ball.pos.x, ball.pos.y, ball.radius);
			bool collideP2 = player2.collided(ball.pos.x, ball.pos.y, ball.radius);
			collidedObs = obstacle.collided(ball.pos.x, ball.pos.y, ball.radius);
			if (collideP1) { ball.bounceStuff(player1.pos.x, player1.pos.y, player1.dim.x, player1.dim.y); }
			else if (collideP2) { ball.bounceStuff(player2.pos.x, player2.pos.y, player2.dim.x, player2.dim.y); }
			else if (collidedObs) { ball.bounceStuff(obstacle.pos.x, obstacle.pos.y, obstacle.dim.x, obstacle.dim.y); }

			//this code tells the paddles to update their respective scores when they score a point
			if (ball.point == 1 || ball2.point == 1) {
				player1.score++;
				ball.point = 0;
				ball2.point = 0;
			}
			if (ball.point == -1 || ball2.point == -1) {
				player2.score++;
				ball.point = 0;
				ball2.point = 0;
			}

			//If either player reaches 5 points, the game declares them the winner
			if (player1.score >= 5) { 
				winner = 1;
				result.setString("Player 1 Wins!");
				result.setColor(sf::Color::Red);
				window.draw(result);
			}
			else if (player2.score >= 5) { 
				winner = 2;
				result.setString("Player 2 Wins!");
				result.setColor(sf::Color::Blue);
				window.draw(result);
			}
			
			window.draw(player1.displayScore(player1.score, 1));
			window.draw(player1.displayScore(player2.score, 2));
			window.display();

			//updates the paddles movement based on player(s) input or ball position in the AI's case
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1.pos.y > 0.0f) { player1.pos.y -= player1.vel.y*dt; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (player1.pos.y + player1.dim.y) < lengthWin) { player1.pos.y += player1.vel.y*dt; }
			if (players == 1) {
				if (player2.pos.y + (player2.dim.y / 2) > ball.pos.y && player2.pos.y > 0.0f) { player2.pos.y -= player2.vel.y*dt*.75; }
				else if (player2.pos.y + (player2.dim.y / 2) < ball.pos.y && (player2.pos.y + player2.dim.y) < lengthWin) { player2.pos.y += player2.vel.y*dt*.75; }
			}
			else if (players == 2) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player2.pos.y > 0.0f) { player2.pos.y -= player2.vel.y*dt; }
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (player2.pos.y + player2.dim.y) < lengthWin) { player2.pos.y += player2.vel.y*dt; }
			}
		}

		//When someone has won, the player(s) can press 'space' to start a new game
		if (winner != 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			player1.score = 0;
			player2.score = 0;
			player1.pos.y = ypos1;
			player2.pos.y = ypos2;
			obstacle.pos.y = 0;
			winner = 0;
			float dt = clock.restart().asSeconds();
			result.setString("");
		}

		//takes the input from the very beginning of the game to determine the number of players
		if (checkForPlayers) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				players = 1;
				checkForPlayers = false;
				result.setString("");
				result.setPosition(xposBstart - xposBstart / 2, yposBstart);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				players = 2;
				checkForPlayers = false;
				result.setString("");
				result.setPosition(xposBstart - xposBstart / 2, yposBstart);
			}
		}
	}

	return 0;
}

//sets up the initial values for all of paddle's attributes 
void Paddle::set_properties(float w, float l, float x, float y, sf::Color c) {
	dim.x = w;
	dim.y = l;
	pos.x = x;
	pos.y = y;
	color = c;
	score = 0;
	rect.setSize(sf::Vector2f(dim.x, dim.y));
	rect.setPosition(pos.x,pos.y);
	rect.setFillColor(color);
}

//constantly updates the paddle's position 
sf::RectangleShape Paddle::update() {
	rect.setPosition(pos.x, pos.y);
	rect.setFillColor(color);
	return rect;
}

//This portion of code provided by Walter's example
//checks whether or not the paddle is colliding with the ball
bool Paddle::collided(float xpos, float ypos, float rad) {
	return pos.x + dim.x >= xpos &&
		pos.x < xpos + rad*2 &&
		pos.y + dim.y >= ypos &&
		pos.y < ypos + rad*2;
}

//This part of the code provided by SFML tutorials
//displays the player of the respective paddle's score on the top
sf::Text Paddle::displayScore(int currScore, int numP) {
	std::stringstream ss;
	ss << currScore;
	std::string str = ss.str();
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(100);
	text.setColor(sf::Color::Yellow);
	text.setStyle(sf::Text::Bold);
	text.setString(str);
	if (numP == 1) { text.setPosition((widthWin / 2) - widthWin / 4, 0.0f); }
	else { text.setPosition((widthWin / 2) - 50 + widthWin / 4, 0.0f); }
	return text;
}

//This portion of code provided by Walter's example
//gives the obstacle its own method of movement and detecting boundary lines
sf::RectangleShape Paddle::obs_update(float dt) {
	pos.y += vel.y * dt;
	if (pos.y + dim.y > lengthWin && vel.y > 0)
		vel.y = -vel.y;
	if (pos.y < 0 && vel.y < 0)
		vel.y = -vel.y;
	if (pos.x + dim.x > widthWin && vel.x > 0) {
		vel.x = -vel.x;
	}
	if (pos.x < 0 && vel.x < 0) {
		vel.x = -vel.x;
	}
	rect.setPosition(pos.x, pos.y);
	rect.setFillColor(sf::Color::Green);
	return rect;
}

//sets up the initial values for all of ball's attributes 
void Ball::set_properties(float r, float x, float y, float dx, float dy, sf::Color c) {
	radius = r;
	pos.x = x;
	pos.y = y;
	color = c;
	vel.x = dx;
	vel.y = dy;
	sphere.setRadius(radius);
	sphere.setPosition(pos.x, pos.y);
	sphere.setFillColor(color);
}

//This portion of code provided by Walter's example
//constantly updates the ball's position and checks for boundary collision; also checks if players have scored a point
sf::CircleShape Ball::update(float dt) {
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	if (pos.y + radius*2 > lengthWin && vel.y > 0)
		vel.y = -vel.y;
	if (pos.y < 0 && vel.y < 0)
		vel.y = -vel.y;
	if (pos.x > widthWin && vel.x > 0) {
		point = 1;
		isSpawn = true;
		sound.play();
	}
	if (pos.x + radius*2 < 0 && vel.x < 0) {
		point = -1;
		isSpawn = true;
		sound.play();
	}
	sphere.setPosition(pos.x, pos.y);
	return sphere;
}

//gives the ball a random y velocity, random x direction and a random "delay" timer
double Ball::startRound() {
	pos.x = xposBstart;
	pos.y = yposBstart;
	vel.x = rand() % 2;
	vel.y = rand() % 2;
	if (vel.x == 0.0f) { vel.x = -1.0f; }
	if (vel.y == 0.0f) { vel.y = -1.0f; }

	vel.x = (vel.x * 400.f);

	float numY = rand() % 10;
	numY++;
	vel.y = (vel.y * 50 * numY);

	double pause = rand() % 3;
	pause++;

	return pause;
}

//checks the point on the paddle where the ball collides with it and adjusts its velocities accordingly
void Ball::bounceStuff(float xpos, float ypos, float width, float length) {
	if (vel.x > 0) { vel.x += 25.0f; }
	else { vel.x -= 25.0f; }
	vel.x = -vel.x;
	float midpoint = length / 2;
	float point = (pos.y + radius) - ypos;
	float value = (point - midpoint)/midpoint;
	float angle = sinf(value*PI/2);
	vel.y = 500.0f * angle;
}