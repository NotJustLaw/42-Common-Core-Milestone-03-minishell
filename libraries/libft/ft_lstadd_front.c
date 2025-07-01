/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:04:57 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/02/24 13:41:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	ft_lst_addfront(t_stack *lst, t_node *new)
// {
// 	if(!lst || !new)
// 		return;
// 	if(lst->head == NULL)
// 	{
// 		lst->head = new;
// 		lst->tail = new;
// 		new->prev = NULL;
// 		new->next = NULL;
// 	}
// 	new->next = lst->head;
// 	lst->head->prev = new;
// 	lst->head = new;
// 	new->prev = NULL;
// 	lst->lenght++;
// }

/* void print_list(t_list *lst)
{
	while(lst)
	{
		printf("Content: %s\n", (char *)lst->content);
		lst = lst->next;
	}
}

 int main()
{
	t_list *node1 = malloc(sizeof(t_list));
	t_list *node2 = malloc(sizeof(t_list));
	t_list *node3 = malloc(sizeof(t_list));

	if(!node1|!node2|!node3)
	{
		printf("Memory allocation failed\n");
		return(1);
	}

	node1->content = "First node";
	node1->next = NULL;

	node2->content = "Second node";
	node2->next = NULL;

	node3->content = "Third node";
	node3->next = NULL;

	t_list *list = NULL;

	ft_lstadd_front(&list, node3);
	ft_lstadd_front(&list, node2);
	ft_lstadd_front(&list, node1);

	printf("List after adding nodes to the front:\n");
	print_list(list);

	free(node1);
	free(node2);
	free(node3);

	return(0);
} */ 