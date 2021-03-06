#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include "linked_list.h"
#include "higher_order.h"

void list_test();

void high_order_test();

Node *cli_fill_list();

#define ELEM_COUNTER 10

void speed_comparing_test() {
    int *array = calloc(ELEM_COUNTER, sizeof(int));
    assert(array != NULL);

    for (int i = 0; i < ELEM_COUNTER; ++i) {
        array[i] = i;
    }

    Node *list = NULL;
    for (int i = 0; i < ELEM_COUNTER; ++i) {
        if (0 == i) {
            list = list_create(i);
            continue;
        }
        push_back(list, i);
    }


    printf("N ==== Array Time  ==== List Time   ======== V1 / V2\n");
    for (size_t j = 0; j < ELEM_COUNTER; ++j) {
        if (j > 10 && j % 10 != 0) continue;

        clock_t arr_begin = clock();

        volatile int arr_elem = 0xDEADF00D;
        for (int k = 0; k < 100000; ++k) {
            arr_elem = array[j];
        }

        clock_t arr_end = clock();
        double arr_time = (double) (arr_end - arr_begin) / CLOCKS_PER_SEC;

        clock_t lst_begin = clock();

        volatile int lst_elem = 0xDEADF00D;
        for (int k = 0; k < 100000; ++k) {
            lst_elem = list_get(list, j);
        }

        clock_t lst_end = clock();
        double lst_time = (double) (lst_end - lst_begin) / CLOCKS_PER_SEC;

        printf("%lu ---- %lf ms ---- %lf ms -------- %d  /  %d\n", j, arr_time, lst_time, arr_elem, lst_elem);
    }


}

int main() {
    // list_test();

    // high_order_test();

    speed_comparing_test();

    return 0;
}

Node *cli_fill_list() {
    printf("Enter integers to the new list, then press 'Enter': \n");

    char *str = calloc(256, sizeof(char));
    int x = 0;
    Node *list = NULL;

    fgets(str, 254, stdin);

    char *token = strtok(str, " \n");
    assert(token != NULL);

    while (token != NULL) {
        x = (int) strtol(token, NULL, 10);

        char parsed_as_str[21];
        sprintf(parsed_as_str, "%d", x);

        if (strcmp(parsed_as_str, token) != 0) {
            token = strtok(NULL, " \n");
            continue;
        }

        if (list == NULL) {
            list = list_create(x);
            token = strtok(NULL, " \n");
            continue;
        }

        list = push_front(list, x);
        token = strtok(NULL, " \n");
    }

    assert(list != NULL);

    free(str);
    return list;
}

void print_int_with_space(int x) {
    printf("%d ", x);
}

void print_int_with_newline(int x) {
    printf("%d\n", x);
}

int sqr(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}

int min(int left, int right) {
    if (left < right) return left;
    else return right;
}

int max(int left, int right) {
    if (left > right) return left;
    else return right;
}

int sum(int left, int right) {
    return left + right;
}

int double_it(int x) {
    return 2 * x;
}

void high_order_test() {
    Node *list = cli_fill_list();

    printf("\n=== Print list with space character ===\n");
    foreach(list, &print_int_with_space);

    printf("\n\n=== Print list with newline character ===\n");
    foreach(list, &print_int_with_newline);

    Node *sqr_list = map(list, &sqr);
    printf("\n=== Print squares of the list ===\n");
    list_print(sqr_list, " ");

    Node *cube_list = map(list, &cube);
    printf("\n=== Print cubes of the list ===\n");
    list_print(cube_list, " ");

    printf("\n=== Print the minimal value of the list ===\n");
    printf("%d\n", foldl(list, &min, INT_MAX));

    printf("\n=== Print the maximum value of the list ===\n");
    printf("%d\n", foldl(list, &max, INT_MIN));

    printf("\n=== Print the sum of all elements value of the list ===\n");
    printf("%d\n", foldl(list, &sum, 0));

    printf("\n=== Print the module of each value of the list ===\n");
    map_mut(list, &abs);
    list_print(list, " ");

    list_free(list);
    list_free(sqr_list);
    list_free(cube_list);
}

void list_test() {
    Node *list = cli_fill_list();

    printf("List items:\n\t");
    list_print(list, " ");
    printf("Length of the list: %lu\n", list_length(list));
    printf("Sum of elements: %d\n", list_sum(list));

    list_free(list);

    Node *seq_list = list_create(1);
    iterate(seq_list, &double_it, 10);
    printf("\n=== Print first 10 values of the sequence degrees of 2 ===\n");
    list_print(seq_list, " ");

    printf("\n=== Saving the seq_list to the text file ===\n");
    if (save(seq_list, "linked_list")) {
        printf("\t Saving is successful \n");
        printf("\n=== Loading the seq_list from the text file ===\n");
        Node *read_list = NULL;

        if (load(&read_list, "linked_list")) {
            printf("\t Loading is successful \n");
            printf("\t Comparing source and received list.. \n");
            if (compare_lists(seq_list, read_list)) {
                printf("\t\t Lists are equals\n");
            } else {
                printf("\t\t Lists are different, test failed :(\n");
            }
        } else {
            printf("\t Loading failed :( \n");
        }

        list_free(read_list);
    } else {
        printf("\t Saving failed :( \n");
    }

    printf("\n=== Saving the seq_list to the binary file ===\n");
    if (serialize(seq_list, "linked_list_binary")) {
        printf("\t Saving is successful \n");
        printf("\n=== Loading the seq_list from the binary file ===\n");
        Node *read_list = NULL;

        if (deserialize(&read_list, "linked_list_binary")) {
            printf("\t Loading is successful \n");
            printf("\t Comparing source and received list.. \n");
            if (compare_lists(seq_list, read_list)) {
                printf("\t\t Lists are equals\n");
            } else {
                printf("\t\t Lists are different, test failed :(\n");
            }
        } else {
            printf("\t Loading failed :( \n");
        }

        list_free(read_list);
    } else {
        printf("\t Saving failed :( \n");
    }

    list_free(seq_list);
}
