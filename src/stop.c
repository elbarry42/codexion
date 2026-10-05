/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:45 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/05 13:58:23 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_stopped(t_simulation	*sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state_mutex);
	stopped = sim->stopped;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stopped);
}

static void	wake_coders(t_simulation	*sim)
{
	int	i;

	i = 0;
	while (i < sim->coder_threads_created)
	{
		pthread_mutex_lock(&sim->coders[i].cond_mutex);
		pthread_cond_broadcast(&sim->coders[i].cond);
		pthread_mutex_unlock(&sim->coders[i].cond_mutex);
		i++;
	}
}

void	log_burnout(t_simulation *sim, int coder_id)
{
	char	buffer[128];
	int		len;

	pthread_mutex_lock(&sim->output_mutex);
	pthread_mutex_lock(&sim->state_mutex);
	sim->stopped = 1;
	pthread_cond_broadcast(&sim->state_cond);
	pthread_mutex_unlock(&sim->state_mutex);
	len = snprintf(buffer, sizeof(buffer), "%lld %d burned out\n",
			elapsed_ms(sim), coder_id);
	write(1, buffer, len);
	pthread_mutex_unlock(&sim->output_mutex);
}

void	request_stop(t_simulation	*sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stopped = 1;
	pthread_cond_broadcast(&sim->state_cond);
	pthread_mutex_unlock(&sim->state_mutex);
	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
	wake_coders(sim);
}
