/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

static void	init_mlx_pointers(t_env *env, char *s)
{
	t_mlx	*tmp;
	int		e;
	int		bpp;
	int		width;

	tmp = (t_mlx*)malloc(sizeof(t_mlx));
	tmp->mlx = mlx_init();
	tmp->win = mlx_new_window(tmp->mlx, WIN_W, WIN_H, s);
	tmp->img = mlx_new_image(tmp->mlx, WIN_W, WIN_H);
	e = ENDIAN;
	bpp = BITS_PER_PIXEL;
	width = WIN_W;
	tmp->data = (int*)mlx_get_data_addr(tmp->img, &bpp, &width, &e);
	env->mlx = tmp;
}

int			main(void)
{
	t_env	env;

	init_mlx_pointers(&env, "Wolf 3D");
	menu(&env);
}
