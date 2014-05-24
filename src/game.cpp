#include "./game.h"
#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <unistd.h>
#include "./timer.h"
#include "./communicator_master.h"
#include "./solver.h"
#include "./slave.h"

int RANK;
int WORLD_SIZE;

unsigned int WIDTH = 7;
unsigned int HEIGHT = 6;
unsigned int DEPTH = 8;

game_state_t state(WIDTH, HEIGHT);
CommunicatorMaster *comm;

void computer_turn() {

    int move = 0;

    Timer t("calculating");
    if (WORLD_SIZE < 2) {
        move = solve_local(state, DEPTH);
    } else {
        move = solve_remote(state, DEPTH, 0.35 * DEPTH, comm);
    }
    t.end(true);

    state.play_column(COMPUTER, move);
}

void draw_field() {

    for (int i = state.get_height() - 1; i >= 0; --i) {
        printf("|");

        for (int j = 0, w = state.get_width(); j < w; ++j) {
            char player = state.get(i, j);
            if (player == 0)    printf(".");
            else                printf("%c", player);
        }

        printf("|\n");
    }

    for (int i = 0, w = state.get_width() + 2; i < w; ++i) {
        printf("-");
    }
    printf("\n");

    printf("|");
    for (int i = 0, w = state.get_width(); i < w; ++i) {
        printf("%d", i);
    }
    printf("|\n");
}

void game() {

    int column;

    while (!state.is_full()) {

        draw_field();

        do {
            printf("Where to, my friend [0..%d]> ", WIDTH - 1);
            fflush(stdout);
            if (scanf("%d", &column) == -1) return;
        } while (state.play_column(PLAYER, column) == -1);

        if (state.winner() == PLAYER) {
            printf("you won!!!\n");
            draw_field();
            return;
        }

        computer_turn();
        if (state.winner() == COMPUTER) {
            printf("you lost!!!\n");
            draw_field();
            return;
        }
    }
}

void parse_args(int argc, char** argv) {

    int c;
    while ((c = getopt(argc, argv, "d:w:h:")) != -1) {
        switch (c) {
            case 'd':
                DEPTH = atoi(optarg);
                break;
            case 'w':
                WIDTH = atoi(optarg);
                break;
            case 'h':
                HEIGHT = atoi(optarg);
                break;
            case '?':
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                break;
            default:
                abort();
        }
    }

}

int main(int argc, char** argv) {

    parse_args(argc, argv);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
    MPI_Comm_size(MPI_COMM_WORLD, &WORLD_SIZE);

    if (RANK == 0) {

        printf("* width: %u\n", WIDTH);
        printf("* height: %u\n", HEIGHT);
        printf("* depth: %u\n", DEPTH);
        printf("\n");

        comm = new CommunicatorMaster(WORLD_SIZE - 1);
        game();
        delete comm;

    } else {

        Slave slave;
        slave.run();
    }

    MPI_Finalize();

    return 0;
}
