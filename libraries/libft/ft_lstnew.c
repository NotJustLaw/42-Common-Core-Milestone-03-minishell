/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:13:51 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/02/03 17:54:53 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*elm;

	elm = malloc(sizeof(t_list));
	if (!elm)
		return (NULL);
	elm->content = content;
	elm->next = NULL;
	return (elm);
}
/*int main ()
{
	char *str = "Tamos ai";

	t_list *new_node = ft_lstnew((void *)str);
	if (new_node)
	{
		printf("New list node created!\n");
		printf("Content: %s\n", (char *)new_node->content);
		printf("Next: %p\n", (void *)new_node->next);
	}
	else
		printf("Failed to create list node.\n");
	free(new_node);
	return(0);
}*/