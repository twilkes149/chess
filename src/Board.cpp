#include "Board.h"

#include <iostream>
#include <bitset>

Board::Board(std::string fen)
{
    // TODO: parse other fields of fen string
    this->curr_turn = WHITE;

    // clear all boards
    for (int i = 0; i < NUM_WHITE_PIECES; i++)
    {
        white_boards[i] = 0;
        black_boards[i] = 0;
    }

    for (int i = 0; i < NUM_SQUARES; i++)
    {
        white_pawn_attacks[i] = 0;
        black_pawn_attacks[i] = 0;
        rook_attacks[i]       = 0;
        knight_attacks[i]     = 0;
        bishop_attacks[i]     = 0;
        king_attacks[i]       = 0;
        queen_attacks[i]      = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = 0;
        }
    }

    // Parse fen string
    int file = 0;
    for (char c = fen[0], index = 0, rank=MAX_RANK; index < fen.length(); index++, c = fen[index])
    {
        if (isdigit(c))
        {
            file = c - '0';
        }
        else
        {
            switch (c)
            {
                // black pieces:
                case 'p': // pawn
                    set_bit(rank, file, black_boards[PAWN_INDEX]);
                    board[rank][file] = BLACK|PAWN;
                    file++;
                break;

                case 'n': // knight
                    set_bit(rank, file, black_boards[KNIGHT_INDEX]);
                    board[rank][file] = BLACK|KNIGHT;
                    file++;
                break;

                case 'b': // bishop
                    set_bit(rank, file, black_boards[BISHOP_INDEX]);
                    board[rank][file] = BLACK|BISHOP;
                    file++;
                break;

                case 'r': // rook
                    set_bit(rank, file, black_boards[ROOK_INDEX]);
                    board[rank][file] = BLACK|ROOK;
                    file++;
                break;

                case 'q': // queen
                    set_bit(rank, file, black_boards[QUEEN_INDEX]);
                    board[rank][file] = BLACK|QUEEN;
                    file++;
                break;

                case 'k': // king
                    set_bit(rank, file, black_boards[KING_INDEX]);
                    board[rank][file] = BLACK|KING;
                    file++;
                break;

                // white pieces
                case 'P': // pawn
                    set_bit(rank, file, white_boards[PAWN_INDEX]);
                    board[rank][file] = WHITE|PAWN;
                    file++;
                break;

                case 'N': // knight
                    set_bit(rank, file, white_boards[KNIGHT_INDEX]);
                    board[rank][file] = WHITE|KNIGHT;
                    file++;
                break;

                case 'B': // bishop
                    set_bit(rank, file, white_boards[BISHOP_INDEX]);
                    board[rank][file] = WHITE|BISHOP;
                    file++;
                break;

                case 'R': // rook
                    set_bit(rank, file, white_boards[ROOK_INDEX]);
                    board[rank][file] = WHITE|ROOK;
                    file++;
                break;

                case 'Q': // queen
                    set_bit(rank, file, white_boards[QUEEN_INDEX]);
                    board[rank][file] = WHITE|QUEEN;
                    file++;
                break;

                case 'K': // king
                    set_bit(rank, file, white_boards[KING_INDEX]);
                    board[rank][file] = WHITE|KING;
                    file++;
                break;

                case '/':
                    rank--;
                    file = 0;
                break;
            }
        }
    }

    // Generate all attack bitboards
    this->generate_white_pawn_attacks();
    this->generate_black_pawn_attacks();
    this->generate_rook_attacks();
    this->generate_bishop_attacks();
    this->generate_knight_attacks();
    this->generate_queen_attacks();
    this->generate_king_attacks();

    this->print_bitboard(knight_attacks[h4]);
}

inline void Board::set_bit(int rank, int file, uint64_t& board)
{
    board |= (static_cast<uint64_t>(0x1)<<(MAX_FILE-file + (8*rank)));
}

inline void Board::clear_bit(int rank, int file, uint64_t& board)
{
    board &= ~(static_cast<uint64_t>(0x1)<<(MAX_FILE-file + 8*rank));
}

inline bool Board::on_board(int square)
{
    return (square >= a1 && square <= h8);
}

inline bool Board::on_board(int rank, int file)
{
    return (rank >= 0 && rank <= MAX_RANK && file >= 0 && file <= MAX_FILE);
}

void Board::generate_white_pawn_attacks()
{
    // for each square on the board
    for (int rank = 1; rank < 7; rank++) // white pawns don't exist on rank 0 or 7
    {
        for (int file = a; file < NUM_FILES; file++)
        {
            if (file+1 <= MAX_FILE)
                this->set_bit(rank+1, file+1, white_pawn_attacks[RANK_FILE_TO_SQUARE(rank,file)]);
            if (file-1 >= 0)
                this->set_bit(rank+1, file-1, white_pawn_attacks[RANK_FILE_TO_SQUARE(rank,file)]);
        }
    }
}

void Board::generate_black_pawn_attacks()
{
    // for each square on the board
    for (int rank = 6; rank >= 1; rank--) // black pawns don't exist on rank 7 or 0
    {
        for (int file = a; file < NUM_FILES; file++)
        {
            if (file+1 <= MAX_FILE)
                this->set_bit(rank-1, file+1, black_pawn_attacks[RANK_FILE_TO_SQUARE(rank,file)]);
            if (file-1 >= 0)
                this->set_bit(rank-1, file-1, black_pawn_attacks[RANK_FILE_TO_SQUARE(rank,file)]);
        }
    }
}

uint64_t Board::generate_straight_rays(int square)
{
    uint64_t board = 0;
    int rank = square / 8;
    int file = square % 8;

    board |= (static_cast<uint64_t>(FILE_A_MASK) >> file);
    board |= (static_cast<uint64_t>(RANK_0_MASK) << (rank*8));

    // clear the current space (can't attack itself)
    this->clear_bit(rank,file, board);

    return board;
}

uint64_t Board::generate_diagonal_rays(int square)
{
    int rank       = square / 8;
    int file       = square % 8;
    uint64_t board = 0;

    // Northeast direction
    for (int t_rank = rank+1, t_file = file+1;
            t_rank >= 0 && t_rank <= MAX_RANK &&
            t_file >= 0 && t_file <= MAX_FILE;
            t_rank++, t_file++
        )
    {
        this->set_bit(t_rank, t_file, board);
    }

    // Southeast direction
    for (int t_rank = rank-1, t_file = file+1;
        t_rank >= 0 && t_rank <= MAX_RANK &&
        t_file >= 0 && t_file <= MAX_FILE;
        t_rank--, t_file++
    )
    {
        this->set_bit(t_rank, t_file, board);
    }

    // Northwest direction
    for (int t_rank = rank+1, t_file = file-1;
        t_rank >= 0 && t_rank <= MAX_RANK &&
        t_file >= 0 && t_file <= MAX_FILE;
        t_rank++, t_file--
    )
    {
        this->set_bit(t_rank, t_file, board);
    }

    // Southwest direction
    for (int t_rank = rank-1, t_file = file-1;
        t_rank >= 0 && t_rank <= MAX_RANK &&
        t_file >= 0 && t_file <= MAX_FILE;
        t_rank--, t_file--
    )
    {
        this->set_bit(t_rank, t_file, board);
    }

    return board;
}

void Board::generate_rook_attacks()
{
    // for each rank on the board
    for (int square = a1; square < NUM_SQUARES; square++)
    {
        uint64_t board = this->generate_straight_rays(square);
        rook_attacks[square] |= board;
    }
}

void Board::generate_bishop_attacks()
{
    // for each square on the board
    for (int square = a1; square < NUM_SQUARES; square++)
    {
        uint64_t board = this->generate_diagonal_rays(square);
        bishop_attacks[square] |= board;
    }
}

void Board::generate_knight_attacks()
{
    for (int rank = 0; rank <= MAX_RANK; rank++)
    {
        for (int file = a; file <= MAX_FILE; file++)
        {
            uint64_t& board = knight_attacks[RANK_FILE_TO_SQUARE(rank,file)];

            if (this->on_board(rank+2, file+1))
                this->set_bit(rank+2, file+1, board);
            if (this->on_board(rank+2, file-1))
                this->set_bit(rank+2, file-1, board);

            if (this->on_board(rank-2, file+1))
                this->set_bit(rank-2, file+1, board);
            if (this->on_board(rank-2, file-1))
                this->set_bit(rank-2, file-1, board);

            if (this->on_board(rank+1, file+2))
                this->set_bit(rank+1, file+2, board);
            if (this->on_board(rank-1, file+2))
                this->set_bit(rank-1, file+2, board);

            if (this->on_board(rank+1, file-2))
                this->set_bit(rank+1, file-2, board);
            if (this->on_board(rank-1, file-2))
                this->set_bit(rank-1, file-2, board);
        }
    }
}

void Board::generate_king_attacks()
{
    for (int rank = 0; rank <= MAX_RANK; rank++)
    {
        for (int file = 0; file <= MAX_FILE; file++)
        {
            int square      = RANK_FILE_TO_SQUARE(rank, file);
            uint64_t& board = king_attacks[square];

            if (rank-1 >= 0)
            {
                if (file+1 <= MAX_FILE)
                {
                    this->set_bit(rank-1, file+1, board);
                }
                if (file-1 >= 0)
                {
                    this->set_bit(rank-1, file-1, board);
                }
                this->set_bit(rank-1, file, board);
            }

            if (rank+1 <= MAX_RANK)
            {
                if (file+1 <= MAX_FILE)
                {
                    this->set_bit(rank+1, file+1, board);
                }
                if (file-1 >= 0)
                {
                    this->set_bit(rank+1, file-1, board);
                }
                this->set_bit(rank+1, file, board);
            }
            if (file+1 <= MAX_FILE)
            {
                this->set_bit(rank, file+1, board);
            }
            if (file-1 >= 0)
            {
                this->set_bit(rank, file-1, board);
            }
        }
    }
}

void Board::generate_queen_attacks()
{
    // for each square on the board
    for (int square = 0; square < NUM_SQUARES; square++)
    {
        uint64_t d = this->generate_diagonal_rays(square);
        uint64_t s = this->generate_straight_rays(square);
        queen_attacks[square] |= (d|s);
    }
}

// private function to print a bitboard
void Board::print_bitboard(uint64_t& board)
{
    for (int rank = MAX_RANK; rank >= 0; rank--)
    {
        std::cout << rank << ":  |" << std::bitset<8>((board>>(8*rank)) & 0xff) << "|\n";
    }
    std::cout << "     ABCDEFGH\n";
}

// public function to print the while board
void Board::print_board()
{
    // unicode characters for the chess pieces
    std::string pieces[2][7] = {
        {" ", "\u2659", "\u2656", "\u2658", "\u2657", "\u2654", "\u2655"},
        {" ", "\u265F", "\u265C", "\u265E", "\u265D", "\u265A", "\u265B"}
    };
    for (int rank = MAX_RANK; rank >= 0; rank--)
    {
        std::cout << rank << ": |";
        for (int file = a; file < NUM_FILES; file++)
        {
            bool iswhite = (board[rank][file] & 0x8);
            int index    = iswhite ? 1 : 0;
            int piece    = board[rank][file] & ~(WHITE|BLACK);

            std::cout << pieces[index][piece] << "|";
        }
        std::cout << "\n";
    }
    std::cout << "    A B C D E F G H\n\n";
}
