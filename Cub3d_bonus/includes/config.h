/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 13:58:02 by qthierry          #+#    #+#             */
/*   Updated: 2023/12/04 14:17:39 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

// ----------- WINDOW -------------
# define WIN_X 1280
# define WIN_Y 720

// ----------- PLAYER -----------
# define SPEED 1
# define SPRINT_BOOST 1
# define DFL_FOV 100 // check min is less than min and diff > 0
# define MIN_FOV 70
# define MAX_FOV 130
# define ROTATION_KEYBOARD 125
# define ROTATION_MOUSE 20

// ----------- SOUND -----------
# define DFL_SOUND 0.5 // check between 0 and 1
# define NB_MAX_SOUNDS 16
# define BACKGROUND_MUSIC "./assets/sounds/void.wav"

// ----------- MENU -----------
# define MAX_MENU_FPS 120
# define XPM_DFL_BUTTON "./assets/xpm/mandatory/button.xpm"
# define XPM_HOV_BUTTON "./assets/xpm/mandatory/button_hovered.xpm"
# define XPM_EXIT_BUTTON "./assets/xpm/mandatory/button_exit_option.xpm"
# define XPM_HOR_SLIDER "./assets/xpm/mandatory/slider_hor.xpm"
# define XPM_VERT_SLIDER "./assets/xpm/mandatory/slider_vert.xpm"

// ----------- FOG -----------
# define FOG_COLOR 0x101010
# define DIST_MAX_FOG 15.
# define DIST_MIN_FOG 3.

// ----------- MINIMAP -----------
# define ZOOM_SPEED 50
# define MINIMAP_WALL_COLOR 0x505050
# define MINIMAP_BACKGROUND_COLOR 0x808080
# define MINIMAP_DOOROPEN_COLOR 0x707070
# define MINIMAP_DOORCLOSE_COLOR 0x656565
# define PATH_MMAP_PLAYER "../assets/xpm/mandatory/minimap_player.xpm"
# define MINIMAP_PLAYER_IMAGE "assets/xpm/mandatory/minimap_player.xpm"

// ----------- KEYBINDS -----------
# define DFL_KEY_LEFT_MOVE 'a'
# define DFL_KEY_RIGHT_MOVE 'd'
# define DFL_KEY_FORWARD_MOVE 'w'
# define DFL_KEY_BACKWARD_MOVE 's'
# define DFL_KEY_LEFT_LOOK 0xff51
# define DFL_KEY_RIGHT_LOOK 0xff53
# define DFL_KEY_PAUSE 'p'
# define DFL_KEY_MINIMAP_ZOOM '='
# define DFL_KEY_MINIMAP_DEZOOM '-'
# define DFL_KEY_INTERACT_DOOR ' '
# define DFL_KEY_SPRINT 0xffe1

// ----------- LOADING_SCREEN -----------
# define LOADING_SCREEN "./assets/xpm/test/smiley.xpm"

// ----------- MISCELLANEOUS -----------
# define SPEEP_DOOR_OPENING 100
# define SPEEP_UNLOCK_DOOR_OPENING 50

#endif