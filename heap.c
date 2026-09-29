/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:54 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 12:56:39 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_request_before(t_heap *heap, t_request *a, t_request *b)
{
	if (heap->scheduler == CODEXION_FIFO)
		return (a->sequence < b->sequence);
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	return (a->sequence < b->sequence);
}

void	heap_sift_up(t_heap *heap, size_t index)
{
	size_t		parent;
	t_request	*tmp;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!heap_request_before(heap, heap->items[index], heap->items[parent]))
			break ;
		tmp = heap->items[index];
		heap->items[index] = heap->items[parent];
		heap->items[parent] = tmp;
		index = parent;
	}
}

static size_t	smallest_child(t_heap *heap, size_t index)
{
	size_t	left;
	size_t	right;
	size_t	smallest;

	left = index * 2 + 1;
	right = left + 1;
	smallest = index;
	if (left < heap->size
		&& heap_request_before(heap, heap->items[left], heap->items[smallest]))
		smallest = left;
	if (right < heap->size
		&& heap_request_before(heap, heap->items[right], heap->items[smallest]))
		smallest = right;
	return (smallest);
}

void	heap_sift_down(t_heap *heap, size_t index)
{
	size_t		smallest;
	t_request	*tmp;

	while (index < heap->size)
	{
		smallest = smallest_child(heap, index);
		if (smallest == index)
			break ;
		tmp = heap->items[index];
		heap->items[index] = heap->items[smallest];
		heap->items[smallest] = tmp;
		index = smallest;
	}
}
