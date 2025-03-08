/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbopp <cbopp@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 12:10:09 by cbopp             #+#    #+#             */
/*   Updated: 2024/12/31 12:10:12 by cbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(t_fork) * table->nb_philos);
	if (!table->forks)
		return (ERROR);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&(table->forks[i].mutex), NULL))
			return (ERROR);
		table->forks[i].id = i;
		i++;
	}
	return (SUCCESS);
}

int	init_table(t_table *table, int argc, char **argv)
{
	table->nb_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->must_eat_count = -1;
	if (argc == 6)
		table->must_eat_count = ft_atoi(argv[5]);
	table->simulation_end = false;
	if (init_forks(table) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&(table->write_mutex), NULL))
		return (ERROR);
	if (pthread_mutex_init(&(table->death_mutex), NULL))
	{
		pthread_mutex_destroy(&(table->write_mutex));
		return (ERROR);
	}
	if (pthread_mutex_init(&(table->meal_mutex), NULL))
	{
		pthread_mutex_destroy(&(table->write_mutex));
		pthread_mutex_destroy(&(table->death_mutex));
		return (ERROR);
	}
	return (SUCCESS);
}

static void	assign_forks(t_philo *philo, t_table *table)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % table->nb_philos;
	if (philo->id % 2)
	{
		philo->left_fork = &table->forks[right_fork];
		philo->right_fork = &table->forks[left_fork];
	}
	else
	{
		philo->left_fork = &table->forks[left_fork];
		philo->right_fork = &table->forks[right_fork];
	}
}

int	init_philosophers(t_philo **philos, t_table *table)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!(*philos))
		return (ERROR);
	i = 0;
	while (i < table->nb_philos)
	{
		(*philos)[i].id = i;
		(*philos)[i].eat_count = 0;
		(*philos)[i].last_meal = get_time();
		(*philos)[i].table = table;
		assign_forks(&(*philos)[i], table);
		i++;
	}
	return (SUCCESS);
}
