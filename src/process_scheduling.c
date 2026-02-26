#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.

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
	//If ready_queue or result are NULL return false
	if(ready_queue == NULL || result == NULL){
		return false;
	}
	//Variables used for time analysis
	uint32_t waitTime = 0;
	uint32_t currTime = 0;
	uint32_t totalTurnaround = 0;
	// Gets the size of the process
	size_t processNum = dyn_array_size(ready_queue);

	if(processNum == 0){
		return false;
	}
	

	// Extract all processes, sort by burst time, push back into order
	ProcessControlBlock_t *pcbs = malloc(processNum * sizeof(ProcessControlBlock_t));
	if(pcbs == NULL){
		return false;
	}
	//Extract all from array
	for(size_t i = 0; i < processNum; i++){
		dyn_array_extract_front(ready_queue, &pcbs[i]);
	}
	// Insertion sort by remaining_burst_time
	for(size_t i = 1; i < processNum; i++){
		ProcessControlBlock_t key = pcbs[i];
		int j = (int)i-1;

		while (j>=0 && pcbs[j].remaining_burst_time > key.remaining_burst_time){
			pcbs[j + 1] = pcbs[j];
			j--;
		}
		pcbs[j+1] = key;
	}


	//Push processes back into queue
	for(size_t i = 0; i<processNum; i++){
		dyn_array_push_back(ready_queue, &pcbs[i]);
	}

	free(pcbs);

	//Simulate CPU 
	ProcessControlBlock_t pcb;
	while(!dyn_array_empty(ready_queue)){
		dyn_array_extract_front(ready_queue, &pcb);
		waitTime += currTime - pcb.arrival;

		while(pcb.remaining_burst_time >0){
			virtual_cpu(&pcb);
			currTime ++;
		}
		totalTurnaround += currTime - pcb.arrival;
	}

	//Store values in result struct
	result->average_turnaround_time = (float)totalTurnaround / processNum;
	result->average_waiting_time = (float)waitTime / processNum;
	result->total_run_time = currTime;

	return true;
}




bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	//If ready_queue or result are NULL return false
	if(ready_queue == NULL || result == NULL){
		return false;
	}
	//Get the size of the queue
	size_t processNum = dyn_array_size(ready_queue);
    if (processNum == 0)
    {
        return false;
    }

    uint32_t currTime = 0;
    uint32_t waitTime = 0;
    uint32_t totalTurnaround = 0;
	

	//Extract all Process Control Blocks to sort by priority
	 ProcessControlBlock_t *pcbs = malloc(processNum * sizeof(ProcessControlBlock_t));
    if (pcbs == NULL){
		return false;
	} 

	for(size_t i = 0; i<processNum; i++){
		dyn_array_extract_front(ready_queue, &pcbs[i]);
	}

	// Insertion sort by priority
	for(size_t i = 1; i < processNum; i++){
		ProcessControlBlock_t key = pcbs[i];
		int j = (int)i-1;

		while (j>=0 && pcbs[j].priority > key.priority){
			pcbs[j + 1] = pcbs[j];
			j--;
		}
		pcbs[j+1] = key;
	}
	//Push values back into the queue
	for(size_t i = 0; i<processNum; i++){
		dyn_array_push_back(ready_queue, &pcbs[i]);
	}

	free(pcbs);

	//Simulate the CPU
	ProcessControlBlock_t pcb;
	while(!dyn_array_empty(ready_queue)){
		dyn_array_extract_front(ready_queue, &pcb);
		waitTime += currTime - pcb.arrival;

		while (pcb.remaining_burst_time >0){
			virtual_cpu(&pcb);
			currTime ++;
		}
		totalTurnaround += currTime - pcb.arrival;
	}
	//Store results in result struct
	result->average_waiting_time = (float)waitTime / processNum;
    result->average_turnaround_time = (float)totalTurnaround / processNum;
    result->total_run_time = currTime;

	return true;

}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
	//If ready_queue or result are NULL or if quantum is 0, return false
    if (ready_queue == NULL || result == NULL || quantum == 0)
    {
        return false;
    }
	//Get queue size
    size_t processNum = dyn_array_size(ready_queue);
    if (processNum == 0)
    {
        return false;
    }

    uint32_t currTime = 0;
    uint32_t waitTime = 0;
    uint32_t totalTurnaround = 0;

	
    ProcessControlBlock_t *pcbs = malloc(processNum * sizeof(ProcessControlBlock_t));
    if (pcbs == NULL){
		return false;
	} 
	//extract all burst times for each process
    uint32_t *originalBurst = malloc(processNum * sizeof(uint32_t));
    if (originalBurst == NULL)
    {
        free(pcbs);
        return false;
    }

    // Extract all processes from queue, store in pcbs and store original burst times
    for (size_t i = 0; i < processNum; i++)
    {
        ProcessControlBlock_t *pcb = (ProcessControlBlock_t *)dyn_array_at(ready_queue, i);
        if (pcb == NULL)
        {
            free(pcbs);
            free(originalBurst);
            return false;
        }
        pcbs[i] = *pcb;
        originalBurst[i] = pcbs[i].remaining_burst_time; //store burst time values
    }

	size_t completed = 0;
    size_t idx = 0;

	//While not completed
    while (completed < processNum)
    {
        // Find next process with remaining time
        size_t checks = 0;
		
        while (pcbs[idx].remaining_burst_time == 0 && checks < processNum)
        {
            idx = (idx + 1) % processNum;
            checks++;
        }

        // If no process found exit
        if (checks == processNum)
            break;
		//Run current process for the given quantum time units
		size_t ran = 0;
    	while (pcbs[idx].remaining_burst_time > 0 && ran < quantum)
    	{
        	virtual_cpu(&pcbs[idx]);
        	currTime++;
        	ran++;
    	}
        
		//If process is finished, record time values
        if (pcbs[idx].remaining_burst_time == 0)
        {
            totalTurnaround += currTime - pcbs[idx].arrival;
            waitTime += (currTime - pcbs[idx].arrival) - originalBurst[idx];
            completed++;
        }
		//Move to next process
        idx = (idx + 1) % processNum;
    }
	//Store result values in result struct
    result->average_waiting_time = (float)waitTime / processNum;
    result->average_turnaround_time = (float)totalTurnaround / processNum;
    result->total_run_time = currTime;

    free(pcbs);
    free(originalBurst);
    return true;
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
	fp = fopen(input_file, "rb");
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
		//fread(&pcb, sizeof(pcb), 1, fp);
		fread(&pcb.remaining_burst_time, sizeof(uint32_t), 1, fp);
        fread(&pcb.priority, sizeof(uint32_t), 1, fp);
        fread(&pcb.arrival, sizeof(int32_t), 1, fp);
		//Tells us that the process hasn't been activated on the virtual cpu.
		pcb.started = false;
		//Places the pcb just read at the back of the array and allocates another space for the next element in the file.
		dyn_array_push_back(create_array, &pcb);
	}
	/*
		When all the pcb's have been read we close the file and return the array of pcb's.
	*/
	fclose(fp);
	return create_array;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	//If ready_
	if(ready_queue == NULL || result == NULL){
		return false;
	}

	size_t processNum = dyn_array_size(ready_queue);
	if(processNum == 0){
		return false;
	}

	uint32_t currTime = 0;
	uint32_t waitTime = 0;
	uint32_t totalTurnaround = 0;

	ProcessControlBlock_t *pcbs = malloc(processNum * sizeof(ProcessControlBlock_t));
	if(pcbs == NULL){
		return false;
	}

	uint32_t *originalBurst = malloc(processNum * sizeof(uint32_t));
	if( originalBurst == NULL){
		free(pcbs);
		return false;
	}

	for(size_t i = 0; i < processNum; i++){
		dyn_array_extract_front(ready_queue, &pcbs[i]);
		originalBurst[i] = pcbs[i].remaining_burst_time;
	}

	size_t completed = 0;

	while(completed < processNum){
		int shortest_idx = -1;
		uint32_t shortest_time = UINT32_MAX;

		for (size_t i = 0; i < processNum; i++)
        {
            if (pcbs[i].remaining_burst_time > 0 && pcbs[i].remaining_burst_time < shortest_time)
            {
                shortest_idx = i;
                shortest_time = pcbs[i].remaining_burst_time;
            }
        }

        // If no process found exit
        if (shortest_idx == -1)
            break;

        // Execute for one time unit
        virtual_cpu(&pcbs[shortest_idx]);
        currTime++;

        if (pcbs[shortest_idx].remaining_burst_time == 0)
        {
            totalTurnaround += currTime - pcbs[shortest_idx].arrival;
            waitTime += (currTime - pcbs[shortest_idx].arrival) - originalBurst[shortest_idx];
            completed++;
        }
	}

    result->average_waiting_time    = (float)waitTime / processNum;
    result->average_turnaround_time = (float)totalTurnaround / processNum;
    result->total_run_time          = currTime;

    free(pcbs);
    free(originalBurst);
    return true;


}
