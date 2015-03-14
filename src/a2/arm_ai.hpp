#ifndef ARM_AI_HPP
#define ARM_AI_HPP
#include <vector>

class arm_ai{
    public:
        arm_ai(int p);    
        int calc_move(int board[9]);//didn't modify board
        int is_win(const int board[9]);
        int get_player();
    private:
        int min_max(int board[9],int player);//didn't modify board
        int player;// 1 or -1
};

#endif
