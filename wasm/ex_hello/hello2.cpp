#include <stdio.h>
//#include <iostream>
#include <emscripten/emscripten.h>


/*
 * local prototypes methods
 *
 */


int main(int argc, char **argv) {
	//std::cout << "Hello World" << std::endl;
	printf("Hello World\n");
	return 0;
}



#ifdef __cplusplus
extern "C" {
#endif

void EMSCRIPTEN_KEEPALIVE my_function(int argc, char ** argv) {
	printf("my_function call successful.\n");
}

#ifdef __cplusplus
}
#endif
