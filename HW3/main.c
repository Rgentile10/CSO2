#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char *getoutput(const char *command);
extern char *parallelgetoutput(int count, const char **argv_base);

int main() {
    // Testing getoutput
    printf("Hi!\n");
    char *output1 = getoutput("echo 1 2 3; sleep 2; echo 5 5");
    if (output1) {
        printf("Text: [[[%s]]]\n", output1);
        free(output1); // Free the allocated memory
    } else {
        printf("Error executing getoutput\n");
    }
    printf("Bye!\n");

    // Testing parallelgetoutput 1
    {
        const char *argv_base1[] = {"/bin/echo", "running", NULL};
        char *output2 = parallelgetoutput(2, argv_base1);
        if (output2) {
            printf("Text: [%s]\n", output2);
            free(output2); // Free the allocated memory
        } else {
            printf("Error executing parallelgetoutput\n");
        }
    }

    // Testing parallelgetoutput 2
    {
        const char *argv_base2[] = {"./args.py", "first", "second", "third", NULL};
        char *output3 = parallelgetoutput(3, argv_base2);
        if (output3) {
            printf("Text: [%s]\n", output3);
            free(output3); // Free the allocated memory
        } else {
            printf("Error executing parallelgetoutput\n");
        }
    }

    return 0;
}
