#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"
#define SRT "SRT"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
	if (argc < 3) 
	{
		printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
		return EXIT_FAILURE;
	}

	//Creates a queue that loads the PCB's
	dyn_array_t* ready_queue = load_process_control_blocks(argv[1]);
	//Makes sure the array is not null.
	if(ready_queue == NULL)
	{
		printf("Error loading PCB's from file\n");
		return EXIT_FAILURE;
	}
	//Define a result variable.
	ScheduleResult_t result;
	/*
		Makes sure the algorithm we are calling is the correct algorithm.
	*/
	if(strcmp(argv[2], "fcfs") == 0)
	{
		/*
			Makes sure there are no errors in the file that makes the algorithm return an error.
		*/
		if(!first_come_first_serve(ready_queue, &result))
		{
			printf("Error running First Come First Serve\n");
			dyn_array_destroy(ready_queue);
			return EXIT_FAILURE;
		}
	}
	/*
		If we ask for the wrong algorithm or type the wrong thing returns an error saying that string input is not 
		recognized
	*/
	else
	{
		printf("Unkown algorithm: %s\n", argv[2]);
		dyn_array_destroy(ready_queue);
		return EXIT_FAILURE;
	}
	//Prints the final values after the FCFS algorithm is completed.
	printf("Average turn around time: %f\n", result.average_turnaround_time);
	printf("Average waiting time: %f\n", result.average_waiting_time);
	printf("Total time: %ld\n", result.total_run_time);
	dyn_array_destroy(ready_queue);

	return EXIT_SUCCESS;
}
