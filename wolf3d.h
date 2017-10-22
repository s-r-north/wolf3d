/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "./libs/minilibx_macos_10.11/mlx.h"
# include "./libs/libft/libft.h"
# include <math.h>
# include <pthread.h>
# include <limits.h>
# include <fcntl.h>

# define BITS_PER_PIXEL 32
# define ENDIAN 1
# define WIN_W 1280
# define WIN_H 720

# define SCALE 262144
# define FOV (M_PI / 4)
# define WALL_HEIGHT (SCALE * WIN_H)
# define ANGLE_SHIFT (FOV / WIN_W)

# define NTEX 11
# define SKY_RES 5760
# define TEX_RES 128
# define RENDER_SCALE (SCALE / TEX_RES)

# define MOVE_SPEED (10 * (RENDER_SCALE))
# define TURN_SPEED (M_PI / 128)
# define MOUSE_SENS (4 * M_PI / WIN_W)
# define COLLISION 10

# define MMAP_POS_RAD 4
# define MMAP_CONE_LEN 20

# define KEY_UP 126
# define KEY_DN 125
# define KEY_LF 123
# define KEY_RT 124
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_Q 12
# define KEY_E 14
# define KEY_F 3
# define KEY_G 5
# define KEY_0 29
# define KEY_R 15
# define KEY_X 7
# define KEY_Z 6
# define KEY_C 8
# define KEY_ESC 53
# define KEY_SP 49
# define KEY_LSHIFT 257
# define KEY_M 46

# define NTHREAD 8

typedef struct	s_mlx
{
	void		*mlx;
	void		*win;
	void		*img;
	int			*data;
}				t_mlx;

typedef struct	s_ray
{
	int			dist;
	int			off;
	int			tex;
	int			slice;
	double		depth;
	double		angle;
}				t_ray;

typedef struct	s_xy
{
	int			x;
	int			y;
}				t_xy;

typedef struct	s_fxy
{
	double		x;
	double		y;
}				t_fxy;

typedef struct	s_key
{
	int			w : 1;
	int			a : 1;
	int			s : 1;
	int			d : 1;
	int			rt : 1;
	int			lf : 1;
	int			shift : 1;
	int			mute : 1;
	int			mouse;
}				t_key;

typedef struct	s_text
{
	void		*ptr;
	int			*img;
	int			e;
	int			bpp;
	int			h;
	int			w;
	int			sl;
}				t_text;

typedef struct	s_menu
{
	void		*ptr[3];
	int			*img[3];
	int			w[3];
	int			h[3];
	int			e;
	int			bpp;
	int			sl;
}				t_menu;

typedef struct	s_minimap
{
	void		*ptr[2];
	int			*img[2];
	t_fxy		pos;
	double		dir;
}				t_minimap;

typedef struct	s_env
{
	t_mlx		*mlx;
	int			**world;
	int			map_w;
	int			map_h;
	t_xy		start;
	t_xy		pos;
	t_fxy		dir;
	t_ray		hor[NTHREAD];
	t_ray		ver[NTHREAD];
	t_key		key;
	t_minimap	*map;
	t_text		tex[NTEX];
	t_menu		*menu;
	pthread_t	tid[NTHREAD];
	int			music;
}				t_env;

void			wolf(t_env *env);

/*
**	Menu
*/

void			menu(t_env *env);
void			menu_loop(t_env *env, t_menu *menu);

/*
**	Read
*/

void			read_map(t_env *env, int fd);
void			free_map(int ***map, int x, int y);

/*
**	Ray Casting
*/

void			multithread(t_env *env);
void			*set_thread(void *arg);
void			raycast(t_env *env, int th);
void			horizontal_ray(t_env *env, double angle, int th);
void			vertical_ray(t_env *env, double angle, int th);

/*
**	Image Drawing
*/

void			draw(t_env *env, t_ray ray, int x);

/*
**	Minimap
*/

void			minimap(t_env *env);

/*
**	Event Hooks
*/

int				press_key(int keycode, t_env *env);
int				release_key(int keycode, t_env *env);
int				mouse_pos(int x, int y, t_env *env);
int				exit_hook(t_env *env);

/*
**	Movement
*/

int				hook_func(t_env *env);

int				check_grid(t_env *env, int x, int y);

#endif
