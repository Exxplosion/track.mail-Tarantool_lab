#include "quick_sort.h"

void qs(int *s_arr, int first, int last)
{
    if (first < last)
    {
        int left = first, right = last, middle = s_arr[(left + right) / 2];
        do
        {
            while (s_arr[left] < middle)
                left++;
            while (s_arr[right] > middle)
                right--;
            if (left <= right)
            {
                int tmp = s_arr[left];
                s_arr[left] = s_arr[right];
                s_arr[right] = tmp;
                left++;
                right--;
            }
        } while (left <= right);
        qs(s_arr, first, right);
        qs(s_arr, left, last);
    }
}

void swap_cour(int i, ucontext_t *uctx_, int argc, arr_int *int_arrays)
{
    int_arrays[i].summ_sec += labs(int_arrays[i].last_sec - clock());
    int_arrays[i].last_sec = clock();
    //int_arrays[i % argc + 1].last_sec = clock();
    //printf("")
    printf("BY SWAP fucn%d: swapcontext(&uctx_[%d], &uctx_[%d])\n", i, i, i % argc + 1);
    if (swapcontext(&uctx_[i], &uctx_[i % argc + 1]) == -1)
        handle_error("swapcontext");
}

void qs_cour(int *s_arr, int first, int last, int i, ucontext_t *uctx_, int argc, arr_int *int_arrays)
{
    swap_cour(i, uctx_, argc, int_arrays);

    if (first < last)
    {
        swap_cour(i, uctx_, argc, int_arrays);
        int left = first, right = last, middle = s_arr[(left + right) / 2];
        swap_cour(i, uctx_, argc, int_arrays);
        do
        {
            while (s_arr[left] < middle)
                left++;
            swap_cour(i, uctx_, argc, int_arrays);

            while (s_arr[right] > middle)
                right--;
            swap_cour(i, uctx_, argc, int_arrays);

            if (left <= right)
            {
                int tmp = s_arr[left];
                s_arr[left] = s_arr[right];
                s_arr[right] = tmp;
                left++;
                right--;
                swap_cour(i, uctx_, argc, int_arrays);
            }
        } while (left <= right);
        swap_cour(i, uctx_, argc, int_arrays);

        qs(s_arr, first, right);
        swap_cour(i, uctx_, argc, int_arrays);

        //void swap_cour(int i, ucontext_t *uctx_, int argc);

        qs(s_arr, left, last);
        swap_cour(i, uctx_, argc, int_arrays);

        //void swap_cour(int i, ucontext_t *uctx_, int argc);
    }
}