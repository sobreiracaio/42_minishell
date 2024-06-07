/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 18:28:07 by luide-so          #+#    #+#             */
/*   Updated: 2024/06/07 02:12:32 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const char	*ps;

	ps = (const char *)s;
	while (n--)
		if (*ps++ == (const char)c)
			return ((void *)--ps);
	return (NULL);
}
