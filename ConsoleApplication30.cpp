#include <iostream>
#include <string>
#include "Random.h"

class Creature {
protected:
	std::string m_name{};
	char m_simb{};
	int m_helth{};
	int m_damege{};
	int m_gold{};
public:

	Creature(const std::string& name, char simb, int helth, int damege, int gold) :
		m_name{name}, m_simb{simb}, m_helth{helth}, m_damege{damege}, m_gold{gold}
	{
	}

	void reduceHealth(int dam) { m_helth -= dam; }
	void addGold(int gol) { m_gold += gol; }
	bool isDead() { return (m_helth <= 0); }

	const std::string& getName() const { return m_name; }
	char getSymbol() const { return m_simb; }
	int getHealth() const { return m_helth; }
	int getDamage() const { return m_damege; }
	int getGold() const { return m_gold; }

};


class Player : public Creature
{
private:
	int m_level{1};
public:
	Player(const std::string& name) : Creature{name,'@',10,1,0}
	{
	}

	void levelUp()
	{
		m_level += 1;
		m_damege += 1;
		m_helth += 4;
	}

	int getLevel() { return m_level; }
	bool hasWon() { return (m_level >= 20); }

};


class Monster : public Creature
{
public:

	enum Type {
		dragon,
		orc,
		slime,
		max_types,
	};

	static inline Creature monsterData[]{ {"dragon",'d',20,4,100}, {"orc",'o',4,2,25}, {"slime",'s',1,1,10} };
	static_assert(std::size(monsterData) == max_types);

	Monster(Type type) : Creature{monsterData[type]}
	{
	}

	static Monster getRandomMonster() {
		int rand_num{Random::get(0,max_types-1)};
		return Monster{ static_cast<Type>(rand_num) };
	}

};

void attacPlayer(Player& p, Monster& m) {
	std::cout << "You hit the " << m.getName() << " for " << p.getDamage() << " damage.\n";
	m.reduceHealth(p.getDamage());
}

void attacMonster(Player& p, Monster& m) {
	std::cout << "The " << m.getName() << " hit you for " << m.getDamage() << " damage.\n";
	p.reduceHealth(m.getDamage());
}



bool fightMonster(char user_com, Player& p, Monster& m) {
	if (user_com == 'f')
	{
		attacPlayer(p, m);
		if (m.isDead()) {
			return false;
		}
		attacMonster(p, m);

		if (p.isDead()) {
			return true;
		}

		return false;
	}
	if (user_com == 'r') {
		int rand{Random::get(0,1)};
		if (rand) {
			std::cout << "You successfully fled.\n";
			return true;
		}
		else
		{
			std::cout << "You failed to flee.\n";
			attacMonster(p, m);
			return false;
		}
	}
}



int main()
{
	std::cout << "Enter your name: ";
	std::string playerName;
	std::cin >> playerName;

	Player player{ playerName };
	std::cout << "Welcome, " << player.getName() << ".\n";

	while (!player.isDead()) {
		Monster monster{Monster::getRandomMonster()};
		std::cout << "You have encountered a " << monster.getName() << " (" << monster.getSymbol() << ")\n";

		while (true) {
			char user_com{};
			std::cout << "(R)un or (F)ight: ";
			std::cin >> user_com;
			if (fightMonster(user_com, player, monster)) break;

			if (player.isDead()) break;

			if (monster.isDead()) {
				std::cout << "You killed the " << monster.getName() << '\n';
				player.addGold(monster.getGold());
				player.levelUp();
				std::cout << "You are now level " << player.getLevel() << "\n";
				std::cout << "You found " << player.getGold() << " gold\n";
				break;
			}
		}

		if (player.hasWon()) {
			std::cout << "\n\n You won!\n";
			return 0;
		}

		if (player.isDead()) {
			std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";
			std::cout << "Too bad you can’t take it with you!\n";
		}

		
	}

	return 0;
}
