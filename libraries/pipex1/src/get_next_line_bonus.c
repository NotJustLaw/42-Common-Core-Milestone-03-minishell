/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuhlcke <skuhlcke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:59:56 by justlaw           #+#    #+#             */
/*   Updated: 2025/06/03 18:15:26 by skuhlcke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_next(char *s);
char	*ft_line(char *s);

char	*get_next_line(int fd)
{
	static char	*stash[FD_MAX];
	char		*buf;
	char		*line;
	int			r;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FD_MAX)
		return (NULL);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	r = 1;
	while (r > 0 && (!stash[fd] || !ft_strchr(stash[fd], '\n')))
	{
		r = read(fd, buf, BUFFER_SIZE);
		if (r == -1)
			return (free(buf), free(stash[fd]), stash[fd] = NULL, NULL);
		buf[r] = 0;
		stash[fd] = ft_strjoin_gnl(stash[fd], buf);
	}
	free(buf);
	if (!stash[fd])
		return (NULL);
	line = ft_line(stash[fd]);
	stash[fd] = ft_next(stash[fd]);
	return (line);
}

char	*ft_line(char *s)
{
	char	*line;
	int		i;

	if (!s || !s[0])
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
	{
		line[i] = s[i];
		i++;
	}
	if (s[i] == '\n')
		line[i++] = '\n';
	line[i] = 0;
	return (line);
}

char	*ft_next(char *s)
{
	char	*res;
	int		i;
	int		j;

	i = 0;
	while (s && s[i] && s[i] != '\n')
		i++;
	if (!s || !s[i])
	{
		free(s);
		return (NULL);
	}
	if (!s[i + 1])
		return (free(s), NULL);
	res = malloc(ft_strlen(&s[i + 1]) + 1);
	if (!res)
		return (free(s), NULL);
	i++;
	j = 0;
	while (s[i])
		res[j++] = s[i++];
	res[j] = 0;
	free(s);
	return (res);
}
