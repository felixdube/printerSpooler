#include "common.h"
#include <unistd.h>

int fd;
int errno;
Buffer* shared_buffer;

int setup_shared_memory(){
    fd = shm_open(MY_SHM, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        printf("shm_open() failed\n");
        exit(1);
    }
    ftruncate(fd, sizeof(Buffer));
}

int attach_shared_memory(){
    shared_buffer = (Buffer*)  mmap(NULL, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shared_buffer == MAP_FAILED){
        printf("mmap() failed\n");
        exit(1);
    }

    return 0;
}

int init_shared_memory() {
    sem_init(&(shared_buffer->full), 1, 0);
    sem_init(&(shared_buffer->empty), 1, 10);

    shared_buffer->queue_position.index = 0;
    sem_init(&(shared_buffer->queue_position.mutex), 1, 1);
}

int main() {
            printf("start1");
    setup_shared_memory();
            printf("start2");
    attach_shared_memory();
            printf("start3");
    init_shared_memory();
    printf("start4");


    Job current_job;
    
    //while (1) {
        ///* wait for a spot to be filled on the buffer */
        //sem_wait(&shared_buffer->full);
        //printf("here");
        // /* wait for the critical section */
        //sem_wait(&shared_buffer -> queue[ shared_buffer->queue_position.index ].mutex);

        // /* get the job from the buffer */
        // current_job = shared_buffer -> queue[ shared_buffer->queue_position.index ];

        // /* release the critical section */
        // sem_post(&shared_buffer -> queue[ shared_buffer->queue_position.index ].mutex);
        // sem_post(&shared_buffer->empty);

        // /* decrement the index of the queue */
        // sem_wait(&shared_buffer->queue_position.mutex);
        // shared_buffer->queue_position.index--;
        // sem_post(&shared_buffer->queue_position.mutex);

        // /* print the job */
        // printf("Printer starts printing %i pages from Buffer[%i]", current_job.data, shared_buffer->queue_position.index);
        // sleep(current_job.data);
        // printf("Printer starts printing %i pages from Buffer[%i]", current_job.data, shared_buffer->queue_position.index);
    //}

    return 0;
}
