#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *current, *next;
    list_for_each_safe (current, next, head) {
        element_t *elem = list_entry(current, element_t, list);
        free(elem->value);
        free(elem);
    }

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *new_elememt_t = malloc(sizeof(element_t));

    if (!new_elememt_t)
        return false;

    new_elememt_t->value = strdup(s);
    if (!new_elememt_t->value) {
        free(new_elememt_t);
        return false;
    }

    list_add(&new_elememt_t->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new_element_t = malloc(sizeof(element_t));
    if (!new_element_t)
        return false;

    new_element_t->value = strdup(s);
    if (!new_element_t->value) {
        free(new_element_t);
        return false;
    }

    list_add_tail(&new_element_t->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *first = head->next;
    element_t *elem = list_entry(first, element_t, list);
    list_del(first);

    if (sp && bufsize > 0) {
        strncpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return elem;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *last = head->prev;
    element_t *elem = list_entry(last, element_t, list);

    if (sp && elem->value) {
        strncpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(last);
    return elem;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int number = 0;

    if (!head || list_empty(head))
        return number;

    struct list_head *current;

    list_for_each (current, head)
        number++;


    return number;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    int size = q_size(head);
    int mid_index = size / 2;

    struct list_head *node = head->next;
    for (int i = 0; i < mid_index; i++)
        node = node->next;

    list_del(node);
    element_t *elem = list_entry(node, element_t, list);
    free(elem->value);
    free(elem);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *current = head->next;

    while (current != head) {
        element_t *current_entry = list_entry(current, element_t, list);
        struct list_head *next = current->next;
        bool duplicate = false;

        while (next != head) {
            element_t *next_entry = list_entry(next, element_t, list);
            if (strcmp(current_entry->value, next_entry->value) != 0)
                break;

            struct list_head *safe = next->next;
            duplicate = true;
            list_del(next);
            free(next_entry->value);
            free(next_entry);
            next = safe;
        }

        if (duplicate) {
            list_del(current);
            free(current_entry->value);
            free(current_entry);
        }

        current = next;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || head->next->next == head)
        return;

    struct list_head *first = head->next, *second = first->next;

    while (first != head && second != head) {
        struct list_head *prev = first->prev, *next = second->next;

        prev->next = second;
        second->prev = prev;
        second->next = first;
        first->prev = second;
        first->next = next;
        next->prev = first;

        first = first->next;
        second = first->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *current, *safe;
    list_for_each_safe (current, safe, head) {
        list_move(current, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/

    if (!head || list_empty(head) || head->next->next == head || k <= 1)
        return;

    struct list_head *current = head->next, *safe = head->next;

    while (current != head) {
        bool is_valid = true;
        struct list_head *group_head = current->prev;

        for (int i = 0; i < k; i++) {
            if (safe == head) {
                is_valid = false;
                break;
            }
            safe = safe->next;
        }

        if (is_valid) {
            while (current != safe) {
                struct list_head *temp = current->next;

                list_move(current, group_head);
                current = temp;
            }
        } else
            break;
    }
}

void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head sorted;
    INIT_LIST_HEAD(&sorted);

    while (!list_empty(head)) {
        struct list_head *node = head->next;
        list_del(node);

        struct list_head *pos;
        bool inserted = false;
        list_for_each (pos, &sorted) {
            const element_t *entry_sorted = list_entry(pos, element_t, list);
            const element_t *entry_node = list_entry(node, element_t, list);


            int cmp = strcmp(entry_node->value, entry_sorted->value);
            if (descend)
                cmp = -cmp;

            if (cmp < 0) {
                node->next = pos;
                node->prev = pos->prev;
                pos->prev->next = node;
                pos->prev = node;
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            list_add_tail(node, &sorted);
        }
    }
    list_splice_tail(&sorted, head);
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return 1;

    struct list_head *current = head->prev, *safe;
    element_t *current_entry;
    const char *min_val = list_entry(current, element_t, list)->value;

    while (current != head) {
        safe = current->prev;
        current_entry = list_entry(current, element_t, list);

        if (strcmp(current_entry->value, min_val) > 0) {
            list_del(current);
            free(current_entry);
        } else if (strcmp(current_entry->value, min_val) < 0) {
            min_val = current_entry->value;
        }

        current = safe;
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}