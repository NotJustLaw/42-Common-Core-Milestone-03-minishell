/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:09:13 by hcarrasq          #+#    #+#             */
/*   Updated: 2024/11/21 13:43:58 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next)
		temp = temp->next;
	return (temp);
}

/* int main(void)
{
	t_list *list = NULL;
	t_list *last_node;

	list = (t_list *)malloc(sizeof(t_list));
	if (!list) {
		printf("Memory allocation failed\n");
		return 1;
	}
	int *content1 = (int *)malloc(sizeof(int));
	if (!content1) {
		printf("Memory allocation for content failed\n");
		return 1;
	}
	*content1 = 10;
	list->content = content1;
	list->next = NULL;

	t_list *second = (t_list *)malloc(sizeof(t_list));
	if (!second) {
		printf("Memory allocation failed\n");
		return 1;
	}
	int *content2 = (int *)malloc(sizeof(int));
	if (!content2) {
		printf("Memory allocation for content failed\n");
		return 1;
	}
	*content2 = 20;
	second->content = content2;
	second->next = NULL;
	list->next = second;

	t_list *third = (t_list *)malloc(sizeof(t_list));
	if (!third) {
		printf("Memory allocation failed\n");
		return 1;
	}
	int *content3 = (int *)malloc(sizeof(int));
	if (!content3) {
		printf("Memory allocation for content failed\n");
		return 1;
	}
	*content3 = 30;
	third->content = content3;
	third->next = NULL;
	second->next = third;

	last_node = ft_lstlast(list);

	if (last_node) {
		printf("The last node value is: %d\n", *(int *)(last_node->content));
	} else {
		printf("The list is empty.\n");
	}

	t_list *temp;
	while (list) {
		temp = list;
		list = list->next;

		free(temp->content);
		free(temp);
	}

	return 0;
} */