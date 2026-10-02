/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:05 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_simulation(t_simulation *sim)
{
	if (init_sync(sim))
		return (1);
	if (heap_init(&sim->queue, sim->coder_count, sim->scheduler))
		return (1);
	if (init_resources(sim))
		return (1);
	return (0);
}

static void	wake_all(t_simulation *sim)
{
	int	i;

	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
	i = 0;
	while (i < sim->coder_count)
	{
		pthread_mutex_lock(&sim->coders[i].cond_mutex);
		pthread_cond_broadcast(&sim->coders[i].cond);
		pthread_mutex_unlock(&sim->coders[i].cond_mutex);
		i++;
	}
}

void	stop_and_wake(t_simulation *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stopped = 1;
	pthread_cond_broadcast(&sim->state_cond);
	pthread_mutex_unlock(&sim->state_mutex);
	wake_all(sim);
}

int	start_threads(t_simulation *sim)
{
	int	i;

	sim->start_time = monotonic_ms();
	i = 0;
	while (i < sim->coder_count)
	{
		sim->coders[i].last_compile_start = sim->start_time;
		i++;
	}
	if (pthread_create(&sim->scheduler_thread, NULL, scheduler_routine, sim))
		return (1);
	sim->scheduler_thread_created = 1;
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
		return (1);
	sim->monitor_thread_created = 1;
	return (start_coder_threads(sim));
}

void	join_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->coder_count)
	{
		if (sim->coders[i].thread)
			pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	if (sim->monitor_thread_created)
		pthread_join(sim->monitor_thread, NULL);
	if (sim->scheduler_thread_created)
		pthread_join(sim->scheduler_thread, NULL);
}
