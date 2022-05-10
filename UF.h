//
// Created by Alis Daniel.
//

#ifndef UF_H
#define UF_H


#define ROOT -1
#define NOT_ROOT -2

#include "RankAVL.h"
#include <iostream>



class PlayerKey{
    int id;
    int level;
public:
    PlayerKey() : id(-1),level(-1){};
    PlayerKey(int id,int level) : id(id),level(level){}
    friend bool operator<(const PlayerKey& player1,const PlayerKey& player2);
    friend bool operator>(const PlayerKey& player1,const PlayerKey& player2);
    PlayerKey& operator=(const PlayerKey& player)
    {
        if (this == &player)
        {
            return *this;
        }
        this->id = player.id;
        this->level = player.level;
        return *this;
    }
    int getID() const
    {
        return this->id;
    }
    int getLevel() const
    {
        return this->level;
    }
    bool operator==(const PlayerKey& player) const
    {
        if (this->level == player.level && this->id == player.id)
        {
            return true;
        }
        return false;
    }
    bool operator!=(const PlayerKey& player) const
    {
        return !(*this==player);
    }
};

inline bool operator<(const PlayerKey& player1,const PlayerKey& player2)
{
    if(player1.level < player2.level)
    {
        return true;
    }
    else if(player1.level == player2.level && player1.id < player2.id)
    {
        return true;
    }
    return false;
}
inline bool operator>(const PlayerKey& player1,const PlayerKey& player2)
{
    return player2<player1;
}

class UF{
    int* parent;
    int* size;

    RankAVL<PlayerKey,PlayerKey>*** players;

    int scale;
    int elements;
public:
    RankAVL<PlayerKey,PlayerKey>** allPlayers;
    int* numOfPlayers;
    int** numOfZeroLevel;
    UF(int size,int scale);
    ~UF();
    void Makeset(int x);
    int Find(int x);
    void Union(int p,int q);
    void UnionAux(int big,int small);
    RankAVL<PlayerKey,PlayerKey>& getScoreTree(int x,int score);
    void setLevel(int x,PlayerKey playerKey);
    void removeLevel(int x,PlayerKey playerKey);
};




#endif //UF_H
