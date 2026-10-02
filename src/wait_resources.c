/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_resources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:00 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	next_wakeup_for_queue(t_simulation *sim, t_request *request,
	long long now)
{
	int			left;
	int			right;
	long long	wake;
	long long	value;

	pair_ids(sim, request->coder, &left, &right);
	wake = now + 5;
	pthread_mutex_lock(&sim->dongles[left].mutex);
	value = sim->dongles[left].available_at;
	pthread_mutex_unlock(&sim->dongles[left].mutex);
	if (value > now && value < wake)
		wake = value;
	if (left != right)
	{
		pthread_mutex_lock(&sim->dongles[right].mutex);
		value = sim->dongles[right].available_at;
		pthread_mutex_unlock(&sim->dongles[right].mutex);
		if (value > now && value < wake)
			wake = value;
	}
	return (wake);
}
