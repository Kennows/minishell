/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:47:22 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/09 18:19:00 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	t_list	*temp;

	if (lst != NULL && f != NULL && del != NULL)
	{
		new_list = NULL;
		while (lst != NULL)
		{
			temp = f(lst->content);
			new_node = ft_lstnew(temp);
			if (new_node == NULL)
			{
				ft_lstclear(&new_list, del);
				del(temp);
				return (0);
			}
			ft_lstadd_back(&new_list, new_node);
			lst = lst->next;
		}
		return (new_list);
	}
	return (0);
}
