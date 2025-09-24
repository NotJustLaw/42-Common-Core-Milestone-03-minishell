/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:33:56 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/03/26 15:51:07 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlenbuf(char *str)
{
	size_t	idx;

	idx = 0;
	while (str && str[idx] != '\0' && str[idx] != '\n')
		idx++;
	if (str && str[idx] == '\n')
		idx++;
	return (idx);
}

char	*ft_strjoinbuf(char *buf, char *str)
{
	size_t	i;
	size_t	l;
	char	*line;

	i = 0;
	l = 0;
	line = malloc((ft_strlen(buf) + ft_strlen(str) + 1) * sizeof(char));
	if (!line)
		return (free(line), NULL);
	while (str && str[i])
	{
		line[i] = str[i];
		i++;
	}
	while (buf[l] != '\n' && buf[l])
	{
		line[i + l] = buf[l];
		l++;
	}
	if (buf[l] == '\n')
		line[i + l++] = '\n';
	line[i + l] = '\0';
	return (free(str), line);
}

void	buf_cleaner(char *str)
{
	int	i;
	int	l;

	i = 0;
	l = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\n')
		i++;
	while (str[l] != '\0')
	{
		str[l] = str[i];
		i++;
		l++;
	}
	str[l] = '\0';
}
