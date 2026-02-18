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
	EXPECT_EQ(arr, nullptr);
}

//Tests to make sure all the pcb's in the folder have been added to the array.
TEST(load_process_control_blocks, GoodRead)
{
	dyn_array_t* array = load_process_control_blocks('pcb.bin');
	EXPECT_EQ(arr, 7);
	dyn_array_destroy(array);
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	// ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
	return RUN_ALL_TESTS();
}
