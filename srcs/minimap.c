/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

static int	fov_color(int length, int bg)
{
	int rgb1[3];
	int rgb2[3];

	rgb1[0] = 0xff - (length * 0xff / MMAP_CONE_LEN);
	rgb1[1] = 0;
	rgb1[2] = 0;
	rgb2[0] = (bg >> 16) * length / MMAP_CONE_LEN;
	rgb2[1] = ((bg >> 8) & 0xff) * length / MMAP_CONE_LEN;
	rgb2[2] = (bg & 0xff) * length / MMAP_CONE_LEN;
	return ((((rgb1[0] + rgb2[0]) & 0xff) << 16) | (((rgb1[1] + rgb2[1])\
			& 0xff) << 8) | (((rgb1[2] + rgb2[2]) & 0xff)));
}

static void	bresen_angle(t_env *env, int x1, int y1)
{
	t_fxy	xy2;
	double	a1;
	double	a2;
	int		length;

	length = MMAP_POS_RAD - 1;
	a2 = env->map->dir - (FOV);
	while (++length <= MMAP_CONE_LEN)
	{
		a1 = env->map->dir + (FOV);
		while (a1 >= a2)
		{
			xy2.x = x1 + (length * cos(a1));
			xy2.y = y1 + (length * sin(a1));
			if ((int)xy2.x >= 0 && (int)xy2.x < 120 && (int)xy2.y >= 0 &&
					(int)xy2.y < 120)
				env->map->img[1][(int)xy2.x + (int)xy2.y * 120] =
						fov_color(length, env->map->img[1]\
						[(int)xy2.x + (int)xy2.y * 120]);
			a1 -= (FOV / 30);
		}
	}
}

static int	mmap_opacity(int map, int env)
{
	int rgbout[3];
	int i;

	i = -1;
	while (++i < 3)
	{
		rgbout[i] = (((env >> (16 - (8 * i))) & 0xff) / 2) +
				(((map >> (16 - (8 * i))) & 0xff) / 4);
		if (rgbout[i] > 0xff)
			rgbout[i] = 0xff;
	}
	return ((rgbout[0] << 16) | (rgbout[1] << 8) | (rgbout[2]));
}

void		minimap(t_env *env)
{
	int i;
	int x1;
	int y1;

	env->map->pos.x = 5 * (double)env->pos.x / SCALE;
	env->map->pos.y = 5 * (double)env->pos.y / SCALE;
	env->map->dir = -env->dir.x;
	i = -1;
	while (++i < (120 * 120))
		env->map->img[1][i] = mmap_opacity(env->map->img[0][i],
				env->mlx->data[(i % 120) + 20 + (i / 120) *
				WIN_W + (WIN_H - 140) * WIN_W]);
	y1 = -MMAP_POS_RAD - 1;
	while (++y1 <= MMAP_POS_RAD)
	{
		x1 = -MMAP_POS_RAD - 1;
		while (++x1 <= MMAP_POS_RAD)
			if (sqrt(x1 * x1 + y1 * y1) < MMAP_POS_RAD)
				env->map->img[1][(int)(env->map->pos.x + x1) +
						(int)(env->map->pos.y + y1) * 120] = 0x00ff00;
	}
	bresen_angle(env, (int)env->map->pos.x, (int)env->map->pos.y);
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, env->map->ptr[1], 20,
			WIN_H - 140);
}
