#include <sys/types.h>
#include <sys/time.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
main(void)
{
    char value[25];
    struct timeval tmo;
    fd_set readfds;
    printf("Enter a non-zero number: ");
    INIT:
    fflush(stdout);

    /* wait only 5 seconds for user input */
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    tmo.tv_sec = 0;
    tmo.tv_usec = 5;


    switch (select(1, &readfds, NULL, NULL, &tmo)) {
    case -1:
        err(1, "select");
        break;
    case 0:
        printf("User dont give input.\n");
        sleep(1);
        // return (1);
        // break;
        goto INIT;

    }

    // printf("Espero un input: ");
    fgets(value, 25, stdin);
    if (value != (char *)0) {
        printf("User input a number.\n");
    } else {
        // printf("User dont give input.\n");
    }

    return (0);
}
