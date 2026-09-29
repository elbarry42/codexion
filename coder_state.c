/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:00 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 10:39:00 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coder_done(t_simulation	*sim, t_coder	*coder)
{
	int	done;

	pthread_mutex_lock(&sim->state_mutex);
	done = coder->done;
	pthread_mutex_unlock(&sim->state_mutex);
	return (done);
}

long long	coder_last_start(t_simulation	*sim, t_coder	*coder)
{
	long long	start;

	pthread_mutex_lock(&sim->queue_mutex);
	start = coder->last_compile_start;
	pthread_mutex_unlock(&sim->queue_mutex);
	return (start);
}
