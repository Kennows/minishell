/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:46:59 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/09 18:49:12 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*temp;

	if (lst != NULL && del != NULL)
	{
		current = *lst;
		while (current != NULL)
		{
			temp = current->next;
			current->next = NULL;
			del(current->content);
			free(current);
			current = temp;
		}
		*lst = NULL;
	}
}
