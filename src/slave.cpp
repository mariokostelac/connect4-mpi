#include "./slave.h"
#include <cstring>
#include "./solver.h"

void draw_field_d(game_state_t state) {

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
}

Slave::Slave(): have_header(false), have_state(false), state(game_state_t(0, 0)) {
    send_ready();
}

void Slave::send_ready() {

    int ready = 1;
    MPI_Send(&ready, 1, MPI_INT, 0, MSG_READY, MPI_COMM_WORLD);
}

void Slave::run() {

    MPI_Status status;

    while (true) {

        // wait for any message (just in case next message is STOP message)
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == MSG_STOP) {
            break;
        }

        if (!have_header) {
            MPI_Probe(MPI_ANY_SOURCE, MSG_TASK_HEADER, MPI_COMM_WORLD, &status);
        } else if (!have_state) {
            MPI_Probe(MPI_ANY_SOURCE, MSG_TASK_DATA, MPI_COMM_WORLD, &status);
        }

        if (status.MPI_TAG == MSG_TASK_HEADER) {

            char* msg = new char[sizeof(task_header_t)];
            MPI_Recv(msg, sizeof(task_header_t), MPI_BYTE, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

            memcpy(&header, msg, sizeof(task_header_t));
            delete[] msg;

            have_header = true;

        } else if (status.MPI_TAG == MSG_TASK_DATA) {

            char *msg = new char[header.width * header.height];
            MPI_Recv(msg, header.width * header.height, MPI_BYTE, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

            state = game_state_t(header.width, header.height, msg);
            delete[] msg;

            state.last_x = header.last_x;
            state.last_y = header.last_y;

            have_state = true;
        }

        calculate_and_send();
    }
}

void Slave::calculate_and_send() {

    if (have_header && have_state) {

        solve_local(state, header.depth);

        float score = state.get_score();
        MPI_Send(&score, 1, MPI_FLOAT, 0, MSG_TASK_RESULT, MPI_COMM_WORLD);
        send_ready();

        have_header = have_state = false;
    }
}
