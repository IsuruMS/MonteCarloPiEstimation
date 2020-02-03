#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<stdlib.h>

long insideCircle = 0;
long totalPoints = 0;      //initial number of points generated is 100
int pointsPerThread;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *Algorithm()
{

    long insideCirclrInThread = 0;  
    long pointsInThread = 0;
    unsigned int points = rand();

    for(int i=0; i<pointsPerThread; i++)
    {
        double xCord = rand_r(&points) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double yCord = rand_r(&points) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        pointsInThread++;
        //printf("insideCircle = %ld\n", pointsInThread);

        //checking for which of the above points are inside the circle with the radius 1 units
        if(xCord*xCord + yCord*yCord <1)
        {
            insideCirclrInThread++;
        }
    }
    pthread_mutex_lock(&lock);
    insideCircle += insideCirclrInThread;
    totalPoints += pointsInThread;
    pthread_mutex_unlock(&lock);
}

//main fucntion gets two inputs
//number of threads and number of points in the begining

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: ./<compiled_name> <initial no. of threads>\n");
        exit(1);
    }

    int threadsNum = atoi(argv[1]);

    FILE *outFile;
    outFile = fopen("results2.txt" , "w+");

    FILE *outFile1;
    outFile1 = fopen("TotalPoints2.txt" , "w+");

        fprintf(outFile,"Number of Threads = %d\n\n", threadsNum);
        for(int j=0; j<10000; j++)       //incrementing the number of made points
        {        
            insideCircle = 0;
            totalPoints = 0;

            pointsPerThread++;

            time_t start = clock();

            pthread_t *threads = malloc(threadsNum * sizeof(pthread_t));

            pthread_attr_t attribute;
            pthread_attr_init(&attribute);

            for(int k=0; k < threadsNum; k++)
            {
                pthread_create(&threads[k], &attribute, Algorithm, (void *) NULL);
            }

            for(int k=0; k < threadsNum; k++)
            {
                pthread_join(threads[k], NULL);
            }
            time_t end = clock();
            pthread_mutex_destroy(&lock);
            free(threads);

            long double pi = 4 * (long double)insideCircle / (long double)totalPoints;
            float timeTaken = ((end - start)/CLOCKS_PER_SEC) * 1000;
            
            fprintf(outFile,"%Lf\n", pi);
            fprintf(outFile1,"%ld\n", totalPoints);

        }
        printf("Done. Check the files results2.txt and TotalPoints2.txt made in the directory.\n");
}