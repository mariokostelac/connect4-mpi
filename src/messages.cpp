#include "./messages.h"

task_header_t::task_header_t() {}

task_header_t::task_header_t(game_state_t& state, int d) {

    id = state.get_id();
    width = state.get_width();
    height = state.get_height();
    last_x = state.last_x;
    last_y = state.last_y;
    depth = d;
}

