#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "main.h"

struct Node bestMove(int* game,  int depth, int playerMove){
    struct Node move;
    int index, val;
    int bestValue = INT_MIN;
    int maximizingPlayer = (playerMove==CROSS)?1:0;
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            index = i * SIZE + j;
            if(game[index]==EMPTY){
                // we evaluate the move
                game[index] = playerMove;
                val = minimax(game, depth, !maximizingPlayer);
                // the evaluations for the circle are still negative,
                // but we want the circle to play perfectly too so
                // we make it positive there
                if(!maximizingPlayer) val*=(-1);
                // if there is an equal chance to win with two squares
                // (with one being the center), the center will
                // be favored (as a rule of thumb).
                if(val>bestValue || (val == bestValue && index==4)){
                    bestValue = val;
                    move.x = i;
                    move.y = j;
                }
                game[index]= EMPTY;
            }
        }
    }
    return move;
}

int minimax(int* game, int depth, int maximizingPlayer){
    int bestValue = INT_MIN;
    int winner = victoryAchieved(game);
    // if the winner is already decided, just stop
    if(winner!=EMPTY){
        return winner==CROSS? 10:-10;
    }
    // if the grid is full, please don't return INT_MIN !!
    else if(gridFull(game)){
        return 0;
    }
    // we want a basic evaluation (either we have only one move or
    // we consider it's enough digging
    if(depth<=0 || onlyOnePossibility(game)){
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
    // evaluation depending on the fact we want the best move for the cross
    // and the "worst" for the circle (from the point of view of the cross)
    else if(maximizingPlayer!=0){
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
        // this decrementation makes them keep on playing normally even when it is
        // a "sure win" or a "sure loss" according to their prediction
        if(bestValue>1) bestValue--;
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
        // this incrementation makes them keep on playing normally even when it is
        // a "sure win" or a "sure loss" according to their prediction
        if(bestValue<-1) bestValue++;

    }
    return bestValue;
}

// return the heuristic value of each case
int heuristicValue(int* game, int index, int playerForm){
    int x = index/3;
    int y = index - x*3;
    int val = 0;
    struct Node n;
    n.x = x;
    n.y = y;
    game[index] = playerForm;
    if(victory(game, n)){
        val = 10;
    }
    // If it is the circle that is winning, we consider that as a loss
    if(playerForm==CIRCLE) val*=-1;
    game[index]=EMPTY;


    return val;
}
int victory(int* game, struct Node lastMove){
    // who made the last move
    int valueLastMove = game[lastMove.x*3+lastMove.y];
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
                index = newx*3+newy;

                if(index >=9 || index<0 || newx <0 || newx>=3 || newy<0 || newy>=3){
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

// go through every square of the grid
// returns the first player that the algorithm
// found had won
int victoryAchieved(int* game){

    struct Node n;
    int res = EMPTY;
    for(int i=0; i<9; i++){

        if(game[i]!=EMPTY){
            n.x = i/3;
            n.y = i -n.x*3;
            if(victory(game, n)){
                res = game[i];
                break;
            }
        }
    }
    return res;

}

// return true if the grid is full
int gridFull(int* game){
    struct Node n;
    int res = 1;
    for(int i=0; i<9; i++){
        if(game[i]==EMPTY){
            res = 0;
            break;
        }
    }
    return res;
}

// return true is there is only one playable square
int onlyOnePossibility(int* game){
    int cmpt = 0;
    for(int i=0; i<9; i++){
        if(game[i]==EMPTY) ++cmpt;
    }
    return cmpt==1;
}

// in the name
void displayGrid(int* game){
    int dim = 3;
    printf("\n");
    for(int i=0; i<dim; i++){
        for(int j = 0; j<dim; j++){

            printf("%c\t", game[i*3+j]==CIRCLE? 'O': (game[i*3+j]==CROSS?'X':'-' ));
        }
        printf("\n");
    }

}
// procedure to test some combination easily
void testConfiguration(int* game, int player){
    displayGrid(game);
    struct Node lastMove;
    int depth=10;
    do{
        lastMove= bestMove(game, depth, player);
        game[lastMove.x*SIZE+lastMove.y] = player;
        player = player==CIRCLE?CROSS:CIRCLE;
        displayGrid(game);
        printf("\nminimax: %d\n", minimax(game, depth, player==CROSS?1:0));
    }while(!victory(jeu, lastMove) && !gridFull(game));
}
// some example of combinations
void config1(){
    printf("--------CONFIG 1---------");
    int game[9] = {CROSS, CIRCLE, CROSS,
    EMPTY, EMPTY, CIRCLE,
    EMPTY, EMPTY, CROSS};
    testConfiguration(game, CIRCLE);

}
void config2(){
    printf("--------CONFIG 2---------");
    int game[9] = {EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY};
    testConfiguration(game, CROSS);
}
void config3(){
    printf("--------CONFIG 3---------");
    int game[9] = {EMPTY, EMPTY, EMPTY,
    CIRCLE, CROSS, EMPTY,
    EMPTY, EMPTY, EMPTY};
    testConfiguration(game, CROSS);
}

void config4(){
    printf("--------CONFIG 4---------");
    int game[9] = {EMPTY, EMPTY, EMPTY,
    CROSS, CIRCLE, CROSS,
    EMPTY, EMPTY, EMPTY};
    testConfiguration(game, CIRCLE);
}


///////////////////////////////////////


int main()
{
    // evaluation is from the point of view of the cross:
    // positive if the cross has a way to win for sure, and
    // negative if there is no way not to lose if the circle plays perfectly
    config1();
    printf("\n\n");
    config2();
    config3();
    config4();
    return 0;
}
