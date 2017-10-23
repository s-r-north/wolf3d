/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

static char	*texture_asset(int num)
{
	if (num == 0)
		return ("assets/floor.XPM");
	if (num == 1)
		return ("assets/wall_1.XPM");
	if (num == 2)
		return ("assets/wall_2_vent.XPM");
	if (num == 3)
		return ("assets/wall_3_tech.XPM");
	if (num == 4)
		return ("assets/wall_4_interior_a.XPM");
	if (num == 5)
		return ("assets/wall_5_interior_b.XPM");
	if (num == 6)
		return ("assets/wall_end_north.XPM");
	if (num == 7)
		return ("assets/wall_end_south.XPM");
	if (num == 8)
		return ("assets/wall_end_east.XPM");
	if (num == 9)
		return ("assets/wall_end_west.XPM");
	if (num == 10)
		return ("assets/skybox.XPM");
	return (NULL);
}

static void	init_minimap(t_env *env)
{
	int x;
	int y;
	int bpp;
	int e;
	int w;

	bpp = BITS_PER_PIXEL;
	e = ENDIAN;
	w = 120;
	env->map = (t_minimap*)malloc(sizeof(t_minimap));
	env->map->ptr[0] = mlx_new_image(env->mlx->mlx, w, w);
	env->map->ptr[1] = mlx_new_image(env->mlx->mlx, w, w);
	env->map->img[0] = (int*)mlx_get_data_addr(env->map->ptr[0], &bpp, &w, &e);
	env->map->img[1] = (int*)mlx_get_data_addr(env->map->ptr[1], &bpp, &w, &e);
	env->map->toggle = 1;
	w /= 4;
	y = -1;
	while (++y < w)
	{
		x = -1;
		while (++x < w)
			env->map->img[0][x + (y * w)] = env->world[y / 5][x / 5] ?
					0x000000 : 0xb0b0b0;
	}
}

static void	init_env(t_env *env)
{
	int i;

	env->music = 1;
	env->pos.x = (env->start.x * SCALE) + (SCALE / 2);
	env->pos.y = (env->start.y * SCALE) + (SCALE / 2);
	env->dir.x = 3 * M_PI / 2;
	env->dir.y = 0;
	ft_bzero(&env->key, sizeof(t_key));
	i = -1;
	while (++i < (NTEX))
	{
		env->tex[i].w = (i == NTEX - 1) ? SKY_RES : TEX_RES;
		env->tex[i].h = (i == NTEX - 1) ? 1200 : TEX_RES;
		env->tex[i].e = 0;
		env->tex[i].sl = 0;
		env->tex[i].bpp = 16;
		env->tex[i].ptr = mlx_xpm_file_to_image(env->mlx->mlx, texture_asset(i),
				&env->tex[i].w, &env->tex[i].h);
		env->tex[i].img = (int*)mlx_get_data_addr(env->tex[i].ptr,
					&env->tex[i].bpp, &env->tex[i].sl, &env->tex[i].e);
	}
	init_minimap(env);
}

void		wolf(t_env *env)
{
	system("afplay assets/boi_basement.mp3 &");
	init_env(env);
	mlx_hook(env->mlx->win, 2, 0, press_key, env);
	mlx_hook(env->mlx->win, 3, 0, release_key, env);
	mlx_hook(env->mlx->win, 6, 0, mouse_pos, env);
	mlx_hook(env->mlx->win, 17, 0, exit_hook, env);
	mlx_loop_hook(env->mlx->mlx, hook_func, env);
	mlx_loop(env->mlx->mlx);
}
