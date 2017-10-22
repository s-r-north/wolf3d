/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"



int		press_key(int keycode, t_env *env)
{
	if (keycode == KEY_ESC)
	{
		system("killall -c \"afplay\"");
		exit(0);
	}
	if (keycode == KEY_RT)
		env->key.rt = 1;
	if (keycode == KEY_LF)
		env->key.lf = 1;
	if (keycode == KEY_W)
		env->key.w = 1;
	if (keycode == KEY_S)
		env->key.s = 1;
	if (keycode == KEY_A)
		env->key.a = 1;
	if (keycode == KEY_D)
		env->key.d = 1;
	if (keycode == KEY_LSHIFT)
		env->key.shift = 1;
	if (keycode == KEY_M)
	{
		if (env->key.mute)
			system("killall -STOP -c \"afplay\"");
		else
			system("killall -CONT -c \"afplay\"");
		env->key.mute = env->key.mute ? 0 : 1;
	}
	// if (keycode == KEY_Q)
	// 	go_back(env);
	return (0);
}

int		release_key(int keycode, t_env *env)
{
	if (keycode == KEY_RT)
		env->key.rt = 0;
	if (keycode == KEY_LF)
		env->key.lf = 0;
	if (keycode == KEY_W)
		env->key.w = 0;
	if (keycode == KEY_S)
		env->key.s = 0;
	if (keycode == KEY_A)
		env->key.a = 0;
	if (keycode == KEY_D)
		env->key.d = 0;
	if (keycode == KEY_LSHIFT)
		env->key.shift = 0;
	printf("key: %d\n", keycode);
	return (0);
}

int		exit_hook(t_env *env)
{
	if (env->music)
		system("killall -c \"afplay\"");
	exit(0);
	return (0);
}

int		mouse_pos(int x, int y, t_env *env)
{
	(void)y;
	if (x < 0 || x > WIN_W || y < 0 || y > WIN_H)
	{
		env->key.mouse = 0;
		return (0);
	}
	if (x >= (3 * WIN_W / 7) && x < (4 * WIN_W / 7))
		env->key.mouse = 0;
	else
		env->key.mouse = (x - (WIN_W / 2));
	return (0);
}
