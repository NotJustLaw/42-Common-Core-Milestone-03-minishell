/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   space_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:16:54 by henrique-re       #+#    #+#             */
/*   Updated: 2025/07/15 12:25:56 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	we_need_space(char *line)
{
	printf("about to check pipes\n");
	if (!pipe_checker(line))
		return (false);
	printf("about to check redirections\n");
	if (!redirection_checker(line))
		return (false);
	printf("about to check expansions\n");
	if (!expansion_chekcker(line))
		return (false);
	return (true);
}

/* static void	*free_split(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	return (free(str), NULL);
} */