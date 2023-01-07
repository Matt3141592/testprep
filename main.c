//
//  main.c
//  testprep
//
//  Created by Matthew Lee on 02/01/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // usually declared as a global
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int count = 0;

typedef struct node //typedef done so that sturct isn't needed when making a node variable
{
    int x;
    struct node *next;
}node;


typedef struct point
{
    int x, y;
}point;

struct pair
{
    int x, y;
};

typedef struct pair Pair; // no longer need struct to use pair and is now called Pair, still able to use struct pair though

int call(void)
{
    static int counter = 0; // value doesn't reset each time the function is called.
    return counter++;
}

int sum(int x, int y)
{
    return x + y;
}

void first(void)
{
    for (int i = 0; i < 5; i++)
    {
        sleep(1);
        printf("%d\n", i);
    }
}

void second(void)
{
    for (int i = 4; i >= 0 ; i--)
    {
        sleep(1);
        printf("%d\n", i);
    }
}

void dead(void)
{
    pthread_mutex_lock(&mutex);
    sleep(1);
    pthread_mutex_lock(&mutex1);
    count++;
    printf("%d\n", count);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex1);
}

void lock(void)
{
    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex);
    count++;
    printf("%d\n", count);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex1);
}



int main(void)
{
    node n = {4, NULL}; // done in the order of the structure, if there are less than the number of types in the structure, the remaining are set to NULL or 0.
    printf("%d\n", n.x);
    
    node *new = malloc(sizeof(node));
    new -> x = 1;
    printf("%d\n", (*new).x);
    free(new);
    
    point a = {1,2};
    printf("%d %d\n\n", a.x, a.y); // must be accessed with . because a is not a pointer.
    
    for (int i = 0; i < 5; i++)
        printf("%d\n", call());
    
    int *arr = malloc(4 * 10);
    for (int i = 0; i < 10; i++)
        arr[i] = i;
    for (int i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    free(arr);
    printf("\n\n");
    
    int *p[4];
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        p[i] = malloc(4 * sizeof(int)); // should be checking whether malloc returns NULL
        for (int j = 0; j < 4; j++)
            p[i][j] = count++; // 4 * i + j also works
    }
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%.2d ", p[i][j]);
        printf("\n");
        free(p[i]);
    }
    
    unsigned int test = 0x12345678; // hexadecimal
    unsigned int temp = test;
    unsigned int result, result1;
    char *ptest = (char *)&test;
    char *presult = (char *)&result; // (char *) type cast not necessary but stops warning
    char *presult1 = (char *)&result1;
    for (int i = 0; i < 4; i++)
        presult[i] = ptest[i]; // copies byte by byte. Little eldian to little eldian
    for (int i = 0; i < 4; i++)
    {
        presult1[i] = temp & 0xFF; // copies last byte and adds to the result. Little eldian so least signficant first.
        temp = temp >> 8;          // not little eldian to little eldian taking mod 256.
    }
    
    printf("%d %d %d\n\n", test, result, result1);
    
    void *voidp;
    int x = 4;
    voidp = &x;
    printf("%d\n", *(int*)voidp); // doesn't work without casting to the correct pointer type first, but then it can be reused to hold a pointer of a different type
    
    int (*intp)(void) = call; // return_type (*name_of_variable)(argument_types).
    int (*pint)(int, int) = sum;
    printf("%p %p\n", intp, pint);
    printf("%d\n", (*pint)(3,6)); // function is called by dereferencing the pointer. Can be used to pass functions into other functions
    
    int length = 3;
    int arr1[length]; // can be useful to malloc for array if size is going to change.
    for (int i = 0; i < 4; i++)
        arr1[i] = i;
    for (int i = 0; i < 4; i++)
        printf("%d ", arr1[i]);
    printf("\n");
    
    //char one[3]; // stored in the stack so stores from a higher memory address first. So a buffer overflow of the second variable will write
    //char two[3]; // to the first
    //printf("Enter here (more than 3 characters): ");
    //scanf("%s", two);
    //printf("%s %s\n", one, two);
    
    int *nums = malloc(4 * sizeof(int));
    void *pnums = nums;
    ((int *)pnums)[0] = 4; // must cast to int pointer first
    
    FILE *file = fopen("test.txt", "w");
    fprintf(file, "Hello");
    fclose(file);
    file = fopen("test.txt", "r");
    char str[10];
    fscanf(file, "%s", str);
    printf("%s\n\n", str);
    
    pthread_t threads[4];
    
    pthread_create(&threads[0], NULL, (void *)first, NULL);
    pthread_create(&threads[1], NULL, (void *)second, NULL);
    //pthread_join(threads[1], NULL);
    
    
    pthread_create(&threads[2], NULL, (void *)dead, NULL); //deadlock
    pthread_create(&threads[3], NULL, (void *)lock, NULL);
    //pthread_join(threads[2], NULL); // uncomment to deadlock
}

// an advantage to passing pointers instead of variables themselves is that only the pointer is copied which could mean less memory is required.
