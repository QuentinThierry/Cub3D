/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:09:30 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 21:47:17 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_H
# define AUDIO_H

# include "raudio/src/raudio.h"
# include "enum.h"

# define BACKGROUND_MUSIC "./assets/sounds/test.wav"

typedef struct s_map	t_map;
typedef struct s_game	t_game;
typedef struct s_image	t_image;

typedef Music	t_music;

typedef struct s_music_name
{
	char				*filename;
	char				*subtitle;
	unsigned int		offset;
	long int			time;
	t_orient			orient;
	char				symbol;
}	t_music_name;

typedef struct s_music_game
{
	t_music		music;
	t_map		*map_cell;
	bool		is_playing;
	bool		is_subtitle;
}	t_music_game;


char			*get_music(t_music_name *filename, int nb_music, char symbol,
					t_orient orient);
t_music_name	*get_narrator(t_music_name *filename, int nb_music, char symbol,
					t_orient orient);
bool			init_audio(t_game *game);
void			update_sounds(t_music_game *music_array);
void			close_audio(t_music_game *music_tab);
void			play_music(t_map *map_cell, t_music_game *music_tab,
					char *filename, unsigned int type);
void			play_narrator(t_game *game, t_map *map_cell,
					t_music_game *music_tab);
void			play_sound_fail(t_game *game, t_map *map_cell,
					t_music_game *music_tab);
void			set_next_narrator(t_map *map_cell);
void			update_map_cell_music(t_map *map_cell, t_map *old_map_cell,
					t_music_game *music_array);
void			clear_sound(t_music_game *music_array);
void			print_subtitle(t_game *game, t_map *map_cell);
void			free_image(void *mlx_ptr, t_image *image);

#endif
