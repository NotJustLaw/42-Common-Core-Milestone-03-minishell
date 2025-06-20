/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:59:22 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/01/28 16:42:09 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	last = *lst;
	if (!last)
		*lst = new;
	else
	{
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

/* int	main()
{
	printf("\t\t----ft_lstadd_back----\n\n");
	int value1 = 42;
	t_list *node1 = ft_lstnew(&value1);
	int value2 = 43;
	t_list *node2 = ft_lstnew(&value2);
	int value3 = 44;
	t_list *node3 = ft_lstnew(&value3);
	int value4 = 45;
	t_list *node4 = ft_lstnew(&value4);
	
	ft_lstadd_back(&node1, node2);
	ft_lstadd_back(&node1, node3);
	ft_lstadd_back(&node1, node4);
	
	printf("  Content of head node: %d\n  Content of the next node: %d\n)

} */