#ifndef MESSAGES_H
#define MESSAGES_H
#include "./game_state.h"
#define MSG_TASK_HEADER 1
#define MSG_TASK_DATA 2
#define MSG_TASK_RESULT 3
#define MSG_READY 4
#define MSG_STOP 5

struct task_header_t {
    int id;
    int width;
    int height;
    int last_x;
    int last_y;
    int depth;

    task_header_t();
    task_header_t(game_state_t& state, int depth);
};
#endif
