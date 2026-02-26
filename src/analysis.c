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

	//FIRST COME FIRST SERVE
	if(strcmp(argv[2], FCFS) == 0)
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
	//Block to run Round Robin and makes sure we are calling the right algorithm
	else if(strcmp(argv[2], RR) == 0)
	{
		//Checks to see if we have a quantum number argument when we run the algorithm
		if(argc < 4)
        {
			//If there is no argument for quantum error is printed and queue is released from memory
            printf("Round Robin requires quantum parameter\n");
            dyn_array_destroy(ready_queue);
            return EXIT_FAILURE;
        }
		//Converts the quantum number as a string to the integer value.
        size_t quantum = atoi(argv[3]);
		//Makes sure there are no errors in the Round Robin algorithm in the process_scheduling file.
        if(!round_robin(ready_queue, &result, quantum))
        {
			//If there are errors a error message is displayed and we release the queue from memory
            printf("Error running Round Robin\n");
            dyn_array_destroy(ready_queue);
            return EXIT_FAILURE;
        }
	}
	//Block to run Shortest job first and makes sure we are calling the right algorithm
	else if(strcmp(argv[2], SJF) == 0)
	{
		//Makes sure there are no errors in the shortest job first algorithm in process_scheduling file.
		if(!shortest_job_first(ready_queue, &result))
        {
			//If there is an error we print an error and release the queue from memory.
            printf("Error running Shortest Job First\n");
            dyn_array_destroy(ready_queue);
            return EXIT_FAILURE;
        }
	}
	//Block to run shortest time remaining first and makes sure we are calling the right algorithm.
	else if(strcmp(argv[2], SRT) == 0)
	{
		//Checks to make sure there are no errors in the SRT algorithm in process_scheduling file.
		if(!shortest_remaining_time_first(ready_queue, &result))
        {
			//If there is an error we print the error statement and release the queue from memory
            printf("Error running Shortest Remaining Time First\n");
            dyn_array_destroy(ready_queue);
            return EXIT_FAILURE;
        }
	}
	//PRIORITY
	else if (strcmp(argv[2], P)==0){
		//Checks to make sure there are no errors in the priority algorithm in process_scheduling file.
		if(!priority(ready_queue, &result)){
			//If there is an error we print the error statement and release the queue from memory
			printf("Error running Priority\n");
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

	//returns if the algorithm that was ran was run successfully.
	return EXIT_SUCCESS;
}
