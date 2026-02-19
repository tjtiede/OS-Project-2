#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
	// decrement the burst time of the pcb
	--process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	//Defines the pcb blocks;
	ProcessControlBlock_t pcb;
	/*
		Checks to make there are values in the ready queue and result if not returns false for an error.
	*/
	if(ready_queue == NULL || result == NULL)
	{
		return false;
	}
	//Defines variables needed to computer the times for the result
	uint32_t waitTime = 0;
	uint32_t currTime = 0;
	uint32_t totalTurnaround = 0;
	//Gets the size of the queue.
	size_t processNum = dyn_array_size(ready_queue);
	/*
		Runs as long as the queue is not empty
	*/
	while(!dyn_array_empty(ready_queue))
	{
		//Removes the object that is on the top of queue and places it at that memory address allocated for the pcb.
		dyn_array_extract_front(ready_queue, &pcb);
		//Calculates how long the wait time was for the process to get taken off the queue.
		waitTime += currTime - pcb.arrival;
		/*
			If there is still burst time for the pcbs we we decrement the burst time and increment the current time.
		*/
		while(pcb.remaining_burst_time > 0)
		{
			virtual_cpu(&pcb);
			currTime++;
		}
		//Calculates the total time the process took.
		totalTurnaround += currTime - pcb.arrival;
	}
	
	//Computes the times for the result using the times calculated during the process of taking the process off the queue.
	result->average_turnaround_time = (float)totalTurnaround / processNum;
	result->average_waiting_time = (float)waitTime / processNum;
	result->total_run_time = currTime;

	return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	UNUSED(quantum);
	return false;
}

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
	/* 
		If the file is null we treat that as an error and return null.
	*/
	if(input_file == NULL)
	{
		return NULL;
	}
	//Creates the file and opens the input file to be read as a binary file.
	FILE* fp = NULL;
	fp.fopen(input_file, "rb");
	/*
		If there is nothing in the file we close the file and return null because of an error.
	*/
	if(!fp)
	{
		fclose(fp);
		return NULL;
	}
	//gives us the variable to store the processes and we read the process from the file and store it.
	uint32_t process_num;
	fread(&process_num, sizeof(process_num), 1, fp);
	//Create the array with the correct size.
	dyn_array_t* create_array = dyn_array_create(process_num, sizeof(ProcessControlBlock_t), NULL);
	/*
		If there is nothing in the array we treat that as an error  close the file and return null.
	*/
	if(create_array == NULL)
	{
		fclose(fp);
		return NULL;
	}
	//Variable for our stuct.
	ProcessControlBlock_t pcb;
	/*
		Loop through each process that we got from the file and read it from the file again.
	*/
	for(uint32_t i = 0; i < process_num; i++)
	{
		fread(&pcb, sizeof(pcb), 1, fp);
		//Tells us that the process hasn't been activated on the virtual cpu.
		pcb.started = false;
		//Places the pcb just read at the back of the array and allocates another space for the next element in the file.
		dyn_array_push_back(&pcb, create_array);
	}
	/*
		When all the pcb's have been read we close the file and return the array of pcb's.
	*/
	fclose(fp);
	return create_array;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}
