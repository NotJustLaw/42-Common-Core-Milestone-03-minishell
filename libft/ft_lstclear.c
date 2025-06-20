/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:19:28 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/21 13:48:46 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;

	if (lst == NULL)
		return ;
	temp = *lst;
	while (temp)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}

/* int main()
{
	printf("\t\t----ft_lstclear----\n\n");
	
	t_list *head = ft_lstnew(ft_strdup("Hello"));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("World")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("123!")));
	
	size_t	i = 1;
	printf("  Before clearing the list:\n");
	while (head)
	{
		printf("  Node %zu: %s\n", i++, (char *)head->content);
		head = head->next;
	}
	
	ft_lstclear(NULL, free);
	
	printf("  After clearing the list:\n");
	if (!head)
		printf("  The list is empty!\n");
} */