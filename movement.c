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

#include "wolf3d.h"

int		check_grid(t_env *env, int x, int y)
{
	x /= SCALE;
	y /= SCALE;
	if (x >= env->map_w || y >= env->map_h || x < 0 || y < 0)
		return (-1);
	return (env->world[y][x]);
}

static void	turn(t_env *env)
{
	if (env->key.mouse)
	{
		env->dir.x -= TURN_SPEED * 2 * (double)env->key.mouse / WIN_W;
		while (env->dir.x < 0 || env->dir.x > 2 * M_PI)
			env->dir.x += (2 * M_PI) * (env->dir.x < 0 ? 1 : -1);
	}
	if (env->key.lf || env->key.rt)
	{
		env->dir.x += TURN_SPEED * (env->key.rt ? -1 : 1);
		while (env->dir.x < 0 || env->dir.x > 2 * M_PI)
			env->dir.x += (2 * M_PI) * (env->dir.x < 0 ? 1 : -1);
		// printf("%lf\n", env->dir.x);
	}
}

static void	move_front_back(t_env *env, int step_x, int step_y)
{
	if (env->key.w)
	{
		if(!check_grid(env, env->pos.x + (COLLISION * step_x), env->pos.y))
			env->pos.x += step_x;
		if(!check_grid(env, env->pos.x, env->pos.y - (COLLISION * step_y)))
			env->pos.y -= step_y;
	}
	if (env->key.s)
	{
		if(!check_grid(env, env->pos.x - (COLLISION * step_x), env->pos.y))
			env->pos.x -= step_x;
		if(!check_grid(env, env->pos.x, env->pos.y + (COLLISION * step_y)))
			env->pos.y += step_y;
	}
}

static void move_sides(t_env *env, int step_x, int step_y)
{
	if (env->key.a)
	{
		if(!check_grid(env, env->pos.x - (COLLISION * step_x), env->pos.y))
			env->pos.x -= step_x;
		if(!check_grid(env, env->pos.x, env->pos.y + (COLLISION * step_y)))
			env->pos.y += step_y;
	}
	if (env->key.d)
	{
		if(!check_grid(env, env->pos.x + (COLLISION * step_x), env->pos.y))
			env->pos.x += step_x;
		if(!check_grid(env, env->pos.x, env->pos.y - (COLLISION * step_y)))
			env->pos.y -= step_y;
	}
}

int		hook_func(t_env *env)
{
	int step_x;
	int step_y;

	turn(env);
	step_x = cos(env->dir.x) * MOVE_SPEED * (env->key.shift ? 1.5 : 1);
	step_y = sin(env->dir.x) * MOVE_SPEED * (env->key.shift ? 1.5 : 1);
	move_front_back(env, step_x, step_y);
	step_x = cos(env->dir.x - (M_PI / 2)) * (MOVE_SPEED / 2) *
			(env->key.shift ? 1.5 : 1);
	step_y = sin(env->dir.x - (M_PI / 2)) * (MOVE_SPEED / 2) *
			(env->key.shift ? 1.5 : 1);
	move_sides(env, step_x, step_y);
	multithread(env);
	return (0);
}
