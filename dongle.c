/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:44 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 12:58:15 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	pair_ids(t_simulation *sim, t_coder	*coder, int	*left, int *right)
{
	*left = (coder->id - 1 + sim->coder_count) % sim->coder_count;
	*right = coder->id % sim->coder_count;
	if (sim->coder_count == 1)
		*right = *left;
}

static void	release_dongle(t_dongle	*dongle, t_simulation *sim,
	long long release_time)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->owner = 0;
	dongle->available_at = release_time + sim->dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_pair(t_simulation	*sim, t_coder	*coder)
{
	int			left;
	int			right;
	long long	release_time;

	pair_ids(sim, coder, &left, &right);
	release_time = monotonic_ms();
	release_dongle(&sim->dongles[left], sim, release_time);
	if (left != right)
		release_dongle(&sim->dongles[right], sim, release_time);
	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
}

int	dongles_ready(t_simulation *sim, t_coder *coder, long long now)
{
	int	left;
	int	right;
	int	ready;

	if (sim->coder_count == 1)
		return (0);
	pair_ids(sim, coder, &left, &right);
	pthread_mutex_lock(&sim->dongles[left].mutex);
	ready = !sim->dongles[left].owner
		&& sim->dongles[left].available_at <= now;
	pthread_mutex_unlock(&sim->dongles[left].mutex);
	if (!ready || left == right)
		return (ready);
	pthread_mutex_lock(&sim->dongles[right].mutex);
	ready = !sim->dongles[right].owner
		&& sim->dongles[right].available_at <= now;
	pthread_mutex_unlock(&sim->dongles[right].mutex);
	return (ready);
}
