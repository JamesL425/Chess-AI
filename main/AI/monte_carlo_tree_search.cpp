//
// Created by jimbo on 23/11/2022.
//

#include "monte_carlo_tree_search.h"

Move monte_carlo_tree_search(FCNN& nn, Position& initial_state) {
    Eigen::VectorXd diff(1, 1);

    diff[0] = nn.evaluate(position_to_matrix(initial_state)) - expand_node(nn, initial_state);

    std::vector<Eigen::VectorXd> run = nn.generate_run(position_to_matrix(initial_state));

    nn.backpropagate(run, -diff);

    return get_best_mc_move(nn, initial_state);
}

double expand_node(FCNN& nn, Position& init_node) {
    Position pos = init_node;
    Move move;

    int counter = 0;
    while(!pos.is_game_over() && counter < 5) {
        move = get_best_move(nn, pos);
        pos.play_move(move, false);
        ++counter;
    }

    int game_over_val = pos.is_game_over();

    if (game_over_val == 0) {
        return nn.evaluate(position_to_matrix(pos));
    }

    if (game_over_val == 1) {
        return 0.0;
    }

    return (pos.white_to_play()) ? -1.0 : 1.0;
}

Move get_best_mc_move(FCNN& nn, Position position) {
    std::vector<Move> possible_moves = position.generate_moves();
    std::vector<double> evals{};

    for (Move move : possible_moves) {
        position.play_move(move, false);

        int game_over_val = position.is_game_over();
        switch (game_over_val) {
            case 0:
                evals.emplace_back((nn.evaluate(position_to_matrix(position)) + expand_node(nn, position) * 2) / 3);
                break;
            case 1:
                evals.emplace_back(0.0);
                break;
            case 2:
                evals.emplace_back((position.white_to_play()) ? -1.0 : 1.0);
                break;
            default:
                break;
        }

        position.undo_move(false);
    }

    long long best_move_loc = ((position.white_to_play()) ? std::max_element(evals.begin(), evals.end()) : std::min_element(evals.begin(), evals.end())) - evals.begin();

    return possible_moves[best_move_loc];
}

Move get_best_move(FCNN& nn, Position position) {
    std::vector<Move> possible_moves = position.generate_moves();
    std::vector<double> evals{};

    for (Move move : possible_moves) {
        position.play_move(move, false);

        int game_over_val = position.is_game_over();
        switch (game_over_val) {
            case 0:
                evals.emplace_back(nn.evaluate(position_to_matrix(position)));
                break;
            case 1:
                evals.emplace_back(0.0);
                break;
            case 2:
                evals.emplace_back((position.white_to_play()) ? -1.0 : 1.0);
                break;
            default:
                break;
        }

        position.undo_move(false);
    }

    long long best_move_loc = ((position.white_to_play()) ? std::max_element(evals.begin(), evals.end()) : std::min_element(evals.begin(), evals.end())) - evals.begin();

    return possible_moves[best_move_loc];
}

void train(FCNN& nn, int games) {
    for (int game = 0; game < games; ++game) {
        Position position{};
        Move move{};

        while (!position.is_game_over()) {
            move = monte_carlo_tree_search(nn, position);
            position.play_move(move, false);
        }

        if (game % 5 == 0) {
            std::cout << game << "\n";
            nn.write_to_file("nn.txt");
        }
    }
}