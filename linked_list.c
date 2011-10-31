/**
 * linked_list.c
 * Implements a singly linked list data structure.
 *
 * The list stores void pointers, so you can store anything you want.  Remember
 * to free the memory pointed to by the stored pointer before deleting the node
 * with List_remove_current().  Alternatively you canuse
 * List_remove_current_and_data().
 *
 * To iterate through the list, use List_start_iteration() and List_next() as
 * shown in this example:
 *
 *   List_start_iteration(bullets);
 *   Bullet* bullet;
 *   while ((bullet = (Bullet*) List_next(bullets)))
 *   {
 *       Bullet_draw(bullet);
 *   }
 *
 * This implementation is slow for large lists.  It could be expanded to
 * allocate memory in chunks if extra speed is needed.
 */

#include <stdlib.h>
#include <stdio.h>

#include "linked_list.h"

/**
 * List_new
 * Create a new, empty List.
 */
List* List_new() {
    List* l = (List*) malloc(sizeof(List));
    if (!l) {
        printf("Could not allocate memory!\n");
        printf("    In List_new()\n");
        exit(1);
    }

    l->head = NULL;
    l->tail = NULL;
    l->current = NULL;
    l->length = 0;

    return l;
}

/**
 * List_append
 * Adds the given item to the end of the list.
 */
void List_append(List* l, void* data)
{
    struct ListNode* node = (struct ListNode*) malloc(sizeof(struct ListNode));
    node->data = data;
    node->next = NULL;

    if (l->length == 0) {
        l->head = node;
    } else {
        l->tail->next = node;
    }
    l->tail = node;
    l->length++;
}

/**
 * List_remove_current
 * Removes the current item in the list.
 *
 * This can be used in conjunction with List_next to remove the last item
 * recieved.
 *
 * Make sure to free the memory of the stored data before calling this
 * function to prevent memory leaks.  Alternatively you could use
 * List_remove_current_and_data().
 */
void List_remove_current(List* l)
{
    if (l->length == 0) return;
    if (l->current == NULL) return; // Pointing before first or after last

    struct ListNode* to_delete = l->current;
    if (l->last == NULL) {
        // The current item is head
        l->head = l->head->next;
    } else {
        l->last->next = l->current->next;
    }

    l->current = l->last;
    if (to_delete == l->tail) {
        // Deleting the last item
        l->tail = l->current;
    }
    free(to_delete);
    l->length--;
}

/**
 * List_remove_current_and_data
 * Removes the current item in the list and frees the data pointer.
 *
 * This can be used in conjunction with List_next to remove the last item
 * recieved.
 */
void List_remove_current_and_data(List* l)
{
    if (l->current == NULL) return;
    free(l->current->data);
    List_remove_current(l);
}

/**
 * List_start_iteration
 * Resets the internal current node pointer to the beggining.
 *
 * This can be used in conjunction with List_next to iterate through every item
 * in a list.
 */
void List_start_iteration(List* l)
{
    l->last = NULL;
    l->current = NULL;
}

/**
 * List_next
 * Gets the next item in the list.  The internal current node pointer will move
 * forward one.
 */
void* List_next(List* l) {
    if (l->head == NULL) return NULL; // Empty list
    if (l->current == NULL) {
        // First time List_next has been called after start_iteration.
        l->current = l->head;
        return l->current->data;
    } else {
        l->last = l->current;
        l->current = l->current->next;
        if (l->current == NULL) {
            return NULL;
        } else {
            return l->current->data;
        }
    }
}

/**
 * List_free
 * Free the memory used by the list.
 *
 * Notice that the items that are stored in the list are not freed, that must
 * be done by the user.
 */
void List_free(List* l) {
    //TODO
}

/**
 * List_print
 * Prints the given list to stdout.
 *
 * Note that this resets the iteration.
 */
void List_print(List* l) {
    printf("List:\n");
    List_start_iteration(l);
    void* item;
    while ((item = List_next(l))) {
        printf("  Data at: %p\n", item);
    }
}
