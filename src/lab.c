#include "lab.h"

// Function to initialize a new list
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (list == NULL) {
        return NULL;
    }
    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;
    list->head = NULL;
    return list;
}

// Function to destroy the list and all associated data
void list_destroy(list_t **list) {
    if (list == NULL || *list == NULL) {
        return;
    }
    node_t *current = (*list)->head;
    if (current != NULL) {
        do {
            node_t *next = current->next;
            if ((*list)->destroy_data) {
                (*list)->destroy_data(current->data);
            }
            free(current);
            current = next;
        } while (current != (*list)->head);
    }
    free(*list);
    *list = NULL;
}

// Function to add data to the front of the list
list_t *list_add(list_t *list, void *data) {
    if (list == NULL) {
        return NULL;
    }
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    if (list->head == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
    } else {
        node_t *tail = list->head->prev;
        new_node->next = list->head;
        new_node->prev = tail;
        tail->next = new_node;
        list->head->prev = new_node;
        list->head = new_node;
    }
    list->size++;
    return list;
}

// Function to remove data at the specified index
void *list_remove_index(list_t *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }
    node_t *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    if (current->prev != NULL) {
        current->prev->next = current->next;
    }
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }
    if (current == list->head) {
        list->head = current->next;
    }
    if (list->head == current) {
        list->head = NULL;
    } else {
        list->head->prev = current->prev;
        current->prev->next = list->head;
    }
    void *data = current->data;
    free(current);
    list->size--;
    return data;
}

// Function to search for any occurrence of data in the list
int list_indexof(list_t *list, void *data) {
    if (list == NULL) {
        return -1;
    }
    node_t *current = list->head;
    size_t index = 0;
    do {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    } while (current != list->head);
    return -1;
}