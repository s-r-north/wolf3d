#include "wolf3d.h"

static int	redshift(int color, double depth)
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

static int	ground_texture(t_env *env, t_ray ray, int y)
{
	double	h0;
	int		horizon;
	t_xy	dist;

	// h0 = (double)(WIN_H / (2 * y - WIN_H));
	// horizon = SCALE * (h0 / ray.dist);
	// dist.x = (horizon * start) + ((SCALE - horizon) * env->pos.x);
	// dist.y = (horizon * start) + ((SCALE - horizon) * env->pos.x);

	h0 = ((double)WIN_H / 2) / ((double)y - ((double)WIN_H / 2)) * 988574;
	horizon = h0 / cos(ray.angle - env->dir.x);
	dist.x = env->pos.x + (horizon * cos(ray.angle));
	dist.y = env->pos.y - (horizon * sin(ray.angle));
	// printf("%d\t%d\t%d\n", col, dist.x, dist.y);
	dist.x = (dist.x / RENDER_SCALE) % TEX_RES;
	dist.y = (dist.y / RENDER_SCALE) % TEX_RES;
	return (redshift(env->tex[0].img[dist.x + (dist.y * TEX_RES)], SCALE / (double)horizon));
}

void		draw(t_env *env, t_ray ray, int x)
{
	double i;
	if ((ray.slice = (WALL_HEIGHT / ray.dist)) > RENDER_SCALE * 2)
		ray.slice = RENDER_SCALE * 2;
	int y = (WIN_H - ray.slice) / 4;
	int ground = y + (ray.slice * 2);
	int j = -1;
	while (++j < y)
		env->mlx->data[x + (j * WIN_W)] = env->tex[NTEX - 1].img\
				[(int)(ray.angle * SKY_RES / (M_PI * 2)) + (j * SKY_RES)];
	i = 0;
	ray.depth = (ray.slice < WIN_H) ? ((double)ray.slice / WIN_H) : 1;
	while (y < ground)
	{
		if (y >= 0 && y < WIN_H)
		{
			env->mlx->data[x + (y * WIN_W)] = redshift(env->tex[ray.tex].img\
					[ray.off + ((int)i * TEX_RES)], ray.depth);
		}
		y++;
		i += (64 / (double)ray.slice);
	}
	--y;
	while (++y < WIN_H)
		env->mlx->data[x + (y * WIN_W)] = ground_texture(env, ray, y); //color(0x808080, ray.depth);
}
