/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:16:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/09/10 16:11:27 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d_bonus.h"

bool	init_audio(t_game *game)
{
	InitAudioDevice();
	if (!IsAudioDeviceReady() || !game)
		return (false);
	
	return (true);
}

void	close_sound(t_game *game)
{
	if (!IsAudioDeviceReady())
		return ;
	
	CloseAudioDevice();
}

// void	load_new_sound_in_array(t_music *music_array, const char *audio_file);

// int	audio_hook(int key, t_game *game)
// {
// 	printf("key %d\n", key);
// 	if (key == 'a')
// 		load_new_sound_in_array(game->music_array, "assets/sounds/app.wav");
// 	else if (key == 's')
// 		load_new_sound_in_array(game->music_array, "assets/sounds/sound.wav");
// 	return (0);
// }

// bool	init_audio(t_game *game)
// {
// 	InitAudioDevice();
// 	if (!IsAudioDeviceReady() || NB_MAX_SOUNDS <= 0)
// 		return (printf("Audio init failed."), false);
// 	game->music_array = ft_calloc(NB_MAX_SOUNDS, sizeof(t_music));
// 	if (!game->music_array)
// 		return (false);
// 	return (true);
// }

// void	close_audio(t_game *game)
// {
// 	if (!IsAudioDeviceReady())
// 		return ;
// 	CloseAudioDevice();
// }

// void	load_new_sound_in_array(t_music *music_array, const char *audio_file)
// {
// 	int	i;

// 	i = 0;
// 	while (i < NB_MAX_SOUNDS)
// 	{
// 		if (!music_array[i].ctxData)
// 		{
// 			music_array[i] = LoadMusicStream(audio_file);
// 			SetMusicPan(music_array[i], 0.1f);
// 			if (!music_array[i].ctxData)
// 				printf("Error creating audio stream.\n");
// 			else
// 			{
// 				music_array[i].looping = false;
// 				PlayMusicStream(music_array[i]);
// 			}
// 			return ;
// 		}
// 		i++;
// 	}
// }

// void	update_playing_sounds(t_music *music_array)
// {
// 	int	i;

// 	i = 0;
// 	while (i < NB_MAX_SOUNDS)
// 	{
// 		if (music_array[i].ctxData)
// 			UpdateMusicStream(music_array[i]);
// 		i++;
// 	}
// }

// int loupouk(t_game *game)
// {
// 	update_playing_sounds(game->music_array);
// 	return (0);
// }

// int main()
// {
// 	t_game	game;
// 	int	i = 1;

// 	game = (t_game){0};

// 	init_audio(&game);
// 	init_mlx(&game);

// 	SetMasterVolume(1.f);

// 	mlx_loop_hook(game.mlx_ptr, loupouk, &game);
// 	mlx_hook(game.win, 2, (1L<<0), audio_hook, &game);
// 	mlx_loop(game.mlx_ptr);
// 	CloseAudioDevice();
	
// }