#include "./communicator_master.h"
#include "mpi.h"

CommunicatorMaster::CommunicatorMaster(int slaves): slaves(slaves) {
    task.resize(slaves, -1);
    state.resize(slaves);
}

CommunicatorMaster::~CommunicatorMaster() {

    for (int i = 0; i < slaves; ++i) {
        int stop = 1;
        MPI_Request req;
        MPI_Isend(&stop, 1, MPI_INT, i + 1, MSG_STOP, MPI_COMM_WORLD, &req);
    }
}

void CommunicatorMaster::add_task(game_state_t& state, int depth) {
    q.push(pair<game_state_t *, int>(&state, depth));
}

void CommunicatorMaster::wait() {

    int busy = 0;
    do {

        check_messages();
        send_tasks();

        busy = 0;
        for (int i = 0; i < slaves; ++i) {
            busy += task[i] > 0;
        }
    } while (busy > 0 || q.size() > 0);
}

void CommunicatorMaster::send_tasks() {

    for (int i = 0; i < slaves && q.size(); ++i) {

        // worker is available
        if (task[i] == 0) {

           pair<game_state_t *, int> job = q.front();
           q.pop();
           task_header_t header(*(job.first), job.second);

           MPI_Send(&header, sizeof(header), MPI_BYTE, i + 1, MSG_TASK_HEADER, MPI_COMM_WORLD);

           // sending the data
           game_state_t* game_state = job.first;
           char *field = new char[game_state->get_width() * game_state->get_height()];

           for (int i = 0; i < game_state->get_height(); ++i) {
               for (int j = 0; j < game_state->get_width(); ++j) {
                   field[i * game_state->get_width() + j] = game_state->get(i, j);
               }
           }

           // blocking call because of freeing the memory
           MPI_Send(field, game_state->get_width() * game_state->get_height(), MPI_BYTE, i + 1, MSG_TASK_DATA, MPI_COMM_WORLD);

           delete[] field;

           task[i] = header.id;
           state[i] = job.first;
        }
    }
}

void CommunicatorMaster::check_messages() {

    int flag;
    MPI_Status status;

    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {

        if (status.MPI_TAG == MSG_READY) {

            int msg;
            MPI_Recv(&msg, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
            task[status.MPI_SOURCE - 1] = 0;
            state[status.MPI_SOURCE - 1] = 0;

        } else if (status.MPI_TAG == MSG_TASK_RESULT) {

            float score;
            MPI_Recv(&score, 1, MPI_FLOAT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
            state[status.MPI_SOURCE - 1]->set_score(score);
        }

        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
}
