#include "life.h"
#include <pthread.h>

// Custom barrier structure
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int waiting;
} CustomBarrier;

// Custom barrier functions
void custom_barrier_init(CustomBarrier *barrier, int count) {
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    barrier->count = count;
    barrier->waiting = 0;
}

void custom_barrier_wait(CustomBarrier *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    barrier->waiting++;
    if (barrier->waiting == barrier->count) {
        barrier->waiting = 0; // Reset for next use
        pthread_cond_broadcast(&barrier->cond);
    } else {
        while (pthread_cond_wait(&barrier->cond, &barrier->mutex) != 0);
    }
    pthread_mutex_unlock(&barrier->mutex);
}

void custom_barrier_destroy(CustomBarrier *barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
}

// Thread function and data structure
typedef struct {
    int start_row, end_row;
    LifeBoard *even_state, *odd_state;
    CustomBarrier *barrier;
    int steps;
} ThreadData;

void* update_segment(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    LifeBoard *current_state, *next_state;

    for (int step = 0; step < data->steps; ++step) {
        if (step % 2 == 0) {
            current_state = data->even_state;
            next_state = data->odd_state;
        } else {
            current_state = data->odd_state;
            next_state = data->even_state;
        }

        for (int y = data->start_row; y < data->end_row; ++y) {
            for (int x = 1; x < current_state->width - 1; ++x) {
                int live_neighbors = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        live_neighbors += LB_get(current_state, x + dx, y + dy);
                    }
                }
                
                int current_cell = LB_get(current_state, x, y);
                if (current_cell) live_neighbors--; // Subtract the cell itself if alive

                int next_cell_state = (live_neighbors == 3 || (live_neighbors == 2 && current_cell));
                LB_set(next_state, x, y, next_cell_state);
            }
        }

        custom_barrier_wait(data->barrier);
    }

    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    pthread_t thread_ids[threads];
    ThreadData thread_data[threads];
    CustomBarrier barrier;
    custom_barrier_init(&barrier, threads);

    LifeBoard *odd_state = LB_new(state->width, state->height);

    int rows_per_thread = state->height / threads;
    for (int i = 0; i < threads; ++i) {
        thread_data[i].start_row = i * rows_per_thread + 1;
        thread_data[i].end_row = (i == threads - 1) ? state->height - 1 : (i + 1) * rows_per_thread + 1;
        thread_data[i].even_state = state;
        thread_data[i].odd_state = odd_state;
        thread_data[i].barrier = &barrier;
        thread_data[i].steps = steps;

        pthread_create(&thread_ids[i], NULL, update_segment, &thread_data[i]);
    }

    for (int i = 0; i < threads; ++i) {
        pthread_join(thread_ids[i], NULL);
    }

    // Final state is in 'odd_state' if the number of steps is odd
    if (steps % 2 != 0) {
        LB_swap(state, odd_state);
    }

    LB_del(odd_state);
    custom_barrier_destroy(&barrier);
}
