/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_ops.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:00 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 10:39:00 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

size_t	snapshot_requests(t_simulation	*sim, t_request	**requests)
{
	size_t	count;
	size_t	i;

	pthread_mutex_lock(&sim->queue_mutex);
	count = 0;
	i = 0;
	while (i < sim->queue.size)
	{
		if (sim->queue.items[i]->active)
			requests[count++] = sim->queue.items[i];
		i++;
	}
	pthread_mutex_unlock(&sim->queue_mutex);
	return (count);
}

int	grant_request(t_simulation	*sim, t_request	*request)
{
	t_coder	*coder;

	coder = request->coder;
	pthread_mutex_lock(&sim->queue_mutex);
	request->active = 0;
	coder->waiting = 0;
	pthread_mutex_unlock(&sim->queue_mutex);
	pthread_mutex_lock(&coder->cond_mutex);
	coder->granted = 1;
	pthread_cond_signal(&coder->cond);
	pthread_mutex_unlock(&coder->cond_mutex);
	return (1);
}

int	remove_request(t_simulation	*sim, t_request	*request)
{
	size_t	i;

	pthread_mutex_lock(&sim->queue_mutex);
	i = 0;
	while (i < sim->queue.size && sim->queue.items[i] != request)
		i++;
	if (i == sim->queue.size)
	{
		pthread_mutex_unlock(&sim->queue_mutex);
		return (0);
	}
	heap_remove_at(&sim->queue, i);
	pthread_mutex_unlock(&sim->queue_mutex);
	return (1);
}

void	requeue_request(t_simulation	*sim, t_request	*request)
{
	pthread_mutex_lock(&sim->queue_mutex);
	heap_push(&sim->queue, request);
	pthread_cond_signal(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
}
