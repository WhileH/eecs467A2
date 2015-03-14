#include <stdio.h>
#include "gameboard.hpp"
#include "arm_ai.hpp"

int main(){
    arm_ai a1 = arm_ai(-1);
    //arm_ai a2 = arm_ai(1);
    gameboard g = gameboard();
    while(1){
        printf("computer turn:\n");
        printf("%d\n",a1.calc_move(g.get_board()));
        g.update_board(a1.calc_move(g.get_board()),a1.get_player());
        g.print_board();
        if(g.is_finished() || g.is_win() != 0){
            break;
        }
        printf("your turn:\n");
        int move;
        //g.update_board(a2.calc_move(g.get_board()),a2.get_player());
        scanf("%d",&move);
        printf("\n");
        g.update_board(move,1);
        g.print_board();
        if(g.is_finished() || g.is_win() != 0){
            break;
        }
    }
    switch(g.is_win()){
        case 1:
            printf("player 1 win\n");
            break;
        case 0:
            printf("draw\n");
            break;
        case -1:
            printf("player -1 win\n");
            break;
    }
    return 0;
}
