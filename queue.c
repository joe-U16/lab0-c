#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"


/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;
    list_ele_t *tmp = q->head;
    while (q->head) {
        q->head = q->head->next;
        tmp->next = NULL;
        free(tmp->value);
        free(tmp);
        tmp = q->head;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
/* Don't forget to allocate space for the string and copy it */
/* What if either call to malloc returns NULL? */
/* TODO: What should you do if the q is NULL? */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }

    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));

    if (!q->head) {
        q->head = q->tail = newh;
        q->tail->next = NULL;
    } else {
        newh->next = q->head;
        q->head = newh;
    }
    q->size += 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;

    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newt->value) {
        free(newt);
        return false;
    }
    memset(newt->value, '\0', strlen(s) + 1);
    strncpy(newt->value, s, strlen(s));
    newt->next = NULL;
    if (!q->tail) {
        q->head = q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }
    q->size += 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
/* TODO: You need to fix up this code. */
/* TODO: Remove the above comment when you are about to implement. */
/*
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) return false;
    if (!sp)
        sp = q->head->value;
    q->size = q->size - 1;
    q->head = q->head->next;
    return true;
}
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }
    if (sp) {
        // Insure copy size is right
        size_t realbufsize = (bufsize > strlen(q->head->value))
                                 ? strlen(q->head->value)
                                 : bufsize - 1;
        memset(sp, '\0', realbufsize + 1);
        strncpy(sp, q->head->value, realbufsize);
    }
    list_ele_t *tmp;
    tmp = q->head;
    q->head = q->head->next;
    tmp->next = NULL;
    free(tmp->value);
    free(tmp);
    q->size -= 1;
    return true;
}
/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */

void q_reverse(queue_t *q)
{
    if (!q || q->size < 2)
        return;

    list_ele_t *tmp = q->head->next->next;
    q->tail->next = q->head->next;
    q->head->next = q->head;
    while (q->tail->next != q->tail) {
        q->tail->next->next = q->head->next;
        q->head->next = q->tail->next;
        q->tail->next = tmp;
        tmp = tmp->next;
    }
    q->tail->next->next = q->head->next;
    tmp = q->head;
    q->head = q->tail;
    q->tail = tmp;
    q->tail->next = NULL;
    tmp = NULL;
    list_ele_t *t = q->head;
    if (t == NULL)
        puts("null head");

    // printf("\n%s\n", q->head->value);
    // printf("\n%s\n", q->head->next->value);
    // printf("\n%s\n", q->tail->value);
}

void mergeSort(list_ele_t **head)
{
    if (!*head || !(*head)->next)
        return;


    list_ele_t *fast = (*head)->next;
    list_ele_t *slow = *head;

    // walk the linked list and slice it to two linked list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    slow = *head;

    mergeSort(&slow);
    mergeSort(&fast);

    *head = NULL;
    list_ele_t **tmp = head;

    while (fast && slow) {
        if (strcasecmp(slow->value, fast->value) < 0) {
            *tmp = slow;
            slow = slow->next;
        } else {
            *tmp = fast;
            fast = fast->next;
        }
        tmp = &(*tmp)->next;
    }
    *tmp = fast ? fast : slow;
}


// void merge(list_ele_t *l1, list_ele_t *l2, list_ele_t **tmp)
// {
//     if (!l2)
//         (*tmp)->next = l1;
//     if (!l1)
//         (*tmp)->next = l2;

//     if (strcasecmp(l1->value, l2->value) < 0) {
//         merge(l1->next, l2, tmp);
//         *tmp = (*tmp)->next;
//     } else
//         merge(l1, l2->next, tmp);
//         *tmp = (*tmp)->next;
// }

// list_ele_t *mergeSortList(list_ele_t *q)
// {
//     if (!q || !q->next)
//         return q;

//     list_ele_t *fast = q->next;
//     list_ele_t *slow = q;

//     while (fast && fast->next) {
//         slow = slow->next;
//         fast = fast->next->next;
//     }
//     fast = slow->next;
//     slow->next = NULL;

//     list_ele_t *l1 = mergeSortList(q);
//     list_ele_t *l2 = mergeSortList(fast);

//     merge(l1, l2, &q);
// }

void q_sort(queue_t *q)
{
    if (!q->head || !q->head->next) {
        return;
    }

    mergeSort(&q->head);

    while (q->tail->next != NULL) {
        q->tail = q->tail->next;
    }
    q->tail->next = NULL;
    return;
}
