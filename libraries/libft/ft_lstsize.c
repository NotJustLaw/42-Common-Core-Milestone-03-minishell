/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:01:54 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/21 13:45:36 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*temp;
	int		i;

	temp = lst;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

/* int main()
{
	t_list *head = malloc(sizeof(t_list));
	t_list *second = malloc(sizeof(t_list));
	t_list *third = malloc(sizeof(t_list));
	t_list *fourth = malloc(sizeof(t_list));

	if(!head || !second || !third || !fourth)
	{
		printf("Memory allocation failed\n");
		return(1);
	}
	head->content = (void *)10;
	head->next = second;

	second->content = (void *)20;
	second->next = third;

	third->content = (void *)30;
	third->next = fourth;

	fourth->content = (void *)40;
	fourth->next = NULL;

	int size = ft_lstsize(head);
	printf("The size of the linked list is: %d\n", size);

	free(fourth);
	free(third);
	free(second);
	free(head);

	return (0);
} */