/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

static int	menu_hook(int mbutton, int x, int y, t_env *env)
{
	int i;
	int fd;

	if (mbutton != 1)
		return (0);
	if (y >= (WIN_H / 2) + 20 && y <= (WIN_H / 2) + 148)
	{
		if (x >= (2 * WIN_W / 3) - 64 && x <= (2 * WIN_W / 3) + 64)
			exit(0);
		if (x >= (WIN_W / 3) - 64 && x <= (WIN_W / 3) + 64)
		{
			i = -1;
			while (++i < 3)
				mlx_destroy_image(env->mlx->mlx, env->menu->ptr[i]);
			fd = open("maps/test.wolf", O_RDONLY);
			if (fd == -1)
			{
				ft_putendl("File Does Not Exist");
				exit(0);
			}
			read_map(env, fd);
			wolf(env);
		}
	}
	return (0);
}

static char	*menu_gfx(int code)
{
	if (code == 0)
		return ("assets/menu_bg.XPM");
	if (code == 1)
		return ("assets/start_statue.XPM");
	if (code == 2)
		return ("assets/exit_wizard.XPM");
	return (NULL);
}

static int	menu_exit(int keycode, t_env *env)
{
	(void)env;
	if (keycode == 53)
		exit(0);
	return (0);
}

void		menu_loop(t_env *env, t_menu *menu)
{
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, menu->ptr[0], 0, 0);
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, menu->ptr[1],
			(WIN_W / 3) - 64, (WIN_H / 2) + 20);
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, menu->ptr[2],
			(2 * WIN_W / 3) - 64, (WIN_H / 2) + 20);
	mlx_string_put(env->mlx->mlx, env->mlx->win, (WIN_W / 3) - 25, (WIN_H / 2),
			0xffffff, "START");
	mlx_string_put(env->mlx->mlx, env->mlx->win, (2 * WIN_W / 3) - 20,
			(WIN_H / 2), 0xffffff, "EXIT");
	mlx_hook(env->mlx->win, 2, 0, menu_exit, env);
	mlx_hook(env->mlx->win, 17, 0, exit_hook, env);
	mlx_mouse_hook(env->mlx->win, menu_hook, env);
	mlx_loop(env->mlx->mlx);
}

void		menu(t_env *env)
{
	int		i;
	t_menu	menu;

	i = -1;
	while (++i < 3)
	{
		menu.w[i] = (i == 0) ? 1280 : 128;
		menu.h[i] = (i == 0) ? 720 : 128;
		menu.e = 0;
		menu.bpp = 16;
		menu.sl = 0;
		menu.ptr[i] = mlx_xpm_file_to_image(env->mlx->mlx,
				menu_gfx(i), &menu.w[i], &menu.h[i]);
		menu.img[i] = (int*)mlx_get_data_addr(menu.ptr[i], &menu.bpp,
				&menu.sl, &menu.e);
	}
	env->music = 0;
	env->menu = &menu;
	menu_loop(env, &menu);
}
