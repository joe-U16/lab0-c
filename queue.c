#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"


queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void q_free(queue_t *q)
{
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

bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    if (sp) {
        size_t buf;
        if (bufsize > strlen(q->head->value)) {
            buf = strlen(q->head->value);
        } else {
            buf = bufsize - 1;
        }
        memset(sp, '\0', buf + 1);
        strncpy(sp, q->head->value, buf);
    }

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    tmp->next = NULL;
    free(tmp->value);
    free(tmp);
    q->size -= 1;
    return true;
}

int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    q->tail = q->head;

    list_ele_t **node = &q->head;
    list_ele_t *left = NULL;
    list_ele_t *right = (*node)->next;
    while (right) {
        (*node)->next = left;
        left = (*node);
        (*node) = right;
        right = right->next;
    }
    (*node)->next = left;
    q->head = (*node);
    left = NULL;
    right = NULL;
    return;
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

void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }

    mergeSort(&q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }

    return;
}
