#ifndef _BOARD_H_
#define _BOARD_H_

#include "types.h"

#include <string>

#define NUM_BLACK_PIECES 6
#define NUM_WHITE_PIECES 6
#define NUM_PIECE_TYPES NUM_BLACK_PIECES + NUM_WHITE_PIECES
#define FILES 8 // colums a-h
#define RANKS 8 // rows 0-7
#define NUM_SQUARES 64
#define MAX_FILE 7
#define MAX_RANK 7

#define RANK_FILE_TO_SQUARE(r,f) ((r*8) + f)
#define FILE_A_MASK       0x8080808080808080
#define RANK_0_MASK       0x00000000000000ff

#define DIAGONAL_MASK     0x8040201008040201
#define REV_DIAGONAL_MASK 0x0102040810204080

/*
     _   _   _   _
  7 |_|@|_|@|_|@|_|@|
  6 |@|_|@|_|@|_|@|_|
  5 |_|@|_|@|_|@|_|@|
  4 |@|_|@|_|@|_|@|_|
  3 |_|@|_|@|_|@|_|@|
  2 |@|_|@|_|@|_|@|_|
  1 |_|@|_|@|_|@|_|@|
  0 |@|_|@|_|@|_|@|_|
     a b c d e f g h

*/

enum PieceIndex_t
{
    PAWN_INDEX   = 0,
    ROOK_INDEX   = 1,
    KNIGHT_INDEX = 2,
    BISHOP_INDEX = 3,
    KING_INDEX   = 4,
    QUEEN_INDEX  = 5
};

enum Piece_t
{
    PAWN   = 1,
    ROOK   = 2,
    KNIGHT = 3,
    BISHOP = 4,
    KING   = 5,
    QUEEN  = 6
};

enum files_t
{
    a = 0,
    b = 1,
    c = 2,
    d = 3,
    e = 4,
    f = 5,
    g = 6,
    h = 7,
    NUM_FILES
};

enum squares_t {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
  };

enum Color_t
{
    WHITE = 0x8,
    BLACK = 0x10
};

class Board
{
    public:
        Board(std::string fen);

        void print_board();

    private:
        Color_t curr_turn;

        // holds encoding for each piece. Only used for printing board
        uint16_t board[8][8];

        uint64_t white_boards[NUM_WHITE_PIECES]; // contains the bit board for each piece
        uint64_t black_boards[NUM_BLACK_PIECES]; // contains the bit board for each piece

        // Attack boards (a bitboard that represents where a piece can attack, where the index of the array is the current space it is on)
        uint64_t white_pawn_attacks[NUM_SQUARES];
        uint64_t black_pawn_attacks[NUM_SQUARES];
        uint64_t rook_attacks[NUM_SQUARES];
        uint64_t knight_attacks[NUM_SQUARES];
        uint64_t bishop_attacks[NUM_SQUARES];
        uint64_t king_attacks[NUM_SQUARES];
        uint64_t queen_attacks[NUM_SQUARES];

        // flags to inidcate if the rooks/king can castle
        bool can_castle[6];

        // Functions to fill out the <piece>_attacks boards
        void generate_white_pawn_attacks();
        void generate_black_pawn_attacks();
        void generate_rook_attacks();
        void generate_bishop_attacks();
        void generate_knight_attacks();
        void generate_king_attacks();
        void generate_queen_attacks();
        uint64_t generate_straight_rays(int square);
        uint64_t generate_diagonal_rays(int square);

        inline bool on_board(int square);
        inline bool on_board(int rank, int file);
        inline void set_bit(int rank, int file, uint64_t& board);
        inline void clear_bit(int rank, int file, uint64_t& board);

        void print_bitboard(uint64_t& board);
};

#endif
