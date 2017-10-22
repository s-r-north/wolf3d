/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		free_map(int ***map, int x, int y)
{
	int		i;

	i = -1;
	while (++i < y)
	{
		ft_bzero((*map)[i], sizeof(int) * x);
		free((*map)[i]);
	}
	free(*map);
}

static void	line_to_map(t_env *env, int *in, char *line, int y)
{
	int		i;

	i = -1;
	while (++i < env->map_w)
	{
		in[i] = ft_atoi(line);
		if ((i == 0 || i == env->map_w - 1 || y == 0 || y == env->map_h)
				&& in[i] == 0)
		{
			ft_putendl("Invalid Map");
			exit(0);
		}
		while (*line && *line != ' ')
			line++;
		while (*line == ' ')
			line++;
	}
}

static void	get_var(t_env *env, char *line)
{
	env->map_w = ft_atoi(line);
	while (ft_isdigit(*line))
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->map_h = ft_atoi(line);
	while (ft_isdigit(*line))
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->start.x = ft_atoi(line);
	while (ft_isdigit(*line))
		line++;
	while (*line == ' ' || *line == '\t')
		line++;
	env->start.y = ft_atoi(line);
	if (!env->map_w || !env->map_h || !env->start.x || !env->start.y ||
			env->start.x >= env->map_w || env->start.y >= env->map_h)
	{
		ft_putendl("Invalid Map");
		exit(0);
	}

}

void		read_map(t_env *env, int fd)
{
	char	*line;
	int		**map;
	int		i;

	line = NULL;
	get_next_line(fd, &line);
	get_var(env, line);
	map = (int**)malloc(sizeof(int*) * env->map_h);
	i = -1;
	while (get_next_line(fd, &line) > 0 && ++i < env->map_h)
	{
		if ((int)ft_count_words(line, ' ') != env->map_w)
		{
			ft_putendl("Invalid Map");
			exit(0);
		}
		map[i] = (int*)malloc(sizeof(int) * env->map_w);
		line_to_map(env, map[i], line, i);
	}
	env->world = map;
}
