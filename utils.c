/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbopp <cbopp@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 12:08:53 by cbopp             #+#    #+#             */
/*   Updated: 2024/12/31 12:16:08 by cbopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (long long)1000000) + tv.tv_usec);
}

void	print_status(t_philo *philo, char *status)
{
	long long	time;

	pthread_mutex_lock(&(philo->table->write_mutex));
	if (!philo->table->simulation_end)
	{
		time = (get_time() - philo->table->start_time) / 1000;
		printf("%lld %d %s\n", time, philo->id + 1, status);
	}
	pthread_mutex_unlock(&(philo->table->write_mutex));
}

void	smart_sleep(long long duration, t_table *table)
{
	long long	start;
	long long	current;

	start = get_time();
	while (!table->simulation_end)
	{
		current = get_time();
		if ((current - start) >= duration)
			break ;
		usleep(100);
	}
}

int	check_arguments(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (ERROR);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (ERROR);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}
