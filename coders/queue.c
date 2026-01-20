/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:43:10 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/20 17:33:17 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	enqueue_fifo(t_data *data, t_wait_node *node);
static void	enqueue_edf(t_data *data, t_wait_node *node);

int	enqueue_coder(t_data *data, t_coder *coder)
{
	t_wait_node	*node;

	node = malloc(sizeof(t_wait_node));
	if (!node)
		return (1);
	node->coder = coder;
	node->next = NULL;
	if (!data->queue_head)
		data->queue_head = node;
	else
	{
		if (data->scheduler == 1)
			enqueue_fifo(data, node);
		else if (data->scheduler == 2)
			enqueue_edf(data, node);
	}
	return (0);
}

void	dequeue_coder(t_data *data, t_coder *coder)
{
	t_wait_node	*node;
	t_wait_node	*prev;

	if (!data->queue_head)
		return ;
	node = data->queue_head;
	if (node->coder == coder)
	{
		data->queue_head = node->next;
		free(node);
		return ;
	}
	while (node)
	{
		if (node->coder == coder)
			break ;
		prev = node;
		node = node->next;
	}
	if (node)
	{
		prev->next = node->next;
		free(node);
	}
}

static void	enqueue_fifo(t_data *data, t_wait_node *node)
{
	t_wait_node	*temp;

	temp = data->queue_head;
	while (temp->next)
		temp = temp->next;
	temp->next = node;
}

static void	enqueue_edf(t_data *data, t_wait_node *node)
{
	t_wait_node	*temp;
	t_wait_node	*prev;

	temp = data->queue_head;
	while (temp)
	{
		if (node->coder->last_compile_time < temp->coder->last_compile_time)
		{
			node->next = temp;
			if (temp == data->queue_head)
			{
				data->queue_head = node;
				return ;
			}
			else
				prev->next = node;
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
	prev->next = node;
}
