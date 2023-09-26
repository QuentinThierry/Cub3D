/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:16:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/26 19:24:36 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	init_audio(t_game *game, t_music_name *music_file, int nb_music)
{
	int	i;
	
	InitAudioDevice();
	if (!IsAudioDeviceReady() || NB_MAX_SOUNDS <= 0)
		return (printf("Error : Audio init failed\n"), false);
	game->music_array = ft_calloc(NB_MAX_SOUNDS, sizeof(t_music_game));
	if (!game->music_array)
		return (false);
	i = 0;
	game->music_array[0].music = LoadMusicStream(BACKGROUND_MUSIC);
	if (!IsMusicReady(game->music_array[0].music))
		return (printf("Error : Background music init failed\n"), false);
	game->music_array->is_playing = true;
	return (true);
}

void	close_audio(t_music_game *music_tab)
{
	int	i;

	if (!IsAudioDeviceReady())
		return (free(music_tab));
	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_tab[i].is_playing == true && IsMusicStreamPlaying(music_tab[i].music))
		{
			StopMusicStream(music_tab[i].music);
			UnloadMusicStream(music_tab[i].music);
		}
		i++;
	}
	CloseAudioDevice();
	free(music_tab);
}

static t_music_game	*find_empty_place(t_music_game *music_tab)
{
	int	i;

	i = 2;
	while (i < NB_MAX_SOUNDS)
	{
		if (!music_tab[i].is_playing)
			return (printf("%d\n", i), &music_tab[i]);
		i++;
	}
	return (NULL);
}

void	play_music(t_map *map_cell, t_music_game *music_tab)
{
	t_music_game	*music;

	if ((map_cell->type & IS_PLAYING_MUSIC) == IS_PLAYING_MUSIC)
		return ;
	music = find_empty_place(music_tab);
	if (music == NULL)
		return ;
	printf("play %s\n", map_cell->music);
	music->music = LoadMusicStream(map_cell->music);
	if (!IsMusicReady(music->music))
		return ;
	music->music.looping = false;
	music->map_cell = map_cell;
	music->is_playing = true;
	map_cell->type |= IS_PLAYING_MUSIC;
	PlayMusicStream(music->music);
}

void	play_narrator(t_map *map_cell, t_music_game *music_tab)
{
	t_music_game	*music;

	if ((map_cell->type & IS_PLAYING_NARRATOR) == IS_PLAYING_NARRATOR)
		return ;
	music = &music_tab[1];
	if (music->is_playing == true)
		return ;
	printf("play narrator %s\n", map_cell->music);
	music->music = LoadMusicStream(map_cell->music);
	if (!IsMusicReady(music->music))
		return ;
	music->music.looping = false;
	music->map_cell = map_cell;
	music->is_playing = true;
	map_cell->type |= IS_PLAYING_NARRATOR;
	map_cell->type ^= NARRATOR;
	PlayMusicStream(music->music);
}

void	update_sounds(t_music_game *music_array)
{
	int	i;

	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_array[i].is_playing == true)
		{
			if (IsMusicStreamPlaying(music_array[i].music))
				UpdateMusicStream(music_array[i].music);
			else
			{
				UnloadMusicStream(music_array[i].music);
				music_array[i].map_cell->type ^= IS_PLAYING_MUSIC;
				music_array[i].map_cell = NULL;
				music_array[i].is_playing = false;
			}
		}
		i++;
	}
}

void	update_map_cell_music(t_map *map_cell, t_map *old_map_cell, t_music_game *music_array)
{
	int	i;
	
	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (music_array[i].map_cell == old_map_cell)
		{
			music_array[i].map_cell = map_cell;
			return ;
		}
		i++;
	}
}


// int loupouk(t_game *game)
// {
// 	static struct timespec	last_time = {0};
// 	struct timespec			cur_time;
// 	long					fps;
// 	static int i = 1;

	
// 	if (last_time.tv_sec == 0)
// 		clock_gettime(CLOCK_REALTIME, &last_time);
// 	usleep(30000);

// 	clock_gettime(CLOCK_REALTIME, &cur_time);
// 	// game->delta_time = (cur_time.tv_sec - last_time.tv_sec
// 	// 		+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
// 	// fps = (long)(1.0 / game->delta_time);
// 	// tot_fps += fps;
// 	// nb_fps++;
// 	// if ((nb_fps % 50) == 0)
// 	// 	printf("fps : %ld\n", fps);
// 	// last_time = cur_time;
// 	if (last_time.tv_sec + 1 < cur_time.tv_sec && i == 1)
// 	{
// 		i = 2;
// 		// if (IsMusicStreamPlaying(game->music_array->music))
// 		// 	StopMusicStream(game->music_array->music);
// 		UnloadMusicStream(game->music_array->music);
// 		printf("test\n");
// 		game->music_array->music = LoadMusicStream("assets/sounds/stereo.wav");
// 		printf("test1\n");
// 		if (!IsMusicReady(game->music_array->music))
// 			return (printf("error loading music\n"), 1);
// 		game->music_array->music.looping = false;
// 		PlayMusicStream(game->music_array->music);
// 	}
// 	else {
// 		UpdateMusicStream(game->music_array->music);
// 	}
// 	return (0);
// }

// int main()
// {
// 	t_game	game;
// 	int	i = 1;
// 	Music	sound;

// 	game = (t_game){0};

// 	init_mlx(&game);
// 	InitAudioDevice();
// 	if (!IsAudioDeviceReady() || NB_MAX_SOUNDS <= 0)
// 		return (printf("Error : Audio init failed\n"), false);

// 	SetMasterVolume(1.f);
	
// 	sound = LoadMusicStream("assets/sounds/sound.wav");
// 	if (!IsMusicReady(sound))
// 		return (printf("error loading music\n"), 1);
// 	sound.looping = false;
// 	PlayMusicStream(sound);
// 	game.music_array = &(t_music_game){sound, NULL, 1};
// 	printf("ciucouc\n");
// 	// game.music_array[1] = LoadMusicStream("assets/sounds/test1.mp3");
// 	// if (!IsMusicReady(game.music_array[1]))
// 	// 	return (printf("error loading music 1\n"), 1);
// 	// game.music_array[1].looping = false;
// 	// PlayMusicStream(game.music_array[1]);

// 	// struct timespec time;
// 	// struct timespec time1;
// 	// clock_gettime(CLOCK_REALTIME, &time);
// 	// sound = LoadSound("assets/sounds/test.mp3");
// 	// if (!IsSoundReady(sound))
// 	// 	return (printf("error loading sound1\n"), 1);
// 	// clock_gettime(CLOCK_REALTIME, &time1);
// 	// printf("time : %ld\n", time_to_long(&time1) - time_to_long(&time));
// 	// SetSoundPan(sound, 0),
// 	// PlaySound(sound);
// 	// if (!IsSoundPlaying(sound))
// 	// 	return (printf("error playing\n"), 1);
// 	// printf("coucou\n");
// 	// sleep(5);
// 	// SetSoundPan(sound, 1.0),
// 	// PlaySound(sound);
// 	// if (!IsSoundPlaying(sound))
// 	// 	return (printf("error playing 1\n"), 1);
// 	// printf("coucou1\n");

// 	// printf("loop : %d type : %d Data : %p frame : %u \n",game.music_array[0].looping
// 	// 	, game.music_array[0].ctxType, game.music_array[0].ctxData, game.music_array[0].frameCount);
// 	// printf("buffer : %p processor : %p\n", game.music_array[0].stream.buffer, game.music_array[0].stream.processor);
// 	// 	printf("loop : %d type : %d Data : %p frame : %u \n",game.music_array[1].looping
// 	// 	, game.music_array[1].ctxType, game.music_array[1].ctxData, game.music_array[1].frameCount);
// 	// printf("buffer : %p processor : %p\n", game.music_array[1].stream.buffer, game.music_array[1].stream.processor);
// 	// fflush(stdout);
// 	mlx_loop_hook(game.mlx_ptr, loupouk, &game);
// 	mlx_loop(game.mlx_ptr);
// 	CloseAudioDevice();
	
// }