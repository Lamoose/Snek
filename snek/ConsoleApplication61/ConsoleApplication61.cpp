#include <iostream>
#include <windows.h>
#include <conio.h>
#include <random>
#include <vector>
#include <fstream>
#include <string>

struct snek
{
	int x=0, y=0;
};

struct osoba
{
	std::string name;
	int Score = 0;
	void napraviScore(std::string str)
	{
		int i = 0;
		while (str[i] != ' ')
		{
			name += str[i];
			i++;
		}
		i++;

		Score = int(str[i])-'0';
	}
	bool operator < (const osoba& sledeci) const
	{
		return (Score < sledeci.Score);
	}

};

bool GameOver = false;


struct player
{
	int Score = 1;
	std::vector<snek> Snek;
	player()
	{
		Snek.resize(1);
	}
};

int GenerateRandomX()
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(1, 78);
	return dist(rd);
}
int GenerateRandomY()
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(1, 22);
	return dist(rd);
}

struct fruit
{
	//max cord 79 23
	int x = GenerateRandomX();
	int y = GenerateRandomY();

	void spawn()
	{
		y = GenerateRandomY();
		x = GenerateRandomX();
	}
};

struct Sortd
{
	int x, y;
	char t;
	bool operator < (const Sortd& sledeci) const
	{
		return (x < sledeci.x);
	}
};

void switchCord(player& Player)
{
	for (int i = Player.Snek.size()-1; i > 0; i--)
	{
		Player.Snek[i].x = Player.Snek[i - 1].x;
		Player.Snek[i].y = Player.Snek[i - 1].y;	
	}
}

void Clear(player& Player)
{
	system("cls");
	for (int i = 0; i < 34; i++)
	{
		std::cout << " ";
	}
	std::cout << "score:" << Player.Score << "\n";
}

void render(fruit& Fruit,player& Player)
{
	Clear(Player);
	int lastX = 0;

	for (int i = 0; i < Player.Snek.size(); i++)
	{
		if (Fruit.x == Player.Snek[i].x && Fruit.y == Player.Snek[i].y)
		{
			Player.Snek.resize(Player.Snek.size() + 1);
			switchCord(Player);
			for (int i = 0; i < Player.Snek.size(); i++)
			{
				for (int j = 0; j < Player.Snek.size(); j++)
				{
					if (Player.Snek[i].x == Player.Snek[j].x && Player.Snek[i].y == Player.Snek[j].y && Player.Snek.size() != 1 && i != j)
					{
						if(Fruit.x> Player.Snek[0].x)Player.Snek[i].x++;
						else Player.Snek[i].x--;
					}
				}
			}
			Fruit.spawn();
			Player.Score++;
		}
	}
	for (int i = 0; i < Player.Snek.size(); i++)
	{
		for (int j = 0; j < Player.Snek.size(); j++)
		{
			if (Player.Snek[i].x == Player.Snek[j].x && Player.Snek[i].y == Player.Snek[j].y && Player.Snek.size() != 1 && i!=j)
			{
				GameOver = true;
				return;
			}
		}
	}


	std::vector<Sortd> sorted(Player.Snek.size()+1);
	sorted[0].x = Fruit.x;
	sorted[0].y = Fruit.y;
	sorted[0].t = 'f';

	for (int i = 1; i < sorted.size(); i++)
	{
		sorted[i].x = Player.Snek[i - 1].x;
		sorted[i].y = Player.Snek[i - 1].y;
		sorted[i].t = 's';
	}

	std::sort(sorted.begin(), sorted.end());


	if (Player.Snek[0].y > 22 || Player.Snek[0].y < 0 || Player.Snek[0].x > 78 || Player.Snek[0].x <0)
	{
		GameOver = true;
		return;
	}

	for (int i = 0; i < 23; i++)
	{	
		for (int j = 0; j < sorted.size(); j++)
		{
			if (sorted[j].y == i)
			{
				for (int l = 0; l < sorted[j].x - lastX; l++)
				{
					std::cout << " ";
				}
				lastX = sorted[j].x + 1;
				if(sorted[j].t == 'f')std::cout << "#";
				else std::cout << "*";
			}

		}
		std::cout << "\n";
		lastX = 0;
	}
	switchCord(Player);
}

void input()
{
	fruit Fruit;
	player Player;
start:
	if (!GameOver)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'a':
				
					Player.Snek[0].x--;
					render(Fruit, Player);
					goto start;
			case 'd':
				Player.Snek[0].x++;
				render(Fruit, Player);
				goto start;
			case 'w':
					Player.Snek[0].y--;
					render(Fruit, Player);
					goto start;
			case 's':
				Player.Snek[0].y++;
				render(Fruit, Player);
				goto start;
			case 'x':
				return;
			default:
				goto start;
			}
		}
		goto start;
	}
	else
	{
		system("cls");
		for (int i = 0; i < 6; i++)
		{
			std::cout << "\n";
		}
		for (int i = 0; i < 30; i++)
		{
			std::cout << " ";
		}
		std::cout << "GameOver score:" << Player.Score << "\n";
		int HighScore = Player.Score;
		std::ifstream ulaz("Scores.txt");
		std::string str;
		std::vector<osoba> vektor(1);
		int i = 0;
		std::string name;
		for (int i = 0; i < 30; i++)
		{
			std::cout << " ";
		}
		if (!ulaz)
		{
			std::cout << "Error opening text file";
			exit(1);
		}

		while (getline(ulaz, str))
		{
			vektor[i].napraviScore(str);
			i++;
			vektor.resize(i + 1);
		}
		std::cout << "Enter Name ";
		std::cin >> name;
		vektor.resize(i + 1);
		vektor[vektor.size() - 1].name = name;
		vektor[vektor.size() - 1].Score = Player.Score;
		std::sort(vektor.begin(), vektor.end());

		for (int i = 0; i < vektor.size(); i++)
		{
			for (int i = 0; i < 35; i++)
			{
				std::cout << " ";
			}
			std::cout << vektor[i].name + ": " << vektor[i].Score << "\n";
		}
		ulaz.close();

		std::ofstream izlaz("Scores.txt");

		for (int j = 0; j < vektor.size(); j++)
		{
			izlaz << vektor[j].name + " ";
			izlaz << vektor[j].Score << "\n";
		}
	}
}

int main()
{
	input();
}