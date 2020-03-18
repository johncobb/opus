#include <iostream>
#include <cstdint>

using namespace std;

uint16_t task_max = 256;

uint16_t mask_low = 0xff;
uint16_t mask_high = 0xff00;

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


void setmask(uint16_t *data, uint16_t mask);

int main() {

	cout << "Welcome to opus..." << endl;
	cout << "task: " << task << endl;
	cout << "masking low order bits: " << endl;
	setmask(&task, mask_high);
	cout << "task: " << (task/task_max) << endl;

	//cout << "After: " << task << endl;

	return 0;
}


void setmask(uint16_t *data, uint16_t mask) {
	 *data = ((*data)& mask);
}
