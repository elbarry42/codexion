/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:49 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap, size_t capacity, int scheduler)
{
	if (capacity < 1)
		return (1);
	heap->items = malloc(capacity * sizeof(*heap->items));
	if (heap->items)
		memset(heap->items, 0, capacity * sizeof(*heap->items));
	if (!heap->items)
		return (1);
	heap->size = 0;
	heap->capacity = capacity;
	heap->scheduler = scheduler;
	return (0);
}

void	heap_destroy(t_heap	*heap)
{
	free(heap->items);
	heap->items = NULL;
	heap->size = 0;
	heap->capacity = 0;
}

t_request	*heap_peek(t_heap	*heap)
{
	if (!heap->size)
		return (NULL);
	return (heap->items[0]);
}

int	heap_push(t_heap	*heap, t_request	*request)
{
	if (heap->size >= heap->capacity)
		return (1);
	heap->items[heap->size] = request;
	heap->size++;
	heap_sift_up(heap, heap->size - 1);
	return (0);
}

t_request	*heap_pop(t_heap	*heap)
{
	return (heap_remove_at(heap, 0));
}
