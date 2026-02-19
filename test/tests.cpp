#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "gtest/gtest.h"
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function mangling
extern "C"
{
#include <dyn_array.h>
}

#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

/*
unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment
{
	public:
		virtual void SetUp()
		{
			score = 0;
			total = 210;
		}

		virtual void TearDown()
		{
			::testing::Test::RecordProperty("points_given", score);
			::testing::Test::RecordProperty("points_total", total);
			std::cout << "SCORE: " << score << '/' << total << std::endl;
		}
};
*/

/*
	Load_Process_Control_block tests
*/

//Tests that if the file is null we get a null back as an error because of a null pointer. 
TEST(load_process_control_blocks, NullFile)
{
	dyn_array_t* array = load_process_control_blocks(NULL);
	EXPECT_EQ(array, nullptr);
}

//Tests to make sure all the pcb's in the folder have been added to the array.
TEST(load_process_control_blocks, GoodRead)
{
	dyn_array_t* array = load_process_control_blocks("pcb.bin");
	EXPECT_NE(array, nullptr);
	EXPECT_EQ(dyn_array_size(array), 4);
	dyn_array_destroy(array);
}

/*
	Tests for FCFS implementation
*/
//Tests to make sure we get an error when we have a null array.
TEST(first_come_first_serve, nullvalue)
{
	ScheduleResult_t result;
	bool success = first_come_first_serve(NULL, &result);
	EXPECT_EQ(success, false);
}

//Tests to make sure we get a result greater than 0 for the times displayed in the result.
TEST(first_come_first_serve, CorrectResult)
{
	dyn_array_t* array = load_process_control_blocks("pcb.bin");
	EXPECT_NE(array, nullptr);
	ScheduleResult_t result;
	bool success = first_come_first_serve(array, &result); 
	EXPECT_EQ(success, true);
	EXPECT_GT(result.average_turnaround_time, 0.0f);
	EXPECT_GT(result.average_waiting_time, 0.0f);
	EXPECT_GT(result.total_run_time, 0UL);
	dyn_array_destroy(array);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	// ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
	return RUN_ALL_TESTS();
}
