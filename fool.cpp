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

void setupConsole() {
	setlocale (LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

class Game {
private:
	vector<int> GoodCards(const int& strokeNumber, const int& index, vector<Card> activeCards);
	bool Beat(const vector<int>& goodCard);
	vector<Card> addCards(vector<Card> Player);
	int translate(const vector<Card>& repellent, const vector<Card>& activeCards);
	vector<Card> uniteVectors(const vector<Card>& vector_1, const vector<Card>& vector_2);
	void takeAll(const int& strokeNumber);
	vector<Card> cleanVector(vector<Card> vector);
	int Move_1(int strokeNumber);
	int Move_2(int strokeNumber);	
	vector<Card> Flip(const int& strokeNumber, const vector<Card>& crad);
	bool Check(const Card& card, const vector<Card>& Player);
	bool Check(const int& card, const vector<int>& goodCard);
	vector <Card> Deck;
	vector <Card> Player_1;
	vector <Card> Player_2;
	vector<Card> activeCards_1, activeCards_2;
	Card TrumpCard;
	map <string, int> cardPriority;
	int startHandSize;
public:
	Game(const string& CardsFile);
	void startGame();
};

void PrintVector(const vector<Card>& vector) {
	for (int i = 0; i < vector.size(); i++) {
		cout << i + 1 << ". " << vector[i].name << " " << vector[i].suit << endl;
	}
}

Game::Game(const string& CardsFile) {
	map<string, int>
	cardPriority{
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
	vector <string> suits {"черви", "буби", "крести", "пики"};
	for( const auto &suit : suits) {
		for ( const auto &card : cardPriority) {
			const auto &name = card -> first;
			Deck.push_back({.name = name, .suit = suit});
		}
	}
	
	srand(time(NULL));	
	random_shuffle(Deck.begin(), Deck.end());
}

vector<int> Game::GoodCards (const int& strokeNumber, const int& index, vector<Card> activeCards) {
	vector<Card> repellent, walking;
	if (strokeNumber % 2 == 1) {
		walking = Player_1;
		repellent = Player_2;
	}
	else {
		walking = Player_2;
		repellent = Player_1;
	}
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

bool Game::Beat(const vector<int>& goodCard) {
	if (goodCard.size() == 0) {
		cout << "мне нечем биться, беру карту" << endl;
		return false;
	}	
	else {
		return true;
	}
}

vector<Card> Game::Flip(const int& strokeNumber, const vector<Card>& activeCards) {
	vector<Card> walking;
	if (strokeNumber % 2 == 1) {
		walking = Player_1;
	}
	else {
		walking = Player_2;
	}
	vector<Card> flipCard;
	for(int j = 0; j < activeCards.size(); j++) {
		for(int i = 0; i < walking.size(); i++) {
			if(activeCards[j].name == walking[i].name && activeCards[j].suit != walking[i].suit) {
				flipCard.push_back(walking[i]);
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
	int translateCard = 100;
	for(int i = 0; i < repellent.size(); i++) {
		if (activeCards[0].name == repellent[i].name){
			translateCard = i;
			return translateCard;
		}
	}
	return translateCard;
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
	if (strokeNumber % 2 == 1) {
		for(int i = 0; i < activeCards_1.size(); i++) {
			Player_2.push_back(activeCards_1[i]);
		}
		for(int i = 0; i < activeCards_2.size(); i++) {
			Player_2.push_back(activeCards_2[i]);
		}		
	}
	else {		
		for(int i = 0; i < activeCards_1.size(); i++) {
			Player_1.push_back(activeCards_1[i]);
		}
		for(int i = 0; i < activeCards_2.size(); i++) {
			Player_1.push_back(activeCards_2[i]);
		}
	}
}

vector<Card> Game::cleanVector(vector<Card> vector){
	while(!vector.empty()){
		vector.erase(vector.begin());
	}
	return vector;
}

int Game::Move_1(int strokeNumber) {	
	vector<Card> flipCard, flipCard_1;
	vector<int> goodCard;
	vector<Card> allActiveCards;
	allActiveCards = uniteVectors(activeCards_1, activeCards_2);
	for(int i = 0; i < allActiveCards.size(); i++) {
		goodCard = GoodCards(strokeNumber, i, allActiveCards);
		if (Beat(goodCard) == false) {
			takeAll(strokeNumber);
			Player_1 = addCards(Player_1);
			--strokeNumber;
			return strokeNumber;
		}
		else {	
			cout << "карту: " << allActiveCards[i].name << " " << allActiveCards[i].suit << endl;
			cout << "отбиваю: " << Player_2[goodCard[0]].name << " " << Player_2[goodCard[0]].suit << endl;
			activeCards_2.push_back(Player_2[goodCard[0]]);
			Player_2.erase(Player_2.begin() + goodCard[0]);	
		}
	}		
	flipCard = uniteVectors(Flip(strokeNumber,  activeCards_1), Flip(strokeNumber,  activeCards_2));
	int selectedCard = 0;	
	while (flipCard.size() > 0) {
		int yes = 0;
		cout << "Вы можете подкинуть карту. если вы хотите подкинуть карту введите 1." << endl;
		cout << "если вы не хотите подкидывать карту введите 0" << endl;
		cin >> yes;
		if (yes == 1) {
			cout << "ваши карты: " << endl;
			PrintVector(Player_1);
			cout << "какую карту вы хотите подкинуть?" << endl;
			cin >> selectedCard;
			--selectedCard;
			if (Check(Player_1[selectedCard], flipCard) == true) {	
				activeCards_1.push_back(Player_1[selectedCard]);
				Player_1.erase(Player_1.begin() + selectedCard);
				goodCard = GoodCards(strokeNumber, activeCards_1.size() - 1, activeCards_1);
				if (Beat(goodCard) == false) {
					cout << "беру карты" << endl;
					takeAll(strokeNumber);
					Player_1 = addCards(Player_1);
					--strokeNumber;
					return strokeNumber;	
				}
				else {
					cout << "отбиваю: " << Player_2[goodCard[0]].name << " " << Player_2[goodCard[0]].suit << endl;
					activeCards_2.push_back(Player_2[goodCard[0]]);	
					Player_2.erase(Player_2.begin() + goodCard[0]);
					
				}
			}
			else {
				cout << "вы не можете подкинуть эту карту" << endl;
			}
		}
		else {
			break;
		}	
		flipCard = Flip(strokeNumber,  activeCards_1);
		flipCard_1 = Flip(strokeNumber,  activeCards_2);
		while (flipCard_1.size() > 0) {
			flipCard.push_back(flipCard_1[0]);
			flipCard_1.erase(flipCard_1.begin());
		}
	}
	Player_1 = addCards(Player_1);
	Player_2 = addCards(Player_2);	
	return strokeNumber;
}

int Game::Move_2(int strokeNumber) {
	vector<Card> flipCard, flipCard_1;
	vector<int> goodCard;
	vector<Card> allActiveCards;
	allActiveCards = uniteVectors(activeCards_1, activeCards_2);
	for(int i =0; i < allActiveCards.size(); i++){
		int stop = 0;
		cout << "карта №" << i + 1 <<": " << allActiveCards[i].name << " " << allActiveCards[i].suit << endl;
		cout << "если вы будете отбиваться, введите 1." << endl;
		cout << "если вы не хотите отбиваться, введите 0." << endl;
		cin >> stop;
		if (stop != 1){
			takeAll(strokeNumber);
			Player_2 = addCards(Player_2);
			--strokeNumber;
			return strokeNumber;
		}
		goodCard = GoodCards(strokeNumber, i, allActiveCards);
		if (goodCard.size() == 0) {
			cout << "вам нечем отбиваться, вы взяли карту" << endl;
			takeAll(strokeNumber);
			Player_2 = addCards(Player_2);
			--strokeNumber;
			return strokeNumber;
		}  
		else {
			 int necCard = 0;
			while(true) {
				cout << "Какой картой вы будете отбиваться?" << endl;
				cout << "ваши карты: " << endl;
				PrintVector(Player_1);
				cin >> necCard;
				--necCard;
				if (Check(necCard, goodCard) == false) {
			 		cout << "этой картой нельзя отбиться, выберите другую" << endl;
				} 
				else {
					cout << "вы отбиваетесь картой: " << Player_1[necCard].name << " " << Player_1[necCard].suit << endl;
					activeCards_1.push_back(Player_1[necCard]);
					Player_1.erase(Player_1.begin() + necCard);
					break;
				}
			}
		}
	}		
	flipCard = uniteVectors(Flip(strokeNumber,  activeCards_1), Flip(strokeNumber,  activeCards_2));
	while (!flipCard.empty()) {
		cout << "подкидываю: " << flipCard[0].name << " " << flipCard[0].suit << endl;
		activeCards_2.push_back(flipCard[0]);
		for(int i = 0; i < Player_2.size(); i++){
			if (Player_2[i].name == flipCard[0].name && Player_2[i].suit == flipCard[0].suit) {
				Player_2.erase(Player_2.begin() + i);
			}
		}
		goodCard = GoodCards(strokeNumber, activeCards_2.size() - 1, activeCards_2);
		if (goodCard.size() == 0) {
			cout << "вам нечем отбиваться, вы взяли карту" << endl;
			takeAll(strokeNumber);
			Player_2 = addCards(Player_2);
			--strokeNumber;
			return strokeNumber;
		}  
		else {
			int stop = 0;
			cout << "если вы будете отбиваться, введите 1." << endl;
			cout << "если вы не хотите отбиваться, введите 0." << endl;
			cin >> stop;
			if (stop != 1){
				takeAll(strokeNumber);
				Player_2 = addCards(Player_2);
				--strokeNumber;
				return strokeNumber;
			}
			int necCard = 0;
			 while(true) {
				cout << "Какой картой вы будете отбиваться?" << endl;
				cout << "ваши карты: " << endl;
				PrintVector(Player_1);
				cin >> necCard;
				--necCard;
				if (Check(necCard, goodCard) == false) {
			 		cout << "этой картой нельзя отбиться, выберите другую" << endl;
				} 
				else {
				 	cout << "вы отбиваетесь картой: " << Player_1[necCard].name << " " << Player_1[necCard].suit << endl;
					activeCards_1.push_back(Player_1[necCard]);
					Player_1.erase(Player_1.begin() + necCard);
					break;
			 	}
			}
		}		
		while (flipCard.size() > 0) {
			flipCard.erase(flipCard.begin());
		}
		flipCard = Flip(strokeNumber,  activeCards_1);
		flipCard_1 = Flip(strokeNumber,  activeCards_2);
		while (flipCard_1.size() > 0) {
			flipCard.push_back(flipCard_1[0]);
			flipCard_1.erase(flipCard.begin());
		}		
	}
	Player_1 = addCards(Player_1);
	Player_2 = addCards(Player_2);	
	return strokeNumber;
}

bool Game::Check(const Card& card, const vector<Card>& Player) {
	for (int i = 0; i < Player.size(); i++) {
		if(card.name == Player[i].name && card.suit == Player[i].suit) {
			return true;
		}
	}
	return false;
}

bool Game::Check(const int& card, const vector<int>& goodCard) {
	for (int i = 0; i < goodCard.size(); i++) {
		if(card == goodCard[i]) {
			return true;
		}
	}
	return false;
}

void Game::startGame() {
	startHandSize = 6;
	for (int i = 0; i < startHandSize; i++) {
		Player_1.push_back(Deck[i]);
	}
	for (int i = startHandSize; i < startHandSize + startHandSize; i++) {
		Player_2.push_back(Deck[i]);
	}     
	Deck.erase(Deck.begin(), Deck.begin() + 12); 
	TrumpCard = Deck[Deck.size() - 1];
	cout << "Козырная карта: " << TrumpCard.name << " " << TrumpCard.suit << endl;
	cout << "ваша колода - №1.." << endl;
	int strokeNumber = 1;
	while (!Player_1.empty() && !Player_2.empty()) {
		cout << "колода №1: " << endl;
		PrintVector(Player_1);
		cout << endl;
		cout << "колода №2: " << endl;
		PrintVector(Player_2);
		int sameCard = 1;
		int translateCard = 100;
		if (strokeNumber % 2 == 1) {			
			int selectedCard = 0;
			cout << "какой картой вы хотите сходить?" << endl << endl;
			cout << "ваши карты: " << endl;
			PrintVector(Player_1);
			cin >> selectedCard;
			--selectedCard;
			cout << "вы ходите картой: " << Player_1[selectedCard].name << " " << Player_1[selectedCard].suit << endl;
			activeCards_1.push_back(Player_1[selectedCard]);
			Player_1.erase(Player_1.begin() + selectedCard);
			while (sameCard < 10) {
				if(sameCard % 2 == 1){
					translateCard = translate(Player_2, activeCards_1);
				}
				else {
					translateCard = translate(Player_1, activeCards_2);
				}
				if (translateCard > 50) {
					if (sameCard % 2 == 1) {
						strokeNumber = Move_1(strokeNumber);
					}
					else {
						++strokeNumber;
						strokeNumber = Move_2(strokeNumber);
					}
					break;
				}
				else {
					if (sameCard % 2 == 1) {
						cout << "перевожу: " << Player_2[translateCard].name << " " << Player_2[translateCard].suit << endl;
						activeCards_2.push_back(Player_2[translateCard]);
						Player_2.erase(Player_2.begin() + translateCard);
					}
					else {
						int good = 0;
						cout << "вы можете перевести. если вы хотите перевести, введит 1." << endl;
						cout << "если вы не будете переводить, введите 0" << endl;
						cin >> good;
						if (good == 1){
							while(true) {
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
							++strokeNumber;
							strokeNumber = Move_2(strokeNumber);
							break;
						}
					}
				}
				++sameCard;
			}			
			cout << "Бита!" << endl;
			system("pause");
		}
		
		else { 
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
			while (sameCard < 10) {
				if(sameCard % 2 == 1){
					translateCard = translate(Player_1, activeCards_2);
				}
				else {
					translateCard = translate(Player_2, activeCards_1);
				}
				
				if (translateCard > 50) {
					if (sameCard % 2 == 1) {
						strokeNumber = Move_2(strokeNumber);
					}
					else {
						++strokeNumber;
						strokeNumber = Move_1(strokeNumber);
					}
					break;
				}
				else {		
					if(sameCard % 2 == 1) {
						int good = 0;
						cout << "вы можете перевести. если вы хотите перевести, введите 1." << endl;
						cout << "если вы не будете переводить, введите 0" << endl;
						cin >> good;
						if (good == 1){
							while(true) {
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
							strokeNumber = Move_2(strokeNumber);
							break;
						}
					}
					else  {
						cout << "перевожу: " << Player_2[translateCard].name << " " << Player_2[translateCard].suit << endl;
						activeCards_2.push_back(Player_2[translateCard]);
						Player_2.erase(Player_2.begin() + translateCard);
					}
				}
				++sameCard;
			}
			cout << "Бита!" << endl;  
			system("pause");     
		}
		++strokeNumber;
		activeCards_1 = cleanVector(activeCards_1);
		activeCards_2 = cleanVector(activeCards_2);
	}
	cout << "игра закончена, победил игрок № " << (Player_1.empty() ? 1 : 2) << endl;
}

int main () {
	setupConsole();
	string cardsFile = "cards.txt";
	Game game(cardsFile);
	game.startGame();
	system("pause");
	return 0;
}
