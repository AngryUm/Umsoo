#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "smemory.h"

main(int argc, char*argv[])
{
   int shm_id;
   void *shm_addr;
   SHM *shm;
   int key;

   int data;

   if (argc != 3) {
      printf("usage: %s key data\n", argv[0]);
      exit(1);
   }

   key = atoi(argv[1]);
   data = atoi(argv[2]);

/*
   FILL IN THIS BLANK
*/
   shm_id = shmget(key, sizeof(SHM), IPC_CREAT | 0666);
   shm_addr = (void *)shmat(shm_id, 0, 0);
   shm = (SHM *)shm_addr;
   shm->sum = data;
   shm->s = 1;

   exit(0);
}
