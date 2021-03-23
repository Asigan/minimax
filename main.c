#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "main.h"
int minimax(int* game, int depth, int maximizingPlayer){
    int bestValue = INT_MIN;
    int winner = victoryAchieved(game);
    if(winner!=EMPTY){
        return winner==CROSS? 10:-10;
    }
    if(depth<=0 || onlyOnePossibility(game, 9)){
        int tmp;
        for(int i =0; i<9; i++){
            if(game[i]==EMPTY){
                tmp = heuristicValue(game, i, maximizingPlayer!=0?CROSS:CIRCLE);
                if(tmp>bestValue){
                    bestValue = tmp;
                }
            }
        }
    }
    else if(maximizingPlayer!=0){
        //int* newGame = copy(game, 9);
        int tmp;
        for(int i=0; i<9; i++){
            if(game[i]==EMPTY){
                game[i] = CROSS;
                tmp = minimax(game, depth-1, 0);
                if(tmp>bestValue){
                    bestValue = tmp;
                }
                game[i] = EMPTY;
            }

        }
    }
    else{
        bestValue = INT_MAX;
        int tmp;
        for(int i=0; i<9; i++){
            if(game[i]==EMPTY){
                game[i] = CIRCLE;
                tmp = minimax(game, depth-1, 1);
                if(tmp<bestValue){
                    bestValue = tmp;
                }
                game[i] = EMPTY;
            }
        }

    }
    return bestValue;
}
int* copy(int* tab, int taille){
    int* newtab = malloc(taille*sizeof(int));
    for(int i=0; i<taille; i++){
        newtab[i] = tab[i];
    }
    return newtab;
}
int heuristicValue(int* game, int index, int playerForm){
    int x = index/3;
    int y = index - x*3;
    int val = 0;
    struct Node n;
    n.x = x;
    n.y = y;
    if(index == 4){
        val = 5;
    }
    game[index] = (playerForm==CROSS)? CIRCLE:CROSS;
    if(victory(game, n)){
        val = -10;
    }
    game[index] = playerForm;
    if(victory(game, n)){
        val = 10;
    }
    if(playerForm==CIRCLE) val*=-1;
    game[index]=EMPTY;


    return val;
}
int victory(int* game, struct Node lastMove){
    // who made the last move
    int valueLastMove = game[lastMove.x+lastMove.y*3];
    // will be used within the loops
    int changeX, changeY;
    int border;
    int index, newx, newy;
    int cmpt=0;
    int changeXTab[4] = {1, 1, 1, 0};
    int changeYTab[4] = {-1, 0, 1, 1};
    int win=0;
    // going through all the orientations
    for(int i=0; i<4; i++){
        changeX = changeXTab[i];
        changeY = changeYTab[i];
        cmpt=1;
        // each orientation has two opposite directions
        for(int k=-1; k<2; k+=2 ){
            border = 0;
            newy = lastMove.y;
            newx = lastMove.x;
            while(border==0){
                newy += k*changeY;
                newx += k*changeX;
                index = newx+newy*3;
                if(index >=9 || index<0){
                    border=1;
                }
                else if(game[index] != valueLastMove){
                    border = 1;
                }
                else{
                    cmpt++;
                }
            }
        }
        if(cmpt>=3){
            win=1;
            break;
        }
    }
    return win;
}
int victoryAchieved(int* jeu){

    struct Node n;
    int res = EMPTY;
    for(int i=0; i<9; i++){

        if(jeu[i]!=EMPTY){
            n.x = i/3;
            n.y = i -n.x*3;
            if(victory(jeu, n)){
                res = jeu[i];
            }
        }
    }
    return res;

}
int onlyOnePossibility(int* game, int taille){
    int cmpt = 0;
    for(int i=0; i<taille; i++){
        if(game[i]==EMPTY) ++cmpt;
    }
    return cmpt==1;
}
void displayGrid(int* game, int taille){
    int dim = sqrt(taille);
    for(int i=0; i<dim; i++){
        for(int j = 0; j<dim; j++){

            printf("%c\t", game[i*3+j]==CIRCLE? 'O': (game[i*3+j]==CROSS?'X':' ' ));
        }
        printf("\n");
    }

}

int main()
{
    int jeu[9] = {CROSS, CIRCLE, CROSS,
    EMPTY, EMPTY, CIRCLE,
    EMPTY, EMPTY, CROSS};
    displayGrid(jeu, 9);
    struct Node lastMove;
    lastMove.x = 1;
    lastMove.y = 1;
    victory(jeu, lastMove);
    printf("victory = %d", minimax(jeu, 5, 0));
    return 0;
}
