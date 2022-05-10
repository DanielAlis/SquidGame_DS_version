//
// Created by Alis Daniel on 30/12/2021.
//

#include "PlayersManager.h"


PMStatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2)
{
    if(GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > k || GroupID2 > k)
    {
        return PM_INVALID_INPUT;
    }
    if(GroupID1 == GroupID2)
    {
        return PM_SUCCESS;
    }
    this->uf->Union(GroupID1,GroupID2);
    return PM_SUCCESS;
}

PMStatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score)
{
    if(GroupID<=0 || GroupID>this->k|| PlayerID <= 0 || score>this->scale || score<= 0)
    {
        return PM_INVALID_INPUT;
    }
    try{
        players.insert(PlayerID,Player(PlayerID,GroupID,score,0));
        this->uf->numOfZeroLevel[this->uf->Find(GroupID)][score]++;
        this->numOfZeroLevel[score]++;
        this->uf->numOfPlayers[this->uf->Find(GroupID)]++;
    }catch (HashTable<int,Player>::keyAlreadyExists& e)
    {
        return PM_FAILURE;
    }
    return PM_SUCCESS;
}

PMStatusType PlayersManager::RemovePlayer(int PlayerID)
{
    if(PlayerID<= 0)
    {
        return PM_INVALID_INPUT;
    }
    try{
        this->uf->getScoreTree(this->uf->Find(this->players[PlayerID].groupID),this->players[PlayerID].score).remove(PlayerKey(PlayerID,this->players[PlayerID].level));
        this->level[this->players[PlayerID].score]->remove(PlayerKey(PlayerID,this->players[PlayerID].level));
        this->allLevels->remove(PlayerKey(PlayerID,this->players[PlayerID].level));
        this->uf->numOfPlayers[this->uf->Find(this->players[PlayerID].groupID)]--;
        this->uf->removeLevel(this->players[PlayerID].groupID,PlayerKey(PlayerID,this->players[PlayerID].level));
        this->players.remove(PlayerID);
        return PM_SUCCESS;
    }catch (HashTable<int,Player>::keyNotFound& e)
    {
        return PM_FAILURE;

    }catch(RankAVL<PlayerKey,PlayerKey>::notFound& e)
    {
        this->uf->numOfZeroLevel[this->uf->Find(this->players[PlayerID].groupID)][this->players[PlayerID].score]--;
        this->uf->numOfPlayers[this->uf->Find(this->players[PlayerID].groupID)]--;
        this->numOfZeroLevel[this->players[PlayerID].score]--;
        this->players.remove(PlayerID);
        return PM_SUCCESS;
    }
}

PMStatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease)
{
    if(LevelIncrease <= 0 || PlayerID <= 0)
    {
        return PM_INVALID_INPUT;
    }
    try{
        this->players[PlayerID].level+=LevelIncrease;
        this->uf->getScoreTree(this->uf->Find(this->players[PlayerID].groupID),this->players[PlayerID].score).remove(PlayerKey(PlayerID,this->players[PlayerID].level - LevelIncrease));
        this->level[this->players[PlayerID].score]->remove(PlayerKey(PlayerID,this->players[PlayerID].level - LevelIncrease));
        this->allLevels->remove(PlayerKey(PlayerID,this->players[PlayerID].level  - LevelIncrease));
        this->uf->removeLevel(this->players[PlayerID].groupID,PlayerKey(PlayerID,this->players[PlayerID].level  - LevelIncrease));
        this->uf->getScoreTree(this->uf->Find(this->players[PlayerID].groupID),this->players[PlayerID].score).insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        this->level[this->players[PlayerID].score]->insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        this->allLevels->insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        this->uf->setLevel(this->players[PlayerID].groupID,PlayerKey(PlayerID,this->players[PlayerID].level));
        return PM_SUCCESS;
    }catch (HashTable<int,Player>::keyNotFound& e)
    {
        return PM_FAILURE;

    }catch(RankAVL<PlayerKey,PlayerKey>::notFound& e)
    {
        this->players[PlayerID].level =LevelIncrease;
        this->uf->getScoreTree(this->uf->Find(this->players[PlayerID].groupID),this->players[PlayerID].score).insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        this->level[this->players[PlayerID].score]->insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        this->allLevels->insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        this->uf->setLevel(this->players[PlayerID].groupID,PlayerKey(PlayerID,this->players[PlayerID].level));
        this->uf->numOfZeroLevel[this->uf->Find(this->players[PlayerID].groupID)][this->players[PlayerID].score]--;
        this->numOfZeroLevel[this->players[PlayerID].score]--;
        return PM_SUCCESS;
    }
}

PMStatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore)
{
    if(PlayerID <=0 || NewScore <= 0 || NewScore > this->scale)
    {
        return PM_INVALID_INPUT;
    }
    try{
        this->uf->getScoreTree(this->uf->Find(this->players[PlayerID].groupID),this->players[PlayerID].score).remove(PlayerKey(PlayerID,this->players[PlayerID].level));
        this->level[this->players[PlayerID].score]->remove(PlayerKey(PlayerID,this->players[PlayerID].level));
        this->players[PlayerID].score = NewScore;
        this->uf->getScoreTree(this->uf->Find(this->players[PlayerID].groupID),this->players[PlayerID].score).insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        this->level[this->players[PlayerID].score]->insert(PlayerKey(PlayerID,this->players[PlayerID].level),PlayerKey(PlayerID,this->players[PlayerID].level),this->players[PlayerID].level);
        return PM_SUCCESS;
    }catch (HashTable<int,Player>::keyNotFound& e)
    {
        return PM_FAILURE;

    }catch(RankAVL<PlayerKey,PlayerKey>::notFound& e)
    {
        this->uf->numOfZeroLevel[this->uf->Find(this->players[PlayerID].groupID)][this->players[PlayerID].score]--;
        this->numOfZeroLevel[this->players[PlayerID].score]--;
        this->players[PlayerID].score = NewScore;
        this->uf->numOfZeroLevel[this->uf->Find(this->players[PlayerID].groupID)][this->players[PlayerID].score]++;
        this->numOfZeroLevel[this->players[PlayerID].score]++;
        return PM_SUCCESS;
    }

}

PMStatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                  double * players)
{
    if(GroupID < 0 || GroupID > this->k ||players == nullptr)
    {
        return PM_INVALID_INPUT;
    }
    if(lowerLevel > higherLevel)
    {
        return PM_FAILURE;
    }
    double calculate;
    if(GroupID == 0) {
        if(score > scale || score <= 0)
        {
            calculate = GetPercentOfPlayersWithScoreInBoundsAux(this->players.getSize(), this->numOfZeroLevel[score],
                                                                lowerLevel, higherLevel,
                                                                this->allLevels, nullptr);
        } else
        {
            calculate = GetPercentOfPlayersWithScoreInBoundsAux(this->players.getSize(), this->numOfZeroLevel[score],
                                                                lowerLevel, higherLevel,
                                                                this->allLevels, this->level[score]);
        }
        if (calculate == FAIL) {
            return PM_FAILURE;
        }
        *players = calculate;
        return PM_SUCCESS;
    }
    if(score > scale || score <= 0)
    {
        calculate = GetPercentOfPlayersWithScoreInBoundsAux(this->uf->numOfPlayers[this->uf->Find(GroupID)], this->uf->numOfZeroLevel[this->uf->Find(GroupID)][score],
                                                            lowerLevel, higherLevel,
                                                            this->uf->allPlayers[this->uf->Find(GroupID)], nullptr);
    }
    else{
        calculate = GetPercentOfPlayersWithScoreInBoundsAux(this->uf->numOfPlayers[this->uf->Find(GroupID)], this->uf->numOfZeroLevel[this->uf->Find(GroupID)][score],
                                                            lowerLevel, higherLevel,
                                                            this->uf->allPlayers[this->uf->Find(GroupID)], &this->uf->getScoreTree(this->uf->Find(GroupID),score));
    }
    if (calculate == FAIL) {
        return PM_FAILURE;
    }
    *players = calculate;
    return PM_SUCCESS;
}

PMStatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level)
{
    if( m <= 0 || GroupID < 0 || GroupID > this->k ||level == nullptr)
    {
        return PM_INVALID_INPUT;
    }
    int sum = 0;
    if (GroupID == 0)
    {
        if (this->players.getSize() < m)
        {
            return PM_FAILURE;
        }
        try{
            sum = this->allLevels->biggerSelect(this->allLevels->rankSelect(m));
        }catch(RankAVL<PlayerKey,PlayerKey>::invalidRank& e)
        {
            try{
                sum = this->allLevels->biggerSelect(this->allLevels->rankSelect(this->allLevels->getSize()));
                *level= sum/(double)m;
            }catch(RankAVL<PlayerKey,PlayerKey>::invalidRank& e)
            {
                sum = 0;
                *level = sum;
            }
        }
        *level= sum/(double)m;
        return PM_SUCCESS;
    }
    if(this->uf->numOfPlayers[this->uf->Find(GroupID)] < m)
    {
        return PM_FAILURE;
    }
    RankAVL<PlayerKey,PlayerKey>* tree = this->uf->allPlayers[this->uf->Find(GroupID)];
    try{
        sum = tree->biggerSelect(tree->rankSelect(m));
    }catch(RankAVL<PlayerKey,PlayerKey>::invalidRank& e)
    {
        try{
            sum = tree->biggerSelect(tree->rankSelect(tree->getSize()));
            *level= sum/(double)m;
        }catch(RankAVL<PlayerKey,PlayerKey>::invalidRank& e)
        {
            sum = 0;
            *level = sum;
        }
    }
    *level= sum/(double)m;
    return PM_SUCCESS;
}

PMStatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                             int * LowerBoundPlayers, int * HigherBoundPlayers) {
    if (LowerBoundPlayers == nullptr || HigherBoundPlayers == nullptr || GroupID < 0
        || GroupID > this->k || m < 0 || score > scale || score <= 0) {
        return PM_INVALID_INPUT;
    }
    if(m==0)
    {
        *LowerBoundPlayers = 0;
        *HigherBoundPlayers = 0;
        return PM_SUCCESS;
    }
    int allZero=0;
    if (GroupID == 0) {
        for(int i = 0; i<=this->scale;i++)
        {
            allZero += this->numOfZeroLevel[i];
        }
        return GetPlayersBoundAux(allZero,this->numOfZeroLevel[score],this->players.getSize(),m,LowerBoundPlayers,HigherBoundPlayers,this->allLevels,this->level[score]);
    }
    for(int i = 0; i<=this->scale;i++)
    {
        allZero += this->uf->numOfZeroLevel[this->uf->Find(GroupID)][i];
    }
    return GetPlayersBoundAux(allZero,this->uf->numOfZeroLevel[this->uf->Find(GroupID)][score],this->uf->numOfPlayers[this->uf->Find(GroupID)],
                              m,LowerBoundPlayers,HigherBoundPlayers,this->uf->allPlayers[this->uf->Find(GroupID)],&this->uf->getScoreTree(this->uf->Find(GroupID),score));
}


int intHashFunction(const int& n){
    return n;
}


double PlayersManager::GetPercentOfPlayersWithScoreInBoundsAux(int sum, int zero, int lowerLevel, int higherLevel ,
                                                               RankAVL<PlayerKey,PlayerKey>* treeAll, RankAVL<PlayerKey,PlayerKey>* treeScore)
{
    int sumAll,sumScore;
    PlayerKey Bigger = PlayerKey(MAX_INT,higherLevel);
    PlayerKey Smaller = PlayerKey(0,lowerLevel);
    bool maxIntExist =treeAll->Exists(PlayerKey(MAX_INT, higherLevel));
    if(!maxIntExist)
    {
        treeAll->insert(Bigger, Bigger,0);
    }
    treeAll->insert(Smaller,Smaller,0);
    sumAll = treeAll->rank(Bigger) - treeAll->rank(Smaller) - 1;
    if(!maxIntExist) {
        treeAll->remove(Bigger);
    }
    treeAll->remove(Smaller);
    if(lowerLevel <= 0 && higherLevel>= 0)
    {
        sumAll += sum - treeAll->getSize();
    }
    if(sumAll == 0)
    {
        return FAIL;
    }
    if(treeScore == nullptr)
    {
        return 0;
    }
    maxIntExist =treeScore->Exists(PlayerKey(MAX_INT, higherLevel));
    if(!maxIntExist) {
        treeScore->insert(Bigger, Bigger, higherLevel);
    }
    treeScore->insert(Smaller,Smaller,lowerLevel);
    sumScore = treeScore->rank(Bigger) - treeScore->rank(Smaller) - 1;
    if(lowerLevel <= 0 && higherLevel>= 0)
    {
        sumScore +=zero;
    }
    if(!maxIntExist) {
        treeScore->remove(Bigger);
    }
    treeScore->remove(Smaller);
    return (sumScore /(double)sumAll) * 100;
}

PMStatusType PlayersManager::GetPlayersBoundAux(int allZero,int zero,int size,int m,int * LowerBoundPlayers, int * HigherBoundPlayers,
                                                RankAVL<PlayerKey,PlayerKey>* treeAll, RankAVL<PlayerKey,PlayerKey>* treeScore)
{
    PlayerKey levelSelect;
    int low = 0, high = 0, new_m=m;
    if (size < m) {
        return PM_FAILURE;
    }
    try {
        levelSelect = treeAll->rankSelect(m);
    }catch (RankAVL<PlayerKey,PlayerKey>::invalidRank& e)
    {
        int new_m = m - treeAll->getSize();
        low += treeScore->getSize();
        high = low;
        int sumScore = zero;
        if (sumScore < new_m)
        {
            high = low + sumScore;
        } else {
            high = low + new_m;
        }
        new_m = new_m -(allZero -zero);
        if (new_m > 0) {
            low += new_m;
        }
        *LowerBoundPlayers = low;
        *HigherBoundPlayers = high;
        return PM_SUCCESS;
    }
    bool maxIntExistAtAll =treeAll->Exists(PlayerKey(MAX_INT, levelSelect.getLevel()));
    bool maxIntExistAtScore =treeScore->Exists(PlayerKey(MAX_INT, levelSelect.getLevel()));
    PlayerKey Bigger = PlayerKey(MAX_INT, levelSelect.getLevel());
    if(!maxIntExistAtAll)
    {
        treeAll->insert(Bigger, Bigger,0);
    }
    if(treeAll->getSize() - treeAll->rank(Bigger) !=0) {
        PlayerKey seq = treeAll->rankSelect(treeAll->getSize() - (treeAll->rank(Bigger)));
        new_m = m - (treeAll->getSize() - (treeAll->rank(seq) - 1));
    }
    treeAll->insert(PlayerKey(0, levelSelect.getLevel()), PlayerKey(0, levelSelect.getLevel()),0);
    int allLevelSelect = treeAll->rank(Bigger) -
            treeAll->rank(PlayerKey(0, levelSelect.getLevel())) - 1;
    if(!maxIntExistAtAll) {
        treeAll->remove(Bigger);
    }
    treeAll->remove(PlayerKey(0, levelSelect.getLevel()));
    if(!maxIntExistAtScore) {
        treeScore->insert(PlayerKey(MAX_INT, levelSelect.getLevel()), PlayerKey(MAX_INT, levelSelect.getLevel()),0);
    }
    low += treeScore->getSize() - treeScore->rank(PlayerKey(MAX_INT, levelSelect.getLevel()));
    treeScore->insert(PlayerKey(0, levelSelect.getLevel()), PlayerKey(0, levelSelect.getLevel()),0);
    int sumScore = treeScore->rank(PlayerKey(MAX_INT, levelSelect.getLevel())) - treeScore->rank(PlayerKey(0, levelSelect.getLevel())) - 1;
    treeScore->remove(PlayerKey(0, levelSelect.getLevel()));
    if(!maxIntExistAtScore) {
        treeScore->remove(PlayerKey(MAX_INT, levelSelect.getLevel()));
    }
    if (sumScore < new_m)
    {
        high = low + sumScore;
    } else {
        high = low + new_m;
    }
    new_m = new_m - (allLevelSelect - sumScore);
    if (new_m > 0) {
        low += new_m;
    }
    *LowerBoundPlayers = low;
    *HigherBoundPlayers = high;
    return PM_SUCCESS;
}
