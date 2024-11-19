#include "life.h"
#include <pthread.h>

typedef struct function_arguments{		
	int height_start;
	int height_end;
	int width_start;
	int width_end;
	LifeBoard * args_state;
	LifeBoard * args_next_state;
	pthread_barrier_t * step_barrier;
} arguments;

void * check_cells(void * args){

	arguments * cc_args = (arguments *) args;
	for (int step = 0; step < steps; step +=1 ){
		for (int y = cc_args->height_start; y < cc_args->height_end ; y += 1) {
			for (int x = 1; x < cc_args->state->widht-1; x += 1) {
				 /* For each cell, examine a 3x3 "window" of cells around it,
					 * and count the number of live (true) cells in the window. */
					int live_in_window = 0;
					for (int y_offset = -1; y_offset <= 1; y_offset += 1)
						for (int x_offset = -1; x_offset <= 1; x_offset += 1)
							if (LB_get(cc_args->args_state, x + x_offset, y + y_offset))
								live_in_window += 1;

					/* Cells with 3 live neighbors remain or become live.
					   Live cells with 2 live neighbors remain live. */
					LB_set(cc_args->args_next_state, x, y,
						live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
						(live_in_window == 4 && LB_get(cc_args->args_state, x, y)) /* live cell with 3 neighbors */
					);

			}
		}
		pthread_barrier_wait(cc_args->step_barrier);
		LB_swap(cc_args->state, cc_args->next_state);
	}
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    /* YOUR CODE HERE */
	
	pthread_t check_threads[threads];
	pthread_barrier_t step_barrier;
	pthread_barrier_init(&step_barrier, NULL, threads);

	LifeBoard *next_state = LB_new(state->width, state->height);

	arguments args[threads];	
	int width_sep = state->width/threads;
	int height_sep = state->height/threads;

	int width_remainder = state->width%threads;
	int height_remainder = state->height%threads;

	int hold_height_start = 0;
	int hold_width_start = 0;       
	for (int thread_num = 0; thread_num < threads; thread_num +=1){
		if (thread_num < (threads-1)){	
			
			args[thread_num].height_start = hold_height_start;
			args[thread_num].height_end = (hold_height_start + height_sep) - 1;
			args[thread_num].width_start = hold_width_start;
			args[thread_num].width_end = (hold_width_start + width_sep) - 1;
			
			hold_height_start = args[thread_num].height_end + 1;
			hold_width_start = args[thread_num].width_end + 1;
		
		} else{
			
			args[thread_num].height_start = hold_height_start;
			args[thread_num].height_end = (hold_height_start + height_remainder) -1;
			args[thread_num].width_start = hold_width_start;
			args[thread_num].width_end = (hold_width_start + width_remainder) -1;
		}

		args[thread_num].args_state = state;
		args[thread_num].args_next_state = next_state;
		args[thread_num].step_barrier = &step_barrier;
		pthread_create(&check_threads[thread_num], NULL, check_cells, &args[thread_num]);		
		
		
	}

	for(int thread_num = 0; thread_num<threads; thread_num++){
		pthread_join(check_threads[thread_num], NULL);
	}
	pthread_barrier_destroy(&step_barrier);
    LB_del(next_state);
}
