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

class Game {
private:
	vector <Card> Deck;
	vector <Card> Player_1;
	vector <Card> Player_2;
	Card TrumpCard;
	map <string, int> cardPriority;
public:
	Game(const string& CardsFile);
	void startGame();
	vector<int> GoodCards(const int& strokeNumber, const int& Index, vector<Card> activeCards);
	bool Beat(const vector<int>& goodCard);
	vector<Card> addCards(vector<Card> Player);
	void takeAll(const int& strokeNumber, vector<Card> activeCards_1, vector<Card> activeCards_2);
	int Move_1(int strokeNumbe);
	int Move_2(int strokeNumbe);	
	vector<Card> Flip(const int& strokeNumber, const vector<Card>& crad);
	bool Check(const Card& card, const vector<Card>& Player);
	bool Check(const int& card, const vector<int>& goodCard);
};

void PrintVector(const vector<Card>& vector) {
	for (int i = 0; i < vector.size(); i++) {
		cout << i + 1 << ". " << vector[i].name << " " << vector[i].suit << endl;
	}
}

Game::Game(const string& CardsFile) {
	ifstream input(CardsFile.c_str());
	Card card;
	int number = 1;
	while(!input.eof()) {
		input >> card.name;
		input >> card.suit;
		Deck.push_back(card);
		
		if (number > 9) {
			number = 1;
		}
		cardPriority[card.name] = number;
		++number;
	}
	
	srand(time(NULL));	
	random_shuffle(Deck.begin(), Deck.end());
}

vector<int> Game::GoodCards (const int& strokeNumber, const int& Index, vector<Card> activeCards) {
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
		if (activeCards[Index].suit == TrumpCard.suit && repellent[i].suit == TrumpCard.suit 
		&& cardPriority[repellent[i].name] > cardPriority[activeCards[Index].name] ) {
			goodCard.push_back(i); 
		}
		else if (activeCards[Index].suit != TrumpCard.suit &&  repellent[i].suit != TrumpCard.suit 
		&& repellent[i].suit == activeCards[Index].suit 
		&& cardPriority[repellent[i].name] > cardPriority[activeCards[Index].name] ) {
			goodCard.push_back(i); 
		}
		else if (activeCards[Index].suit != TrumpCard.suit && repellent[i].suit == TrumpCard.suit  ) {
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
		cout << "��� ����� ������, ���� �����" << endl;
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
		while(Player.size() < 6 && Deck.size() > 0) {
			Player.push_back(Deck[0]);
			Deck.erase(Deck.begin());
		}
		cout << "��� ����� �����" << endl;
	return Player;
}

void Game::takeAll(const int& strokeNumber, vector<Card> activeCards_1, vector<Card> activeCards_2) {
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

int Game::Move_1(int strokeNumber) {
	int selectedCard = 0;
	vector<Card> flipCard, flipCard_1;
	vector<Card> activeCards_1, activeCards_2;
	cout << "������ �1: " << endl;
	PrintVector(Player_1);
	cout << endl;
	cout << "������ �2: " << endl;
	PrintVector(Player_2);
	cout << "����� ������ �� ������ �������?" << endl;
	cin >> selectedCard;
	--selectedCard;
	cout << "�� ������ ������: " << Player_1[selectedCard].name << " " << Player_1[selectedCard].suit << endl;
	activeCards_1.push_back(Player_1[selectedCard]);
	Player_1.erase(Player_1.begin() + selectedCard);
	vector<int> goodCard;
	goodCard = GoodCards(strokeNumber, activeCards_1.size() - 1, activeCards_1);
	
	if (Beat(goodCard) == false) {
		takeAll(strokeNumber, activeCards_1, activeCards_2);
		Player_1 = addCards(Player_1);
		--strokeNumber;
		return strokeNumber;
	}
	else {	
		cout << "�������: " << Player_2[goodCard[0]].name << " " << Player_2[goodCard[0]].suit << endl;
		activeCards_2.push_back(Player_2[goodCard[0]]);
		Player_2.erase(Player_2.begin() + goodCard[0]);
		
	}
	flipCard = Flip(strokeNumber,  activeCards_1);
	flipCard_1 = Flip(strokeNumber,  activeCards_2);
	while (flipCard_1.size() > 0) {
		flipCard.push_back(flipCard_1[0]);
		flipCard_1.erase(flipCard_1.begin());
	}
	while (flipCard.size() > 0) {
		int yes = 0;
		cout << "�� ������ ��������� �����. ���� �� ������ ��������� ����� ������� 1." << endl;
		cout << "���� �� �� ������ ����������� ����� ������� 0" << endl;
		cin >> yes;
		if (yes == 1) {
			cout << "���� �����: " << endl;
			PrintVector(Player_1);
			cout << "����� ����� �� ������ ���������?" << endl;
			cin >> selectedCard;
			--selectedCard;
			if (Check(Player_1[selectedCard], flipCard) == true) {	
				activeCards_1.push_back(Player_1[selectedCard]);
				Player_1.erase(Player_1.begin() + selectedCard);
				goodCard = GoodCards(strokeNumber, activeCards_1.size() - 1, activeCards_1);
				if (Beat(goodCard) == false) {
					cout << "���� �����" << endl;
					takeAll(strokeNumber, activeCards_1, activeCards_2);
					Player_1 = addCards(Player_1);
					--strokeNumber;
					return strokeNumber;	
				}
				else {
					cout << "�������: " << Player_2[goodCard[0]].name << " " << Player_2[goodCard[0]].suit << endl;
					activeCards_2.push_back(Player_2[goodCard[0]]);	
					Player_2.erase(Player_2.begin() + goodCard[0]);
					
				}
			}
			else {
				cout << "�� �� ������ ��������� ��� �����" << endl;
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
	int selectedCard = 0;
	vector<Card> flipCard, flipCard_1;
	vector<Card> activeCards_1, activeCards_2;
	cout << "������ �1: " << endl;
	PrintVector(Player_1);
	cout << endl;
	cout << "������ �2: " << endl;
	PrintVector(Player_2);
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
	cout << "����: " << Player_2[selectedCard].name << " " << Player_2[selectedCard].suit << endl;
	activeCards_2.push_back(Player_2[selectedCard]);
	Player_2.erase(Player_2.begin() + selectedCard);
	int stop = 0;
		cout << "���� �� ������ ����������, ������� 1." << endl;
		cout << "���� �� �� ������ ����������, ������� 0." << endl;
		cin >> stop;
		if (stop != 1){
			takeAll(strokeNumber, activeCards_1, activeCards_2);
			Player_2 = addCards(Player_2);
			--strokeNumber;
			return strokeNumber;
		}
	vector<int> goodCard;
	goodCard = GoodCards(strokeNumber, activeCards_2.size() - 1, activeCards_2);
	if (goodCard.size() == 0) {
		cout << "��� ����� ����������, �� ����� �����" << endl;
		takeAll(strokeNumber, activeCards_1, activeCards_2);
		Player_2 = addCards(Player_2);
		--strokeNumber;
		return strokeNumber;
	}  
	else {
		 int necCard = 0;
		while(true) {
			cout << "����� ������ �� ������ ����������?" << endl;
			cin >> necCard;
			--necCard;
			if (Check(necCard, goodCard) == false) {
		 		cout << "���� ������ ������ ��������, �������� ������" << endl;
			} 
			else {
				cout << "�� ����������� ������: " << Player_1[necCard].name << " " << Player_1[necCard].suit << endl;
				activeCards_1.push_back(Player_1[necCard]);
				Player_1.erase(Player_1.begin() + necCard);
				break;
			}
		}
	}
	flipCard = Flip(strokeNumber,  activeCards_1);
	flipCard_1 = Flip(strokeNumber,  activeCards_2);
	while (flipCard_1.size() > 0) {
		flipCard.push_back(flipCard_1[0]);
		flipCard_1.erase(flipCard_1.begin());
	}
	
	while (flipCard.size() > 0) {
		cout << "����������: " << flipCard[0].name << " " << flipCard[0].suit << endl;
		activeCards_2.push_back(flipCard[0]);
		for(int i = 0; i < Player_2.size(); i++){
			if (Player_2[i].name == flipCard[0].name && Player_2[i].suit == flipCard[0].suit) {
				Player_2.erase(Player_2.begin() + i);
			}
		}
		goodCard = GoodCards(strokeNumber, activeCards_2.size() - 1, activeCards_2);
		if (goodCard.size() == 0) {
			cout << "��� ����� ����������, �� ����� �����" << endl;
			takeAll(strokeNumber, activeCards_1, activeCards_2);
			Player_2 = addCards(Player_2);
			--strokeNumber;
			return strokeNumber;
		}  
		else {
			int stop = 0;
			cout << "���� �� ������ ����������, ������� 1." << endl;
			cout << "���� �� �� ������ ����������, ������� 0." << endl;
			cin >> stop;
			if (stop != 1){
				takeAll(strokeNumber, activeCards_1, activeCards_2);
				Player_2 = addCards(Player_2);
				--strokeNumber;
				return strokeNumber;
			}
			
			int necCard = 0;
			 while(true) {
				cout << "����� ������ �� ������ ����������?" << endl;
				cout << "���� �����: " << endl;
				PrintVector(Player_1);
				cin >> necCard;
				--necCard;
				if (Check(necCard, goodCard) == false) {
			 		cout << "���� ������ ������ ��������, �������� ������" << endl;
				} 
				else {
				 	cout << "�� ����������� ������: " << Player_1[necCard].name << " " << Player_1[necCard].suit << endl;
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
	for (int i = 0; i < 12; i++) {
		if (Player_1.size() < 6) {
			Player_1.push_back(Deck[0]);
			Deck.erase(Deck.begin());
		}
		else if (Player_2.size() < 6) {
			Player_2.push_back(Deck[0]);
			Deck.erase(Deck.begin());
		}
	}
	TrumpCard = Deck[Deck.size() - 1];
	cout << "�������� �����: " << TrumpCard.name << " " << TrumpCard.suit << endl;
	cout << "���� ������ - �1." << endl;
	
	int strokeNumber = 1;
	while (Player_1.size() > 0 && Player_2.size() > 0) {
		if (strokeNumber % 2 == 1) {	
			strokeNumber = Move_1(strokeNumber) + 1;	
			cout << "����!" << endl;
			
			system("pause");
		}
		
		else { 
			strokeNumber = Move_2(strokeNumber) + 1;
			cout << "����!" << endl;  
			
			system("pause");     
		}
	}
	if (Player_1.size() == 0) {
		cout << "���� ���������. ������� ����� �" << 1 <<". " << endl;
	} else {
		cout << "���� ���������. ������� ����� �" << 2 <<". " << endl;
	}

}

int main () {
	setlocale (LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string CardsFile = "cards.txt";
	Game start(CardsFile);
	start.startGame();
	system("pause");
	return 0;
}
