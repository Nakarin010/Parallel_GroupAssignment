#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h> // so that bool, true, and false can be used

// 0-9 buckets = 10 buckets
// 0-9999 the range of the numbers in each bucket
// number threads are 1 4 8 12 16
void merge();
void fill_ran(int array[], int length,int min,int max);
void sortindi (int array[]);
int compare (const void * a, const void * b);// for the qsort built-in function
int* bucket_sort(int length,int num_threads){

    int BUCKETSIZE = 2000000;
    int BUCKETNUM = 10;
    int* array = (int*)malloc(length * sizeof(int));

 // Dynamically allocate bucket arrays
    int *a = malloc(BUCKETSIZE * sizeof(int));
    int *b = malloc(BUCKETSIZE * sizeof(int));
    int *c = malloc(BUCKETSIZE * sizeof(int));
    int *d = malloc(BUCKETSIZE * sizeof(int));
    int *e = malloc(BUCKETSIZE * sizeof(int));
    int *f = malloc(BUCKETSIZE * sizeof(int));
    int *g = malloc(BUCKETSIZE * sizeof(int));
    int *h = malloc(BUCKETSIZE * sizeof(int));
    int *k = malloc(BUCKETSIZE * sizeof(int));
    int *y = malloc(BUCKETSIZE * sizeof(int));

    //for the for loop have to be dynamically allocated and loop able
    int** bucket = (int**)malloc(BUCKETNUM * sizeof(int*));
    bucket[0] = a; bucket[1] = b; bucket[2] = c; bucket[3] = d; bucket[4] = e;
    bucket[5] = f; bucket[6] = g; bucket[7] = h; bucket[8] = k; bucket[9] = y;
    // Check if all bucket allocations were successful
    if (!a || !b || !c || !d || !e || !f || !g || !h || !k || !y) {
        fprintf(stderr, "Memory allocation for buckets failed\n");
        // Free any allocated memory before exiting
        free(array);
        free(a); free(b); free(c); free(d); free(e);
        free(f); free(g); free(h); free(k); free(y);
        exit(EXIT_FAILURE);
    }

 // for the for loop
    // dynamically allocate for the benefit of easier to use with openMP & threads
    int *bucket_count = (int *)malloc(BUCKETNUM * sizeof(int));
    for(int i=0;i<BUCKETNUM;i++){
        bucket_count[i] = 0;
    }
    int* final = (int*)malloc(length * sizeof(int));
    fill_ran(array, length, 0, 99999); // Fill the array with random numbers
    double start= omp_get_wtime(); /// to measure the time of the sorting

    // Distribute array elements into buckets
    #pragma omp parallel for num_threads(num_threads)
    for (int j = 0; j < length; j++) {
        //condition to distribute the array into the buckets 
        int value = array[j];
        if (value >= 0 && value <= 9999) {
            if (bucket_count[0] < BUCKETSIZE) a[bucket_count[0]++] = value;
        } else if (value >= 10000 && value < 20000) {
            if (bucket_count[1]< BUCKETSIZE) b[bucket_count[1]++] = value;
        } else if (value >= 20000 && value < 30000) {
            if (bucket_count[2] < BUCKETSIZE) c[bucket_count[2]++] = value;
        } else if (value >= 30000 && value < 40000) {
            if (bucket_count[3] < BUCKETSIZE) d[bucket_count[3]++] = value;
        } else if (value >= 40000 && value < 50000) {
            if (bucket_count[4] < BUCKETSIZE) e[bucket_count[4]++] = value;
        } else if (value >= 50000 && value < 60000) {
            if (bucket_count[5] < BUCKETSIZE) f[bucket_count[5]++] = value;
        } else if (value >= 60000 && value < 70000) {
            if (bucket_count[6] < BUCKETSIZE) g[bucket_count[6]++] = value;
        } else if (value >= 70000 && value < 80000) {
            if (bucket_count[7] < BUCKETSIZE) h[bucket_count[7]++] = value;
        } else if (value >= 80000 && value < 90000) {
            if (bucket_count[8] < BUCKETSIZE) k[bucket_count[8]++] = value;
        } else if (value >= 90000 && value < 100000) {
            if (bucket_count[9] < BUCKETSIZE) y[bucket_count[9]++] = value;
        }
    }
 //sorting the individual buckets and utilize the OpenMP Threads
    #pragma omp parallel for num_threads(num_threads)
    for(int i=0;i<BUCKETNUM;i++){
        qsort(bucket[i], bucket_count[i], sizeof(int), compare);
    }int index = 0;
    for (int i = 0; i < BUCKETNUM; i++) {
        for (int j = 0; j < bucket_count[i]; j++) {
            final[index++] = bucket[i][j];
        }
    }
    double end = omp_get_wtime(); 
    double execution_time = end - start;
    // Example: Print sorted buckets (only Bucket A for brevity)
// printf("\n");
//     printf("\n");
    // printf("Merged and sorted array: ");
    // for (int i = 0; i < index; i++) {
    //     printf("%d ", final[i]);
    // }
    printf("Data: %4d | No. of Threads: %2d | Total_ex: %4f\n",length,num_threads,execution_time);

    //free the allocated memmory
    free(a); free(b); free(c); free(d); free(e);
    free(f); free(g); free(h); free(k); free(y);
    free(bucket);
    free(bucket_count);
    free(array);

    return final;
}
int main(void){
    printf("-----------------------HELLO---------------------------\n");
    printf("-------------------------------------------------------\n");
    printf("-------------------------------------------------------\n");
    bucket_sort(10000,1);
    bucket_sort(10000,4);
    bucket_sort(10000,8);
    bucket_sort(10000,12);
    bucket_sort(10000,16);
    printf("-------------------------------------------------------\n");
    printf("-------------------------------------------------------\n");
    bucket_sort(100000,1);
    bucket_sort(100000,4);
    bucket_sort(100000,8);
    bucket_sort(100000,12);
    bucket_sort(100000,16);
    printf("-------------------------------------------------------\n");
    printf("-------------------------------------------------------\n");
    bucket_sort(500000,1);
    bucket_sort(500000,4);
    bucket_sort(500000,8);
    bucket_sort(500000,12);
    bucket_sort(500000,16);
    printf("-------------------------------------------------------\n");
    printf("-------------------------------------------------------\n");
    bucket_sort(1000000,1);
    bucket_sort(1000000,4);
    bucket_sort(1000000,8);
    bucket_sort(1000000,12);
    bucket_sort(1000000,16);
    printf("-------------------------------------------------------\n");
    printf("-------------------------------------------------------\n");
    bucket_sort(2000000,1);
    bucket_sort(2000000,4);
    bucket_sort(2000000,8);
    bucket_sort(2000000,12);
    bucket_sort(2000000,16);
    printf("--------------------------END--------------------------\n");
    printf("--------------------------END--------------------------\n");
}
void fill_ran(int array[], int length,int min,int max)
{
    int new_random;
    // bool* unique = (bool*)calloc(max - min + 1, sizeof(bool));
    for(int i=0;i<length;i++){
        array[i] = (rand() % (max - min +1)) + min;
    }
}
int compare(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}
