#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define ITERATIONS 1000  // Number of iterations for scenarios 1 to 4

volatile sig_atomic_t signal_received = 0; // Flag to indicate signal reception

long long start_time; // Global start time for scenario 5

// get current time in nanoseconds
long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000000000LL + t.tv_nsec;
}

// Empty function for scenario 1
__attribute__((noinline)) void empty_function() {
    __asm__("");
}

// Signal handler for scenario 5 (sender)
void signal_handler_5(int sig) {
    if (sig == SIGUSR2) {
        long long end_time = nsecs();
        printf("Round trip signal time: %lld ns\n", end_time - start_time);
        exit(EXIT_SUCCESS);  // End sender process after measurement
    }
}

// Signal handler for receiver in scenario 5
void signal_handler_receiver(int sig) {
    signal_received = 1;
}

// Function to send a signal to a given PID
void send_signal(pid_t pid, int sig) {
    if (kill(pid, sig) == -1) {
        perror("Failed to send signal");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <scenario_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int scenario = atoi(argv[1]);
    pid_t pid_input; // Variable to read PIDs as input
    long long total_time = 0; // Total time for calculating average

    struct sigaction sa_sender, sa_receiver;
    sa_sender.sa_handler = &signal_handler_5;
    sa_receiver.sa_handler = &signal_handler_receiver;
    sigemptyset(&sa_sender.sa_mask);
    sigemptyset(&sa_receiver.sa_mask);
    sa_sender.sa_flags = 0;
    sa_receiver.sa_flags = 0;

    if (scenario >= 1 && scenario <= 4) {
        for (int i = 0; i < ITERATIONS; i++) {
            long long start, end;

            switch (scenario) {
                case 1: // Measure average time for empty function call
                    start = nsecs();
                    empty_function();
                    end = nsecs();
                    total_time += (end - start);
                    break;
                case 2: // Measure average time for getppid function
                    start = nsecs();
                    getppid();
                    end = nsecs();
                    total_time += (end - start);
                    break;
                case 3: // Measure average time for system("/bin/true")
                    start = nsecs();
                    if (system("/bin/true") == -1) { 
                        perror("system call failed");
                    }
                    end = nsecs();
                    total_time += (end - start);
                    break;
                case 4: // Measure average time for sending a signal to the current process
                    signal(SIGUSR1, signal_handler_receiver); // Register signal handler for receiver
                    signal_received = 0; // Reset flag
                    start = nsecs();
                    raise(SIGUSR1); // Send signal to the current process
                    while (!signal_received) {} // Wait for signal handler
                    end = nsecs();
                    total_time += (end - start);
                    break;
            }
        }
        printf("Average time for scenario %d: %lld ns\n", scenario, total_time / ITERATIONS);
    } else if (scenario == 5 || scenario == -1) {
        printf("Process PID: %d\n", getpid());

        if (scenario == -1) { // Receiver
            sigaction(SIGUSR1, &sa_receiver, NULL);  // Setup handler for receiver to receive SIGUSR1
            printf("Enter sender PID: ");
            if (scanf("%d", &pid_input) != 1) {
                fprintf(stderr, "Failed to read sender PID\n");
                exit(EXIT_FAILURE);
            }
            pause();  // Wait for SIGUSR1 from sender
            if (signal_received) {
                send_signal(pid_input, SIGUSR2);  // Send SIGUSR2 back to sender
            }
        } else if (scenario == 5) { // Sender
            sigaction(SIGUSR2, &sa_sender, NULL);
            printf("Enter receiver PID: ");
            if (scanf("%d", &pid_input) != 1) {
                fprintf(stderr, "Failed to read receiver PID\n");
                exit(EXIT_FAILURE);
            }
            start_time = nsecs();
            send_signal(pid_input, SIGUSR1);  // Send SIGUSR1 to receiver
            pause();  // Wait for SIGUSR2 from receiver
        }
    } else {
        fprintf(stderr, "Invalid scenario number: %d\n", scenario);
        exit(EXIT_FAILURE);
    }

    return 0;
}

