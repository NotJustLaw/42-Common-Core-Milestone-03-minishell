/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:30:34 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/21 13:46:32 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*temp;

	temp = lst;
	while (temp)
	{
		f(temp->content);
		temp = temp->next;
	}
}

/* void	ft_toupper_lstiter(void *content)
{
	if (content)
	{
		char *str = (char *)content;
		while (*str)
		{
			*str = ft_toupper(*str);
			str++;
		}
	}
}

int main()
{
	printf("\t\t----ft_lstiter----\n\n");
	
	t_list *head = ft_lstnew(ft_strdup("Hello"));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("World")));
	ft_lstadd_back(&head, ft_lstnew(ft_strdup("123!")));
	
	t_list	*current = head;
	size_t	i = 1;
	printf("  Before iterating the list:\n");
	while (current)
	{
		printf("  Node %zu: %s\n", i++, (char *)current->content);
		current = current->next;
	}
	ft_lstiter(head, &ft_toupper_lstiter);
	i = 1;
	current = head;
	printf("  After iterating the list:\n");
	while (current)
	{
		printf("  Node %zu: %s\n", i++, (char *)current->content);
		current = current->next;
	}
} */