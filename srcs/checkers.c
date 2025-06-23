/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasq <hcarrasq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:59:12 by hcarrasq          #+#    #+#             */
/*   Updated: 2025/06/23 18:27:54 by hcarrasq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool    pipe_checker(char *line)
{
    int i;
    int found_pipes;

    found_pipes = 0;
    i = 0;
    while (is_wspace(line[i]))
        i++;
    if (line[i] == '|')
        return (false);
    else
    {
        while (line[i])
        {
            if (line[i] == '|')
                found_pipes = 1;
            else
                found_pipes = 0;
            i++;
        }
    }
    if (found_pipes == 1)
        return (false);
    return (true);
}
