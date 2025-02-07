#include "life.h"
#include <pthread.h>

typedef struct function_arguments{		
	int height_start;
	int height_end;
	int steps;

	int thread_swap;
	LifeBoard * args_state;
	LifeBoard * args_next_state;
	pthread_barrier_t * step_barrier;
} arguments;

void * check_cells(void * args){

	arguments * cc_args = (arguments *) args;
	for (int step = 0; step < cc_args->steps; step +=1 ){
		for (int y = cc_args->height_start; y < cc_args->height_end; y += 1) {
			for (int x = 1; x < cc_args->args_state->width-1; x += 1) {
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
		if(cc_args->thread_swap) LB_swap(cc_args->args_state, cc_args->args_next_state);

		pthread_barrier_wait(cc_args->step_barrier);
	}
	return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    /* YOUR CODE HERE */
	
	pthread_t check_threads[threads];
	pthread_barrier_t step_barrier;
	pthread_barrier_init(&step_barrier, NULL, threads);

	LifeBoard *next_state = LB_new(state->width, state->height);

	arguments args[threads];	
	int height_sep = (state->height)/threads;

	int height_remainder = (state->height) - (height_sep*threads);

	for (int thread_num = 0; thread_num < threads; thread_num +=1){


		/* designates the first thread as the thread responsible for swapping the states */
		if (thread_num == 0){
			args[thread_num].thread_swap = 1;
		} else {
			args[thread_num].thread_swap = 0;
		}
	
		int add_remainder = 0;
		if(thread_num == threads - 1){
			add_remainder = height_remainder;
		}
		/* basic delegation of threads to different sectors */
		
		args[thread_num].height_start = thread_num*height_sep;
		args[thread_num].height_end = (thread_num + 1)*height_sep + add_remainder;

		
		/* offset the starting height if we are on the first thread */
		if (thread_num == 0){
			args[thread_num].height_start+=1;
		} 
		
		/* offset the ending height if we are on the last thread */
		if (thread_num == threads-1){
			args[thread_num].height_end-=1;
		}

		/* next height start = height_end+1 */
 
//		printf("thread_num: %d, height_start: %d, height_end: %d\n", thread_num, args[thread_num].height_start, args[thread_num].height_end);	

		args[thread_num].steps = steps;
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
