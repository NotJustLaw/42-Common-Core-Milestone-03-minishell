/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:05:58 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/21 13:48:30 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	del(lst->content);
	free(lst);
}

/* int main()
{
	printf("\t\t----ft_lstdelone----\n\n");
	
	t_list *head = ft_lstnew(ft_strdup("Hello"));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("World")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("123!")));
	
	t_list	*current = head;
	size_t	i = 1;
	printf("  Before deleting second node:\n");
	while (current)
	{
		printf("  Node %zu: %s\n", i++, (char *)current->content);
		current = current->next;
	}
	
	t_list	*delete_node = head->next;
	t_list	*relink_node = delete_node->next;
	ft_lstdelone(delete_node, free);
	head->next = relink_node;
	
	current = head;	
	i = 1;
	printf("  After deleting second node:\n");
	while (current)
	{
		printf("  Node %zu: %s\n", i++, (char *)current->content);
		current = current->next;
	}
} */