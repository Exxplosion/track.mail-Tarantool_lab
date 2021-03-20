#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <time.h>
#include <math.h>


#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

typedef struct
{
    int flag_status;
    clock_t last_sec;
    clock_t summ_sec;
    int *start_int; //указатель на начало
    size_t size_arr;
} arr_int;


void qs(int *s_arr, int first, int last);
void qs_cour(int *s_arr, int first, int last, int i, ucontext_t *uctx_, int argc, arr_int *int_arrays);
void swap_cour(int i, ucontext_t *uctx_, int argc, arr_int *int_arrays);

#endif