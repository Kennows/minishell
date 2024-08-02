/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:46:36 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/10 19:11:31 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*current;

	if (new != NULL)
	{
		current = *lst;
		if (*lst == (void *)0)
			*lst = new;
		else
		{
			while (current->next != NULL)
				current = current->next;
			current->next = new;
		}
	}
}
