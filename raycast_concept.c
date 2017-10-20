#include "./libs/minilibx_macos_10.11/mlx.h"
#include "./libs/libft/libft.h"
#include <math.h>
#include <pthread.h>
#include <limits.h>

# define BITS_PER_PIXEL 32
# define ENDIAN 1
# define WIN_W 1000
# define WIN_H 600

# define SCALE 262144
# define FOV (M_PI / 3)
# define WALL_HEIGHT (SCALE * 700)
# define ANGLE_SHIFT (FOV / WIN_W)
# define ANGLE_START (3 * M_PI / 2)

# define NTEX 1
# define TEX_RES 64
# define RENDER_SCALE (SCALE / TEX_RES)

# define MOVE_SPEED (5 * (RENDER_SCALE))
# define TURN_SPEED (M_PI / 128)
# define MOUSE_SENS (4 * M_PI / WIN_W)
# define COLLISION 5

# define mapWidth 24
# define mapHeight 24

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

int g_worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
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
}				t_key;

typedef struct	s_env
{
	t_mlx		*mlx;
	t_xy		pos;
	t_fxy		dir;
	t_ray		H;
	t_ray		V;
	t_key		key;
	void		*tex_img[NTEX];
	int			*texture[NTEX];
}				t_env;

int		check_grid(int x, int y)
{
	x /= SCALE;
	y /= SCALE;
	if (x >= mapWidth || y >= mapHeight || x < 0 || y < 0)
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
	return (0);
}

int		exit_hook()
{
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
	env->H.dist = fabs((env->pos.y - H.y) / sin(angle)) * cos(env->dir.x - angle);
	if (env->H.dist < 0)
		env->H.dist = INT_MAX;
	env->H.off = (H.x % SCALE) / (RENDER_SCALE);
}

void	vertical_ray(t_env *env, double angle)
{
	t_xy	V;
	int		Yi;

	Yi = SCALE * tan(angle);
	V.x = ((env->pos.x / SCALE) * SCALE) + ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -1 : SCALE);
	V.y = env->pos.y + ((env->pos.x - V.x) * tan(angle));
	while (!(env->V.tex = check_grid(V.x, V.y)))
	{
		V.x += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? -SCALE : SCALE);
		V.y += ((angle > M_PI / 2 && angle < 3 * M_PI / 2) ? Yi : -Yi);
	}
	env->V.dist = fabs((env->pos.x - V.x) / cos(angle)) * cos(env->dir.x - angle);
	if (env->V.dist < 0)
		env->V.dist = INT_MAX;
	env->V.off = (V.y % SCALE) / (RENDER_SCALE);
}

int		dumbcolor(int tex, int vert)
{
	int color;

	switch(tex)
	{
		case 1:  color = vert == 1 ? 0x800000 : 0xff0000; break; //red
		case 2:  color = vert == 1 ? 0x008000 : 0x00ff00; break; //green
		case 3:  color = vert == 1 ? 0x000080 : 0x0000ff; break; //blue
		case 4:  color = vert == 1 ? 0x808080 : 0xffffff; break; //white
		default: color = vert == 1 ? 0x808000 : 0xffff00; break; //yellow
	}
	return (color);
}

void	draw(t_env *env, t_ray ray, int x, double angle)
{
	double i;
	if ((ray.slice = WALL_HEIGHT / ray.dist) > 5000)
		ray.slice = 5000;
	int y = (WIN_H - ray.slice) / 2;
	int ground = y + ray.slice;
	i = 0;
	while (y < ground)
	{
		if (y >= 0 && y < WIN_H)
			env->mlx->data[x + (y * WIN_W)] = (ray.tex == 1 ? env->texture[ray.tex - 1][ray.off + ((int)i * TEX_RES)] : dumbcolor(ray.tex, (env->V.dist <= env->H.dist)));
		y++;
		i += (64 / (double)ray.slice);
	}
	while (y < WIN_H)
		env->mlx->data[x + (y++ * WIN_W)] = 0xff00ff;
}

void	raytrace(t_env *env)
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

void	init_env(t_env *env)
{
	int texture_size = TEX_RES;
	int text_bpp = 4;
	int text_endian = 0;
	int text_sl = 0;
	env->pos.x = (22 * SCALE) + (SCALE / 2);
	env->pos.y = (12 * SCALE) + (SCALE / 2);
	env->dir.x = M_PI;
	env->dir.y = 0;
	ft_bzero(&env->key, sizeof(t_key));
	for (int i = 0; i < NTEX; i++)
	{
		env->tex_img[i] = mlx_xpm_file_to_image(env->mlx->mlx, "assets/pearlstone_wall.xpm", &texture_size, &texture_size);
		env->texture[i] = (int*)mlx_get_data_addr(env->tex_img[i], &text_bpp, &text_sl, &text_endian);
	}
}

int		press_key(int keycode, t_env *env)
{
	if (keycode == KEY_ESC)
		exit(0);
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
	return (0);
}

int		hook_func(t_env *env)
{
	int movex;
	int movey;

	if (env->key.lf || env->key.rt)
	{
		env->dir.x += TURN_SPEED * (env->key.rt ? -1 : 1);
		while (env->dir.x < 0 || env->dir.x > 2 * M_PI)
			env->dir.x += (2 * M_PI) * (env->dir.x < 0 ? 1 : -1);
	}
	movex = cos(env->dir.x) * MOVE_SPEED;
	movey = sin(env->dir.x) * MOVE_SPEED;
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
	movex = cos(env->dir.x - (M_PI / 2)) * (MOVE_SPEED / 2);
	movey = sin(env->dir.x - (M_PI / 2)) * (MOVE_SPEED / 2);
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
	raytrace(env);
	return (0);
}

int		main()
{
	t_env	env;

	init_mlx_pointers(&env, "hello world");
	init_env(&env);
	raytrace(&env);
	mlx_hook(env.mlx->win, 2, 0, press_key, &env);
	mlx_hook(env.mlx->win, 3, 0, release_key, &env);
	mlx_hook(env.mlx->win, 17, 0, exit_hook, &env);
	mlx_loop_hook(env.mlx->mlx, hook_func, &env);
	mlx_loop(env.mlx->mlx);
}
