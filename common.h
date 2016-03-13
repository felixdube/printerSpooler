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
	sem_t empty;
	sem_t full;
	sem_t mutex;
	sem_t log_mutex;
	int buffer_size;
	int index;
	int queue[0];
} Buffer;

#endif //_INCLUDE_COMMON_H_

