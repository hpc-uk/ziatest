/* -*- C -*-
 *
 * Copyright (c) 2008 Los Alamos National Security, LLC.  All rights reserved.
 *
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * Modified by Sue Kelly, January 2010, to allow for a different 
 * mpirun command and associated options.
 * 
 * $HEADER$
 *
 */
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int nppn;
    char* exe="mpirun -npernode";
    struct timeval tv;
    char *cmd;

    /* check for proper usage */
    if (argc > 3) {
        printf("usage: ziatest <#procs/node> [exe_and_arg]\n");
        exit(1);
    }
    
    nppn = strtol(argv[1], NULL, 10);
    if (argc == 3) {
        exe=argv[2];
    }
    
    /* THIS BEGINS THE OFFICIAL TIMING POINT */

    /* get a starting time stamp */
    gettimeofday(&tv, NULL);
    
    /* form the command */
    asprintf(&cmd, "%s %d ./ziaprobe %d %d %d", exe, nppn, (int)tv.tv_sec, (int)tv.tv_usec, nppn);
    fprintf(stderr,"%s\n",cmd);
    
    /* execute it */
    system(cmd);
    
    /* done */
    free(cmd);
    return 0;
}
