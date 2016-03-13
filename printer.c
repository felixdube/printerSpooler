#include "common.h"
#include <unistd.h>

int fd;
int errno;
Buffer* shared_buffer;
int my_buffer_size;
FILE *myLog;


int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

int setup_shared_memory(){
    fd = shm_open(MY_SHM, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        printf("shm_open() failed\n");
        exit(1);
    }
    ftruncate(fd, sizeof(Buffer) + my_buffer_size*sizeof(int));
}

int attach_shared_memory(){
    shared_buffer = (Buffer*)  mmap(NULL, sizeof(Buffer) + my_buffer_size*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shared_buffer == MAP_FAILED){
        printf("mmap() failed\n");
        exit(1);
    }

    return 0;
}

int init_shared_memory() {
    sem_init(&(shared_buffer->full), 1, 0);
    sem_init(&(shared_buffer->empty), 1, my_buffer_size);
    sem_init(&(shared_buffer->mutex), 1, 1);
    sem_init(&(shared_buffer->log_mutex), 1, 1);

    int i;
    for(i = 0; i < my_buffer_size; i++) {
        shared_buffer->queue[i] = 0;
    }

    shared_buffer->buffer_size = my_buffer_size;
    shared_buffer->index = 0;

    // empty the log
    myLog = fopen("log.txt", "w+");
    fclose(myLog);
}

int main(int argc, char *argv[]) {
    
    // check if the if right number of argument is entered
    if (argc == 2) {

        // check if the client id is a number
        if(!isNumeric(argv[1])){
            printf("Wrong buffer size argument!\n");
            return 0;
        }
        
        // cast the argument into an int
        char *ptr;
        my_buffer_size = strtol(argv[1], &ptr, 10);

        // setup memory
        setup_shared_memory();
        attach_shared_memory();
        init_shared_memory();

        int data;
        int index;
        
        while (1) {

            int current_index;

            // wait for the critical section 
            sem_wait(&shared_buffer->mutex);
            current_index = shared_buffer->index;
            // release the critical section 
            sem_post(&shared_buffer->mutex);


            /* if the buffer is full */
            if(current_index == 0) {
                printf("---\nNo request in buffer, Printer sleeps\n---\n");

                /* save into log */
                //wait for the printing mutex
                sem_wait(&shared_buffer->log_mutex);
                myLog = fopen("log.txt", "a");
                fprintf(myLog, "---\nNo request in buffer, Printer sleeps\n---\n");
                fclose(myLog);
                // release printing mutex 
                sem_post(&shared_buffer->log_mutex);
            }
            // wait for a spot to be filled on the buffer 
            sem_wait(&shared_buffer->full);
            // wait for the critical section 
            sem_wait(&shared_buffer->mutex);

            // get the job from the buffer 
            shared_buffer->index--;
            data = shared_buffer->queue[ shared_buffer->index ];
            index = shared_buffer->index;

            // release the critical section 
            sem_post(&shared_buffer->mutex);
            sem_post(&shared_buffer->empty);



            /* print the job */
            printf("Printer starts printing %i pages from Buffer[%i]\n", data, index);

            /* save into log */
            // wait for the printing mutex
            sem_wait(&shared_buffer->log_mutex);
            myLog = fopen("log.txt", "a");
            fprintf(myLog, "Printer starts printing %i pages from Buffer[%i]\n", data, index);
            fclose(myLog);
            // release printing mutex 
            sem_post(&shared_buffer->log_mutex);

            // wait for the print to be done
            sleep(data);
            printf("Printer done printing %i pages from Buffer[%i]\n", data, index);

            /* save into log */
            // wait for the printing mutex
            sem_wait(&shared_buffer->log_mutex);
            myLog = fopen("log.txt", "a");
            fprintf(myLog, "Printer done printing %i pages from Buffer[%i]\n", data, index);
            fclose(myLog);
            // release printing mutex 
            sem_post(&shared_buffer->log_mutex);
        }

        return 0;
    }
    else {
        printf("Usage: ./printer buffer_size\n");
        return 0;
    }
}
