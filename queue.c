#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (head) {
        INIT_LIST_HEAD(head);
        return head;
    }
    return NULL;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *node, *safe;

    list_for_each_safe (node, safe, head) {
        list_del(node);
        element_t *ele = list_entry(node, element_t, list);
        free(ele->value);
        free(ele);
    }

    free(head);
}

/* Insert an element at head of queue */
// cppcheck-suppress constParameterPointer
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = (element_t *) malloc(sizeof(element_t));
    if (!new)
        return false;

    size_t len = strlen(s);
    new->value = (char *) malloc(len + 1);
    if (!new->value) {
        free(new);
        return false;
    }
    memcpy(new->value, s, len);
    new->value[len] = '\0';

    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
// cppcheck-suppress constParameterPointer
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = (element_t *) malloc(sizeof(element_t));
    if (!new)
        return false;

    size_t len = strlen(s);
    new->value = (char *) malloc(len + 1);
    if (!new->value) {
        free(new);
        return false;
    }
    memcpy(new->value, s, len);
    new->value[len] = '\0';

    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *node = head->next;
    list_del(node);
    element_t *ele = list_entry(node, element_t, list);
    if (sp && bufsize > 0) {
        size_t len = strlen(ele->value) >= bufsize - 1 ? bufsize - 1
                                                       : strlen(ele->value);
        memcpy(sp, ele->value, len);
        sp[len] = '\0';
    }
    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *node = head->prev;
    list_del(node);
    element_t *ele = list_entry(node, element_t, list);
    if (sp && bufsize > 0) {
        size_t len = strlen(ele->value) >= bufsize - 1 ? bufsize - 1
                                                       : strlen(ele->value);
        memcpy(sp, ele->value, len);
        sp[len] = '\0';
    }
    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *node;

    list_for_each (node, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *left = head->next;
    struct list_head *right = head->prev;
    while (left != right && left->next != right) {
        left = left->next;
        right = right->prev;
    }
    list_del(left);
    element_t *ele = list_entry(left, element_t, list);
    free(ele->value);
    free(ele);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
// cppcheck-suppress constParameterPointer
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    element_t *ele, *safe, *prev_ele = NULL;
    bool is_dup = false;

    // TODO: this code looks so fucking awful.
    list_for_each_entry_safe (ele, safe, head, list) {
        if (prev_ele && !strcmp(prev_ele->value, ele->value)) {
            is_dup = true;
            list_del(&ele->list);
            free(ele->value);
            free(ele);
        } else {
            if (is_dup) {
                is_dup = false;
                list_del(&prev_ele->list);
                free(prev_ele->value);
                free(prev_ele);
            }
            prev_ele = ele;
        }
    }
    if (is_dup) {
        list_del(&prev_ele->list);
        free(prev_ele->value);
        free(prev_ele);
    }
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    struct list_head *prev = NULL, *node, *safe;
    list_for_each_safe (node, safe, head) {
        if (!prev) {
            prev = node;
        } else {
            list_del(prev);
            list_add(prev, node);
            prev = NULL;
        }
    }

    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *tail = head->prev;
    while (head->next != tail) {
        struct list_head *node = head->next;
        list_del(node);
        list_add(node, tail);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head))
        return;
    struct list_head *new_head = head, *tail, *temp, *temp_head;
    struct list_head *node, *safe;
    int count = 0;
    list_for_each_safe (node, safe, head) {
        count++;
        if (count == k) {
            count = 0;
            tail = node;
            temp_head = new_head->next;
            while (new_head->next != tail) {
                temp = new_head->next;
                list_del(temp);
                list_add(temp, tail);
            }
            new_head = temp_head;
        }
    }
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

#define STACK_LEVELS 1000  // NOTE: non-recursive quick sort max stack level
#define cmp(a, b)                                  \
    (strcmp(list_entry(a, element_t, list)->value, \
            list_entry(b, element_t, list)->value))

    int sp = 0;  // stack pointer
    struct list_head *beg[STACK_LEVELS], *end[STACK_LEVELS], *L, *R, *pivot,
        *temp;
    beg[0] = head->next;
    end[0] = head->prev;
    while (sp >= 0) {
        L = beg[sp];
        R = end[sp];
        if (L != R) {
            pivot = L;
            while (L != R) {
                while (L != R && cmp(R, pivot) > 0) {
                    R = R->prev;
                }
                if (L != R) {
                    temp = R;
                    R = R->prev;
                    list_del(temp);
                    list_add(temp, L);
                }
                while (L != R && cmp(L, pivot) < 0) {
                    L = L->next;
                }
                if (L != R) {
                    temp = L;
                    L = L->next;
                    list_del(temp);
                    list_add(temp, R);
                }
            }
            list_del(pivot);
            if (cmp(L, pivot) > 0) {
                list_add(pivot, L->prev);
            } else {
                list_add(pivot, L);
            }
            beg[sp + 1] = pivot->next;
            end[sp + 1] = end[sp];
            end[sp++] = pivot->prev;
        } else {
            sp--;
        }
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
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
