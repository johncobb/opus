#include <stdio.h>
#include <emscripten/emscripten.h>


/*
 * local prototypes methods
 *
 */
// void my_function(int argc, char **);

int main(int argc, char **argv) {
	//std::cout << "Hello World" << std::endl;
	printf("Hello World\n");
	return 0;
}

// void my_func_addone(int *input_ptr, int *output_ptr) {
// 	*output_ptr = (*input_ptr) + 1;
// 	printf("my_func_addone: %d call successful.\n", *output_ptr);
// }


#ifdef __cplusplus
extern "C" {
#endif

/*
 * https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm
 * Step 1: ./emsdk activate latest
 * Step 2: source ./emsdk_env.sh
 * Step 3emcc -o hello3.html hello3.cpp -O3 -s WASM=1 --shell-file ../html_template/shell_minimal.html -s NO_EXIT_RUNTIME=1  -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"
 * 

 * . ../run_simplehttp
 */
void EMSCRIPTEN_KEEPALIVE my_func(int argc, char ** argv) {
	printf("my_function call successful.\n");
}
/*
 * https://marcoselvatici.github.io/WASM_tutorial/#files
 * emcc hello3.cpp -o hello3.html -s WASM=1 -s EXPORTED_FUNCTIONS='["my_func_addone"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap", "getValue", "setValue"]'
 */

void EMSCRIPTEN_KEEPALIVE my_func_addone(int *input_ptr, int *output_ptr) {
	*output_ptr = (*input_ptr) + 1;
	printf("my_func_addone: %d call successful.\n", *output_ptr);
}

#ifdef __cplusplus
}
#endif
