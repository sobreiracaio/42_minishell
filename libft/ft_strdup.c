/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 10:08:32 by luide-so          #+#    #+#             */
/*   Updated: 2024/06/07 02:13:01 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*c;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	c = (char *)malloc(len * sizeof(char) + 1);
	if (!c)
		return (NULL);
	i = 0;
	while (i < len)
	{
		c[i] = s[i];
		i++;
	}
	c[i] = '\0';
	return (c);
}
