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

bool isCardInSet(const Card &card, const vector<Card> &cards) {
	for (int i = 0; i < cards.size(); i++) {
		if(card.name == cards[i].name && card.suit == cards[i].suit) {
			return true;
		}
	}
	return false;
}

bool isCardInSet(const int &card, const vector<int> &cards) {
	for (int i = 0; i < cards.size(); i++) {
		if(card == cards[i]) {
			return true;
		}
	}
	return false;
}

vector<Card> uniteVectors(const vector<Card> &vector_1, const vector<Card> &vector_2){
	vector<Card> newVector;
	for(int i = 0; i < vector_1.size(); i++){
		newVector.push_back(vector_1[i]);
	}
	for(int i = 0; i < vector_2.size(); i++){
		newVector.push_back(vector_2[i]);
	}
	return newVector;
}

class Game {
public:
	Game();
	void start();
	
private:	
	const int startHandSize = 6;
	vector <Card> Deck;
	vector <Card> Player_1;
	vector <Card> Player_2;
	vector<Card> activeCards_1, activeCards_2;
	Card TrumpCard;
	map <string, int> cardPriority;
	Text commonReplicas[9] = {
		{"Какой картой вы будете отбиваться?", "отбиваю: "},
		{"этой картой нельзя отбиться, выберите другую ", ""},
		{"вы отбиваетесь картой: ", ""},
		{"какую карту вы будете подкидывать?", "подкидываю: "},
		{"эту карту нельзя подкинуть, выбертье другую карту", ""},
		{"вы подкинули карту: ", ""},
		{"какой картой вы будете переводить?", "прервожу: "},
		{"этой картой нельзя перевести, выберите другую ", ""},
		{"вы переводите картой: ", ""}
	};
	string replicasForPlayer_1[6] = {
		{"если вы будете отбиваться"},
		{"если вы не хотите отбиваться"},
		{"если вы будете подкидывать карту"},
		{"если вы не хотите подкидывать карту"},
		{"если вы будете переводить"},
		{"если вы не хотите переводить"}
	};
	
	int move(int strokeNumber);
	
	vector<int> goodCards(const int &strokeNumber, const int &index, const vector<Card> &activeCards);	
	void whichCardPlayerFighting(const int &strokeNumber, const vector<int> &cards, int replicaIndex);
	void selectionCard(const int &strokeNumber);
	
	int translate(const int& strokeNumber);
	bool canPlayerTakeCards(int replicaIndex);
	vector<int> canPlayerFlip(const int &strokeNumber, const vector<Card> &crad);
	
	void addCards(vector<Card> &Player);
	void takeAll(const int& strokeNumber);
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
		selectionCard(strokeNumber);
		while (true) {
			translateCard = translate(strokeNumber);
			if(translateCard == cardIsAbsent) {
				strokeNumber = move(strokeNumber);	
				break;
			}
			bool stop = strokeNumber % 2 == 1 ? true : canPlayerTakeCards(4);
			if(stop == false) {
				strokeNumber = move(strokeNumber);
				break;
			}
			vector<int> goodCard;
			goodCard.push_back(translateCard);	
			whichCardPlayerFighting(strokeNumber, goodCard, 6);
			++strokeNumber;
		}					
		cout << "бита!" << endl;
		system("pause");		
		++strokeNumber;
		activeCards_1.clear();
		activeCards_2.clear();
	}
	cout << "игра закончена, победил игрок № " << (Player_1.empty() ? 1 : 2) << endl;
}

int Game::move(int strokeNumber) {	
	vector<int> flipCard;
	vector<int> goodCard;
	vector<Card> allActiveCards;
	
	vector<Card> &walking = strokeNumber % 2 == 1 ? Player_1 : Player_2;
	vector<Card> &repellent = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	
	allActiveCards = uniteVectors(activeCards_1, activeCards_2);
	cout << "карты, которые нужно отбить: " << endl;
	PrintVector(allActiveCards);
	
	bool stop = strokeNumber % 2 == 1 ? true : canPlayerTakeCards(0);	
	if (stop == false){
		takeAll(strokeNumber);
		addCards(walking);
		--strokeNumber;
		return strokeNumber;
	}
	
	for(int i = 0; i < allActiveCards.size(); i++) {
		goodCard = goodCards(strokeNumber, i, allActiveCards);
		if (goodCard.empty()) {
			cout << "нечем биться, беру карты" << endl;
			takeAll(strokeNumber);
			addCards(walking);
			--strokeNumber;
			return strokeNumber;
		}
		cout << "карта №" << i + 1 <<": " << allActiveCards[i].name << " " << allActiveCards[i].suit << endl;
		whichCardPlayerFighting(strokeNumber, goodCard, 0);
	}
	
	allActiveCards = uniteVectors(activeCards_1, activeCards_2);
	flipCard = canPlayerFlip(strokeNumber, allActiveCards);	
	while (flipCard.size() > 0) {
		stop = strokeNumber % 2 == 1 ? canPlayerTakeCards(2) : true;
		if (stop == false){
			break;
		} 
		whichCardPlayerFighting(strokeNumber + 1, flipCard, 3);
		bool stop = strokeNumber % 2 == 1 ? true : canPlayerTakeCards(0);	
		if (stop == false){
			takeAll(strokeNumber);
			addCards(walking);
			--strokeNumber;
			return strokeNumber;
		}
		vector<Card> &activeCards = strokeNumber % 2 == 1 ? activeCards_1 :  activeCards_2;
		goodCard = goodCards(strokeNumber, activeCards.size() - 1, activeCards); 
		if (goodCard.empty()) {
			cout << "нечем биться, беру карты" << endl;
			takeAll(strokeNumber);
			addCards(walking);
			--strokeNumber;
			return strokeNumber;	
		}
		whichCardPlayerFighting(strokeNumber, goodCard, 0);	
		allActiveCards = uniteVectors(activeCards_1, activeCards_2);	
		flipCard.clear();
		flipCard = canPlayerFlip(strokeNumber, allActiveCards);
	}
	addCards(Player_1);
	addCards(Player_2);	
	return strokeNumber; 
}

vector<int> Game::goodCards(const int &strokeNumber, const int &index, const vector<Card> &activeCards) {
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

void Game::whichCardPlayerFighting(const int &strokeNumber, const vector<int> &cards, const int replicaIndex){
	if(strokeNumber % 2 == 1){
		cout << commonReplicas[replicaIndex].Player_2 << Player_2[cards[0]].name << " " << Player_2[cards[0]].suit << endl;
		activeCards_2.push_back(Player_2[cards[0]]);
		Player_2.erase(Player_2.begin() + cards[0]);
		return;
	}
	
	while(true) {
		int necCard = 0;
		cout << commonReplicas[replicaIndex].Player_1 << endl;
		cout << "ваши карты: " << endl;
		PrintVector(Player_1);
		cin >> necCard;
		--necCard;
		if (isCardInSet(necCard, cards) == false) {
	 		cout << commonReplicas[replicaIndex + 1].Player_1 << endl;
		} 
		else {
			cout << commonReplicas[replicaIndex + 2].Player_1 << Player_1[necCard].name << " " << Player_1[necCard].suit << endl;
			activeCards_1.push_back(Player_1[necCard]);
			Player_1.erase(Player_1.begin() + necCard);
			break;
		}
	}
	return;
}

void Game::selectionCard(const int &strokeNumber) {
	int selectedCard = 1000;
	if(strokeNumber % 2 == 1) {
		cout << "какой картой вы хотите сходить?" << endl << endl;
		cout << "ваши карты: " << endl;
		PrintVector(Player_1);
		cin >> selectedCard;
		--selectedCard;
		cout << "вы ходите картой: " << Player_1[selectedCard].name << " " << Player_1[selectedCard].suit << endl;
		activeCards_1.push_back(Player_1[selectedCard]);
		Player_1.erase(Player_1.begin() + selectedCard);
		return;
	}
	for(int i = 0; i < Player_2.size(); i++) {
		if(selectedCard == 1000) {
			selectedCard = i;
		}	else if(Player_2[i].suit != TrumpCard.suit && cardPriority[Player_2[i].name] < cardPriority[Player_2[selectedCard].name]) {
			selectedCard = i;
		}
	}
	cout << "Хожу: " << Player_2[selectedCard].name << " " << Player_2[selectedCard].suit << endl;
	activeCards_2.push_back(Player_2[selectedCard]);
	Player_2.erase(Player_2.begin() + selectedCard);	
}

int Game::translate(const int &strokeNumber) {
	const vector<Card> &repellent = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	vector<Card> activeCards;
	activeCards = uniteVectors(activeCards_1, activeCards_2);
	int cardIsAbsent = 1000;	
	for(int i = 0; i < repellent.size(); i++) {
		if (activeCards[0].name == repellent[i].name){
			return i;
		}
	}
	return cardIsAbsent;
}

bool Game::canPlayerTakeCards(const int replicaIndex){
	int stop;
	cout << replicasForPlayer_1[replicaIndex] << ", введите 1." << endl;
	cout << replicasForPlayer_1[replicaIndex + 1] <<", введите 0." << endl;
	cin >> stop;
	if(stop == 1){
		return true;
	}
	return false;
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

void Game::addCards(vector<Card> &Player) {
	while(Player.size() < startHandSize && Deck.size() > 0) {
		Player.push_back(Deck[0]);
		Deck.erase(Deck.begin());
	}
	cout << "все карты взяты" << endl;
}

void Game::takeAll(const int &strokeNumber) {
	auto &activePlayer = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	for(int i = 0; i < activeCards_1.size(); i++) {
		activePlayer.push_back(activeCards_1[i]);
	}
	for(int i = 0; i < activeCards_2.size(); i++) {
		activePlayer.push_back(activeCards_2[i]);
	}			
}

int main () {
	setupConsole();
	Game game;
	game.start();
	system("pause");
	return 0;
}
