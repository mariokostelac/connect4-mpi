#include <vector>
#include "./game_state.h"
#include "./communicator_master.h"
using std::vector;

int solve_local(game_state_t& state, const int depth);
int solve_remote(game_state_t& state, const int depth, const int local_depth, CommunicatorMaster *comm);
int reduce_score(game_state_t& state, vector<game_state_t>& children, const int lo, const int hi);
int reduce_tree(vector<game_state_t>& tree, vector<int>& children, const int node = 0);
void get_tree(vector<game_state_t>& container, vector<int>& children, game_state_t& state, const int depth);
void get_nodes(vector<game_state_t *>& container, vector<game_state_t>& tree, vector<int>& children, const int depth, const int node = 0);

