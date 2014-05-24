#include "./solver.h"
#include <cstdio>
#include <vector>
#include <queue>

using std::vector;

int solve_local(game_state_t& state, const int depth) {

    if (depth == 0) {

        char winner = state.winner();

        if (winner == COMPUTER)     state.set_score(1);
        else if (winner == PLAYER)  state.set_score(-1);

        return -1;
    } else {

        char winner = state.winner();

        if (winner == COMPUTER)     state.set_score(1);
        else if (winner == PLAYER)  state.set_score(-1);

        // prune this branch, we it is lose or win position
        if (winner > 0) return -1;

        // determine next player
        char next_player = state.get_last_player() == PLAYER ? COMPUTER : PLAYER;

        int children = 0;
        float score_sum = 0;

        float best_score = -1;
        int best_move = -1;

        bool child_decides = false;

        for (int i = 0, w = state.get_width(); i < w; ++i) {

            auto child = game_state_t(state);
            if (child.play_column(next_player, i) == -1) continue;

            children++;

            solve_local(child, depth - 1);

            if (child.get_score() > best_score) {
                best_score = child.get_score();
                best_move = i;
            }

            if (next_player == PLAYER && child.get_score() == -1 && child.winner() == PLAYER) {
                child_decides = true;
                state.set_score(-1);
            } else if (next_player == COMPUTER && child.get_score() >= 1 && child.winner() == COMPUTER) {
                child_decides = true;
                state.set_score(1);
            }

            score_sum += child.get_score();
        }

        if (!child_decides)  state.set_score(score_sum / children);

        return best_move;
    }
}

int solve_remote(game_state_t& state, const int depth, const int local_depth, CommunicatorMaster *comm) {

    vector<game_state_t> tree;
    vector<game_state_t *> leafs;
    vector<int> children;

    get_tree(tree, children, state, depth - local_depth);
    get_nodes(leafs, tree, children, depth - local_depth);

    for (int i = 0, len =  leafs.size(); i < len; ++i) {
        comm->add_task(*(leafs[i]), local_depth);
    }

    comm->wait();

    int res = reduce_tree(tree, children);

    return res;
}

int reduce_score(game_state_t& state, vector<game_state_t>& children, int lo, int hi) {

    // determine next player
    char next_player = state.get_last_player() == PLAYER ? COMPUTER : PLAYER;

    float score_sum = 0;
    bool child_decides = false;

    float best_score = -1;
    int best_move = -1;

    for (int i = lo; i < hi; ++i) {

        if (children[i].get_score() > best_score) {
            best_score = children[i].get_score();
            best_move = i;
        }

        if (next_player == PLAYER && children[i].get_score() == -1 && children[i].winner() == PLAYER) {
            child_decides = true;
            state.set_score(-1);
        } else if (next_player == COMPUTER && children[i].get_score() >= 1 && children[i].winner() == COMPUTER) {
            child_decides = true;
            state.set_score(1);
        }

        score_sum += children[i].get_score();
    }

    if (!child_decides)  state.set_score(score_sum / (hi - lo));

    return best_move;
}

int reduce_tree(vector<game_state_t>& tree, vector<int>& children, const int node) {

    // leaf or out of tree
    if (2 * node >= (int) children.size()) return -1;

    int lo = children[2 * node], hi = children[2 * node + 1];
    for (int i = lo; i < hi; ++i) {
        reduce_tree(tree, children, i);
    }

    int best_move = reduce_score(tree[node], tree, lo, hi);
    return tree[best_move].last_y;
}

void get_tree(vector<game_state_t>& tree, vector<int>& children, game_state_t& state, const int target_depth) {

    std::queue<std::pair<game_state_t, int>> q;
    q.push(std::make_pair(state, 0));
    tree.push_back(state);

    int width = state.get_width();

    while (q.size()) {

        auto pair = q.front();
        game_state_t& state = pair.first;
        int depth = pair.second;
        q.pop();

        if (depth >= target_depth) continue;

        // determine next player
        char next_player = state.get_last_player() == PLAYER ? COMPUTER : PLAYER;

        children.push_back(tree.size());

        for (int i = 0; i < width; ++i) {
            auto child = game_state_t(state);

            if (child.play_column(next_player, i) != -1) {
                q.push(std::make_pair(child, depth + 1));
                tree.push_back(child);
            }
        }

        children.push_back(tree.size());
    }
}

void get_nodes(vector<game_state_t *>& container, vector<game_state_t>& tree, vector<int>& children,
        const int depth, const int node) {

    if (depth <= 0 || 2 * node >= (int) tree.size()) {
        container.push_back(&tree[node]);
        return;
    }

    for (int i = children[2 * node]; i < children[2 * node + 1]; ++i) {
        get_nodes(container, tree, children, depth - 1, i);
    }
}
