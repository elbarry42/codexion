/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:41:09 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 11:05:11 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	scheduler_wait(t_simulation	*sim)
{
	t_request	*request;
	long long	now;
	long long	wait_ms;

	pthread_mutex_lock(&sim->queue_mutex);
	request = heap_peek(&sim->queue);
	pthread_mutex_unlock(&sim->queue_mutex);
	if (!request)
	{
		pthread_mutex_lock(&sim->queue_mutex);
		if (!sim->queue.size)
			pthread_cond_wait(&sim->queue_cond, &sim->queue_mutex);
		pthread_mutex_unlock(&sim->queue_mutex);
		return ;
	}
	now = monotonic_ms();
	wait_ms = next_wakeup_for_queue(sim, request, now) - now;
	if (wait_ms < 1)
		wait_ms = 1;
	pthread_mutex_lock(&sim->queue_mutex);
	cond_wait_ms(&sim->queue_cond, &sim->queue_mutex, wait_ms);
	pthread_mutex_unlock(&sim->queue_mutex);
}
