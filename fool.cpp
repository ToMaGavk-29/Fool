
#include <iostream>      // Это стандартная карточная игра "Дурак" для одного человека,
#include <fstream>       // который играет против "компьютера".
#include <string>        // Уже готово: перевод хода, подкидывание карт.
#include <vector>        // Но к сожалению ходы компьютера пока что не доведены до ума (практически рандомные).
#include <windows.h>     // В будующем это будет исправлено.
#include <algorithm>     // Сам код сырой, корявый и очень непонятный(я сам иногда путался в нем)
#include <ctime>        
#include <map>
using namespace std;

struct Card {
	string name;
	string suit;
};
					
struct Text {
	string walking;
	string repellent;
};

void setupConsole() {
	setlocale (LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

class Game {
public:
	Game();
	void start();
private:
	private:	
	const int startHandSize = 6;
	vector <Card> Deck;
	vector <Card> Player_1;
	vector <Card> Player_2;
	vector<Card> activeCards_1, activeCards_2;
	Card TrumpCard;
	map <string, int> cardPriority;
	Text replicas[13] = {
		{"Какой картой вы будете отбиваться?", "отбиваю: "},
		{"этой картой нельзя отбиться, выберите другую ", ""},
		{"вы отбиваетесь картой: ", ""},
		{"какую карту вы будете подкидывать?", "подкидываю"},
		{"эту карту нельзя подкинуть, выбертье другую карту", ""},
		{"вы подкинули карту: ", ""},
		{"переводить", "прервожу: "},
		{"перевести", ""},
		{"переводите", ""},
		{"если вы будете отбиваться", ""},
		{"если вы не хотите отбиваться", ""},
		{"если вы будете подкидывать карту", ""},
		{"если вы не хотите подкидывать карту", ""}
	};
	
	int move_1(int strokeNumber);
	int move_2(int strokeNumber);
	
	vector<int> goodCards(const int &strokeNumber, const int &index, vector<Card> activeCards);	
	int translate(const vector<Card> &repellent, const vector<Card> &activeCards);
	void selectionCard_1();
	void selectionCard_2();
	bool canPlayerTakeCards();
	void whichCardPlayerFighting(const int &strokeNumber, const vector<int> &cards);
	
	vector<Card> addCards(vector<Card> Player);
	vector<Card> uniteVectors(const vector<Card> &vector_1, const vector<Card> &vector_2);
	void takeAll(const int& strokeNumber);
	
	vector<int> canPlayerFlip(const int &strokeNumber, const vector<Card> &crad);
	bool canPlayerBeat(const vector<int> &goodCard);
	bool isCardInSet(const Card &card, const vector<Card> &cards);
	bool isCardInSet(const int &card, const vector<int> &cards);
};

void PrintVector(const vector<Card> &vector) {
	for (int i = 0; i < vector.size(); i++) {
		cout << i + 1 << ". " << vector[i].name << " " << vector[i].suit << endl;
	}
}

Game::Game() {	
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
}

vector<int> Game::goodCards (const int& strokeNumber, const int& index, vector<Card> activeCards) {
	vector<Card> const &walking = strokeNumber % 2 == 1 ? Player_1 : Player_2;
	vector<Card> const &repellent = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	vector<int> goodCard;
	for(int i = 0; i < repellent.size(); i++) {
		if (activeCards[index].suit == TrumpCard.suit && repellent[i].suit == TrumpCard.suit 
		&& cardPriority[repellent[i].name] > cardPriority[activeCards[index].name] ) {
			goodCard.push_back(i); 
		}
		else if (activeCards[index].suit != TrumpCard.suit &&  repellent[i].suit != TrumpCard.suit 
		&& repellent[i].suit == activeCards[index].suit 
		&& cardPriority[repellent[i].name] > cardPriority[activeCards[index].name] ) {
			goodCard.push_back(i); 
		}
		else if (activeCards[index].suit != TrumpCard.suit && repellent[i].suit == TrumpCard.suit  ) {
			goodCard.push_back(i); 
		}
	}
	for (int i = 0; i < goodCard.size(); i++) {
		cout << goodCard[i] + 1 << endl;
	}
	return goodCard;
}

bool Game::canPlayerBeat(const vector<int>& goodCard) {
	if (goodCard.empty()) {
		cout << "нечем биться, беру карту" << endl;
		return false;
	}	
	else {
		return true;
	}
}

vector<int> Game::canPlayerFlip(const int &strokeNumber, const vector<Card> &activeCards) {
	const vector<Card> &walking = strokeNumber % 2 == 1 ? Player_1 : Player_2;
	vector<int> flipCard;
	for(int j = 0; j < activeCards.size(); j++) {
		for(int i = 0; i < walking.size(); i++) {
			if(activeCards[j].name == walking[i].name) {
				flipCard.push_back(i);
			}
		}
	}
	return flipCard;
}

vector<Card> Game::addCards(vector<Card> Player) {
		while(Player.size() < startHandSize && Deck.size() > 0) {
			Player.push_back(Deck[0]);
			Deck.erase(Deck.begin());
		}
		cout << "все карты взяты" << endl;
	return Player;
}

int Game::translate(const vector<Card>& repellent, const vector<Card>& activeCards) {
	int cardIsAbsent = 1000;
	for(int i = 0; i < repellent.size(); i++) {
		if (activeCards[0].name == repellent[i].name){
			return i;
		}
	}
	return cardIsAbsent;
}

void Game::selectionCard_1(){
	int selectedCard = 0;
	cout << "какой картой вы хотите сходить?" << endl << endl;
	cout << "ваши карты: " << endl;
	PrintVector(Player_1);
	cin >> selectedCard;
	--selectedCard;
	cout << "вы ходите картой: " << Player_1[selectedCard].name << " " << Player_1[selectedCard].suit << endl;
	activeCards_1.push_back(Player_1[selectedCard]);
	Player_1.erase(Player_1.begin() + selectedCard);
}

void Game::selectionCard_2(){
	int selectedCard = 0;
	for(int i = 1; i < Player_2.size(); i++) {
		if (Player_2[i - 1].suit != TrumpCard.suit && cardPriority[Player_2[i - 1].name] < cardPriority[Player_2[i].name] ) {
			selectedCard = i - 1;
		}
		else if (Player_2[i - 1].suit != TrumpCard.suit && cardPriority[Player_2[i - 1].name] > cardPriority[Player_2[i].name] ) {
			selectedCard = i;
		}
		else {
			selectedCard = i;
		}	
	}
	cout << "Хожу: " << Player_2[selectedCard].name << " " << Player_2[selectedCard].suit << endl;
	activeCards_2.push_back(Player_2[selectedCard]);
	Player_2.erase(Player_2.begin() + selectedCard);	
}

bool Game::canPlayerTakeCards(int replicaIndex){
	int stop;
	cout << replicas[replicaIndex].Player_1 << ", введите 1." << endl;
	cout << replicas[replicaIndex + 1].Player_1 <<", введите 0." << endl;
	cin >> stop;
	if(stop == 1){
		return true;
	}
	return false;
}

void Game::whichCardPlayerFighting(const int &strokeNumber, const vector<int> &cards, int replicaIndex){
	if(strokeNumber % 2 == 1){
		cout << replicas[replicaIndex].Player_2 << Player_2[cards[0]].name << " " << Player_2[cards[0]].suit << endl;
		activeCards_2.push_back(Player_2[cards[0]]);
		Player_2.erase(Player_2.begin() + cards[0]);
		return;
	}
	
	while(true) {
		int necCard = 0;
		cout << replicas[replicaIndex].Player_1 << endl;
		cout << "ваши карты: " << endl;
		PrintVector(Player_1);
		cin >> necCard;
		--necCard;
		if (isCardInSet(necCard, cards) == false) {
	 		cout << replicas[replicaIndex + 1].Player_1 << endl;
		} 
		else {
			cout << replicas[replicaIndex + 2].Player_1 << Player_1[necCard].name << " " << Player_1[necCard].suit << endl;
			activeCards_1.push_back(Player_1[necCard]);
			Player_1.erase(Player_1.begin() + necCard);
			break;
		}
	}
	return;
}

vector<Card> Game::uniteVectors(const vector<Card>& vector_1, const vector<Card>& vector_2){
	vector<Card> newVector;
	for(int i = 0; i < vector_1.size(); i++){
		newVector.push_back(vector_1[i]);
	}
	for(int i = 0; i < vector_2.size(); i++){
		newVector.push_back(vector_2[i]);
	}
	return newVector;
}

void Game::takeAll(const int& strokeNumber) {
	auto &activePlayer = strokeNumber % 2 == 1 ? Player_2 : Player_1;
		for(int i = 0; i < activeCards_1.size(); i++) {
			activePlayer.push_back(activeCards_1[i]);
		}
		for(int i = 0; i < activeCards_2.size(); i++) {
			activePlayer.push_back(activeCards_2[i]);
		}			
}

int Game::move_1(int strokeNumber) {	
	vector<int> flipCard;
	vector<int> goodCard;
	vector<Card> allActiveCards;
	
	vector<Card> &walking = strokeNumber % 2 == 1 ? Player_1 : Player_2;
	vector<Card> &repellent = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	
	allActiveCards = uniteVectors(activeCards_1, activeCards_2);
	cout << "карты, которые нужно отбить: " << endl;
	PrintVector(allActiveCards);
	
	bool stop = strokeNumber % 2 == 1 ? true : canPlayerTakeCards();	
	if (stop == false){
		takeAll(strokeNumber);
		walking = addCards(walking);
		--strokeNumber;
		return strokeNumber;
	}
	
	for(int i = 0; i < allActiveCards.size(); i++) {
		goodCard = goodCards(strokeNumber, i, allActiveCards);
		if (canPlayerBeat(goodCard) == false) {
			takeAll(strokeNumber);
			walking = addCards(walking);
			--strokeNumber;
			return strokeNumber;
		}
		cout << "карта №" << i + 1 <<": " << allActiveCards[i].name << " " << allActiveCards[i].suit << endl;
		whichCardPlayerFighting(strokeNumber, goodCard);
	}
			
	flipCard = canPlayerFlip(strokeNumber, allActiveCards);	
	while (flipCard.size() > 0) {
		stop = strokeNumber % 2 == 1 ? canPlayerTakeCards(11) : true;
		if (stop == false){
			break;
		}
		
		cout << "подкинуть ..." << endl;
		
		whichCardPlayerFighting(strokeNumber + 1, flipCard, 3);
		
		bool stop = strokeNumber % 2 == 1 ? true : canPlayerTakeCards(9);	
		if (stop == false){
			takeAll(strokeNumber);
			walking = addCards(walking);
			--strokeNumber;
			return strokeNumber;
		}
		vector<Card> &activeCards = strokeNumber % 2 == 1 ? activeCards_1 :  activeCards_2;
		goodCard = goodCards(strokeNumber, activeCards.size() - 1, activeCards); 
		if (canPlayerBeat(goodCard) == false) {
			cout << "беру карты" << endl;
			takeAll(strokeNumber);
			walking = addCards(walking);
			--strokeNumber;
			return strokeNumber;	
		}
		
		whichCardPlayerFighting(strokeNumber, goodCard, 0);		
		flipCard.clear();
		flipCard = canPlayerFlip(strokeNumber, allActiveCards);
	}
	Player_1 = addCards(Player_1);
	Player_2 = addCards(Player_2);	
	return strokeNumber;
}

bool Game::isCardInSet(const Card& card, const vector<Card>& cards) {
	for (int i = 0; i < cards.size(); i++) {
		if(card.name == cards[i].name && card.suit == cards[i].suit) {
			return true;
		}
	}
	return false;
}

bool Game::isCardInSet(const int& card, const vector<int>& cards) {
	for (int i = 0; i < cards.size(); i++) {
		if(card == cards[i]) {
			return true;
		}
	}
	return false;
}

void Game::start() { 
	for (int i = 0; i < startHandSize; i++) {
		Player_1.push_back(Deck[i]);
	}
	for (int i = startHandSize; i < startHandSize * 2; i++) {
		Player_2.push_back(Deck[i]);
	}     
	Deck.erase(Deck.begin(), Deck.begin() + 12);                                     
	TrumpCard = Deck[Deck.size() - 1];
	cout << "Козырная карта: " << TrumpCard.name << " " << TrumpCard.suit << endl;
	cout << "ваша колода - №1." << endl;
	
	int strokeNumber = 1;
	while (!Player_1.empty() && !Player_2.empty()) {
		cout << "колода №1: " << endl;
		PrintVector(Player_1);
		cout << endl;
		cout << "колода №2: " << endl;
		PrintVector(Player_2);
		int translateCard;
		const int cardIsAbsent = 1000;
				
		strokeNumber % 2 == 1 ? selectionCard_1() : selectionCard_2();
		while (true) {
			if(strokeNumber % 2 == 1){
				translateCard = translate(Player_2, activeCards_1);
			}
			else {
				translateCard = translate(Player_1, activeCards_2);
			}
			if (translateCard == cardIsAbsent) {
				strokeNumber = move_1(strokeNumber);	
				break;
			}
			else {
				if (strokeNumber % 2 == 1) {
					cout << "перевожу: " << Player_2[translateCard].name << " " << Player_2[translateCard].suit << endl;
					activeCards_2.push_back(Player_2[translateCard]);
					Player_2.erase(Player_2.begin() + translateCard);
				}
				else {
					int good = 0;
					cout << "вы можете перевести. если вы хотите перевести, введите 1." << endl;
					cout << "если вы не будете переводить, введите 0" << endl;
					cin >> good;
					if (good == 1){
						while(true) {
							int selectedCard = 0;
							cout << "какой картой вы будете переводить?" << endl << endl;
							cout << "ваши карты: " << endl;
							PrintVector(Player_1);
							cin >> selectedCard;
							--selectedCard;
							if (Player_1[selectedCard].name == activeCards_2[0].name) {
								cout << "вы переводите картой: " << Player_1[translateCard].name << " " << Player_1[translateCard].suit << endl;
								activeCards_1.push_back(Player_1[translateCard]);
								Player_1.erase(Player_1.begin() + translateCard); 
								break;
							}
							else{
								cout << "этой картой нельзя переводить, выберите другую карту" << endl;	
							}
						}	
					}
					else{
						strokeNumber = move_1(strokeNumber);
						break;
					}
				}
			}
			++strokeNumber;
		}			
		cout << "Бита!" << endl;
		system("pause");		
		++strokeNumber;
		activeCards_1.clear();
		activeCards_2.clear();
	}
	cout << "игра закончена, победил игрок № " << (Player_1.empty() ? 1 : 2) << endl;
}

int main () {
	setupConsole();
	Game game;
	game.start();
	system("pause");
	return 0;
}			
						
