/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIST_SORT_H
#define _LIST_SORT_H

struct list_head;

#define likely(x) (__builtin_expect(!!(x), 1))

typedef int
    __attribute__((nonnull(1, 2))) (*list_cmp_func_t)(const struct list_head *,
                                                      const struct list_head *);

__attribute__((nonnull(1, 2))) void list_sort(struct list_head *head,
                                              list_cmp_func_t cmp);
#endif
