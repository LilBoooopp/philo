/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbopp <cbopp@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 12:08:37 by cbopp             #+#    #+#             */
/*   Updated: 2024/12/31 12:08:38 by cbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat ");
	printf("time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
}

static int	init_simulation(t_table *table, t_philo **philosophers,
		int argc, char **argv)
{
	if (init_table(table, argc, argv) == ERROR)
		return (ERROR);
	if (init_mutexes(table) == ERROR)
	{
		cleanup(NULL, table);
		return (ERROR);
	}
	if (init_philosophers(philosophers, table) == ERROR)
	{
		cleanup(NULL, table);
		return (ERROR);
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_table	table;
	t_philo	*philosophers;

	if (check_arguments(argc, argv) == ERROR)
	{
		print_usage();
		return (ERROR);
	}
	if (init_simulation(&table, &philosophers, argc, argv) == ERROR)
		return (ERROR);
	if (start_simulation(philosophers, &table) == ERROR)
	{
		cleanup(philosophers, &table);
		return (ERROR);
	}
	cleanup(philosophers, &table);
	return (SUCCESS);
}
