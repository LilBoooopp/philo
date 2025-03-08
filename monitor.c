/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbopp <cbopp@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 12:08:44 by cbopp             #+#    #+#             */
/*   Updated: 2025/01/07 13:02:04 by cbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	check_death(t_philo *philo)
{
	bool		is_dead;
	long long	time_since_meal;

	pthread_mutex_lock(&(philo->table->meal_mutex));
	time_since_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&(philo->table->meal_mutex));
	pthread_mutex_lock(&(philo->table->death_mutex));
	is_dead = false;
	if (!philo->table->simulation_end && 
		time_since_meal > (philo->table->time_to_die * 1000))
	{
		print_status(philo, "died");
		philo->table->simulation_end = true;
		is_dead = true;
	}
	pthread_mutex_unlock(&(philo->table->death_mutex));
	return (is_dead);
}

bool	check_meals(t_philo *philos, t_table *table)
{
	int	i;
	int	finished_eating;

	if (table->must_eat_count == -1)
		return (false);
	i = 0;
	finished_eating = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&(table->meal_mutex));
		if (philos[i].eat_count >= table->must_eat_count)
			finished_eating++;
		pthread_mutex_unlock(&(table->meal_mutex));
		i++;
	}
	pthread_mutex_lock(&(table->death_mutex));
	if (finished_eating == table->nb_philos && !table->simulation_end)
	{
		table->simulation_end = true;
		pthread_mutex_unlock(&(table->death_mutex));
		return (true);
	}
	pthread_mutex_unlock(&(table->death_mutex));
	return (false);
}

static bool	should_continue(t_table *table)
{
	bool	result;

	pthread_mutex_lock(&(table->death_mutex));
	result = !table->simulation_end;
	pthread_mutex_unlock(&(table->death_mutex));
	return (result);
}

void	*death_monitor(void *arg)
{
	t_philo	*philos;
	t_table	*table;
	int		i;

	philos = (t_philo *)arg;
	table = philos[0].table;
	while (should_continue(table))
	{
		i = 0;
		while (i < table->nb_philos && should_continue(table))
		{
			if (check_death(&philos[i]) || check_meals(philos, table))
				return (NULL);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}

int	start_simulation(t_philo *philos, t_table *table)
{
	int			i;
	pthread_t	monitor;

	table->start_time = get_time();
	i = -1;
	while (++i < table->nb_philos)
	{
		philos[i].last_meal = table->start_time;
		if (pthread_create(&(philos[i].thread), NULL, philosopher_routine, &philos[i]))
			return (ERROR);
	}
	if (pthread_create(&monitor, NULL, death_monitor, philos))
		return (ERROR);
	i = -1;
	while (++i < table->nb_philos)
	if (pthread_join(philos[i].thread, NULL))
		return (ERROR);
	if (pthread_join(monitor, NULL))
		return (ERROR);
	return (SUCCESS);
}
