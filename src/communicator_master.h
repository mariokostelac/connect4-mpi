#ifndef COMMUNICATOR_MASTER_H
#define COMMUNICATOR_MASTER_H
#include "./game_state.h"
#include <vector>
#include <queue>
#include <utility>
#include "./messages.h"
using std::queue;
using std::pair;
using std::vector;

class CommunicatorMaster {
 private:
     int slaves;
     vector<int> task;
     queue<pair<game_state_t *, int>> q;
     vector<game_state_t *> state;

     void send_tasks();
     void check_messages();

 public:
     CommunicatorMaster(int slaves);
     ~CommunicatorMaster();
     void add_task(game_state_t& state, int depth);
     void wait();
};
#endif
