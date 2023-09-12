//
// Created by jimbo on 17/07/2022.
//

#ifndef MOVE_GENERATOR_TEST_H
#define MOVE_GENERATOR_TEST_H

#include "position.h"
#include <iostream>
#include <chrono>
#include <iomanip>

#endif //MOVE_GENERATOR_TEST_H

//    const std::array<std::string, 15>  reachable_positions {
//        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", // A00 Initial Position
//        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1", // B00 King's Pawn Game
//        "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2", // B20 Sicilian Defense
//        "rn2kbnr/pp1qpppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 5", // B52 Sicilian Defense: Moscow Variation, Main Line
//        "r1bqkbnr/1ppp1ppp/p1n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4", // C70 Ruy Lopez: Morphy Defense
//        "r1bqk2r/pppp1ppp/2n2n2/4p3/1b1PP3/2N2N2/PPP2PPP/R1BQKB1R w KQkq - 1 5", // C47 Four Knights Game: Scotch Variation
//        "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/3P4/PPP2PPP/RNBQK1NR w KQkq - 1 4", // C24 Bishop's Opening: Berlin Defense
//        "rnbqkbnr/pp2pppp/8/2ppP3/3P4/8/PPP2PPP/RNBQKBNR w KQkq - 0 4", // B12 Caro-Kann Defense: Advance Variation, Botvinnik-Carls Defense
//        "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1", // A40 Queen's Pawn Game
//        "rnbqkbnr/ppp2ppp/4p3/3p4/2PP4/8/PP2PPPP/RNBQKBNR w KQkq - 0 3", // D30 Queen's Gambit Declined
//        "rn1qkbnr/ppp2ppp/4p3/3p1b2/3P1B2/5N2/PPPNPPPP/R2QKB1R b KQkq - 1 4", // D00 Queen's Pawn Game: Accelerated London System
//        "rnbqkb1r/ppp1pppp/5n2/3p4/8/5NP1/PPPPPPBP/RNBQK2R b KQkq - 2 3", // A05 King's Indian Attack
//        "rnbqkb1r/pppppppp/5n2/8/2P5/8/PP1PPPPP/RNBQKBNR w KQkq - 1 2", // A15 English Opening: Anglo-Indian Defense
//        "rnbqkb1r/pppp1ppp/5n2/8/2BpP3/5N2/PPP2PPP/RNBQK2R b KQkq - 1 4", // C43 Bishop's Opening: Urusov Gambit
//        "r1bq1rk1/1pp2pb1/p1np1npp/4p3/B3P2B/2PP1N2/PP1N1PPP/R2QK2R w KQ - 2 10", // C77 Ruy Lopez: Morphy Defense, Anderssen Variation
//        // World Chess Championship Game 2019 - Carlsen M. vs Caruana F.
//    };
//    const std::array<std::string, 15>  reachable_positions_strs {
//            "A00 Initial Position",
//            "B00 King's Pawn Game",
//            "B20 Sicilian Defense",
//            "B52 Sicilian Defense: Moscow Variation, Main Line",
//            "C70 Ruy Lopez: Morphy Defense",
//            "C47 Four Knights Game: Scotch Variation",
//            "C24 Bishop's Opening: Berlin Defense",
//            "B12 Caro-Kann Defense: Advance Variation, Botvinnik-Carls Defense",
//            "A40 Queen's Pawn Game",
//            "D30 Queen's Gambit Declined",
//            "D00 Queen's Pawn Game: Accelerated London System",
//            "A05 King's Indian Attack",
//            "A15 English Opening: Anglo-Indian Defense",
//            "C43 Bishop's Opening: Urusov Gambit",
//            "C77 Ruy Lopez: Morphy Defense, Anderssen Variation - World Chess Championship Game 2019 - Carlsen M. vs Caruana F."
//    };


//    for (int i = 0; i < 4; ++i) {
//        std::cout << random_positions[i] << "\n";
//        Position position {random_positions[i]};
//        std::vector<Move> possible_moves = position.generate_moves();
//        int j = 1;
//        for (auto move : possible_moves) {
//            std::cout << move.as_string() << " ";
//            if (j % 4 == 0) {
//                std::cout << "\n";
//            }
//            ++j;
//        }
//        std::cout << std::endl;
//    }