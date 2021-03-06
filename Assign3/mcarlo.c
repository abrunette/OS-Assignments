//Name: Aaron Brunette
//Assignment 3
//CS 433

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

//Global variable
double hit_count = 0;

//Purpose: Generate a random double variable and return it.
double random_double()
{
	return rand() / ((double)RAND_MAX + 1);
}

//Purpose: In a new thread, generate a number of random points.
// If the points fall inside the circle, increments the global
// variable. returns nothing;

//Algorithm: Use a for loop to generate a specified number of
// random points within a 1x1 box. Creates a random x and y
// coordinate. Increments the global variable if the product
// of the too coordinates multiplied and added together is
// less then one.
void *monteCarlo(void* argv) 
{
	//convert argument to int
	char* theConvert = (char*)argv;
	int pointNum = atoi(theConvert);

	double x, y;	//coordinate variables
	int i;		//counter

	srand(time(NULL));	//seed random

	//while there are still points to be made
	//continues to generate coordinates
	for(i = 0; i < pointNum; i++) {
		x = random_double() * 2.0 - 1.0;
		y = random_double() * 2.0 - 1.0;

		//find position, if position is
		//within circle, increment hit_count
		if (sqrt(x*x + y*y) < 1.0)
			++hit_count;
	}

	//exit thread
	pthread_exit(0);	
}

//Purpose: To create a new thread that generates a specified
// number of random coordinates in order to calulate the
// approximate value of pi.
int main(int argc, char *argv[]) 
{
	int tCount = 5;		//number of threads to be created
	pthread_t tid[tCount];	//declare threads
	int i = 0;		//counter

	//check if there is too few arguments
	if(argc == 1) {
		printf("Too few arguments, requires at least 1.");
		return 0;
	}
	//check if there are too many arguments
	else if(argc > 2) {
		printf("Too many arguments, requires 1.");
		return 0;
	}

	//convert argument to double
	double userInput = atoi(argv[1]);

	//create threads
    	for (i = 0; i < tCount; i++)
    	{
		//printf("Creating thread %d ... \n", i);
		pthread_create(&tid[i], NULL, &monteCarlo, (void *)argv[1]);
    	}

    	//join and wait for all threads
    	for (i = 0; i < tCount; i++)
    	{
		phread_join(tid[i], NULL);
		//printf("Completed join with thread %d \n", i);
    	}

	//estimate pi based on the number of total hits in the circle by
	//the total number of points generated
	double pi = ((4 * hit_count) / userInput) / tCount;

	printf("\nPi is approximately: %f\n", pi);

	return 0;
}
