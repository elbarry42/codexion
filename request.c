/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:35 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/30 16:24:54 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_dongles(t_coder	*coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	if (simulation_stopped(sim))
		return (0);
	pthread_mutex_lock(&coder->cond_mutex);
	coder->granted = 0;
	pthread_mutex_unlock(&coder->cond_mutex);
	pthread_mutex_lock(&sim->queue_mutex);
	coder->request.sequence = sim->next_sequence++;
	coder->request.deadline = coder->last_compile_start + sim->time_to_burnout;
	coder->request.active = 1;
	coder->waiting = 1;
	if (heap_push(&sim->queue, &coder->request))
	{
		coder->request.active = 0;
		coder->waiting = 0;
		pthread_mutex_unlock(&sim->queue_mutex);
		request_stop(sim);
		return (0);
	}
	pthread_cond_signal(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
	return (wait_for_grant(coder));
}

int	wait_for_grant(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&coder->cond_mutex);
	while (!coder->granted)
	{
		cond_wait_ms(&coder->cond, &coder->cond_mutex, 5);
		pthread_mutex_unlock(&coder->cond_mutex);
		if (simulation_stopped(sim))
			return (0);
		pthread_mutex_lock(&coder->cond_mutex);
	}
	coder->granted = 0;
	pthread_mutex_unlock(&coder->cond_mutex);
	return (1);
}

void	mark_coder_complete(t_simulation	*sim, t_coder	*coder)
{
	pthread_mutex_lock(&sim->state_mutex);
	coder->done = 1;
	sim->completed_coders++;
	if (sim->completed_coders == sim->coder_count)
		sim->stopped = 1;
	pthread_cond_broadcast(&sim->state_cond);
	pthread_mutex_unlock(&sim->state_mutex);
	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
}
