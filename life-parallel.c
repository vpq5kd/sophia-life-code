#include "life.h"
#include <pthread.h>

void check_cells(int  height_start, int height_end, int width_start, int width_end){

	for (height_start; y < height_end ; y += 1) {
    	for (width_start; x < state->width_end; x += 1) {


		}
	}
};

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    /* YOUR CODE HERE */
	
	pthread_t threads[threads];

	LifeBoard *next_state = LB_new(state->width, state->height);
	for (int step = 0; step < steps; step += 1){
		
	}
	
}
