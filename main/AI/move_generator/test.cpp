//
// Created by jimbo on 17/07/2022.
//

#include "test.h"

int count_property(std::vector<Move>& moves, MoveFlag move_flag) {
    return std::count_if(std::begin(moves), std::end(moves),
                         [&move_flag] (const Move& move) {return move.flag == move_flag; });
}

int test_func(Position& p, unsigned int depth) {
    int nodes_evaluated = 0;

    std::vector<Move> moves = {p.generate_moves()};

    if (depth == 0) {
        return moves.size();
    }

    for (Move move : moves) {
        p.play_move(move, true);

        nodes_evaluated += test_func(p, depth - 1);

        p.undo_move(true);
    }

    return nodes_evaluated;
}

int main() {
    Position p{};

    auto t1 = std::chrono::high_resolution_clock::now();
    int nodes = test_func(p, 5);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t2 - t1;

    std::cout << "time elapsed (ms): " << ms.count() << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Nodes Per Second (N/s): " << double (nodes / ms.count()) * 1000;

    return 0;
}