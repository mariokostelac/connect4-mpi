#include <mpi.h>
#include "./messages.h"

class Slave {
 public:
     Slave();
     void run();

 private:
     bool have_header;
     bool have_state;
     task_header_t header;
     game_state_t state;

     void send_ready();
     void calculate_and_send();
};
