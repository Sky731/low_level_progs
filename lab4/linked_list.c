#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "linked_list.h"

Node *list_create(int data) {
    Node *head = (Node *) calloc(1, sizeof(Node));

    assert(head != NULL);

    head->data = data;
    head->next = NULL;
    return head;
}

Node *list_add_front(Node *const lst, Node *const elem) {
    assert(elem != NULL);
    assert(lst != NULL);

    elem->next = lst;
    return elem;
}

void list_add_back(Node *lst, Node *const elem) {
    assert(elem != NULL);
    assert(lst != NULL);

    while (lst->next != NULL) {
        lst = lst->next;
    }
    lst->next = elem;
}

int list_get(Node const *lst, size_t index) {
    if (lst == NULL) return 0;

    for (int i = 0; i < index; ++i) {
        if (lst->next == NULL) return 0;
        lst = lst->next;
    }
    return lst->data;
}

void list_free(Node *lst) {
    assert(lst != NULL);

    while (lst != NULL) {
        Node *next = lst->next;
        free(lst);
        lst = next;
    }
}

size_t list_length(Node const *lst) {
    for (size_t i = 0;; ++i) {
        if (lst == NULL) return i;
        lst = lst->next;
    }
}

Node *list_node_at(Node *lst, size_t index) {
    if (lst == NULL) return NULL;

    for (int i = 0; i < index; ++i) {
        if (lst->next == NULL) return NULL;
        lst = lst->next;
    }
    return lst;
}

int list_sum(Node const *lst) {
    assert(lst != NULL);

    int sum = 0;
    for (int i = 0; lst != NULL; ++i) {
        sum += lst->data;
        lst = lst->next;
    }
    return sum;
}

void push_back(Node *lst, int data) {
    assert(lst != NULL);

    Node *item = list_create(data);
    list_add_back(lst, item);
}

Node *push_front(Node *const lst, int data) {
    assert(lst != NULL);

    Node *item = list_create(data);
    Node *head = list_add_front(lst, item);
    return head;
}

void list_print(Node const *lst, char const *const delim) {
    assert(lst != NULL);
    assert(delim != NULL);

    while (lst != NULL) {
        printf("%d%s", lst->data, delim);
        lst = lst->next;
    }
    printf("\n");
}

bool compare_lists(Node const *left, Node const *right) {
    assert(left != NULL);
    assert(right != NULL);

    size_t left_size = list_length(left);
    if (left_size != list_length(right)) return false;

    for (size_t i = 0; i < left_size; ++i) {
        if (left->data != right->data) return false;
        left = left->next;
        right = right->next;
    }
    return true;
}

bool save(Node const *lst, char const *filename) {
    assert(lst != NULL);
    assert(filename != NULL);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error while opening the file \"%s\"", filename);
        return false;
    }

    size_t lst_size = list_length(lst);
    for (size_t i = 0; i < lst_size; ++i) {
        fprintf(file, "%d ", lst->data);
        lst = lst->next;
    }
    fclose(file);
    return true;
}

bool load(Node *lst, char const *filename) {
    assert(lst != NULL);
    assert(filename != NULL);
    /** To avoid memory leaks */
    assert(list_length(lst) == 1);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error while opening the file \"%s\"", filename);
        return false;
    }

    int read = 0;
    int elem = 0xDEADF00D;
    Node *prelast = NULL; // FIXME
    while ((read = fscanf(file, "%d", &elem)) != EOF) {
        if (read <= 0) {
            fclose(file);
            return false;
        }

        lst->data = elem;
        lst->next = (Node *) calloc(1, sizeof(Node));
        prelast = lst;
        lst = lst->next;
    }

    if (prelast != NULL) { // FIXME you can ask me what the ..
        free(prelast->next);
        prelast->next = NULL;
    }

    fclose(file);
    return true;
}
