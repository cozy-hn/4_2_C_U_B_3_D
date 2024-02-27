/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiko <jiko@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 05:32:22 by sumjo             #+#    #+#             */
/*   Updated: 2024/02/27 17:34:22 by jiko             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	copy_texture(t_data *img, t_var *var, int idx)
{
	int	i;
	int	j;
	int	color;
	int	result_row;
	int	result_col;

	i = -1;
	while (++i < TEXTHEIGHT)
	{
		j = -1;
		while (++j < TEXTWIDTH)
		{
			result_row = (img->height * i) / TEXTHEIGHT;
			result_col = (img->width * j) / TEXTWIDTH;
			if (result_row < 0)
				result_row = 0;
			if (result_col < 0)
				result_col = 0;
			color = img->addr_tex[img->width * result_row + result_col];
			var->tex[idx].texture[TEXTWIDTH * i + j] = color;
		}
	}
	var->tex[idx].width = TEXTWIDTH;
	var->tex[idx].height = TEXTHEIGHT;
}

void	init_texture_direction(char *path, t_data *img, t_var *var, int idx)
{
	t_data	img2;

	var->tex[idx].texture = (int *)ft_calloc(TEXTHEIGHT * TEXTWIDTH, sizeof(int));
	img2.img = mlx_xpm_file_to_image(img->mlx, path, &img2.width, &img2.height);
	if (!img2.img)
		ft_exit(1, "Error\nFailed to open xmp\n");
	img2.addr_tex = (int *)mlx_get_data_addr \
	(img2.img, &img2.bits_pixel, &img2.line_length, &img2.endian);
	copy_texture(&img2, var, idx);
	mlx_destroy_image(img->mlx, img2.img);
}

void	init_texture(t_data *img, t_var *var)
{
	init_texture_direction(var->map->no, img, var, 0);
	init_texture_direction(var->map->so, img, var, 1);
	init_texture_direction(var->map->we, img, var, 2);
	init_texture_direction(var->map->ea, img, var, 3);
}

void	texture_mapping(t_var *var)
{
	double	wallx;
	int		texX;

	if (var->ray->side == 0)
		wallx = var->vec->pos.y + var->ray->perpWallDist * var->ray->rayDir.y;
	else
		wallx = var->vec->pos.x + var->ray->perpWallDist * var->ray->rayDir.x;
	wallx -= floor((wallx));
	texX = (int)(wallx * (double)var->tex->width);
	if (var->ray->side == 0 && var->ray->rayDir.x > 0)
		texX = var->tex->width - texX - 1;
	if (var->ray->side == 1 && var->ray->rayDir.y < 0)
		texX = var->tex->width - texX - 1;
	var->tex_var.wallx = wallx;
	var->tex_var.texX = texX;
}
