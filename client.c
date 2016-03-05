#include "common.h"

int fd;
Buffer* shared_buffer;

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

int main() {
            printf("start client1");
    setup_shared_memory();
            printf("start client2");
    attach_shared_memory();

        printf("start client");


    int ID = 2;

    Job current_job;
    current_job.data = 1;
    sem_init(&(current_job.mutex), 1, 1);

    /* if the buffer is full */
    if(shared_buffer->queue_position.index == 10) {
        printf("Client %i has %i page(s) to print, buffer full, sleeps", ID, current_job.data);
        
        /* wait for an empty spot on the buffer */
        sem_wait(&shared_buffer->empty);
        /* wait for the index in the queue to be free */
        sem_wait(&shared_buffer->queue_position.mutex);
        /* wait on the critical section*/
        sem_wait(&shared_buffer -> queue[ shared_buffer->queue_position.index ].mutex);

        /* place the job on the queue */
        printf("Client %i wakes up, puts request in Buffer[%i]", ID, shared_buffer->queue_position.index);
        shared_buffer->queue[ shared_buffer->queue_position.index ] = current_job;

        /* release the critical section */
        sem_post(&shared_buffer -> queue[ shared_buffer->queue_position.index ].mutex);
        /* signal that one more spot have been filled */
        sem_post(&shared_buffer->full);

        /* decrement the index of the queue */
        shared_buffer->queue_position.index++;
        sem_post(&shared_buffer->queue_position.mutex);
    }

    /* if the buffer is not full */
    else {
        printf("there");
        /* wait for an empty spot on the buffer */
        sem_wait(&shared_buffer->empty);
        /* wait for the index of the queue to be free */
        sem_wait(&shared_buffer->queue_position.mutex);
        /* wait for the critical section */
        sem_wait(&shared_buffer -> queue[ shared_buffer->queue_position.index ].mutex);

        /* place the job on the queue */
        printf("Client %i has %i page(s) to print, puts request in Buffer[%i]", ID, current_job.data, shared_buffer->queue_position.index);
        shared_buffer->queue[ shared_buffer->queue_position.index ] = current_job;

        /* release the critical section */
        sem_post(&shared_buffer -> queue[ shared_buffer->queue_position.index ].mutex);
        /* signal that one more spot have been filled */
        sem_post(&shared_buffer->full);

        /* decrement the index of the queue */
        shared_buffer->queue_position.index++;
        sem_post(&shared_buffer->queue_position.mutex);
    }



    return 0;
}
