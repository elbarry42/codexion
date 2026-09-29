/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:17 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 13:01:27 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	lock_pair(t_simulation *sim, int left, int right)
{
	pthread_mutex_lock(&sim->dongles[left].mutex);
	if (left != right)
		pthread_mutex_lock(&sim->dongles[right].mutex);
}

static int	pair_available(t_simulation *sim, int left, int right,
	long long now)
{
	if (sim->dongles[left].owner || sim->dongles[left].available_at > now)
		return (0);
	if (left != right && (sim->dongles[right].owner
			|| sim->dongles[right].available_at > now))
		return (0);
	return (1);
}

static void	unlock_pair(t_simulation *sim, int left, int right)
{
	if (left != right)
		pthread_mutex_unlock(&sim->dongles[right].mutex);
	pthread_mutex_unlock(&sim->dongles[left].mutex);
}

static void	sort_pair(int	*left, int	*right)
{
	int	tmp;

	if (*left > *right)
	{
		tmp = *left;
		*left = *right;
		*right = tmp;
	}
}

int	take_pair(t_simulation	*sim, t_coder	*coder)
{
	int			left;
	int			right;
	long long	now;

	if (sim->coder_count == 1)
		return (0);
	pair_ids(sim, coder, &left, &right);
	sort_pair(&left, &right);
	now = monotonic_ms();
	lock_pair(sim, left, right);
	if (pair_available(sim, left, right, now))
	{
		sim->dongles[left].owner = coder->id;
		if (left != right)
			sim->dongles[right].owner = coder->id;
		unlock_pair(sim, left, right);
		return (1);
	}
	unlock_pair(sim, left, right);
	return (0);
}
