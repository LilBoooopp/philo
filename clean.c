/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbopp <cbopp@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 12:08:41 by cbopp             #+#    #+#             */
/*   Updated: 2024/12/31 12:25:39 by cbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroy_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_destroy(&(table->forks[i].mutex));
		i++;
	}
	pthread_mutex_destroy(&(table->write_mutex));
	pthread_mutex_destroy(&(table->death_mutex));
	pthread_mutex_destroy(&(table->meal_mutex));
}

void	cleanup(t_philo *philos, t_table *table)
{
	destroy_mutexes(table);
	if (table->forks)
		free(table->forks);
	if (philos)
		free(philos);
}
