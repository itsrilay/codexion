/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:50:03 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/15 16:27:33 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	long long		next_available_time;
}					t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long long		last_compile_time;
	long long		time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}					t_coder;

typedef struct t_data
{
	t_coder			*coders;
	t_dongle		*dongles;
	int				dongle_cooldown;
	int				is_over;
	int				number_of_coders;
	int				scheduler;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	sim_lock;
}					t_data;

#endif