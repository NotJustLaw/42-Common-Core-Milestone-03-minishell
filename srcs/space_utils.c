/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   space_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:16:54 by henrique-re       #+#    #+#             */
/*   Updated: 2025/09/01 18:26:29 by hcarrasq         ###   ########.fr       */
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

int strncat_realloc(char **dest, const char *source, size_t n)
{
	if (!dest || !source)
		return (-1);
	size_t	old_len;
	size_t	new_len;
	if (!*dest)
		old_len = 0;
	else
		old_len = ft_strlen(*dest);
	new_len = old_len + n;
	char	*new_str;

	new_str = ft_realloc(*dest, old_len + 1, new_len + 1);
	if (!new_str)
		return (-1);
	ft_memcpy(new_str + old_len, source, new_len);
	new_str[new_len] = '\0';
	
	*dest = new_str;
	return (0);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t	copy_size;
	void	*new_ptr;

	new_ptr = NULL;
	copy_size = 0;
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (new_size < old_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	len;
	size_t	i;
	char	*frase;

	len = 0;
	i = 0;
	if (!s1)
		return (NULL);
	while (len < n && s1[len] != '\0')
		len++;
	frase = (char *)malloc(len + 1);
	if (!frase)
		return (NULL);
	while (i < len)
	{
		frase[i] = s1[i];
		i++;
	}
	frase[i] = '\0';
	return (frase);
}

bool ft_strchr2(const char *s, int c)
{
	int		i;
	char	*dest;

	i = 0;
	dest = (char *)s;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			return (true);
		i++;
	}
	if (s[i] == (unsigned char)c)
		return (true);
	return (false);
}