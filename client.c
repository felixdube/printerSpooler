#include "common.h"
#include <ctype.h>
#include <stdlib.h>

int fd;
Buffer* shared_buffer;
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
    fd = shm_open(MY_SHM, O_RDWR, 0666);
    if(fd == -1){
        printf("shm_open() failed\n");
        exit(1);
    }
}

int attach_shared_memory(){
    shared_buffer = (Buffer*) mmap(NULL, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shared_buffer == MAP_FAILED){
        printf("mmap() failed\n");
        exit(1);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    
    // check if the if right number of argument is entered
    if (argc == 3) {

        // check if the client id is a number
        if(!isNumeric(argv[1])){
            printf("Wrong client ID argument!\n");
            return 0;
        }

        // check if the number of pages is a number
        if(!isNumeric(argv[2])){
            printf("Wrong number of pages argument!\n");
            return 0;
        }

        // setup memory
        setup_shared_memory();
        attach_shared_memory();

        // cast the arguments to ints
        char *ptr;
        int ID = strtol(argv[1], &ptr, 10);
        int data = strtol(argv[2], &ptr, 10);

        int current_index;
        int current_buffer_size;

        /* wait for the critical section */
        sem_wait(&shared_buffer->mutex);
        current_index = shared_buffer->index;
        current_buffer_size = shared_buffer->buffer_size;
        /* release the critical section */
        sem_post(&shared_buffer->mutex);


        /* if the buffer is full */
        if(current_index == current_buffer_size) {
            printf("Client %i has %i page(s) to print, buffer full, sleeps\n", ID, data);

            /* save into log */
            //wait for the printing mutex
            sem_wait(&shared_buffer->log_mutex);
            myLog = fopen("log.txt", "a");
            fprintf(myLog, "Client %i has %i page(s) to print, buffer full, sleeps\n", ID, data);
            fclose(myLog);
            // release printing mutex 
            sem_post(&shared_buffer->log_mutex);
            
            /* wait for an empty spot on the buffer */
            sem_wait(&shared_buffer->empty);
            /* wait for the critical section */
            sem_wait(&shared_buffer->mutex);
            /* place the job on the queue */
            printf("Client %i wakes up, puts request in Buffer[%i]\n", ID, shared_buffer->index);

            /* save into log */
            //wait for the printing mutex
            sem_wait(&shared_buffer->log_mutex);
            myLog = fopen("log.txt", "a");
            fprintf(myLog, "Client %i wakes up, puts request in Buffer[%i]\n", ID, shared_buffer->index);
            fclose(myLog);
            // release printing mutex 
            sem_post(&shared_buffer->log_mutex);


            // place data in memory
            shared_buffer->queue[ shared_buffer->index ] = data;

            /* increment the index of the queue */
            shared_buffer->index++;

            /* release the critical section */
            sem_post(&shared_buffer->mutex);
            /* signal that one more spot have been filled */
            sem_post(&shared_buffer->full);
        }

        /* if the buffer is not full */
        else {
            /* wait for an empty spot on the buffer */
            sem_wait(&shared_buffer->empty);
            /* wait for the critical section */
            sem_wait(&shared_buffer->mutex);

            /* place the job on the queue */
            printf("Client %i has %i page(s) to print, puts request in Buffer[%i]\n", ID, data, shared_buffer->index);
            shared_buffer->queue[ shared_buffer->index ] = data;

            /* save into log */
            //wait for the printing mutex
            sem_wait(&shared_buffer->log_mutex);
            myLog = fopen("log.txt", "a");
            fprintf(myLog, "Client %i has %i page(s) to print, puts request in Buffer[%i]\n", ID, data, shared_buffer->index);
            fclose(myLog);
            // release printing mutex 
            sem_post(&shared_buffer->log_mutex);

            /* increment the index of the queue */
            shared_buffer->index++;

            /* release the critical section */
            sem_post(&shared_buffer->mutex);
            /* signal that one more spot have been filled */
            sem_post(&shared_buffer->full);

        }
    }
    else {
        printf("Usage: ./client client_id number_pages\n");
        return 0;
    }



    return 0;
}
