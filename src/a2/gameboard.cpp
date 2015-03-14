#include "gameboard.hpp"
#include <stdio.h>
#include <cstdlib>
gameboard::gameboard(){
    for(int i=0;i<9;++i){
        board[i] = 0;
    }
}

char gameboard::print_grid(int i){
    switch(i){
        case -1:
            return 'x';
        case 1:
            return 'o';
        default:
            return ' ';
    }
    return ' ';
}

void gameboard::print_board(){
    printf(" %c | %c | %c\n",print_grid(board[0]),print_grid(board[1]),print_grid(board[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",print_grid(board[3]),print_grid(board[4]),print_grid(board[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",print_grid(board[6]),print_grid(board[7]),print_grid(board[8]));
}

void gameboard::update_board(int move, int player){
    if(move >=0 && move < 9 && board[move] == 0){
        board[move] = -1*player;
    }
}

int gameboard::is_win(){
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
        if(board[wins[i][0]] != 0 &&
                board[wins[i][0]] == board[wins[i][1]] &&
                board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    return 0;
}

bool gameboard::is_valid(){
    int p1_count = 0;//1
    int p2_count = 0;//-1
    for(int i=0;i<9;++i){
        if(board[i] == 1){
            ++p1_count;
        }
        else if(board[i] == -1){
            ++p2_count;
        }
        else if(board[i] == 0){
            continue;
        }
        else{
            return false;
        }
    }
    if(abs(p1_count - p2_count) > 1){
        return false;
    }
    return true;
}

bool gameboard::is_finished(){
    for(int i=0;i<9;++i){
        if(board[i] == 0){
            return false;
        }
    }
    return true;
}

int* gameboard::get_board(){
    return board;
}
