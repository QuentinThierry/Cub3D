/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:16:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/24 15:21:16 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	init_audio(t_game *game, t_music_name *music_file, int nb_music)
{
	int	i;
	
	InitAudioDevice();
	if (!IsAudioDeviceReady() || NB_MAX_SOUNDS <= 0)
		return (printf("Error : Audio init failed\n"), false);
	game->music_array = ft_calloc(NB_MAX_SOUNDS, sizeof(t_music));
	if (!game->music_array)
		return (false);
	i = 0;
	while (i < nb_music)
	{
		if (music_file[i].subtitle == NULL)
		{
			music_file[i].sound = LoadSound(music_file[i].filename);
			if (!IsSoundReady(music_file[i].sound))
				return (printf("Error : Sound init failed %s\n", music_file[i].filename), false);
		}
		i++;
	}
	game->music_array[0] = LoadMusicStream(BACKGROUND_MUSIC);
	if (!IsMusicReady(game->music_array[0]))
		return (printf("Error : Background music init failed\n"), false);
	return (true);
}

void	close_audio(t_game *game, t_music *music_tab, t_music_name *music_file, int nb_music)
{
	int	i;

	if (!IsAudioDeviceReady())
		return ;
	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (IsMusicStreamPlaying(music_tab[i]))
		{
			StopMusicStream(music_tab[i]);
			UnloadMusicStream(music_tab[i]);
		}
		i++;
	}
	i = 0;
	while (i < nb_music)
	{
		if (music_file[i].subtitle == NULL)
		{
			if (IsSoundPlaying(music_file[i].sound))
				StopSound(music_file[i].sound);
			UnloadSound(music_file[i].sound);
		}
		i++;
	}
	CloseAudioDevice();
}

void	update_sounds(t_music *music_array)
{
	int	i;

	i = 0;
	while (i < NB_MAX_SOUNDS)
	{
		if (IsMusicStreamPlaying(music_array[i]))
			UpdateMusicStream(music_array[i]);
		i++;
	}
}

// int loupouk(t_game *game)
// {
// 	static struct timespec	last_time = {0};
// 	struct timespec			cur_time;
// 	long					fps;

	
// 	if (last_time.tv_sec == 0)
// 		clock_gettime(CLOCK_REALTIME, &last_time);
// 	usleep(30000);
// 	update_sounds(game->music_array);
// 	clock_gettime(CLOCK_REALTIME, &cur_time);
// 	game->delta_time = (cur_time.tv_sec - last_time.tv_sec
// 			+ (cur_time.tv_nsec - last_time.tv_nsec) / 1000000000.F);
// 	fps = (long)(1.0 / game->delta_time);
// 	tot_fps += fps;
// 	nb_fps++;
// 	if ((nb_fps % 50) == 0)
// 		printf("fps : %ld\n", fps);
// 	last_time = cur_time;
// 	return (0);
// }

// int main()
// {
// 	t_game	game;
// 	int	i = 1;
// 	Sound	sound;

// 	game = (t_game){0};

// 	init_audio(&game);
// 	init_mlx(&game);

// 	SetMasterVolume(1.f);
	
// 	game.music_array[0] = LoadMusicStream("assets/sounds/test.mp3");
// 	if (!IsMusicReady(game.music_array[0]))
// 		return (printf("error loading music\n"), 1);
	// PlayMusicStream(game.music_array[0]);

// 	game.music_array[1] = LoadMusicStream("assets/sounds/test1.mp3");
// 	if (!IsMusicReady(game.music_array[1]))
// 		return (printf("error loading music 1\n"), 1);
// 	game.music_array[1].looping = false;
// 	PlayMusicStream(game.music_array[1]);

	// struct timespec time;
	// struct timespec time1;
	// clock_gettime(CLOCK_REALTIME, &time);
	// sound = LoadSound("assets/sounds/test.mp3");
	// if (!IsSoundReady(sound))
	// 	return (printf("error loading sound1\n"), 1);
	// clock_gettime(CLOCK_REALTIME, &time1);
	// printf("time : %ld\n", time_to_long(&time1) - time_to_long(&time));
	// SetSoundPan(sound, 0),
	// PlaySound(sound);
	// if (!IsSoundPlaying(sound))
	// 	return (printf("error playing\n"), 1);
	// printf("coucou\n");
	// sleep(5);
	// SetSoundPan(sound, 1.0),
	// PlaySound(sound);
	// if (!IsSoundPlaying(sound))
	// 	return (printf("error playing 1\n"), 1);
// 	printf("coucou1\n");

// 	printf("loop : %d type : %d Data : %p frame : %u \n",game.music_array[0].looping
// 		, game.music_array[0].ctxType, game.music_array[0].ctxData, game.music_array[0].frameCount);
// 	printf("buffer : %p processor : %p\n", game.music_array[0].stream.buffer, game.music_array[0].stream.processor);
// 		printf("loop : %d type : %d Data : %p frame : %u \n",game.music_array[1].looping
// 		, game.music_array[1].ctxType, game.music_array[1].ctxData, game.music_array[1].frameCount);
// 	printf("buffer : %p processor : %p\n", game.music_array[1].stream.buffer, game.music_array[1].stream.processor);
// 	fflush(stdout);
// 	mlx_loop_hook(game.mlx_ptr, loupouk, &game);
	// mlx_hook(game.win, 2, (1L<<0), audio_hook, &game);
// 	mlx_loop(game.mlx_ptr);
// 	CloseAudioDevice();
	
// }