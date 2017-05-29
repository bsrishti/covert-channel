#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define size 6ULL*1024ULL*1024ULL

unsigned long long a[size];

/*

writeFile function writes files on shared disk 
1. takes i th position of message array as argument
2. creates a file in /home/sender/files/file[i]
3. calculates time=t1 before beginning to write
4. calculates time=t2 after writing 1024 bytes
5. calculates elapsedTime = t2-t1
6. if(elapsedTime < 4000) continues loop, else returns to main()

*/
double writeFile(int i)
{
	FILE* pFile;

	struct timeval t1, t2;
	double elapsedTime=0;
	char filename[40];
	
	sprintf(filename,"/home/sender/files/file%d.binary",i);
    pFile = fopen(filename, "wb");
   
		gettimeofday(&t1, NULL);
		for (unsigned long long j = 0; j < 1024; ++j)
		{
			if(elapsedTime<4000)
			{
			
			fwrite(a, 1, size, pFile);
			gettimeofday(&t2, NULL);
 			elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;    // sec to ms
    			elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;			
			}
		}
    fclose(pFile);

	return elapsedTime;
}
int main()

{
	sleep(1); // sleep command for syncing results of sender and receiver
	double elapsedTime=0;
	int message[8]={1,1,1,1,0,1,1,1};
	int i=0;
	double end=4000;

	//system("sync; echo 3 > /proc/sys/vm/drop_caches"); // code to clear cache; needs admin permission; needs to be run as root

	for (i=0; i<8; i++)
	{
		printf("%d\n i:",i);
		if(message[i]==1)
		{
			elapsedTime=writeFile(i);
			printf("elapsed1time: %f\n",elapsedTime);

			if(elapsedTime<end)
			{

				// sender & receiver operate in time frames of 4000 ms; the sender is made to sleep for (end-elapsedTime)ms if write() completes early
				sleep((end-elapsedTime)/1000);
			}
			
		}	

}
system("rm /home/sender/files/*");
return 0;

}
