#ifndef LAB2_SRC_GAME_STATE_H_
#define LAB2_SRC_GAME_STATE_H_
#define COMPUTER 'o'
#define PLAYER 'x'
#include <vector>

class game_state_t {

 private:
    static int _id;
    int id;
    int width;
    int height;
    int played;
    float score;
    std::vector<std::vector<char>> field;

 public:
    int last_x;
    int last_y;

    game_state_t(int width, int height);
    game_state_t(int width, int height, char* field);
    game_state_t(const game_state_t& other);
    int play_column(char player, int col);
    int get_width();
    int get_height();
    int get(int x, int y);
    char winner();
    bool is_full();
    void set_score(float score);
    float get_score();
    char get_last_player();
    int get_id();
};
#endif  // LAB2_SRC_GAME_STATE_H_
