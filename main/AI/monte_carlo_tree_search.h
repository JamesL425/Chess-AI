//
// Created by jimbo on 23/11/2022.
//

#ifndef AI_MONTE_CARLO_TREE_SEARCH_H
#define AI_MONTE_CARLO_TREE_SEARCH_H

#include "FCNN.h"
#include <iostream>

Move monte_carlo_tree_search(FCNN& nn, Position& initial_state);

double expand_node(FCNN& nn, Position& init_node);

Move get_best_mc_move(FCNN& nn, Position position);

Move get_best_move(FCNN& nn, Position position);

void train(FCNN& nn, int games);

#endif //AI_MONTE_CARLO_TREE_SEARCH_H
