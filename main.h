#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#define EMPTY -1
#define CROSS 1
#define CIRCLE 2

struct Node{
    int x;
    int y;
} Node;
int minimax(int* game, int depth, int maximizingPlayer);
int heuristicValue(int* game, int index, int playerForm);
int victory(int* game, struct Node lastMove);
int onlyOnePossibility(int* game, int taille);
void displayGrid(int* game, int taille);
#endif // MAIN_H_INCLUDED
