/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbopp <cbopp@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 12:08:48 by cbopp             #+#    #+#             */
/*   Updated: 2024/12/31 12:23:55 by cbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	should_continue(t_philo *philo)
{
	bool	result;

	pthread_mutex_lock(&(philo->table->death_mutex));
	result = !philo->table->simulation_end;
	pthread_mutex_unlock(&(philo->table->death_mutex));
	return (result);
}

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2)
		usleep(1000);
	pthread_mutex_lock(&(philo->left_fork->mutex));
	if (should_continue(philo))
		print_status(philo, "has taken a fork");
	pthread_mutex_lock(&(philo->right_fork->mutex));
	if (should_continue(philo))
		print_status(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	take_forks(philo);
	if (should_continue(philo))
	{
		pthread_mutex_lock(&(philo->table->meal_mutex));
		philo->last_meal = get_time();
		philo->eat_count++;
		pthread_mutex_unlock(&(philo->table->meal_mutex));
		print_status(philo, "is eating");
		smart_sleep(philo->table->time_to_eat * 1000, philo->table);
	}
	pthread_mutex_unlock(&(philo->right_fork->mutex));
	pthread_mutex_unlock(&(philo->left_fork->mutex));
}

void	sleep_and_think(t_philo *philo)
{
	if (should_continue(philo))
	{
		print_status(philo, "is sleeping");
		smart_sleep(philo->table->time_to_sleep * 1000, philo->table);
	}
	if (should_continue(philo))
		print_status(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->nb_philos == 1)
	{
		print_status(philo, "has taken a fork");
		smart_sleep(philo->table->time_to_die * 1000, philo->table);
		return (NULL);
	}
	while (should_continue(philo))
	{
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
