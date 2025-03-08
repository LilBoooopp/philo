/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbopp <cbopp@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 12:08:50 by cbopp             #+#    #+#             */
/*   Updated: 2024/12/31 12:25:18 by cbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

# define SUCCESS 0
# define ERROR 1

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;

typedef struct s_table
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	long long		start_time;
	bool			simulation_end;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	t_fork			*forks;
}	t_table;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long long		last_meal;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_table			*table;
	pthread_t		thread;
}	t_philo;

/* init.c */
int			init_table(t_table *table, int argc, char **argv);
int			init_philosophers(t_philo **philos, t_table *table);
int			init_mutexes(t_table *table);

/* utils.c */
int			ft_atoi(const char *str);
long long	get_time(void);
void		print_status(t_philo *philo, char *status);
void		smart_sleep(long long duration, t_table *table);
int			check_arguments(int argc, char **argv);

/* routine.c */
void		*philosopher_routine(void *arg);
void		eat(t_philo *philo);
void		sleep_and_think(t_philo *philo);

/* monitor.c */
int			start_simulation(t_philo *philos, t_table *table);
void		*death_monitor(void *arg);
bool		check_death(t_philo *philo);
bool		check_meals(t_philo *philos, t_table *table);

/* cleanup.c */
void		cleanup(t_philo *philos, t_table *table);
void		destroy_mutexes(t_table *table);

#endif
