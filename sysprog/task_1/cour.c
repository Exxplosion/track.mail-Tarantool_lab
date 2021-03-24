#include "working_file.h"
#include "quick_sort.h"
#include <ucontext.h>
#include <signal.h>
#include <time.h>
#include <sys/mman.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

#define stack_size 1024 * 1024

static void *
allocate_stack_mprot()
{
    void *stack = malloc(stack_size);
    mprotect(stack, stack_size, PROT_READ | PROT_WRITE | PROT_EXEC);
    return stack;
}

/*
static void* allocate_stack_sig()
{
    void *stack = malloc(stack_size);
    stack_t ss;
    ss.ss_sp = stack;
    ss.ss_size = stack_size;
    ss.ss_flags = 0;
    sigaltstack(&ss, NULL);
    return stack;
}
*/

static ucontext_t uctx_main;

static void my_coroutine(ucontext_t *uctx_, int argc, int i, arr_int *int_arrays)
{
    printf("func%d: started\n", i);
    
    int ALL_READY = 0;
    
    if (i != argc)
    {
        printf("fucn%d: swapcontext(&uctx_[%d], &uctx_[%d])\n", i, i, i+1);
        if (swapcontext(&uctx_[i], &uctx_[i + 1]) == -1)
            handle_error("swapcontext");
    }
    qs_cour(int_arrays[i].start_int, 0, int_arrays[i].size_arr - 1, i, uctx_, argc, int_arrays);

    do
    {
        ALL_READY = 1;
        int_arrays[i].flag_status = 1;
        printf("fucn%d: swapcontext(&uctx_[%d], &uctx_[%d])\n", i, i, i % argc + 1);
        if (swapcontext(&uctx_[i], &uctx_[i % argc + 1]) == -1)
            handle_error("swapcontext");
        
        for (int j = 1; j < argc + 1; j++)
        {
            if (int_arrays[j].flag_status == 0)
            {
                ALL_READY = 0;
            }
        }
    } while (ALL_READY == 0);



    printf("I AM COUR [%d] MY TIME - %ld MS\n", i, (int_arrays[i].summ_sec)*1000/CLOCKS_PER_SEC);
    printf("func%d: returning\n", i);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("argc = %d, select file!!!\n", argc);
        return 0;
    }
    ucontext_t* uctx_ = (ucontext_t *) calloc(argc, sizeof(ucontext_t));
    size_t summary_size_arr = 0;

    arr_int* int_arrays = (arr_int *) calloc(argc, sizeof(arr_int));
    for (int i = 1; i < argc; i++)
    {
        printf("processing %d file \n", i);
        if (getcontext(&uctx_[i]) == -1)
            handle_error("getcontext");
        if ((uctx_[i].uc_stack.ss_sp = allocate_stack_mprot()) == NULL)
            handle_error("allocate_stack_sig");
        uctx_[i].uc_stack.ss_size = stack_size;


        char *current_file_array = read_file_in_array(argv[i]);
        size_t size_arr = 0;
        lines* words = start_of_lines(current_file_array, &size_arr);
        int* int_array = formation_int(words, size_arr);
        free(current_file_array);
        free(words);
        /* получен массив чисел int_array, размером size_arr из исходного файла*/
        //qs(int_array, 0, size_arr - 1); //делаем в корутине

        int_arrays[i].start_int = int_array;
        int_arrays[i].size_arr = size_arr;

        summary_size_arr += size_arr;
        
    }

    uctx_[argc - 1].uc_link = &uctx_main;
    for (int j = 1; j < argc - 1; j++)
    {
        printf("| %d | --> | %d |\n", j, j + 1);
        uctx_[j].uc_link = &uctx_[j + 1];
    }

    for (int i = 1; i < argc; i++)
    {
        int_arrays[i].flag_status = 0;
        int_arrays[i].summ_sec = 0.0;
        int_arrays[i].last_sec = time(NULL);
        makecontext(&uctx_[i], (void *)my_coroutine, 4, uctx_, argc - 1, i, int_arrays);
    }

    printf("main: swapcontext(&uctx_main, &uctx_1)\n");
    if (swapcontext(&uctx_main, &uctx_[1]) == -1)
        handle_error("swapcontext");

    /*слияние в один большой массив */
    int *summ_arr = (int *)calloc(summary_size_arr, sizeof(int));

    size_t tmp = 0;
    for(int i = 1; i < argc; i++)
    {
        for (unsigned int j = 0; j < int_arrays[i].size_arr; j++)
        {
            summ_arr[j + tmp] = int_arrays[i].start_int[j];
        }
        tmp += int_arrays[i].size_arr;
    }

  

    qs(summ_arr, 0, summary_size_arr - 1);

    FILE *f = fopen("result.txt", "w+");

    for (unsigned int j = 0; j < summary_size_arr; j++)
    {
        if (f == NULL)
        {
            printf("error open wile!!!");
            return 0;
        }
        fprintf(f, "%d ", summ_arr[j]);
    }
    fclose(f);

    clock_t summ_t = clock();
    for (int i = 1; i < argc; i++)
    {
        summ_t += int_arrays[i].summ_sec;
    }

    for(int i = 1; i<argc; i++)
    {
        free(int_arrays[i].start_int);
        free(uctx_[i].uc_stack.ss_sp);
    }
    free(int_arrays);
    free(uctx_);
    free(summ_arr);

    printf("main exiting\n");
    printf("programm working -  %ld MS, bye!\n", ((clock() + summ_t)*1000 /CLOCKS_PER_SEC) );
    return 0;
}