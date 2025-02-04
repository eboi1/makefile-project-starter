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

    // Initialize the sentinel node
    list->head = (node_t *)malloc(sizeof(node_t));
    if (list->head == NULL) {
        free(list);
        return NULL;
    }
    list->head->data = NULL;
    list->head->next = list->head;
    list->head->prev = list->head;

    return list;
}

// Function to destroy the list and all associated data
void list_destroy(list_t **list) {
    if (list == NULL || *list == NULL) {
        return;
    }
    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *next = current->next;
        if ((*list)->destroy_data) {
            (*list)->destroy_data(current->data);
        }
        free(current);
        current = next;
    }
    free((*list)->head); // Free the sentinel node
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

    // Insert the new node after the sentinel node
    new_node->next = list->head->next;
    new_node->prev = list->head;
    list->head->next->prev = new_node;
    list->head->next = new_node;

    // Update the sentinel node's prev pointer if this is the first node
    if (list->size == 0) {
        list->head->prev = new_node;
    }

    list->size++;
    return list;
}

// Function to remove data at the specified index
void *list_remove_index(list_t *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }
    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    current->prev->next = current->next;
    current->next->prev = current->prev;

    //Removed unneccesary condition blocks after code review here

    void *data = current->data;
    free(current);
    list->size--;
    if (list->size == 0) {
        list->head->next = list->head;
        list->head->prev = list->head;
    }
    return data;
}

// Function to search for any occurrence of data in the list
int list_indexof(list_t *list, void *data) {
    if (list == NULL) {
        return -1;
    }
    node_t *current = list->head->next;
    size_t index = 0;
    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}