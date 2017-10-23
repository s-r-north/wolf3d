/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

void	horizontal_ray(t_env *env, double angle, int th)
{
	t_xy	ray;
	int		slope;

	slope = SCALE / tan(angle);
	ray.y = ((env->pos.y / SCALE) * SCALE) + ((angle > M_PI) ? SCALE : -1);
	ray.x = env->pos.x + ((env->pos.y - ray.y) / tan(angle));
	while (!(env->hor[th].tex = check_grid(env, ray.x, ray.y)))
	{
		ray.x += ((angle > M_PI) ? -slope : slope);
		ray.y += ((angle > M_PI) ? SCALE : -SCALE);
	}
	env->hor[th].dist = fabs((env->pos.y - ray.y) /
			sin(angle)) * cos(env->dir.x - angle);
	if (env->hor[th].dist < 0)
		env->hor[th].dist = INT_MAX;
	env->hor[th].off = (ray.x % SCALE) / (RENDER_SCALE);
	env->hor[th].angle = angle;
}

void	vertical_ray(t_env *env, double angle, int th)
{
	t_xy	ray;
	int		slope;

	slope = SCALE * tan(angle);
	ray.x = ((env->pos.x / SCALE) * SCALE) +
			((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -1 : SCALE);
	ray.y = env->pos.y + ((env->pos.x - ray.x) * tan(angle));
	while (!(env->ver[th].tex = check_grid(env, ray.x, ray.y)))
	{
		ray.x += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -SCALE : SCALE);
		ray.y += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? slope : -slope);
	}
	env->ver[th].dist = fabs((env->pos.x - ray.x) /
			cos(angle)) * cos(env->dir.x - angle);
	if (env->ver[th].dist < 0)
		env->ver[th].dist = INT_MAX;
	env->ver[th].off = (ray.y % SCALE) / (RENDER_SCALE);
	env->ver[th].angle = angle;
}

void	raycast(t_env *env, int th)
{
	double	angle;
	int		x;

	angle = env->dir.x + (FOV / 2) - (ANGLE_SHIFT * th * WIN_W / NTHREAD);
	while (angle > 2 * M_PI)
		angle -= (2 * M_PI);
	x = th * (WIN_W / NTHREAD) - 1;
	while (++x < (th + 1) * WIN_W / NTHREAD)
	{
		while (angle < 0)
			angle += (2 * M_PI);
		horizontal_ray(env, angle, th);
		vertical_ray(env, angle, th);
		draw(env, (env->hor[th].dist <= env->ver[th].dist ?
				env->hor[th] : env->ver[th]), x);
		angle -= ANGLE_SHIFT;
	}
}

void	*set_thread(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env*)arg;
	i = -1;
	while (++i < NTHREAD)
	{
		if (pthread_equal(pthread_self(), env->tid[i]))
			raycast(env, i);
	}
	return (NULL);
}

void	multithread(t_env *env)
{
	int		i;

	i = -1;
	while (++i < WIN_W * WIN_H)
		env->mlx->data[i] = 0;
	mlx_clear_window(env->mlx->mlx, env->mlx->win);
	i = -1;
	while (++i < NTHREAD)
		pthread_create(&env->tid[i], NULL, set_thread, env);
	i = -1;
	while (++i < NTHREAD)
		pthread_join(env->tid[i], NULL);
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, env->mlx->img, 0, 0);
	if (env->map->toggle)
		minimap(env);
}
