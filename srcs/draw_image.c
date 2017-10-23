/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

static int	redshift(int color, double depth)
{
	int		rgb[3];
	double	drgb[3];

	rgb[0] = color >> 16;
	rgb[1] = (color >> 8) & 0xff;
	rgb[2] = (color) & 0xff;
	drgb[0] = (double)rgb[0] * sqrt(depth);
	drgb[1] = (double)rgb[1] * depth;
	drgb[2] = (double)rgb[2] * depth * depth;
	rgb[0] = ((int)drgb[0] & 0xff) << 16;
	rgb[1] = ((int)drgb[1] & 0xff) << 8;
	rgb[2] = ((int)drgb[2] & 0xff);
	return (rgb[0] | rgb[1] | rgb[2]);
}

static int	glass_opacity(int map, int env)
{
	int		rgbout[3];
	int		i;

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

void		draw(t_env *env, t_ray ray, int x)
{
	double	i;
	int		j;

	if ((ray.slice = (WALL_HEIGHT / ray.dist)) > RENDER_SCALE)
		ray.slice = RENDER_SCALE;
	ray.ceil = (WIN_H - ray.slice) / 4;
	ray.floor = ray.ceil + (ray.slice);
	j = -1;
	while (++j < WIN_H)
		env->mlx->data[x + (j * WIN_W)] = env->tex[NTEX - 1].img\
				[(int)(ray.angle * SKY_RES / (M_PI * 2)) + (j * SKY_RES)];
	i = 0;
	ray.depth = (ray.slice < WIN_H) ? ((double)ray.slice / WIN_H) : 1;
	while (ray.ceil < ray.floor)
	{
		if (ray.ceil >= 0 && ray.ceil < WIN_H)
			env->mlx->data[x + (ray.ceil * WIN_W)] = redshift(env->tex[ray.tex]\
					.img[ray.off + ((int)i * TEX_RES)], ray.depth);
		ray.ceil++;
		i += (TEX_RES / (double)ray.slice);
	}
	--ray.floor;
	while (++ray.floor < WIN_H)
		env->mlx->data[x + (ray.floor * WIN_W)] = redshift(glass_opacity(
				0xb0b080, env->mlx->data[x + (ray.floor * WIN_W)]), ray.depth);
}
