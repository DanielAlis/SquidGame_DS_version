
#include "UF.h"


UF::UF(int size,int scale)
{
    this->size = new int[size];
    this->parent = new int[size];
    this->numOfPlayers = new int[size];
    this->numOfZeroLevel= new int*[size];
    this->players = new RankAVL<PlayerKey,PlayerKey>** [size];
    this->allPlayers = new RankAVL<PlayerKey,PlayerKey>* [size];
    this->scale = scale;
    this->elements = size;
    for(int i = 1; i < size; i++)
    {
        Makeset(i);
    }
}

UF::~UF()
{
    delete[] this->size;
    delete[] this->parent;
    delete[] this->numOfPlayers;
    for (int i = 1; i<this->elements ; i++)
    {
        for (int j = 0 ; j <= this->scale ; j++)
        {
            delete this->players[i][j];
        }
        delete[] this->numOfZeroLevel[i];
        delete this->allPlayers[i];
        delete[] this->players[i];
    }
    delete[] this->numOfZeroLevel;
    delete[] this->players;
    delete[] this->allPlayers;
}

void UF::Makeset(int x)
{
    this->size[x] = 1;
    this->parent[x] = ROOT;
    this->numOfPlayers[x] = 0;
    this->numOfZeroLevel[x] = new int[this->scale + 1];
    this->players[x] =new RankAVL<PlayerKey,PlayerKey>* [this->scale + 1];
    this->allPlayers[x] = new RankAVL<PlayerKey,PlayerKey>();
    for (int i = 0; i<=this->scale ; i++)
    {
        (this->players[x])[i] =  new RankAVL<PlayerKey,PlayerKey>();
        (this->numOfZeroLevel[x])[i] = 0;
    }

}

int UF::Find(int x)
{
    int root;
    if(this->parent[x] == ROOT)
    {
        root = x;
    }
    else
    {
        int temp = x;
        while(this->parent[temp] != ROOT)
        {
            temp = this->parent[temp];
        }
        root = temp;
        temp = x;
        int shrink;
        while(this->parent[temp] != ROOT)
        {
            shrink = temp;
            temp = this->parent[temp];
            this->parent[shrink] = root;
        }
    }
    return root;
}

void UF::Union(int p,int q)
{
    int big;
    int small;
    if(this->size[Find(p)] > this->size[Find(q)]) {
        big = Find(p);
        small = Find(q);
    }
    else {
        big = Find(q);
        small = Find(p);
    }
    if(big!= small)
    {
        UnionAux(big,small);
    }
}

void UF::UnionAux(int big,int small)
{
    int new_size;
    for(int i = 0; i <= this->scale ; i++) {
        new_size = this->players[big][i]->getSize() + this->players[small][i]->getSize();
        PlayerKey *array = new PlayerKey[new_size];
        int *array2 = new int[new_size];
        RankAVL<PlayerKey, PlayerKey>::iterator iter1 = this->players[big][i]->begin();
        RankAVL<PlayerKey, PlayerKey>::iterator iter2 = this->players[small][i]->begin();
        for (int j = 0 ; j < new_size ; j++ ){
            if (iter1 != this->players[big][i]->end() && iter2 != this->players[small][i]->end())
            {
                if (iter1.getKey() < iter2.getKey()) {
                    array[j] = iter1.getKey();
                    array2[j] = iter1.getKey().getLevel();
                    iter1++;
                } else {
                    array[j] = iter2.getKey();
                    array2[j] = iter2.getKey().getLevel();
                    iter2++;
                }
            }
            else
            {
                if (iter1 != this->players[big][i]->end()) {
                    array[j] = iter1.getKey();
                    array2[j] = iter1.getKey().getLevel();
                    iter1++;
                }
                else{
                    array[j] = iter2.getKey();
                    array2[j] = iter2.getKey().getLevel();
                    iter2++;
                }
            }
        }
        delete this->players[big][i];
        delete this->players[small][i];
        this->players[small][i] = nullptr;
        this->players[big][i] = new RankAVL<PlayerKey,PlayerKey>();
        if (new_size != 0)
        {
            this->players[big][i]->makeTree(this->players[big][i],array,array,array2,new_size);
        }
        this->numOfZeroLevel[big][i] +=this->numOfZeroLevel [small][i];
        this->numOfZeroLevel [small][i] = NOT_ROOT;
        delete[] array;
        delete[] array2;
    }
    this->parent[small] = big;
    this->size[small] = NOT_ROOT;
    this->numOfPlayers[big] += this->numOfPlayers[small];
    this->numOfPlayers[small] = NOT_ROOT;
    new_size = this->allPlayers[big]->getSize() + this->allPlayers[small]->getSize();
    PlayerKey *array = new PlayerKey[new_size];
    int *array2 = new int[new_size];
    RankAVL<PlayerKey, PlayerKey>::iterator iter1 = this->allPlayers[big]->begin();
    RankAVL<PlayerKey, PlayerKey>::iterator iter2 = this->allPlayers[small]->begin();
    for (int j = 0 ; j < new_size ; j++ ){
        if (iter1 != this->allPlayers[big]->end() && iter2 != this->allPlayers[small]->end())
        {
            if (iter1.getKey() < iter2.getKey()) {
                array[j] = iter1.getKey();
                array2[j] = iter1.getKey().getLevel();
                iter1++;
            } else {
                array[j] = iter2.getKey();
                array2[j] = iter2.getKey().getLevel();
                iter2++;
            }
        }
        else
        {
            if (iter1 != this->allPlayers[big]->end()) {
                array[j] = iter1.getKey();
                array2[j] = iter1.getKey().getLevel();
                iter1++;
            }
            else{
                array[j] = iter2.getKey();
                array2[j] = iter2.getKey().getLevel();
                iter2++;
            }
        }
    }
    delete this->allPlayers[big];
    delete this->allPlayers[small];
    this->allPlayers[small] = nullptr;
    this->allPlayers[big] = new RankAVL<PlayerKey,PlayerKey>();
    if(new_size != 0)
    {
        this->allPlayers[big]->makeTree(this->allPlayers[big],array,array,array2,new_size);
    }
    delete[] array;
    delete[] array2;
}

RankAVL<PlayerKey,PlayerKey>& UF::getScoreTree(int x,int score) {
    return *((this->players[Find(x)])[score]);
}
void UF::setLevel(int x,PlayerKey playerKey) {
    this->allPlayers[Find(x)]->insert(playerKey,playerKey,playerKey.getLevel());
}
void UF::removeLevel(int x,PlayerKey playerKey) {
    this->allPlayers[Find(x)]->remove(playerKey);
}