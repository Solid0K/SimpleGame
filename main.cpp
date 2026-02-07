#include <iostream>
#include <string>
#include <random>
#include "Random.h"
#include <algorithm>

class Creature {
private:
    std::string m_name{};
    char m_symbol{};
    int m_health{};
    int m_attackDamage{};
    int m_gold{};
public:
    Creature(const std::string_view name,const char symbol,const int health,const int attackDamage,const int gold):
    m_name{name},m_symbol{symbol},m_health{health},m_attackDamage{attackDamage},m_gold{gold}{}

    Creature()=default;

    std::string& get_name() {
        return m_name;
    }

    char& get_symbol() {
        return m_symbol;
    }

    int& get_health() {
        return m_health;
    }

    int& get_attackDamage() {
        return m_attackDamage;
    }

    int& get_gold() {
        return m_gold;
    }

    void set_name(const std::string& name) {
        m_name=name;
    }

    void set_symbol(const char& symbol) {
        m_symbol=symbol;
    }

    void set_health(const int& health) {
        m_health=health;
    }

    void set_attackDamage(const int& attack) {
        m_attackDamage=attack;
    }

    void set_gold(const int& gold) {
        m_gold=gold;
    }

    void reduceHealth(const int damage) {
        m_health-=damage;
    }

    bool isDead() const {
        if (m_health<=0) {
            return true;
        }
        return false;
    }

    void addGold(const int gold) {
        m_gold+=gold;
    }
};

class Player : public Creature {
private:
    int m_playerLevel{1};
public:
    explicit Player(const std::string_view name):
    Creature(name,'@',10,1,0){}

    int& get_level() {
        return m_playerLevel;
    }

    void levelUp() {
        m_playerLevel++;
        get_attackDamage()++;
    }

    bool hasWon() const {
        if (m_playerLevel>=20) {
            return true;
        }
        return false;
    }
};

class Monster : public Creature {
public:
    enum Type {
        dragon,
        orc,
        slime,
        max_type,
    };

private:
    inline static Creature monsters[]{
        Creature{"dragon",'D',20,4,100},
        Creature{"orc",'o',4,2,25},
        Creature{"slime",'s',1,1,10}
    };

    static_assert(std::size(monsters)==max_type);

public:
    explicit Monster(const Type type):
    Creature(monsters[type]){}

    static Type getRandomMonster() {
        return static_cast<Type>(std::uniform_int_distribution<int>{0,max_type-1}(Random::mt));
    }
};

std::string get_playerInput() {
    while (true) {
        std::cout<<"Enter your Name: ";
        std::string name{};
        std::cin>>name;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"That an invalid input try again\n";
            continue;
        }
        return name;
    }
}

char yourChoice() {
    while (true) {
        std::cout<<"(R)un or (F)ight: ";
        char choice{};
        std::cin>>choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"That an invalid input try again\n";
            continue;
        }
        if (std::tolower(choice)=='r' || std::tolower(choice)=='f') {
            return std::tolower(choice);
        }else {
            std::cout<<"That's an invalid input try again\n";
            continue;
        }
    }
}

void journey(Player& player) {
    Monster monster{Monster::getRandomMonster()};
    std::cout<<"You encountered a "<<monster.get_name()<<'\n';
    while (!monster.isDead() && !player.isDead()) {
        char choice{yourChoice()};
        if (choice=='f') {
            monster.reduceHealth(player.get_attackDamage());
            std::cout<<"You hit the "<<monster.get_name()<<" for "<<player.get_attackDamage()<<" damage\n";
            std::cout<<monster.get_name()<<" health: "<<monster.get_health()<<'\n';
            if (monster.isDead()) {
                break;
            }
            player.reduceHealth(monster.get_attackDamage());
            std::cout<<"The "<<monster.get_name()<<" hit you for "<<monster.get_attackDamage()<<" damage\n";
            std::cout<<"Your health: "<<player.get_health()<<'\n';
        }else {
            int chance{std::uniform_int_distribution<int>{0,1}(Random::mt)};
            if (chance) {
                std::cout<<"You successfully fled.\n";
                Monster temp{Monster::getRandomMonster()};
                monster=temp;
                std::cout<<"You encountered a "<<monster.get_name()<<'\n';
                continue;
            }else {
                std::cout<<"You failed to fled\n";
                player.reduceHealth(monster.get_attackDamage());
                std::cout<<"The "<<monster.get_name()<<" hit you for "<<monster.get_attackDamage()<<" damage\n";
                std::cout<<"Your health: "<<player.get_health()<<'\n';
            }
        }
    }
    if (monster.isDead()) {
        std::cout<<"You kill the "<<monster.get_name()<<'\n';
        player.levelUp();
        std::cout<<"You are now on level "<<player.get_level()<<'\n';
        player.addGold(monster.get_gold());
        std::cout<<"You found "<<monster.get_gold()<<" gold\n";
    }
}

int main() {
    const std::string name{get_playerInput()};
    Player player{name};
    while (!player.hasWon() && !player.isDead()) {
        journey(player);
    }
    if (player.isDead()) {
        std::cout<<"OOPs too bad you died on level "<<player.get_level()<<" with "<<player.get_gold()<<" gold"<<'\n';
        std::cout<<"Better luck next time\n";
    }else {
        std::cout<<"Wow you reached level "<<player.get_level()<<" well Played!!!!\n";
    }
    return 0;
}