/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiko <jiko@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:06:24 by jiko              #+#    #+#             */
/*   Updated: 2024/02/27 17:54:04 by jiko             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	is_map(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != '1' && line[i] != '0' && line[i] != 'S' && line[i] != 'N'\
		&& line[i] != 'W' && line[i] != 'E' && line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_space_side(char **line, t_map *map)
{
	int		i;
	int		len;
	char	*tmp;

	i = 0;
	if (!(*line))
		return ;
	tmp = (*line);
	len = ft_strlen((*line));
	if (len == 0)
		return ;
	while (!is_param_full(map) && (*line)[i] == ' ')
		i++;
	if (len == i)
	{
		free(*line);
		*line = wft_calloc(1, sizeof(char));
		return ;
	}
	while ((*line)[len - 1] == ' ')
		len--;
	*line = wft_substr(*line, i, len - i);
	free(tmp);
}

static void	set_param(int fd, t_map *map)
{
	char	*line;
	char	**s;
	int		ret;

	while ((ret = get_next_line(fd, &line)) > 0)
	{
		map->map_start++;
		remove_space_side(&line, map);
		if (line == NULL || line[0] == '\0')
		{
			if (is_param_full(map) && map->map_height)
			{
				map->map_start--;
				map->map_flag = 1;
			}
			safe_free(line);
			continue ;
		}
		s = wft_split(line, ' ');
		if (!ft_strncmp(s[0], "NO", 3) || !ft_strncmp(s[0], "SO", 3)
			|| !ft_strncmp(s[0], "WE", 3) || !ft_strncmp(s[0], "EA", 3)
			|| !ft_strncmp(s[0], "F", 2) || !ft_strncmp(s[0], "C", 2))
		{
			if (ft_strlen_doble(s) != 2)
				ft_exit(1, "Error\nInvalid parameter\n");
			if (!ft_strncmp(s[0], "NO", 3) && !map->no)
				map->no = wft_strdup(s[1]);
			else if (!ft_strncmp(s[0], "SO", 3) && !map->so)
				map->so = wft_strdup(s[1]);
			else if (!ft_strncmp(s[0], "WE", 3) && !map->we)
				map->we = wft_strdup(s[1]);
			else if (!ft_strncmp(s[0], "EA", 3) && !map->ea)
				map->ea = wft_strdup(s[1]);
			else if (!ft_strncmp(s[0], "F", 2) && !map->floor)
				map->floor = ft_color(s[1]);
			else if (!ft_strncmp(s[0], "C", 2) && !map->ceiling)
				map->ceiling = ft_color(s[1]);
			else
				ft_exit(1, "Error\nInvalid parameter\n");
		}
		else if (is_param_full(map) && is_map(line))
		{
			map->map_start--;
			if (map->map_flag)
				ft_exit(1, "Error\nInvalid map\n");
			map->map_height++;
			map->map_width = ft_max(map->map_width, ft_strlen(line));
		}
		else
			ft_exit(1, "Error\nInvalid parameter\n");
		free(line);
		double_free(ft_strlen_doble(s), s);
	}
	if (ret == -1)
		ft_exit(1, "Error\nFailed to read file\n");
}

static void set_map(int fd, t_map *t_map)
{
	int		**map;
	char	*line;
	int		i;
	int		j;

	map = wft_calloc(t_map->map_height, sizeof(int *));
	i = -1;
	while (++i < t_map->map_height)
		map[i] = wft_calloc(t_map->map_width, sizeof(int));
	i = -1;
	while (++i < t_map->map_height)
	{
		j = -1;
		while (++j < t_map->map_width)
			map[i][j] = 2;
	}
	i = -1;
	while (++i < t_map->map_start)
	{
		get_next_line(fd, &line);
		free(line);
	}
	i = -1;
	while (++i < t_map->map_height)
	{
		get_next_line(fd, &line);
		remove_space_side(&line, t_map);
		j = -1;
		while (++j < (int)ft_strlen(line))
		{
			if (line[j] == '1')
				map[i][j] = 1;
			else if (line[j] == '0')
				map[i][j] = 0;
			else if (line[j] == 'N' || line[j] == 'S'
				|| line[j] == 'W' || line[j] == 'E')
			{
				if (t_map->player_dir)
					ft_exit(1, "Error\nInvalid map\n");
				map[i][j] = 0;
				t_map->player_x = j;
				t_map->player_y = i;
				t_map->player_dir = line[j];
			}
		}
		free(line);
	}
	if (!t_map->player_dir)
		ft_exit(1, "Error\nInvalid map\n");
	t_map->map = map;
}

void	init_map(int argv, char **argc, t_map *map)
{
	int	fd;

	fd = open_cube(argv, argc, map);
	set_param(fd, map);
	close(fd);
	if (!is_param_full(map) || !map->map_height || !map->map_width)
		ft_exit(1, "Error\nInvalid map\n");
	fd = open(argc[1], O_RDONLY);
	set_map(fd, map);
	close(fd);
	dfs_valid_map(map);
}

