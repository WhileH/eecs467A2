#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

class gameboard{
    public:
        gameboard();
        void print_board();
        void update_board(int move,int player);
        int  is_win();
        bool is_valid();
        bool is_finished();
        int* get_board(); 
    private:
        char print_grid(int i);
        int board[9];
};

#endif
