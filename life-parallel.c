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

		int hold_height_start = 0;
		int hold_width_start = 0;
		for (int thread_num = 0; thread_num < threads; thread_num +=1){
			if (thread_num < (threads-1)){	
				arguments args;
				args.height_start = hold_height_start;
				args.height_end = (hold_height_start + height_sep) - 1;
				args.width_start = hold_width_start;
				args.width_end = (hold_width_end + width_sep) - 1;
			};

			
		}		
	}
	
}
