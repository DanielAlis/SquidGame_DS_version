//
// Created by Alis Daniel.
//

#include "library2.h"
#include"PlayersManager.h"

StatusType Status(PMStatusType type) {
    StatusType result;
    switch (type) {

        case (PM_SUCCESS):
            result = SUCCESS;
            break;
        case (PM_FAILURE):
            result = FAILURE;
            break;
        case (PM_INVALID_INPUT):
            result = INVALID_INPUT;
            break;
    }
    return result;
}
void* Init(int k, int scale) {
    try{
        PlayersManager *DS = new PlayersManager(k, scale);
        return (void*)DS;
    }catch (std::bad_alloc& e){
        return nullptr;
    }
}
StatusType MergeGroups(void *DS, int GroupID1, int GroupID2) {
    if (DS == nullptr) {
        return INVALID_INPUT;
    }
    try {
        return Status(((PlayersManager *) DS)->MergeGroups(GroupID1, GroupID2));
    } catch (...) {
        return ALLOCATION_ERROR;
    }
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if(DS== nullptr)
    {
        return INVALID_INPUT;
    }
    try{
        return Status(((PlayersManager*)DS)-> AddPlayer(PlayerID, GroupID, score));
    }catch(...) {
        return ALLOCATION_ERROR;
    }
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if(DS== nullptr)
    {
        return INVALID_INPUT;
    }
    try{
        return Status(((PlayersManager*)DS)-> RemovePlayer(PlayerID));
    }catch(...) {
        return ALLOCATION_ERROR;
    }

}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if(DS== nullptr)
    {
        return INVALID_INPUT;
    }
    try{
        return Status(((PlayersManager*)DS)-> IncreasePlayerIDLevel(PlayerID, LevelIncrease));
    }catch(...) {
        return ALLOCATION_ERROR;
    }

}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    if(DS== nullptr)
    {
        return INVALID_INPUT;
    }
    try{
        return Status(((PlayersManager*)DS)-> ChangePlayerIDScore(PlayerID, NewScore));
    }catch(...) {
        return ALLOCATION_ERROR;
    }
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players)
{
    if(DS== nullptr)
    {
        return INVALID_INPUT;
    }
    try{
        return Status(((PlayersManager*)DS)-> GetPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel,players));
    }catch(...) {
        return ALLOCATION_ERROR;
    }
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
    if(DS== nullptr)
    {
        return INVALID_INPUT;
    }
    try{
        return Status(((PlayersManager*)DS)-> AverageHighestPlayerLevelByGroup(GroupID, m, level));
    }catch(...) {
        return ALLOCATION_ERROR;
    }

}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers){
    if(DS== nullptr)
    {
        return INVALID_INPUT;
    }
    try{
        return Status(((PlayersManager*)DS)-> GetPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers));
    }catch(...) {
        return ALLOCATION_ERROR;
    }

}

void Quit(void** DS)
{
    if(DS == nullptr || *DS == nullptr)
    {
        return;
    }
    delete *(PlayersManager**)DS;
    *DS = nullptr;
}
