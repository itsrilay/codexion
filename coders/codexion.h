/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:50:03 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 14:25:47 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	long long		next_available_time;
}					t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long long		time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				compiles_required;
	long long		last_compile_time;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_t		thread;
	struct t_data	*data;
}					t_coder;

typedef struct t_data
{
	t_coder			*coders;
	t_dongle		*dongles;
	long long		time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				compiles_required;
	int				dongle_cooldown;
	int				is_over;
	int				number_of_coders;
	int				scheduler;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	sim_lock;
	long long		start_time;
}					t_data;

int					parse_args(t_data *data, char **argv);
int					init_data(t_data *data);
long long			get_time(void);
void				create_threads(t_data *data);
void				join_threads(t_data *data);
void				smart_sleep(long long time);
void				print_status(char *str, t_coder *coder);

#endif