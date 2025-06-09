#include "Board.h"

#include <string>

int main()
{
    Board board(std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));

    board.print_board();

    return 0;
}