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
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
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
