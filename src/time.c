/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:41:05 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	monotonic_ms(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
		return (0);
	return ((long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL);
}

long long	elapsed_ms(t_simulation *sim)
{
	return (monotonic_ms() - sim->start_time);
}

static void	add_ms_to_timespec(struct timespec *ts, long long delay_ms)
{
	ts->tv_sec += delay_ms / 1000LL;
	ts->tv_nsec += (delay_ms % 1000LL) * 1000000L;
	if (ts->tv_nsec >= 1000000000L)
	{
		ts->tv_sec++;
		ts->tv_nsec -= 1000000000L;
	}
}

int	cond_wait_ms(pthread_cond_t *cond, pthread_mutex_t *mutex,
	long long delay_ms)
{
	struct timespec	ts;
	int				rc;

	if (delay_ms < 1 || clock_gettime(CLOCK_REALTIME, &ts) != 0)
		return (1);
	add_ms_to_timespec(&ts, delay_ms);
	rc = pthread_cond_timedwait(cond, mutex, &ts);
	if (rc != 0 && rc != ETIMEDOUT)
		return (1);
	return (0);
}
