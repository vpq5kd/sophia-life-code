#include "life.h"
#include <pthread.h>

void check_cells(void * args){
	typdef struct function_arguments(){		
		int height_start;
		int height_end;
		int width_start;
		int width_end;
	} arguments;

	arguments * cc_args = (arguments *) args;

	for (int y = cc_args->height_start; y < cc_args->height_end ; y += 1) {
    	for (int x = cc_args->width_start; x < cc_args->width_end; x += 1) {


		}
	}
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    /* YOUR CODE HERE */
	
	pthread_t check_threads[threads];

	LifeBoard *next_state = LB_new(state->width, state->height);
	
	int width_sep = state->width/threads;
	int height_sep = state->height/threads;

	int width_remainder = state->width%threads;
	int height_remainder = state->height%threads;

	typedef struct function_arguments{
		int height_start;
		int height_end;
		int width_start;
		int width_end;
	} arguments;

	
	for (int step = 0; step < steps; step += 1){
		
	}
	
}
