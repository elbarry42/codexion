/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:33 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 16:07:01 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

# define CODEXION_FIFO 0
# define CODEXION_EDF 1

typedef struct s_simulation	t_simulation;
typedef struct s_coder		t_coder;
typedef struct s_request	t_request;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	int				mutex_initialized;
	long long		available_at;
	int				owner;
}	t_dongle;

typedef struct s_heap
{
	t_request	**items;
	size_t		size;
	size_t		capacity;
	int			scheduler;
}	t_heap;

typedef struct s_request
{
	t_coder				*coder;
	unsigned long long	sequence;
	long long			deadline;
	int					active;
}	t_request;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	pthread_cond_t	cond;
	pthread_mutex_t	cond_mutex;
	int				cond_initialized;
	int				cond_mutex_initialized;
	t_simulation	*sim;
	t_request		request;
	long long		last_compile_start;
	int				compile_count;
	int				done;
	int				waiting;
	int				granted;
}	t_coder;

typedef struct s_simulation
{
	int					coder_count;
	long long			time_to_burnout;
	long long			time_to_compile;
	long long			time_to_debug;
	long long			time_to_refactor;
	int					required_compiles;
	long long			dongle_cooldown;
	int					scheduler;
	long long			start_time;
	int					stopped;
	int					monitor_done;
	int					scheduler_done;
	int					initialization_failed;
	int					completed_coders;
	unsigned long long	next_sequence;
	pthread_mutex_t		state_mutex;
	pthread_mutex_t		output_mutex;
	pthread_mutex_t		queue_mutex;
	pthread_cond_t		queue_cond;
	pthread_cond_t		state_cond;
	int					state_mutex_initialized;
	int					output_mutex_initialized;
	int					queue_mutex_initialized;
	int					queue_cond_initialized;
	int					state_cond_initialized;
	pthread_t			scheduler_thread;
	pthread_t			monitor_thread;
	int					scheduler_thread_created;
	int					monitor_thread_created;
	int					coder_threads_created;
	int					thread_creation_failed;
	t_dongle			*dongles;
	t_coder				*coders;
	t_heap				queue;
}	t_simulation;

/* Parsing. */
int			parse_args(int argc, char **argv, t_simulation *sim);
long long	parse_nonnegative_ll(const char	*s, int	*ok);
long long	parse_positive_ll(const char *s, int *ok);
int			parse_positive_int(const char *s, int *ok);

/* Time and waits. */
long long	monotonic_ms(void);
long long	elapsed_ms(t_simulation	*sim);
int			cond_wait_ms(pthread_cond_t	*cond, pthread_mutex_t *mutex,
				long long delay_ms);

/* Heap. */
int			heap_init(t_heap *heap, size_t capacity, int scheduler);
void		heap_destroy(t_heap	*heap);
int			heap_push(t_heap	*heap, t_request	*request);
void		heap_sift_up(t_heap	*heap, size_t	index);
void		heap_sift_down(t_heap	*heap, size_t	index);
int			heap_request_before(t_heap	*heap, t_request *a, t_request *b);
t_request	*heap_peek(t_heap *heap);
t_request	*heap_pop(t_heap *heap);
t_request	*heap_remove_at(t_heap	*heap, size_t	index);

/* Synchronization and logging. */
int			simulation_stopped(t_simulation	*sim);
void		request_stop(t_simulation *sim);
void		log_state(t_simulation	*sim, int coder_id, const char	*message);
void		log_compile_start(t_simulation *sim, int coder_id);
void		pair_ids(t_simulation *sim, t_coder *coder, int	*left, int *right);

/* Dongles and resource scheduling. */
int			take_pair(t_simulation	*sim, t_coder	*coder);
void		release_pair(t_simulation	*sim, t_coder	*coder);
int			dongles_ready(t_simulation *sim, t_coder *coder, long long now);
long long	next_wakeup_for_queue(t_simulation	*sim, t_request	*request,
				long long now);

/* Scheduler queue operations. */
size_t		snapshot_requests(t_simulation	*sim, t_request	**requests);
int			grant_request(t_simulation	*sim, t_request	*request);
int			remove_request(t_simulation	*sim, t_request	*request);
void		requeue_request(t_simulation *sim, t_request *request);

/* Threads. */
void		*scheduler_routine(void	*arg);
void		scheduler_wait(t_simulation	*sim);
void		*coder_routine(void	*arg);
void		*monitor_routine(void *arg);
long long	next_deadline(t_simulation *sim, long long now);
void		monitor_wait(t_simulation *sim, long long wait_ms);
int			coder_done(t_simulation	*sim, t_coder *coder);
long long	coder_last_start(t_simulation *sim, t_coder *coder);
int			request_dongles(t_coder	*coder);
int			wait_for_grant(t_coder	*coder);
void		mark_coder_complete(t_simulation *sim, t_coder *coder);

/* Lifecycle. */
int			init_simulation(t_simulation *sim);
int			init_sync(t_simulation *sim);
int			init_resources(t_simulation	*sim);
int			start_threads(t_simulation	*sim);
int			start_coder_threads(t_simulation *sim);
void		stop_and_wake(t_simulation *sim);
void		join_threads(t_simulation *sim);
void		destroy_simulation(t_simulation *sim);

#endif
