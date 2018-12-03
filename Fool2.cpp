#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <ctime>
#include <map>

using namespace std;

struct Card {
	string name;
	string suit;
};

struct Text {
	string Player_1;
	string Player_2;
};

void PrintVector(const vector<Card> &vector);

void setupConsole() {
	setlocale (LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

class Cards {
public:
	Cards();
protected:
	map<string, int> cardPriority;
	Card TrumpCard;
	vector <Card> Deck;
	int strokeNumber;
};

class Player: protected Cards {
public:
	Player();
	vector <Card> player_cards;	
	vector <Card> activeCards;	
};

class StartGame: public Player {
public:
	StartGame() {};
	Player player_1;
	Player player_2;
	void Start();
	
};

int main () {
	setupConsole();
	StartGame game;
	game.Start();
	system("pause");
	return 0;
}

Cards::Cards() {	
	cout << "11111" << endl;
	map<string, int>
	cardsPriority{
		{"6", 6},
		{"7", 7},
		{"8", 8},
		{"9", 9},
		{"10", 10},
		{"валет", 11},
		{"дама", 12},
		{"король", 13},
		{"туз", 14}
	};
	
	cardPriority = cardsPriority;

	vector <string> suits {"черви", "буби", "крести", "пики"};
	for( const auto &suit : suits) {
		for ( const auto &card : cardPriority) {
			const auto &name = card.first;
			Deck.push_back({.name = name, .suit = suit});
		}
	}
	
	srand(time(NULL));	
	random_shuffle(Deck.begin(), Deck.end());
	TrumpCard = Deck[Deck.size() - 1];
}

Player::Player() {
	for (int i = 0; i < 6; i++) {
		player_cards.push_back(Deck[i]);
	}
	Deck.erase(Deck.begin(), Deck.begin() + 6);
}

void StartGame::Start() {
	
	cout << "карты игрока №1" << endl;
	PrintVector(player_1.player_cards);
	cout << "карты игрока №2" << endl;
	PrintVector(player_2.player_cards);
	cout << "козырная карта - " << TrumpCard.name << " " << TrumpCard.suit  << endl;
	
}

void PrintVector(const vector<Card> &vector) {
	for (int i = 0; i < vector.size(); i++) {
		cout << i + 1 << ". " << vector[i].name << " " << vector[i].suit << endl;
	}
}

