#include <stdio.h>
#include <stdlib.h>
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

   int sum = 0;

   if (argc != 2) {
      printf("usage: %s key\n", argv[0]);
      exit(1);
   }

   key = atoi(argv[1]);

/*
   FILL IN THIS BLANK
*/
   shm_id = shmget(key, sizeof(SHM), IPC_CREAT|0666);
   shm_addr = (void *)shmat(shm_id, 0, 0);
   shm = (SHM *)shm_addr;

   shm->s = 0;
   do{
	while(!shm->s);
	if(shm->sum == -1) break;
	shm->s = 0;
	sum += shm->sum;
	printf("총 합: %d\n", sum);
   }while(1);

   shmdt((void *)shm_addr);
   shmctl(shm_id, IPC_RMID, 0);

   exit(0);
}
