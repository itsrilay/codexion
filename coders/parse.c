/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:46:17 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/20 13:09:59 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_coders_dongles(t_data *data);
static int	parse_int(char *str);

int	parse_args(t_data *data, char **argv)
{
	data->number_of_coders = parse_int(argv[1]);
	data->time_to_burnout = parse_int(argv[2]);
	data->time_to_compile = parse_int(argv[3]);
	data->time_to_debug = parse_int(argv[4]);
	data->time_to_refactor = parse_int(argv[5]);
	data->compiles_required = parse_int(argv[6]);
	data->dongle_cooldown = parse_int(argv[7]);
	if (data->number_of_coders < 1 || data->time_to_burnout < 0
		|| data->time_to_compile < 0 || data->time_to_debug < 0
		|| data->time_to_refactor < 0 || data->compiles_required < 0
		|| data->dongle_cooldown < 0)
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = 1;
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = 2;
	else
		return (1);
	return (0);
}

int	init_data(t_data *data)
{
	data->is_over = 0;
	data->queue_head = NULL;
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (1);
	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (free(data->coders), 1);
	init_coders_dongles(data);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->sim_lock, NULL);
	return (0);
}

static int	parse_int(char *str)
{
	int	n;
	int	digit;

	if (!str || !*str)
		return (-1);
	n = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		digit = *str - '0';
		if (n > (INT_MAX - digit) / 10)
			return (-1);
		n = (n * 10) + digit;
		str++;
	}
	return (n);
}

static void	init_coders_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_init(&data->dongles[i].lock, NULL);
		pthread_cond_init(&data->coders[i].wait_cond, NULL);
		data->dongles[i].is_available = 1;
		data->dongles[i].next_available_time = data->dongle_cooldown;
		data->coders[i].id = i + 1;
		data->coders[i].time_to_burnout = data->time_to_burnout;
		data->coders[i].time_to_compile = data->time_to_compile;
		data->coders[i].time_to_debug = data->time_to_debug;
		data->coders[i].time_to_refactor = data->time_to_refactor;
		data->coders[i].compiles_required = data->compiles_required;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[(i + 1)
			% data->number_of_coders];
		data->coders[i].compile_count = 0;
		data->coders[i].last_compile_time = 0;
		data->coders[i].data = data;
		i++;
	}
}
