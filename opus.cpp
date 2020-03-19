#include <iostream>
#include <cstdint>

using namespace std;

uint16_t task_max = 256;
uint16_t byte_one = 0xff;
uint16_t byte_two = 0xff00;

/*
 * after masking the 8 low order bits
 * of the binary pattern we get the integer
 * result on the right. uncomment the 
 * uint16_t task variables below to 
 * experiment.
 * 
 * 0000 0001 0000 0001 = 0x101
 * 0000 0010 0000 0001 = 0x201
 * 0000 0011 0000 0001 = 0x301
 */

// uint16_t task = 0x101;
// uint16_t task = 0x201;
uint16_t task = 0x301;


void setmask(uint16_t task, uint16_t mask, uint16_t *data);
void get_taskid(uint16_t task, uint16_t *task_id);
void calculate_range(uint16_t task, uint16_t *prev, uint16_t *next);

int main() {


	uint16_t task_base; /* extract the base message id */
	uint16_t task_id; /* friendly id which is base/max tasks */
	uint16_t task_prev; /* task_base less task_max */
	uint16_t task_next; /* task_base plus task_max */

	cout << "Welcome to opus..." << endl;

	get_taskid(task, &task_base);
	task_id = task_base/task_max;
	cout << "task: " << task << " task_base: " << task_base << " task_id: " << task_id << endl;
	calculate_range(task, &task_prev, &task_next);
	cout << "task_prev: " << task_prev << " task_next: " << task_next << endl;
	return 0;
	
}

void get_taskid(uint16_t task, uint16_t *task_id) {
	 setmask(task, byte_two, task_id);
}

void setmask(uint16_t task, uint16_t mask, uint16_t *data) {
	 *data = ((task)& mask);
}

/*
 * range calculation generates integers for use in querying a RDBMS database.
 * since we are bit encoding the task/sub-tasks we need to get back to the root task id so that
 * we can generate a range of values from which to find a specific task. we don't look for a task id 
 * we look for the task that falls between the previous and next. this helps prevent
 * row scans that might occur when using bitwise operations in sql queries.  a simple api can
 * be built so that the web/api developers are insulated from the complexity of the bit manipulation.
 */

/*
 * 0000 0001 0000 0001 = 0x101
 * 0000 0010 0000 0001 = 0x201
 * 0000 0011 0000 0001 = 0x301
 * 
 * Let's say we want to find record represented in example 0x201 we start by masking the low order bits
 * to generate the value:
 * 000 0010 0000 0000 = 0x200 = 512
 * this generates the root task id
  * subtracting max tasks (256) to the task id gives us the prev task id
 * 000 0001 0000 0000 = 0x100 = 256
 * adding max tasks (256) to the task id gives us the next task id
 * 000 0011 0000 0000 = 0x300 = 768
 * 
 * we now have a range of values we can query the RDBMS table from
 * 
 * SELECT TaskId, TaskName, Epoch FROM TaskJournal WHERE TaskId BEWTWEN 256 AND 768
 * 
 * This calculation can be further refined to maks out the sub-task range of values further narrowing
 * our range of values to search.
 */
void calculate_range(uint16_t task, uint16_t *prev, uint16_t *next) {
	*prev = task - task_max;
	*next = task + task_max;
}


// void setmask(uint16_t *data, uint16_t mask);

// void setmask(uint16_t *data, uint16_t mask) {
// 	 *data = ((*data)& mask);
// }