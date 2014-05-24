#include "./game_state.h"
#include <cstring>
#include <vector>
using std::vector;

int game_state_t::_id = 1;

game_state_t::game_state_t(int width, int height)
    : id(_id++), width(width), height(height), played(0), score(0.), last_x(-1), last_y(-1) {

    field.resize(height);
    for (int i = 0; i < height; ++i) field[i].resize(width);
}

game_state_t::game_state_t(int width, int height, char *data)
    : id(_id++), width(width), height(height), played(0), score(0.), last_x(-1), last_y(-1) {

    field.resize(height);
    for (int i = 0; i < height; ++i) field[i].resize(width);

    for (int i = 0, len = width * height; i < len; ++i) {

        field[i / width][i % width] = data[i];
        if (data[i] != 0) played++;
    }
}

game_state_t::game_state_t(const game_state_t& other) {

    id = _id++;
    width = other.width;
    height = other.height;
    last_x = other.last_x;
    last_y = other.last_y;
    field = other.field;
    played = other.played;
    score = other.score;
}

int game_state_t::play_column(char player, int col) {

    if (col < 0 || col >= width) return -1;

    for (int row = 0; row < height; ++row) {
        if (field[row][col] == 0) {
            field[row][col] = player;
            last_x = row;
            last_y = col;
            played++;
            return 1;
        }
    }

    return -1;
}

int game_state_t::get_width() {
    return width;
}

int game_state_t::get_height() {
    return height;
}

int game_state_t::get(int x, int y) {
    return field[x][y];
}

char game_state_t::winner() {

    int x = last_x, y = last_y;
    char player = field[x][y], score;

    // check left and right
    score = 0;
    for (int col = y; col >= 0 && field[x][col] == player; --col) {
        score++;
    }
    for (int col = y + 1; col < width && field[x][col] == player; ++col) {
        score++;
    }
    if (score >= 4) return player;

    // check up and down
    score = 0;
    for (int row = x; row >= 0 && field[row][y] == player; --row) {
        score++;
    }
    for (int row = x + 1; row < height && field[row][y] == player; ++row) {
        score++;
    }
    if (score >= 4) return player;

    // check diagonal (dx = dy)
    score = 0;
    for (int row = x, col = y; row >= 0 && col >= 0 && field[row][col] == player; --col, --row) {
        score++;
    }
    for (int row = x + 1, col = y + 1; row < height && col < width && field[row][col] == player; ++col, ++row) {
        score++;
    }
    if (score >= 4) return player;

    // check diagonal (dx = -dy)
    score = 0;
    for (int row = x, col = y; row >= 0 && col < width && field[row][col] == player; ++col, --row) {
        score++;
    }
    for (int row = x + 1, col = y - 1; row < height && col >= 0 && field[row][col] == player; --col, ++row) {
        score++;
    }
    if (score >= 4) return player;

    return -1;
}

bool game_state_t::is_full() {
    return played >= width * height;
}

void game_state_t::set_score(float s) {
    score = s;
}

float game_state_t::get_score() {
    return score;
}

char game_state_t::get_last_player() {
    return field[last_x][last_y];
}

int game_state_t::get_id() {
    return id;
}
