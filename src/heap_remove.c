/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:00 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	*heap_remove_at(t_heap	*heap, size_t	index)
{
	t_request	*result;

	if (index >= heap->size)
		return (NULL);
	result = heap->items[index];
	heap->size--;
	if (index != heap->size)
	{
		heap->items[index] = heap->items[heap->size];
		heap->items[heap->size] = NULL;
		if (index > 0
			&& heap_request_before(heap, heap->items[index],
				heap->items[(index - 1) / 2]))
			heap_sift_up(heap, index);
		else
			heap_sift_down(heap, index);
	}
	else
		heap->items[index] = NULL;
	return (result);
}
