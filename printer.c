#include "common.h"
#include <unistd.h>

int fd;
int errno;
Buffer* shared_buffer;
int my_buffer_size;

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

    int i;
    for(i = 0; i < my_buffer_size; i++) {
        shared_buffer->queue[i] = 0;
    }

    shared_buffer->buffer_size = my_buffer_size;
    shared_buffer->index = 0;
}

int main() {
    my_buffer_size = 20;

    setup_shared_memory();
    attach_shared_memory();
    init_shared_memory();

    printf("%i  %i \n", shared_buffer->buffer_size, shared_buffer->index);

    int data;
    int index;
    
    while (1) {

        /* wait for a spot to be filled on the buffer */
        sem_wait(&shared_buffer->full);
        /* wait for the critical section */
        sem_wait(&shared_buffer->mutex);

        /* get the job from the buffer */
        shared_buffer->index--;
        data = shared_buffer->queue[ shared_buffer->index ];
        index = shared_buffer->index;

        /* release the critical section */
        sem_post(&shared_buffer->mutex);
        sem_post(&shared_buffer->empty);



        /* print the job */
        printf("Printer starts printing %i pages from Buffer[%i]\n", data, index);
        sleep(data);
        printf("Printer done printing %i pages from Buffer[%i]\n", data, index);
    }

    return 0;
}
