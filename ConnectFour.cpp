#include <iostream>
#include <string>
#include <sstream>

using namespace std;

char whoseTurn(int);
void makeBoard(bool);
void makeMove(char);
char checkWinner(char);
bool checkLegal(int);
bool checkRemove(int, char);
char HorizontalCheck(int, int, char);
char VerticalCheck(int, int, char);
char DiagonalCheck(int, int, char);
char DiagonalCheck2(int, int, char);

char field[7][7];
char winner;
char winner2;
bool wrap;
bool rem;

int main()
{
	//initial values to start the game
	winner = 'A';
	int turn = 0;
	string playAgain = "";
	string mode = "";
	string mode2 = "";
	wrap = false;
	rem = false;

	cout << "Hello, welcome to Connect Four!" << endl;

	//will check to see if player wants to include wrap option and will proceed once the player complies
	cout << endl << "Would you like to play classic or wrap around Connect Four? (type 'c' for classic and 'w' for wrap mode)." << endl << "Your response : ";
	while (mode == "") {
		getline(cin, mode);
		if (mode == "c" || mode == "C") { cout << endl << "Great, we'll play classic Connect Four" << endl; }
		else if (mode == "w" || mode == "W") {
			cout << endl << "Great, we'll play the wrap around version of Connect Four" << endl;
			wrap = true;
		}
		else {
			cout << endl << "Sorry that response is invalid. Try again." << endl << "Your response: ";
			mode = "";
		}
	}

	//will check to see if player wants to include the remove option and will proceed once the player complies
	cout << endl << "Additionally, would you like to include the option to remove your bottom pieces? (y/n)." << endl << "Your response: ";
	while (mode2 == "") {
		getline(cin, mode2);
		if (mode2 == "y" || mode2 == "Y") {
			cout << endl << "Great, we'll include the remove piece option." << endl;
			rem = true;
		}
		else if (mode2 == "n" || mode2 == "N") { cout << endl << "Great, we'll play without the option to remove pieces." << endl; }
		else {
			cout << endl << "Sorry that response is invalid. Try again." << endl << "Your response: ";
			mode2 = "";
		}
	}

	//The game will alternate between the 2 players' moves and constantly check for win conditions for both
	while (winner == 'A') {
		turn++;
		char player = whoseTurn(turn);
		char otherPlayer = whoseTurn(turn + 1);
		makeBoard(false);
		if (!rem) { cout << "Player " << player << " type the column number where you would like to place your piece." << endl << "Your choice: "; }
		else { cout << "Player " << player << " type the column number where you would like to place your piece or the negative column number to remove it." << endl << "Your choice: "; }
		makeMove(player);
		winner = checkWinner(player);
		winner2 = checkWinner(otherPlayer);
	}

	//once someone wins, it will print out the appropriate congratulations message along with the option to play again
	makeBoard(false);
	if ((winner == 'X' && winner2 == 'O') || (winner2 == 'X' && winner == 'O')) { cout << "congratulations both players, you both win!!" << endl; }
	else if (winner == 'X' || winner == 'O') { cout << "congratulations Player " << winner << ", you've won!" << endl; }
	else { cout << "This game has ended in a Draw..." << endl; }

	cout << endl << "Would you like to play again? (y/n)" << endl << "Your choice: ";
	while (playAgain == "") {
		getline(cin, playAgain);
		if (playAgain == "y" || playAgain == "Y") {
			makeBoard(true);
			main();
		}
		else if (playAgain == "n" || playAgain == "N") { return 0; }
		else {
			playAgain = "";
			cout << endl << "Sorry that response is invalid. Try again." << endl << "Your choice: ";
		}
	}
}

//returns the player whose turn it is
char whoseTurn(int numTurn) {
	if (numTurn % 2 == 1) { return 'X'; }
	else { return 'O'; }
}

//draws out the board and updates constantly
void makeBoard(bool restart) {

	cout << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			if (i == 0) { cout << j + 1; }
			else if (restart) {
				field[i][j] = 'A';
				cout << 'L';
			}
			else if (field[i][j] == 'X') { cout << 'X'; }
			else if (field[i][j] == 'O') { cout << 'O'; }
			else { cout << 'L'; }
		}
		cout << endl;
	}
	cout << endl;
}

//makes move for the current player
void makeMove(char currPlayer) {

	bool isLegal = false;
	bool isPoss = false;
	bool isPoss2 = false;
	string input = "";
	int numColumn = 0;

	int legalMoves[7];
	int legalRemoves[7];

	//stores all the columns where the player can drop a piece in LegalMoves
	for (int i = 0; i < 7; i++) {
		isLegal = checkLegal(i);
		if (isLegal) { legalMoves[i] = i + 1; }
		else { legalMoves[i] = 0; }
	}
	//stores all the columns where the player can remove their bottom piece in LegalRemoves
	if (rem) {
		for (int i = 0; i < 7; i++) {
			isLegal = checkRemove(i, currPlayer);
			if (isLegal) { legalRemoves[i] = (i + 1)*-1; }
			else { legalRemoves[i] = 0; }
		}
	}

	//checks for the player's input (move) and then acts accordingly
	while (!isPoss && !isPoss2) {
		getline(cin, input);
		stringstream myStream(input);
		myStream >> numColumn;

		if (numColumn > 0) {
			for (int i = 0; i < 7; i++) {
				if (legalMoves[i] == numColumn) { isPoss = true; }
			}
		}
		else if (numColumn < 0 && rem) {
			for (int i = 0; i < 7; i++) {
				if (legalRemoves[i] == numColumn) { isPoss2 = true; }
			}
		}
		if (!isPoss && !isPoss2) { cout << endl << "Sorry that choice is invalid. Try Again." << endl << "Your choice: "; }
		//drops a piece to a column
		else if (isPoss) {
			numColumn--;
			for (int i = 6; i >= 0; i--) {
				if ((field[i][numColumn] != 'X') && (field[i][numColumn] != 'O')) {
					field[i][numColumn] = currPlayer;
					break;
				}
			}
		}
		//removes the bottom piece from a column
		else if (isPoss2) {
			numColumn++;
			numColumn = numColumn * -1;
			for (int i = 6; i > 0; i--) {
				if (i == 1) { field[i][numColumn] = 'Q'; }
				field[i][numColumn] = field[i - 1][numColumn];
			}
		}
	}
}

//calls to check for a connect four for the current player
char checkWinner(char currPlayer) {
	char win;
	int boardCounter = 0;
	for (int i = 1; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			if (field[i][j] == currPlayer) {
				win = HorizontalCheck(i, j, currPlayer);
				if (win == currPlayer) { return currPlayer; }
				win = VerticalCheck(i, j, currPlayer);
				if (win == currPlayer) { return currPlayer; }
				win = DiagonalCheck(i, j, currPlayer);
				if (win == currPlayer) { return currPlayer; }
				win = DiagonalCheck2(i, j, currPlayer);
				if (win == currPlayer) { return currPlayer; }
			}
			if ((field[i][j] != 'X') && (field[i][j] != 'O')) { boardCounter++; }
		}
	}
	if (boardCounter == 0) { return 'Z'; }
	else { return 'A'; }
}

//helper function for LegalMoves
bool checkLegal(int Column) {
	for (int i = 1; i < 7; i++) {
		if ((field[i][Column] != 'X') && (field[i][Column] != 'O')) { return true; }
	}
	return false;
}

//helper function for LegalRemoves
bool checkRemove(int Column, char player) {
	if ((field[6][Column] == player)) { return true; }
	else { return false; }
}

//checks for a horizontal four in a row and includes the wrap option
char HorizontalCheck(int x, int y, char thisPlayer) {
	if (wrap == false) {
		for (int i = 1; i <= 3; i++) {
			if (y >= 4) { return 'A'; }
			if (field[x][y + i] != field[x][y]) { return 'A'; }
		}
		return thisPlayer;
	}
	else {
		int z = y;
		for (int i = 1; i <= 3; i++) {
			if (y + i > 6) { y = y - 7; }
			if (field[x][y + i] != field[x][z]) { return 'A'; }
		}
		return thisPlayer;
	}
}

//checks for a vertical four in a row
char VerticalCheck(int x, int y, char thisPlayer) {
	for (int i = 1; i <= 3; i++) {
		if (x <= 3) { return 'A'; }
		if (field[x - i][y] != field[x][y]) { return 'A'; }
	}
	return thisPlayer;
}

//checks for an upwards diagonal four in a row
char DiagonalCheck(int x, int y, char thisPlayer) {
	for (int i = 1; i <= 3; i++) {
		if (x <= 3 || y >= 4) { return 'A'; }
		if (field[x - i][y + i] != field[x][y]) { return 'A'; }
	}
	return thisPlayer;
}

//checks for a downwards diagonal four in a row
char DiagonalCheck2(int x, int y, char thisPlayer) {
	for (int i = 1; i <= 3; i++) {
		if (x <= 3 || y <= 2) { return 'A'; }
		if (field[x - i][y - i] != field[x][y]) { return 'A'; }
	}
	return thisPlayer;
}