#include "CNN.h"
#include <iostream>
#include <chrono>
#include "monte_carlo_tree_search.h"

Outcome output_game(FCNN nn) {
    Position position{};

    while (!(position.is_game_over())) {
        Move best_move;

        if (position.white_to_play()) {
            best_move = monte_carlo_tree_search(nn, position);
        } else {
            best_move = monte_carlo_tree_search(nn, position);
        }

        std::cout << "Neural Net chose: " << best_move.as_string() << "\n";

        position.play_move(best_move, false);

        std::cout << position.as_string() << "\n";
    }

    std::cout << position.game_over_txt() << "\n";

    return position.get_game_outcome();
}

Outcome play_game(CNN n1, CNN n2, int depth) {
    Position position{};

    while (!(position.is_game_over())) {
        if (position.white_to_play()) {
            position.play_move(n1.get_best_move(position, depth), false);
        } else {
            position.play_move(n2.get_best_move(position, depth), false);
        }
    }

    return position.get_game_outcome();
}

CNN get_best(CNN n1, CNN n2, int n_games) {
    Outcome outcome;

    const int epsilon = 50;
    const int win_factor = 1;

    for (int i = 0; i < n_games; ++i) {
//        if (i % 1000 == 999) {
//            outcome = (i % 2 == 0) ? output_game(n1, n2, 0) : output_game(n2, n1, 0);
//        }
//        else {
//            outcome = (i % 2 == 0) ? play_game(n1, n2, 0) : play_game(n2, n1, 0);
//        }

        outcome = (i % 2 == 0) ? play_game(n1, n2, 0) : play_game(n2, n1, 0);


        switch(outcome) {
            case BLACK_WIN:
            case DRAW:
                if (generate_random_int(0, 100) < epsilon) {
                    (i % 2 == 0) ? n1.randomise_values() : n2.randomise_values();
                } else {
                    (i % 2 == 0) ? n1.slightly_alter_other(n2, win_factor) : n2.slightly_alter_other(n1, win_factor);
                }
                break;

            case WHITE_WIN:
                if (generate_random_int(0, 100) < epsilon) {
                    (i % 2 == 0) ? n2.slightly_alter_other(n2, win_factor) : n1.slightly_alter_other(n1, win_factor);
                } else {
                    (i % 2 == 0) ? n2.slightly_alter_other(n1, win_factor) : n1.slightly_alter_other(n2, win_factor);
                }
                break;

            case NO_OUTCOME:
                break;
        }

        if (i % 20 == 19) {
            std::cout << i + 1 << " games played\n";
        }

        if (i % 100 == 99) {
            switch (outcome) {
                case BLACK_WIN:
                case DRAW:
                    n1.write_to_file("cnn.txt");
                    break;

                case WHITE_WIN:
                    n2.write_to_file("cnn.txt");
                    break;

                case NO_OUTCOME:
                    break;
            }
        }
    }

    if (n_games % 2 == 0) {
        return (outcome == BLACK_WIN) ? n1 : n2;
    }

    return (outcome == BLACK_WIN) ? n2 : n1;
}

std::string file_to_string(const std::string& file_name) {
    std::ifstream file(file_name);
    return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

int main() {
    std::vector<int> layer_structure {64, 16, 32, 32, 16, 1};
    FCNN nn {layer_structure, &tanh};

    train(nn, 100000);

    std::cout << "finished";

    output_game(nn);

    nn.write_to_file("nn_final.txt");

    return 0;
}
