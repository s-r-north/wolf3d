#include "./libs/minilibx_macos_10.11/mlx.h"
#include "./libs/libft/libft.h"
#include <math.h>
#include <pthread.h>
#include <limits.h>

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

# define MAP_WIDTH 24
# define MAP_HEIGHT 24

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

int worldMap[MAP_WIDTH][MAP_HEIGHT]=
{
  {1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,2,1,2,1,2,1,3,1,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {3,0,0,0,0,0,4,4,4,4,4,0,0,0,0,5,0,5,0,5,0,0,0,2},
  {2,0,0,0,0,0,4,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,4,0,0,0,4,0,0,0,0,5,0,0,0,5,0,0,0,3},
  {2,0,0,0,0,0,4,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,4,4,0,4,4,0,0,0,0,5,0,5,0,5,0,0,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,5,0,5,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,6,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,4,0,5,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,5,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,5,4,5,4,5,4,5,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {1,1,2,1,3,1,2,1,2,1,2,1,3,1,2,1,1,1,2,1,2,1,2,1}
};

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
	t_xy		pos;
	t_fxy		dir;
	t_ray		H;
	t_ray		V;
	t_key		key;
	t_minimap	*map;
	t_text		tex[NTEX];
	t_menu		*menu;
	int			music;
}				t_env;

int		check_grid(int x, int y)
{
	x /= SCALE;
	y /= SCALE;
	if (x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0)
		return (-1);
	return (worldMap[y][x]);
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
	if (keycode == KEY_M)
	{
		if (env->key.mute)
			system("killall -STOP -c \"afplay\"");
		else
			system("killall -CONT -c \"afplay\"");
		env->key.mute = env->key.mute ? 0 : 1;
	}
	return (0);
}

int		exit_hook(t_env *env)
{
	if (env->music)
		system("killall -c \"afplay\"");
	exit(0);
	return (0);
}

void	clear_image(t_mlx *mlx)
{
	int i;

	i = -1;
	while (++i < WIN_W * WIN_H)
		mlx->data[i] = 0;
	mlx_clear_window(mlx->mlx, mlx->win);
}

int		fov_color(int length, int bg)
{
	int rgb1[3];
	int rgb2[3];
	int c2;

	c2 = 0xb0 / 8;
	rgb1[0] = 0xff - (length * 0xff / 8);
	rgb1[1] = 0;
	rgb1[2] = 0;
	rgb2[0] = (bg >> 16) * length / 8;
	rgb2[1] = ((bg >> 8) & 0xff) * length / 8;
	rgb2[2] = (bg & 0xff) * length / 8;
	return ((((rgb1[0] + rgb2[0]) & 0xff) << 16) | (((rgb1[1] + rgb2[1])\
			& 0xff) << 8) | (((rgb1[2] + rgb2[2]) & 0xff)));
}

void	bresen_angle(t_env *env, int x1, int y1)
{
	t_fxy xy2;
	double a1;
	double a2;
	int length;

	length = 0;
	a2 = env->map->dir - (FOV);
	while (++length <= 8)
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
			a1 -= (FOV / 10);
		}
	}
}

void	minimap(t_env *env)
{
	int i;
	int x1;
	int y1;

	env->map->pos.x = 5 * (double)env->pos.x / SCALE;
	env->map->pos.y = 5 * (double)env->pos.y / SCALE;
	env->map->dir = -env->dir.x;
	i = -1;
	while (++i < (120 * 120))
		env->map->img[1][i] = env->map->img[0][i];
	y1 = -3;
	while (++y1 <= 2)
	{
		x1 = -3;
		while (++x1 <= 2)
			if (sqrt(x1 * x1 + y1 * y1) < 2)
				env->map->img[1][(int)(env->map->pos.x + x1) +
						(int)(env->map->pos.y + y1) * 120] = 0x00ff00;
	}
	bresen_angle(env, (int)env->map->pos.x, (int)env->map->pos.y);
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, env->map->ptr[1], 20,
			WIN_H - 140);
}

void	horizontal_ray(t_env *env, double angle)
{
	t_xy	H;
	int		Xi;

	Xi = SCALE / tan(angle);
	H.y = ((env->pos.y / SCALE) * SCALE) + ((angle > M_PI) ? SCALE : -1);
	H.x = env->pos.x + ((env->pos.y - H.y) / tan(angle));
	while (!(env->H.tex = check_grid(H.x, H.y)))
	{
		H.x += ((angle > M_PI) ? -Xi : Xi);
		H.y += ((angle > M_PI) ? SCALE : -SCALE);
	}
	env->H.dist = fabs((env->pos.y - H.y) /
			sin(angle)) * cos(env->dir.x - angle);
	if (env->H.dist < 0)
		env->H.dist = INT_MAX;
	env->H.off = (H.x % SCALE) / (RENDER_SCALE);
}

void	vertical_ray(t_env *env, double angle)
{
	t_xy	V;
	int		Yi;

	Yi = SCALE * tan(angle);
	V.x = ((env->pos.x / SCALE) * SCALE) +
			((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -1 : SCALE);
	V.y = env->pos.y + ((env->pos.x - V.x) * tan(angle));
	while (!(env->V.tex = check_grid(V.x, V.y)))
	{
		V.x += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -SCALE : SCALE);
		V.y += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? Yi : -Yi);
	}
	env->V.dist = fabs((env->pos.x - V.x) /
			cos(angle)) * cos(env->dir.x - angle);
	if (env->V.dist < 0)
		env->V.dist = INT_MAX;
	env->V.off = (V.y % SCALE) / (RENDER_SCALE);
}

int		color(int color, double depth)
{
	int rgb[3];
	double drgb[3];

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

void	draw(t_env *env, t_ray ray, int x, double angle)
{
	double i;
	if ((ray.slice = (WALL_HEIGHT / ray.dist)) > RENDER_SCALE)
		ray.slice = RENDER_SCALE;
	int y = (WIN_H - ray.slice) / 2;
	int ground = y + (ray.slice * 2);
	int j = -1;
	while (++j < y)
		env->mlx->data[x + (j * WIN_W)] = env->tex[NTEX - 1].img\
				[(int)(angle * SKY_RES / (M_PI * 2)) + (j * SKY_RES)];
	i = 0;
	ray.depth = (ray.slice < WIN_H) ? ((double)ray.slice / WIN_H) : 1;
	while (y < ground)
	{
		if (y >= 0 && y < WIN_H)
		{
			env->mlx->data[x + (y * WIN_W)] = color(env->tex[ray.tex].img\
					[ray.off + ((int)i * TEX_RES)], ray.depth);
		}
		y++;
		i += (64 / (double)ray.slice);
	}
	while (y < WIN_H)
		env->mlx->data[x + (y++ * WIN_W)] = color(0x808080, ray.depth);
}

void	raycast(t_env *env)
{
	clear_image(env->mlx);
	double angle = env->dir.x + (FOV / 2);
	while (angle > 2 * M_PI)
		angle -= (2 * M_PI);
	int x = -1;
	while (++x < WIN_W)
	{
		while (angle < 0)
			angle += (2 * M_PI);
		horizontal_ray(env, angle);
		vertical_ray(env, angle);
		draw(env, (env->H.dist <= env->V.dist ? env->H : env->V), x, angle);
		angle -= ANGLE_SHIFT;
	}
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, env->mlx->img, 0, 0);
	minimap(env);
}

void	init_mlx_pointers(t_env *env, char *s)
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

char	*texture_asset(int num)
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

void	init_map(t_env *env)
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
	w /= 4;
	y = -1;
	while (++y < w)
	{
		x = -1;
		while (++x < w)
			env->map->img[0][x + (y * w)] =  worldMap[y / 5][x / 5] ?
					0x000000 : 0xb0b0b0;
	}
}

void	init_env(t_env *env)
{
	int i;

	env->music = 1;
	env->pos.x = (22 * SCALE) + (SCALE / 2);
	env->pos.y = (12 * SCALE) + (SCALE / 2);
	env->dir.x = M_PI;
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
	init_map(env);
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
	return (0);
}

int		hook_func(t_env *env)
{
	int movex;
	int movey;

	if (env->key.mouse)
	{
		env->dir.x -= TURN_SPEED * (double)env->key.mouse / WIN_W;
		while (env->dir.x < 0 || env->dir.x > 2 * M_PI)
			env->dir.x += (2 * M_PI) * (env->dir.x < 0 ? 1 : -1);
	}
	if (env->key.lf || env->key.rt)
	{
		env->dir.x += TURN_SPEED * (env->key.rt ? -1 : 1);
		while (env->dir.x < 0 || env->dir.x > 2 * M_PI)
			env->dir.x += (2 * M_PI) * (env->dir.x < 0 ? 1 : -1);
		printf("%lf\n", env->dir.x);
	}
	movex = cos(env->dir.x) * MOVE_SPEED * (env->key.shift ? 1.5 : 1);
	movey = sin(env->dir.x) * MOVE_SPEED * (env->key.shift ? 1.5 : 1);
	if (env->key.w)
	{
		if(!check_grid(env->pos.x + (COLLISION * movex), env->pos.y))
			env->pos.x += movex;
		if(!check_grid(env->pos.x, env->pos.y - (COLLISION * movey)))
			env->pos.y -= movey;
	}
	if (env->key.s)
	{
		if(!check_grid(env->pos.x - (COLLISION * movex), env->pos.y))
			env->pos.x -= movex;
		if(!check_grid(env->pos.x, env->pos.y + (COLLISION * movey)))
			env->pos.y += movey;
	}
	movex = cos(env->dir.x - (M_PI / 2)) * (MOVE_SPEED / 2) * (env->key.shift ? 1.5 : 1);
	movey = sin(env->dir.x - (M_PI / 2)) * (MOVE_SPEED / 2) * (env->key.shift ? 1.5 : 1);
	if (env->key.a)
	{
		if(!check_grid(env->pos.x - (COLLISION * movex), env->pos.y))
			env->pos.x -= movex;
		if(!check_grid(env->pos.x, env->pos.y + (COLLISION * movey)))
			env->pos.y += movey;
	}
	if (env->key.d)
	{
		if(!check_grid(env->pos.x + (COLLISION * movex), env->pos.y))
			env->pos.x += movex;
		if(!check_grid(env->pos.x, env->pos.y - (COLLISION * movey)))
			env->pos.y -= movey;
	}
	raycast(env);
	return (0);
}

void	wolf(t_env *env)
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

int		menu_hook(int mbutton, int x, int y, t_env *env)
{
	int i;

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
			wolf(env);
		}
	}
	return (0);
}

char	*menu_gfx(int code)
{
	if (code == 0)
		return ("assets/menu_bg.XPM");
	if (code == 1)
		return ("assets/start_statue.XPM");
	if (code == 2)
		return ("assets/exit_wizard.XPM");
	return (NULL);
}

int		menu_exit(int keycode, t_env *env)
{
	(void)env;
	if (keycode == 53)
		exit(0);
	return (0);
}

void	menu_loop(t_env *env, t_menu *menu)
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

void	menu(t_env *env)
{
	int i;
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
		menu.img[i] = (int*)mlx_get_data_addr(menu.ptr[i], &menu.bpp, &menu.sl, &menu.e);
	}
	env->music = 0;
	env->menu = &menu;
	menu_loop(env, &menu);
}

int		main()
{
	t_env	env;

	init_mlx_pointers(&env, "Wolf 3D");
	menu(&env);
}
