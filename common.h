#ifndef _INCLUDE_COMMON_H_
#define _INCLUDE_COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// from `man shm_open`
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include <semaphore.h>

#define MY_SHM "/JIT"

typedef struct {
    sem_t mutex;
    int data;
} Job;

typedef struct {
	sem_t mutex;
	int index;
} Index;

typedef struct {
	sem_t empty;
	sem_t full;
	Job queue[10];
	Index queue_position;
} Buffer;

#endif //_INCLUDE_COMMON_H_

