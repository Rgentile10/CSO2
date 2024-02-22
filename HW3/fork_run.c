#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// grabs output from shell command
char *getoutput(const char *command) {
    if (!command) {
        fprintf(stderr, "command is null.\n");
        return NULL;
    }

    int fds[2];
    pid_t child_pid;
    char *result = NULL;
    size_t buffer_len = 0;

//create pipeline
    if (pipe(fds) != 0) {
        perror("failed to create pipe");
        return NULL;
    }

// fork
    child_pid = fork();
    if (child_pid < 0) {
        perror("didn't fork");
        return NULL;
    } 
    
// Child process does the actual command execution.
    else if (child_pid == 0) {
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);
        execl("/bin/sh", "sh", "-c", command, NULL);
        perror("didn't execute");
        _exit(1);
    } 
    
// Parent waits and collects the output
    else { 
        close(fds[1]);
        FILE *stream = fdopen(fds[0], "r");
        if (!stream) {
            perror("could not open stream");
            close(fds[0]);
            return NULL;
        }

// Read the output
        if (getdelim(&result, &buffer_len, '\0', stream) < 0) {
            if (feof(stream)) {
                result = calloc(1, sizeof(char));
            } else {
                perror("could not read from pipe");
            }
            fclose(stream);
            waitpid(child_pid, NULL, 0);
            return result;
        }

        fclose(stream);
        waitpid(child_pid, NULL, 0);
    }

    return result;
}


// Executes multiple commands in parallel and combines their outputs

char *parallelgetoutput(int count, const char **argv_base) {
    if (count <= 0 || !argv_base) {
        fprintf(stderr, "wrong input for parallel execution.\n");
        return NULL;
    }

    int pipe_ends[2];
    
    pid_t *children = calloc(count, sizeof(pid_t));
    if (!children) {
        perror("couldnt allocate memory --> child PIDs");
        return NULL;
    }

    char *collected_output = NULL;
    size_t total_capacity = 0, current_size = 0;

// set up another pipe
    if (pipe(pipe_ends) != 0) {
        perror("couldnt create pipe");
        free(children);
        return NULL;
    }

// fork off child processes.
    for (int i = 0; i < count; ++i) {
        children[i] = fork();
        if (children[i] < 0) {
            perror("didn't fork");
            continue; // Skip the missing child, try next
        } 
        
        else if (children[i] == 0) {
            close(pipe_ends[0]);
            dup2(pipe_ends[1], STDOUT_FILENO);

            char index_str[12];
            snprintf(index_str, sizeof(index_str), "%d", i); // Tag each output with its index.

            int arg_count = 0;
            while (argv_base[arg_count] != NULL) arg_count++;

            char *child_args[arg_count + 2]; // Extra space for index and NULL terminator.
            for (int j = 0; j < arg_count; ++j) {
                child_args[j] = (char *)argv_base[j];
            }
            child_args[arg_count] = index_str;
            child_args[arg_count + 1] = NULL;

            execvp(argv_base[0], child_args);
            perror("never executed");
            _exit(1);
        }
    }

    close(pipe_ends[1]);

    FILE *read_stream = fdopen(pipe_ends[0], "r");
    
    if (!read_stream) {
        perror("couldn't open read stream");
        close(pipe_ends[0]);
        free(children);
        return NULL;
    }

    char temp_buffer[256];
    while (fgets(temp_buffer, sizeof(temp_buffer), read_stream) != NULL) {
        size_t line_length = strlen(temp_buffer);
        if (current_size + line_length + 1 > total_capacity) {
            total_capacity = (current_size + line_length + 1) * 2;
            char *new_buffer = realloc(collected_output, total_capacity);
            
            if (!new_buffer) {
                perror("couldnt reallocate memory");
                fclose(read_stream);
                free(collected_output);
                free(children);
                return NULL;
            }
            collected_output = new_buffer;
        }
        memcpy(collected_output + current_size, temp_buffer, line_length);
        current_size += line_length;
    }
    collected_output[current_size] = '\0';

    fclose(read_stream);

// Wait for all all child processes to finish
    for (int i = 0; i < count; ++i) {
        if (children[i] > 0) {
            waitpid(children[i], NULL, 0);
        }
    }

    free(children);
    return collected_output;
}
