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
    list->head->next = NULL;
    list->head->prev = NULL;

    return list;
}

// Function to destroy the list and all associated data
void list_destroy(list_t **list) {
    if (list == NULL || *list == NULL) {
        return;
    }
    node_t *current = (*list)->head->next; // Skip the sentinel node
    while (current != NULL) {
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
    if (list->head->next != NULL) {
        list->head->next->prev = new_node;
    }
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
    node_t *current = list->head->next; // Skip the sentinel node
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    if (current->prev != NULL) {
        current->prev->next = current->next;
    }
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }
    if (current == list->head->next) {
        list->head->next = current->next;
    }
    if (current == list->head->prev) {
        list->head->prev = current->prev;
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
    node_t *current = list->head->next; // Skip the sentinel node
    size_t index = 0;
    while (current != NULL) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}