
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * ListNode
 * A node in a linked list defined by List.
 */
struct ListNode {
    void* data; // The data stored by this node
    struct ListNode* next; // The next node in the linked list
};

/**
 * List
 * Represents a linked list.
 */
typedef struct {
    struct ListNode* head; // The first item in the list
    struct ListNode* tail; // The last item in the list
    struct ListNode* current; // The current item being looked at
    struct ListNode* last; // The last item looked at
    unsigned int length; // Number of items in the list
} List;

List* List_new();
void List_append(List* l, void* data);
void List_remove_current(List* l);
void List_remove_current_and_data(List* l);
void List_start_iteration(List* l);
void* List_next(List* l);
void List_free(List* l);
void List_print(List* l);

#endif
