#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <map>

using namespace std;

struct Card
{
	string name;
	string suit;
};

void setupConsole();
bool isCardInSet(const int &card, const vector<int> &cards);
vector<Card> uniteVectors(const vector<Card> &vector_1, const vector<Card> &vector_2);
void PrintVector(const vector<Card> &vector);

class Game
{
public:
	Game();
	void start();
private:
	const int startHandSize = 6;
	const int cardIsAbsent = 1000;
	int strokeNumber = 1;

	vector <Card> Deck;
	vector <Card> Player_1;
	vector <Card> Player_2;
	vector<Card> activeCards_1, activeCards_2;
	Card TrumpCard;

	map <string, int> cardPriority = {
		{"6", 6},
		{"7", 7},
		{"8", 8},
		{"9", 9},
		{"10", 10},
		{"âàëåò", 11},
		{"äàìà", 12},
		{"êîðîëü", 13},
		{"òóç", 14}
	};
	string replicas_1_Player[12] =
	{
		{"Какой картой вы будете отбиваться?"},
		{"этой картой нельзя отбиться, выберите другую "},
		{"вы отбиваетесь картой: "},
		{"какую карту вы будете подкидывать?"},
		{"эту карту нельзя подкинуть, выбертье другую карту"},
		{"вы подкинули карту: "},
		{"какой картой вы будете переводить?"},
		{"этой картой нельзя перевести, выберите другую ",},
		{"вы переводите картой: "},
		{"какой картой вы хотите сходить?"},
		{"этой карты у вас нет, выберите другую "},
		{"вы ходите картой: "}
	};
	string replicasComp[4] =
	{
		{"отбиваю: "},
		{"подкидываю: "},
		{"прервожу: "},
		{"Хожу: "}
	};
	string replicas_2_Player[6] =
	{
		{"если вы будете отбиваться"},
		{"если вы не хотите отбиваться"},
		{"если вы будете подкидывать карту"},
		{"если вы не хотите подкидывать карту"},
		{"если вы будете переводить"},
		{"если вы не хотите переводить"}
	};

	void move();

	vector<int> goodCards(const int &index, const vector<Card> &activeCards);
	int selectionCard();

	void whichCardPlayerFighting(const vector<int> &cards, const int &replicaIndex);
	void whichCardComputerFighting(const int &card, const int &replicaIndex);

	void translate();
	int canPlayerTranslate();
	bool canPlayerTakeCards(int replicaIndex);
	vector<int> canPlayerFlip(const vector<Card> &crad);

	void addCards(vector<Card> &Player);
	void takeAll();
};

int main()
{
	setupConsole();
	Game game;
	game.start();
	system("pause");
	return 0;
}

void PrintVector(const vector<Card> &vector)
{
	for (int i = 0; i < vector.size(); i++)
	{
		cout << i + 1 << ". " << vector[i].name << " " << vector[i].suit << endl;
	}
}

vector<Card> uniteVectors(const vector<Card> &vector_1, const vector<Card> &vector_2)
{
	vector<Card> newVector;
	copy(vector_1.begin(), vector_1.end(), back_inserter(newVector));
	copy(vector_2.begin(), vector_2.end(), back_inserter(newVector));
	return newVector;
}

void setupConsole()
{
	setlocale (LC_ALL, "Russian");
}

Game::Game()
{
	vector <string> suits {"черви", "буби", "крести", "пики"};
	for( const auto &suit : suits)
	{
		for ( const auto &card : cardPriority)
		{
			const auto &name = card.first;
			Deck.push_back({.name = name, .suit = suit});
		}
	}

	srand(time(NULL));
	random_shuffle(Deck.begin(), Deck.end());

	for (int i = 0; i < startHandSize; i++)
	{
		Player_1.push_back(Deck[i]);
	}
	for (int i = startHandSize; i < startHandSize * 2; i++)
	{
		Player_2.push_back(Deck[i]);
	}
	Deck.erase(Deck.begin(), Deck.begin() + startHandSize * 2);
	TrumpCard = Deck[Deck.size() - 1];
}

void Game::start()
{
	cout << "Козырная карта: " << TrumpCard.name << " " << TrumpCard.suit << endl;
	cout << "ваша колода - №1." << endl;

	while (!Player_1.empty() && !Player_2.empty()) 				// основной цикл
	{
		cout << "êîëîäà ¹1: " << endl;
		PrintVector(Player_1);
		cout << endl;
		cout << "êîëîäà ¹2: " << endl;
		PrintVector(Player_2);
		vector <int> cardsNumbers;
		for(int i = 0; i < (strokeNumber % 2 == 1 ? Player_1.size() : Player_2.size() ); i++)
		{
			cardsNumbers.push_back(i);
		}
		int cardNumber = strokeNumber % 2 == 1 ? 0 : selectionCard();
		strokeNumber % 2 == 1 ? whichCardPlayerFighting(cardsNumbers, 9) : whichCardComputerFighting(cardNumber, 3); // выбор карты, которой будем ходить
		translate();							// перевод хода
		move();								// функция одного хода, в ней происходит:							
		                                                                // отбитие карт, подкидывание карт, отбитие подкинутых карт
		cout << "Бита!" << endl;
		system("pause");
		++strokeNumber;
		activeCards_1.clear();
		activeCards_2.clear();
	}
	cout << "игра закончена, победил игрок № " << (Player_1.empty() ? 1 : 2) << endl;
}

void Game::move()
{
	vector<int> flipCard;
	vector<int> goodCard;
	vector<Card> allActiveCards;
	bool stop;

	vector<Card> &walking = strokeNumber % 2 == 1 ? Player_1 : Player_2;
	vector<Card> &repellent = strokeNumber % 2 == 1 ? Player_2 : Player_1;

	allActiveCards = uniteVectors(activeCards_1, activeCards_2);
	cout << "карты, которые нужно отбить: " << endl;
	PrintVector(allActiveCards);

	if (!(strokeNumber % 2 == 1 ? true : canPlayerTakeCards(0))) 	// спрашиваем, будет ли игрок отбиваться
	{
		takeAll();
		addCards(walking);
		--strokeNumber;
		return;
	}

	for(int i = 0; i < allActiveCards.size(); i++) 			// отбивание всех карт
	{
		goodCard = goodCards(i, allActiveCards);
		if (goodCard.empty())
		{
			cout << "нечем биться, беру карты" << endl;
			takeAll();
			addCards(walking);
			--strokeNumber;
			return;
		}
		cout << "карта №" << i + 1 <<": " << allActiveCards[i].name << " " << allActiveCards[i].suit << endl;
		strokeNumber % 2 == 1 ? whichCardComputerFighting(goodCard[0], 0) : whichCardPlayerFighting(goodCard, 0);
	}
	//							   		
	allActiveCards = uniteVectors(activeCards_1, activeCards_2);
	flipCard = canPlayerFlip(allActiveCards);
	while (flipCard.size() > 0) 						// подкидывание и отбивание подкинутых карт				
	{
		if (!(strokeNumber % 2 == 1 ? canPlayerTakeCards(2) : true)) // спрашиваем, будет ли игрок подкидывать
		{
			break;
		} 
		
		strokeNumber % 2 == 1 ? whichCardPlayerFighting(flipCard, 3) : whichCardComputerFighting(flipCard[0], 1);	
		if (!(strokeNumber % 2 == 1 ? true : canPlayerTakeCards(0))) // спрашиваем, будет ли игрок отбиваться
		{
			takeAll();
			addCards(walking);
			--strokeNumber;
			return;
		}
		
		vector<Card> &activeCards = strokeNumber % 2 == 1 ? activeCards_1 : activeCards_2;
		goodCard = goodCards(activeCards.size() - 1, activeCards); 
		if (goodCard.empty())
		{
			cout << "нечем биться, беру карты" << endl;
			takeAll();
			addCards(walking);
			--strokeNumber;
			return;	
		}
		
		strokeNumber % 2 == 1 ? whichCardComputerFighting(goodCard[0], 0) : whichCardPlayerFighting(goodCard, 0);// отбиваемся
		allActiveCards = uniteVectors(activeCards_1, activeCards_2);	
		flipCard.clear();
		flipCard = canPlayerFlip(allActiveCards);
	}
	addCards(Player_1);
	addCards(Player_2);	
	return; 
}

vector<int> Game::goodCards(const int &index, const vector<Card> &activeCards)
{
	vector<Card> const &walking = strokeNumber % 2 == 1 ? Player_1 : Player_2;
	vector<Card> const &repellent = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	vector<int> goodCard;
	for(int i = 0; i < repellent.size(); i++)
	{
		if (activeCards[index].suit == TrumpCard.suit && repellent[i].suit == TrumpCard.suit
		&& cardPriority[repellent[i].name] > cardPriority[activeCards[index].name])
		{
			goodCard.push_back(i);
		}
		else if (activeCards[index].suit != TrumpCard.suit &&  repellent[i].suit != TrumpCard.suit 
		&& repellent[i].suit == activeCards[index].suit
		&& cardPriority[repellent[i].name] > cardPriority[activeCards[index].name]) 
		{
			goodCard.push_back(i); 
		}
		else if (activeCards[index].suit != TrumpCard.suit && repellent[i].suit == TrumpCard.suit) 
		{
			goodCard.push_back(i); 
		}
	}
	for (int i = 0; i < goodCard.size(); i++)
	{
		cout << goodCard[i] + 1 << endl;
	}
	return goodCard;
}

void Game::whichCardComputerFighting(const int &card, const int &replicaIndex)
{
	cout << endl << replicasComp[replicaIndex] << Player_2[card].name << " " << Player_2[card].suit << endl << endl;
	activeCards_2.push_back(Player_2[card]);
	Player_2.erase(Player_2.begin() + card);
	return;
}

void Game::whichCardPlayerFighting(const vector<int> &cards, const int &replicaIndex)
{
	int necCard = 0;
	while(true)
	{
		cout << endl << replicas_1_Player[replicaIndex] << endl << endl;
		cout << "ваши карты: " << endl;
		PrintVector(Player_1);
		cin >> necCard;
		--necCard;
		if (isCardInSet(necCard, cards) == true)
		{
	 		break;
		}
		else
		{
			cout << replicas_1_Player[replicaIndex + 1] << endl;
		}
	}
	cout << endl << replicas_1_Player[replicaIndex + 2] << Player_1[necCard].name << " " << Player_1[necCard].suit << endl << endl;
	activeCards_1.push_back(Player_1[necCard]);
	Player_1.erase(Player_1.begin() + necCard);
	return;
}

int Game::selectionCard()
{
	int selectedCard = cardIsAbsent;
	for(int i = 0; i < Player_2.size(); i++) 
	{
		if(selectedCard == cardIsAbsent) 
		{
			selectedCard = i;
		}	
		else if(Player_2[i].suit != TrumpCard.suit && cardPriority[Player_2[i].name] < cardPriority[Player_2[selectedCard].name]) 
		{
			selectedCard = i;
		}
	}
	return selectedCard;	
}

void Game::translate()		
{
	int translateCa
	vector<int> goodCard;
	while (true) 								//переводим ход на другого игрока
	{
		translateCard = canPlayerTranslate();
		if(translateCard == cardIsAbsent)
		{
			break;
		}
		if(!(strokeNumber % 2 == 1 ? true : canPlayerTakeCards(4)))   // спрашиваем, будет ли игрок переводить
		{
			break;
		}
		goodCard.push_back(translateCard);
		strokeNumber % 2 == 1 ? whichCardComputerFighting(goodCard[0], 2) : whichCardPlayerFighting(goodCard, 6); // выбираем карту для перевода
		++strokeNumber;
	}
}

int Game::canPlayerTranslate() 
{
	const vector<Card> &repellent = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	vector<Card> activeCards;
	activeCards = uniteVectors(activeCards_1, activeCards_2);	
	for(int i = 0; i < repellent.size(); i++) 
	{
		if (activeCards[0].name == repellent[i].name)
		{
			return i;
		}
	}
	return cardIsAbsent;
}

bool Game::canPlayerTakeCards(const int replicaIndex)
{
	int stop;
	cout << replicas_2_Player[replicaIndex] << ", введите 1." << endl;
	cout << replicas_2_Player[replicaIndex + 1] <<", введите 0." << endl;
	cin >> stop;
	if(stop == 1)
	{
		return true;
	}
	return false;
}

vector<int> Game::canPlayerFlip(const vector<Card> &activeCards) 
{
	const vector<Card> &walking = strokeNumber % 2 == 1 ? Player_1 : Player_2;
	vector<int> flipCard;
	for(int j = 0; j < activeCards.size(); j++) 
	{
		for(int i = 0; i < walking.size(); i++) 
		{
			if(activeCards[j].name == walking[i].name) 
			{
				flipCard.push_back(i);
			}
		}
	}
	return flipCard;
}

void Game::addCards(vector<Card> &Player) 
{
	while(Player.size() < startHandSize && Deck.size() > 0) 
	{
		Player.push_back(Deck[0]);
		Deck.erase(Deck.begin());
	}
	cout << "все карты взяты" << endl;
}

void Game::takeAll() 
{
	auto &activePlayer = strokeNumber % 2 == 1 ? Player_2 : Player_1;
	for(int i = 0; i < activeCards_1.size(); i++)
	{
		activePlayer.push_back(activeCards_1[i]);
	}
	for(int i = 0; i < activeCards_2.size(); i++) 
	{
		activePlayer.push_back(activeCards_2[i]);
	}			
}

bool isCardInSet(const int &card, const vector<int> &cards) 
{
	auto found = find(cards.begin(), cards.end(), card);
	return found != cards.end();	
}
