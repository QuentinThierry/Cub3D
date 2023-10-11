/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:31:22 by jvigny            #+#    #+#             */
/*   Updated: 2023/10/09 21:32:26 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

typedef enum e_orientation
{
	e_north = 0,
	e_east,
	e_south,
	e_west,
	e_floor,
	e_ceiling,
	e_wall,
	e_door,
	e_door_lock,
	e_door_unlock,
	e_object_wall,
	e_object_entity,
	e_object_interactive,
	e_object_interactive_hand,
	e_object_interactive_before,
	e_object_interactive_after,
	e_receptacle_empty,
	e_receptacle_full,
	e_exit,
	e_music,
	e_music_object,
	e_music_receptacle,
	e_music_receptacle_complete,
	e_narrator,
	e_narrator_receptacle,
	e_narrator_receptacle_complete,
	e_end_screen,
	e_object_image = e_north,
	e_door_image = e_north,
	e_object_interactive_image = e_north,
	e_object_interactive_hand_image,
	e_object_interactive_before_image,
	e_object_interactive_after_image,
	e_receptacle_empty_image = e_north,
	e_receptacle_full_image
}	t_orient;

enum e_keybinds
{
	e_key_left_move,
	e_key_right_move,
	e_key_forward_move,
	e_key_backward_move,
	e_key_left_look,
	e_key_right_look,
	e_key_pause,
	e_key_minimap_zoom,
	e_key_minimap_dezoom,
	e_key_interact_door,
	e_key_sprint
};

enum e_status
{
	e_game = -1,
	e_go_in_font_of_door = 0,
	e_open_door,
	e_walk_through_door,
	e_end
};

#endif