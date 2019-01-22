#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct card {
	int value;
	string suit;
	//string state;
	card* next;
};

struct deck {
	card* head;
	int* array;
	int capacity;
	int count;
};

struct hand {
	int* array;
	//int capacity;
	//int count;
};

void first_card(deck*, int);
void new_card(deck*, int);
deck* new_deck();
deck* create_inDeck();
void add_inDeck(deck*, int);
hand* create_hand();
void play_game(deck*, int*);
void draw_card(deck*, int);
void swap_card(deck*, int);
void remove_card_in_deck(int);
int win_condition(hand*);
void delete_arrays();
void print(deck*);
void print_hand(hand*);

int* money;
bool keepPlaying;
deck* Deck;
deck* InDeck;
hand* Hand;
string input;
string input2;
int leftOver;
hand* orderedHand;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	srand(time(NULL));
	int a = 10;
	money = &a;
	keepPlaying = true;

	//creates deck and fills it with the 52 cards
	Deck = new_deck();
	for (int i = 0; i < 52; i++) { new_card(Deck, i); }
	
	InDeck = create_inDeck();
	Hand = create_hand();
	orderedHand = create_hand();
	for (int i = 0; i < 52; i++) { add_inDeck(Deck, i); }

	//keeps playing until money runs out or player quits
	while (*money > 0 && keepPlaying == true) {
		play_game(Deck, &a);
	}
	if (keepPlaying == false) { cout << endl << "Thanks for playing" << endl << endl; }
	else { cout << endl << "Game Over" << endl << endl; }
	delete_arrays();
	system("pause");
	return 0;
}

//creates first card in the deck
void first_card(deck* deck, int index) {
	card* newCard = new card;
	newCard->value = (index / 4) + 2;
	switch (index % 4) {
	case 0:
		newCard->suit = "spades";
		break;
	case 1:
		newCard->suit = "hearts";
		break;
	case 2:
		newCard->suit = "diamonds";
		break;
	case 3:
		newCard->suit = "clubs";
		break;
	}
	//newCard->state = "deck";
	newCard->next = deck->head;
	deck->head = newCard;
}

//creates a new card into the deck
void new_card(deck* deck, int index) {
	if (deck->head == nullptr) {
		first_card(deck, index);
		return;
	}

	card* temp = deck->head;
	while (temp->next != nullptr) {
		temp = temp->next;
	}

	card* newCard = new card;
	newCard->value = (index / 4) + 2;
	switch (index % 4) {
	case 0:
		newCard->suit = "spades";
		break;
	case 1:
		newCard->suit = "hearts";
		break;
	case 2:
		newCard->suit = "diamonds";
		break;
	case 3:
		newCard->suit = "clubs";
		break;
	}
	newCard->next = nullptr;
	temp->next = newCard;
}

deck* new_deck() {
	deck* list = new deck;
	list->head = nullptr;
	return list;
}

//creates the deck that the player will use
deck* create_inDeck() {
	const int INIT_CAP = 52;
	deck* list = new deck;
	list->array = new int[INIT_CAP];
	list->capacity = INIT_CAP;
	list->count = 0;
	return list;
}

//adds a card into the deck that the player will use
void add_inDeck(deck* currDeck, int index) {
	card* temp = currDeck->head;
	for (int i = 0; i < index; i++) { temp = temp->next; }
	//if (temp->state != "deck") { return; }
	if (InDeck->count == InDeck->capacity) {
		int new_capacity = InDeck->capacity * 2;
		int* new_array = new int[new_capacity];
		for (int i = 0; i < 52; i++) { new_array[i] = InDeck->array[i]; }
		delete[] InDeck->array;
		InDeck->array = new_array;
		InDeck->capacity = new_capacity;
	}
	InDeck->array[InDeck->count] = index;
	InDeck->count++;
}

hand* create_hand() {
	const int INIT_CAP = 5;
	hand* list = new hand;
	list->array = new int[INIT_CAP];
	//list->capacity = INIT_CAP;
	//list->count = 0;
	return list;
}

void play_game(deck* currDeck, int* currMoney) {
	*currMoney = *currMoney - 1;
	input = "none";
	//checks for user input and then acts accordingly
	cout << endl << "You currently have $" << *currMoney << " after paying $1 to play this round." << endl;
	cout << endl << "Type 'look' to see the deck, 'play' to start the round, 'swap' for testing purposes or 'quit' to exit." << endl << "Your response : ";
	while (input == "none") {
		getline(cin, input);
		if (input != "quit" && input != "play" && input != "swap") {
			if (input == "look"){
				print(InDeck);
				cout << endl << "Type 'look' to see the deck, 'play' to start the round, 'swap' for testing purposes or 'quit' to exit." << endl << "Your response : ";
			}
			else { cout << endl << "Sorry that is not a valid response. Try again." << endl << "Your response: "; }
			input = "none"; 
		}
	}
	input2 = "Q";
	if (input == "quit") {
		keepPlaying = false;
		return;
	}
	else if (input == "play" || input == "swap") {
		//draws new hand
		for (int i = 0; i < 5; i++) { draw_card(InDeck, i); }
		print_hand(Hand);
		//determines which cards will be replaced
		cout << endl << "Type 'look' to see the deck, the letter(s) of the card(s) you want to keep or just press enter to discard/swap all.\nYour Response: ";
		getline(cin, input2);
		while (input2.find_first_of("ABCDEabcde") == std::string::npos && input2 != "") {
			if (input2 == "look"){ 
				print(InDeck);
				print_hand(Hand);
				cout << endl << "Type 'look' to see the deck, the letter(s) of the card(s) you want to keep or just press enter to discard/swap all.\nYour Response: ";
				getline(cin, input2);
			}
			else {
				cout << "\nSorry invalid choice.\nYour Response: ";
				getline(cin, input2);
			}
		}
		leftOver = InDeck->count;
		//replaces discarded cards
		if (input == "play") {
			if (input2.find_first_of("Aa") == std::string::npos) { draw_card(InDeck, 0); }
			if (input2.find_first_of("Bb") == std::string::npos) { draw_card(InDeck, 1); }
			if (input2.find_first_of("Cc") == std::string::npos) { draw_card(InDeck, 2); }
			if (input2.find_first_of("Dd") == std::string::npos) { draw_card(InDeck, 3); }
			if (input2.find_first_of("Ee") == std::string::npos) { draw_card(InDeck, 4); }
		}
		//allows player to swap discarded cards with the desired ones
		else {
			if (input2.find_first_of("Aa") == std::string::npos) { swap_card(InDeck, 0); }
			if (input2.find_first_of("Bb") == std::string::npos) { swap_card(InDeck, 1); }
			if (input2.find_first_of("Cc") == std::string::npos) { swap_card(InDeck, 2); }
			if (input2.find_first_of("Dd") == std::string::npos) { swap_card(InDeck, 3); }
			if (input2.find_first_of("Ee") == std::string::npos) { swap_card(InDeck, 4); }
		}
		print_hand(Hand);
		*currMoney = *currMoney + win_condition(Hand);
	}
}

//draws a new card into the hand
void draw_card(deck* thisDeck, int handIndex) {
	bool copy;
	if (InDeck->count == 0) { 
		if (leftOver > 5) { leftOver = 5; }
		for (int i = 0; i < 52; i++) {
			copy = false;
			for (int j = 0; j < leftOver; j++) { if (Hand->array[j] == i) { copy = true; } }
			if (!copy) { add_inDeck(Deck, i); }
		} 
	}
	int index = rand() % InDeck->count;
	Hand->array[handIndex] = InDeck->array[index];
	remove_card_in_deck(index);
}

//allows user to swap a card
void swap_card(deck* thisDeck, int handIndex) {
	bool copy;
	string cardLetter;
	string input3;
	int inputNum;
	int index = -1;
	if (InDeck->count == 0) {
		if (leftOver > 5) { leftOver = 5; }
		for (int i = 0; i < 52; i++) {
			copy = false;
			for (int j = 0; j < leftOver; j++) { if (Hand->array[j] == i) { copy = true; } }
			if (!copy) { add_inDeck(Deck, i); }
		}
	}
	switch (handIndex) {
		case 0:
			cardLetter = "A";
			break;
		case 1:
			cardLetter = "B";
			break;
		case 2:
			cardLetter = "C";
			break;
		case 3:
			cardLetter = "D";
			break;
		case 4:
			cardLetter = "E";
			break;
	}
	cout << endl << "Please enter the index number of the new card in the deck to replace Card " << cardLetter << " with.\nYour Response: ";
	while (index < 0) {
		getline(cin, input3);
		stringstream(input3) >> inputNum;
		for (int i = 0; i < InDeck->count; i++) {
			if (InDeck->array[i] == inputNum) { index = i; }
		}
		if (index < 0) { cout << "\nSorry invalid choice.\nYour Response: "; }
	}
	Hand->array[handIndex] = InDeck->array[index];
	remove_card_in_deck(index);
}

//removes a specified card from deck; gets called immediately after card gets added/swapped into deck
void remove_card_in_deck(int index){
	if (index < 0 || index >= InDeck->count) {
		cout << "Error! out of bound" << endl;
		return;
	}
	for (int i = index; i < InDeck->count - 1; i++) { InDeck->array[i] = InDeck->array[i + 1]; }
	InDeck->count--;
}

//checks for all win conditions from parts G and N
int win_condition(hand* currHand) {
	int cloneCount;
	bool straight;
	bool flush;
	int thisIndex;
	hand* temp = currHand;

	for (int j = 0; j < 5; j++) {
		int min = 1000;
		for (int i = 0; i < 5; i++) {
			if (temp->array[i] < min) {
				min = temp->array[i];
				thisIndex = i;
			}
		}
		orderedHand->array[j] = min;
		temp->array[thisIndex] = 2000; 
	}
	//checks for royal flush
	switch (orderedHand->array[0] % 4) {
		case 0:
			if (orderedHand->array[0] == 32 && orderedHand->array[1] == 36
				&& orderedHand->array[2] == 40 && orderedHand->array[3] ==
				44 && orderedHand->array[4] == 48) {
				cout << endl << "Congratulations! You got a Royal Flush and earned $800!" << endl;
				return 800;
			}
		case 1:
			if (orderedHand->array[0] == 33 && orderedHand->array[1] == 37
				&& orderedHand->array[2] == 41 && orderedHand->array[3] ==
				45 && orderedHand->array[4] == 49) {
				cout << endl << "Congratulations! You got a Royal Flush and earned $800!" << endl;
				return 800;
			}
		case 2:
			if (orderedHand->array[0] == 34 && orderedHand->array[1] == 38
				&& orderedHand->array[2] == 42 && orderedHand->array[3] ==
				46 && orderedHand->array[4] == 50) { 
				cout << endl << "Congratulations! You got a Royal Flush and earned $800!" << endl;
				return 800; 
			}
		case 3:
			if (orderedHand->array[0] == 35 && orderedHand->array[1] == 39
				&& orderedHand->array[2] == 43 && orderedHand->array[3] ==
				47 && orderedHand->array[4] == 51) { 
				cout << endl << "Congratulations! You got a Royal Flush and earned $800!" << endl;
				return 800; 
			}
	}
	//checks for flush 
	if ((orderedHand->array[0] % 4) == (orderedHand->array[1] % 4) && (orderedHand->array[1] % 4) == (orderedHand->array[2] % 4)
		&& (orderedHand->array[2] % 4) == (orderedHand->array[3] % 4) && (orderedHand->array[3] % 4) == (orderedHand->array[4] % 4)) { flush = true; }
	else { flush = false; }
	//checks for straight
	if (((orderedHand->array[0] / 4) + 4) == ((orderedHand->array[1] / 4) + 3) && ((orderedHand->array[1] / 4) + 3) == ((orderedHand->array[2] / 4) + 2)
		&& ((orderedHand->array[2] / 4) + 2) == ((orderedHand->array[3] / 4) + 1) && ((orderedHand->array[3] / 4) + 1) == orderedHand->array[4] / 4) { straight = true; }
	else { straight = false; }
	if (straight && flush) {
		cout << endl << "Congratulations! You got a Straight Flush and earned $50!" << endl;
		return 50;
	}
	cloneCount = 0;
	for (int m = 0; m < 5; m++) {
		if (orderedHand->array[m] / 4 == orderedHand->array[2] / 4) { cloneCount++; }
	}
	//cheecks for Four of a Kind
	if (cloneCount == 4) { 
		cout << endl << "Congratulations! You got a Four of a Kind and earned $25!" << endl; 
		return 25; 
	}
	//checks for a Full House
	if (cloneCount == 3 && orderedHand->array[0] / 4 == orderedHand->array[1] / 4 && 
		orderedHand->array[3] / 4 == orderedHand->array[4] / 4) { 
		cout << endl << "Congratulations! You got a Full House and earned $9!" << endl; 
		return 9;
	}
	if (flush) { 
		cout << endl << "Congratulations! You got a Flush and earned $6!" << endl;
		return 6; 
	}
	if (straight) { 
		cout << endl << "Congratulations! You got a Straight and earned $4!" << endl; 
		return 4; 
	}
	//checks for three of a Kind
	if (cloneCount == 3) { 
		cout << endl << "Congratulations! You got a 3 of a Kind and earned $3!" << endl;
		return 3; 
	}
	//checks for Two Pair
	if (orderedHand->array[0] / 4 == orderedHand->array[1] / 4 && orderedHand->array[2] / 4 == orderedHand->array[3] / 4) { 
		cout << endl << "Congratulations! You got a Two Pair and earned $2!" << endl;
		return 2; 
	}
	if (orderedHand->array[0] / 4 == orderedHand->array[1] / 4 && orderedHand->array[3] / 4 == orderedHand->array[4] / 4) { 
		cout << endl << "Congratulations! You got a Two Pair and earned $2!" << endl; 
		return 2; 
	}
	if (orderedHand->array[1] / 4 == orderedHand->array[2] / 4 && orderedHand->array[3] / 4 == orderedHand->array[4] / 4) { 
		cout << endl << "Congratulations! You got a Two Pair and earned $2!" << endl; 
		return 2;
	}
	//checks for a Pair greater than 10
	if ((orderedHand->array[0] / 4 == orderedHand->array[1] / 4 && orderedHand->array[0] > 35) || (orderedHand->array[1] / 4
		== orderedHand->array[2] / 4 && orderedHand->array[1] > 35) || (orderedHand->array[2] / 4 == orderedHand->array[3] / 4
		&& orderedHand->array[2] > 35) || (orderedHand->array[3] / 4 == orderedHand->array[4] / 4 && orderedHand->array[3] > 35)) {
		cout << endl << "Congratulations! You got a Pair and earned $1!" << endl;
		return 1;
	}
	return 0;
}

void delete_arrays() {
	delete[] Hand->array;
	delete Hand;
	delete[] InDeck->array;
	delete InDeck;
	delete[] orderedHand->array;
	delete orderedHand;
	for (int i = 0; i < 52; i++) {
		card* n = Deck->head;
		Deck->head = n->next;
		delete n;
	}
	delete[] Deck;

}

//prints the current deck
void print(deck* thisDeck) {
	card* temp = Deck->head;
	cout << endl << "Cards in Deck:" << endl;
	for (int i = 0; i < 52; i++) { 
		for (int j = 0; j < thisDeck->count; j++) {
			if (i == thisDeck->array[j]) {
				switch (temp->value){
					case 14:
						cout << "Ace of " << temp->suit << "; Index: " << i << endl;
						break;
					case 11:
						cout << "Jack of " << temp->suit << "; Index: " << i << endl;
						break;
					case 12:
						cout << "Queen of " << temp->suit << "; Index: " << i << endl;
						break;
					case 13:
						cout << "King of " << temp->suit << "; Index: " << i << endl;
						break;
					default:
						cout << temp->value << " of " << temp->suit << "; Index: " << i << endl;
						break;
				}
				break;
			}
		}
	temp = temp->next;
	}
}

//prints player's current hand
void print_hand(hand* currHand) {
	int counter = 0;
	cout << endl << "Your Hand:" << endl;
	for (int i = 0; i < 5; i++) {
		card* temp = Deck->head;
		for (int j = 0; j < currHand->array[i]; j++) { temp = temp->next; }
		switch (i) {
		case 0:
			cout << "A: ";
			break;
		case 1:
			cout << "B: ";
			break;
		case 2:
			cout << "C: ";
			break;
		case 3:
			cout << "D: ";
			break;
		case 4:
			cout << "E: ";
			break;
		}
		switch (temp->value) {
		case 14:
			cout << "Ace of " << temp->suit;
			break;
		case 11:
			cout << "Jack of " << temp->suit;
			break;
		case 12:
			cout << "Queen of " << temp->suit;
			break;
		case 13:
			cout << "King of " << temp->suit;
			break;
		default:
			cout << temp->value << " of " << temp->suit;
			break;
		}
		if (i == 0 && input2.find_first_of("Aa") != std::string::npos) { cout << " (kept)"; counter++; }
		if (i == 1 && input2.find_first_of("Bb") != std::string::npos) { cout << " (kept)"; counter++; }
		if (i == 2 && input2.find_first_of("Cc") != std::string::npos) { cout << " (kept)"; counter++; }
		if (i == 3 && input2.find_first_of("Dd") != std::string::npos) { cout << " (kept)"; counter++; }
		if (i == 4 && input2.find_first_of("Ee") != std::string::npos) { cout << " (kept)"; counter++; }
		cout << endl;
	}
	if (counter == 0 && (input2 != "Q" && input2 != "look")) { cout << endl << "You discarded all cards" << endl; }
	else if (counter == 1) { cout << endl << "You kept 1 card" << endl; }
	else if (counter == 5) { cout << endl << "You kept all cards" << endl; }
	else if (counter == 0) { cout << ""; }
	else { cout << endl << "You kept " << counter << " cards" << endl; }
}