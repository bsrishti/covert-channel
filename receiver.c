#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
readFile function reads files fromk fixed locations on shared disk
fixed location = ./files/test.binary

*/
double readFile()
{

    struct stat fileSize;
    char *buffGB, *hashString;

    FILE *fp;
    struct timeval t1, t2;
    double elapsedTime;

    char fname[40]="./files/test.binary";

    // handle file, open file, and read in binary form
    fp = fopen(fname, "rb");
    if (fp == NULL)
    {
        printf("Cannot open %s for reading\n", fname);
        exit(1);
    }

    while (fname[strlen(fname) - 1] == '\n')
    {
        fname[strlen(fname) -1] = '\0';
        printf("read1");
    }

    size_t size=0;
    stat(fname, &fileSize);
    size = fileSize.st_size;
    buffGB = malloc(size); // allocate memory for one gb

    double interval=0;
    gettimeofday(&t1,NULL);
       
    if(elapsedTime<4000) //6000 for cloudlab
    {       
	fread(buffGB, sizeof(*buffGB), size, fp);
        gettimeofday(&t2,NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;   
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    }

    free(buffGB);

    fclose (fp); // close files
    return elapsedTime;
}

int main()
{
        double elapsedTime=0;
        int i=0;
        double end=4000;
	int message[8]={0,0,0,0,0,0,0,0};
	
       // system("echo 3 > /proc/sys/vm/drop_caches"); // command to clear cache; requires root privileges

        for (i=0; i<8; i++)
        {
        printf("i : %d ",i);

        elapsedTime=readFile();
        printf("%f\n",elapsedTime);
        if(elapsedTime<end)
        {
           sleep((end-elapsedTime)/1000);
        }
    
        if(elapsedTime>1990)//6300 for cloudlab
        message[i]=1;
        else
        message[i]=0;
        }

        printf("OUTPUT:");
        for(i=0;i<8;i++)
        printf("%d",message[i]);


return 0;

}

