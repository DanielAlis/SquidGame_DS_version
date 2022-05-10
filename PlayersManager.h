//
// Created by Alis Daniel.
//

#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#define MAX_INT 2147483647
#define  FAIL -1.0

#include "RankAVL.h"
#include "HashTable.h"
#include "UF.h"
#include "DList.h"

typedef enum {PM_SUCCESS, PM_FAILURE, PM_INVALID_INPUT,} PMStatusType;
int intHashFunction (const int& n);

class Player{
    int id;

public:
    int groupID;
    int score;
    int level;
    Player()
    {
        this->id = -1;
        this->groupID = -1;
        this->level = -1 ;
    }
    Player(int id ,int groupID,int score, int level) :id(id),groupID(groupID),score(score),level(level){};
};


class PlayersManager {
    UF* uf;
    HashTable<int,Player> players;
    int scale;
    int k;
    RankAVL<PlayerKey, PlayerKey>* allLevels;
    RankAVL<PlayerKey, PlayerKey>** level;
    int* numOfZeroLevel;




public:

    PlayersManager(int k, int scale) :uf(new UF(k+1,scale)),players(&intHashFunction),scale(scale),k(k),allLevels(new RankAVL<PlayerKey, PlayerKey>())
    {
        this->level = new RankAVL<PlayerKey,PlayerKey>* [scale + 1];
        this->numOfZeroLevel = new int[scale +1];
        for (int i = 0 ; i <=scale ; i++) {
            this->level[i] = new RankAVL<PlayerKey, PlayerKey>();
            this->numOfZeroLevel[i] = 0;
        }
    }
    ~PlayersManager()
    {
        for (int i = 0 ; i <=scale ; i++) {
            delete this->level[i];
        }
        delete[] numOfZeroLevel;
        delete[] level;
        delete allLevels;
        delete uf;
    }
    PMStatusType MergeGroups(int GroupID1, int GroupID2);

    PMStatusType AddPlayer(int PlayerID, int GroupID, int score);

    PMStatusType RemovePlayer(int PlayerID);

    PMStatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);

    PMStatusType ChangePlayerIDScore(int PlayerID, int NewScore);

    PMStatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                    double * players);

    PMStatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);

    PMStatusType GetPlayersBound(int GroupID, int score, int m,
                               int * LowerBoundPlayers, int * HigherBoundPlayers);

    double static GetPercentOfPlayersWithScoreInBoundsAux(int sum, int zero, int lowerLevel, int higherLevel ,RankAVL<PlayerKey,PlayerKey>* treeAll, RankAVL<PlayerKey,PlayerKey>* treeScore);

    PMStatusType static GetPlayersBoundAux(int allZero,int zero,int size,int m,int * LowerBoundPlayers, int * HigherBoundPlayers,
                                                    RankAVL<PlayerKey,PlayerKey>* treeAll, RankAVL<PlayerKey,PlayerKey>* treeScore);

};






#endif //PLAYERSMANAGER_H
