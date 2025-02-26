<h1 align=center>💻 Cub3D</h1>
<p align="center">
  <img src="img/cub3d.png?raw=true"/>
</p>

<span style="color: #f00000">We strongly recommend you to try *Le dernier voyage* game with bonuses.</span>

## About

> This project is inspired by the world-famous Wolfenstein 3D game, which
> was the first FPS ever. It will enable you to explore ray-casting. Your goal will be to
> make a dynamic view inside a maze, in which you’ll have to find your way

[Subject Cub3D Project 42](cub3d.pdf)

Create a 3D game using ray-casting and your creativity to generate a compelling visual experience.

<details>
<summary style="font-size: x-large; font-weight: bold"> Setup and Usage</summary>

**Clone the Repository**:
```bash
git clone https://github.com/QuentinThierry/Cub3D.git;
cd Cub3D
```

### Mandatory Part
```bash
cd Cub3D;
make
```

### Bonus Part
```bash
cd Cub3D_bonus;
make
```

## Usage
### Mandatory Part
```bash
./cub3D maps/valid.cub
```

### Bonus Part
```bash
./le_dernier_voyage maps/le_dernier_voyage.cub
```
1. Run the program with a map file as the first argument. The map file must have a `.cub` extension.
2. A game window will open.
3. Use `Esc` or the top-right cross to exit the game.
4. Use the options menu to configure key bindings.

- Controls:
  - **a/w/s/d** to move.
  - **left/right arrows** or **mouse** to rotate the view.
  - **p** to pause the game.
  - **Space** to open doors.
  - **Left-click** to interact with objects and locked doors.
  - Walk over items on the floor to pick them up.
  - **Left-shift** to sprint
  - **=/-** to zoom in/out on the minimap

</details>

---------

## Overview of *Le dernier voyage*'s map

#### 1. General
- The game has been designed in full ```raycasting```.
- A particular attention has been given to ```optimization``` with clever C stack/heap utilization.
- Involved ```maths``` behind the walls, floor/ceiling, objects and blurs have be recreated for the project.
- All the sprites have been ```hand-made``` based on raw images.
- ```Audio dialogs``` were recorded by ourselves.
- The map is ```fully customizable``` and anyone can create one (see [this](#how-to-create-a-map) section)

#### 2. The menu/keybinds

Keys can be totally remapped, except for ``Esc`` that will always quit the game. 

![image](https://github.com/user-attachments/assets/64939f87-e05f-4ef3-b2ae-1c13d9174302)

#### 3. Walls, floor and ceiling

Walls, floor and ceiling are drawn with complex raycating mathematics, e.g trigonometry, arctans.

![image](https://github.com/user-attachments/assets/f70992a1-d76a-4733-8dc8-4d3c0620cc4c)

#### 4. Doors

Door's calculation are handly made, they are double doors and can be openned and closed at any time if possible; There is two types of doors, locked and unlocked ones. Locked one can not be close after being openned with an object interaction.

![image](https://github.com/user-attachments/assets/870a3700-d53a-410b-a2db-f894d27f3741)

#### 5. Objects and interactions

Objects will always face the player and are drawn after the raycasting is done. They support simple transparency and can be interacted with on a mouse click.
Come of them can be pick up, others can receive objects to unlock the ending.

![image](https://github.com/user-attachments/assets/08fcf7b0-0eb7-456b-ba03-9c708b5180f6)

#### 6. Story and subtitles

There is a story behind the game, based on memory and life of a man. The story has been audio recorded and written in french. Subtitles can be found on s creen when a objects, fdoor, or displacement triggers one.

![image](https://github.com/user-attachments/assets/ac0dbbde-2221-4ee6-b498-0904631db93a)


#### 7. Animated sprites



## How to create a map
<details>
<summary style="font-size: large;">Unroll map rules</summary>

### 1.Mandatory part of the map:

#### Legend/Header Requirements:
 - **NO** <.xpm file's name>      define the wall's north texture
 - **SO** <.xpm file's name>      define the wall's south texture
 - **EA** <.xpm file's name>      define the wall's east texture
 - **WE** <.xpm file's name>      define the wall's west texture
 - **C** <color R,G,B>           define the ceiling's color
 - **F** <color R,G,B>           define the floor's color

**R, G, B** needs to be between 0 and 255.

#### In the map:
 - **1** is a wall
 - **0** is an empty cell
 - To define the player :
   - **N** the player is facing north
   - **S** the player is facing south
   - **E** the player is facing east
   - **W** the player is facing west

:warning: Important :  
- The spaces are a valid part of the map.  
- There is only one player by map.   
- The map must be closed/surrounded by walls.  
- The map's name ends by **".cub"**  

Example :
```
NO ./path_to_the_north_texture.xpm
SO ./path_to_the_south_texture.xpm
WE ./path_to_the_west_texture.xpm
EA ./path_to_the_east_texture.xpm
F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000001100000010001
10000000000000001101010010001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111
```

### 2.Bonus part of the map:

#### 2.1.Items required in the map

##### In the legend/header:
 - **NO** <.xpm file's name> define the wall's north texture
 - **SO** <.xpm file's name> define the wall's south texture
 - **EA** <.xpm file's name> define the wall's east texture
 - **WE** <.xpm file's name> define the wall's west texture
 - **C** <.xpm file's name> define the ceiling
 - **F** <.xpm file's name> define the floor

Between the legend and the map you need to write **"MAP"** on a new line.

##### In the map:
 - **1** is a wall
 - **0** is an empty cell
 - To define the player :
   - **N** the player is facing north
   - **S** the player is facing south
   - **E** the player is facing east
   - **W** the player is facing west

:warning: Important :  
- The spaces are a valid part of the map.
- There is only one player by map.  
- The map must be closed/surrounded by walls.
- The map's name ends by **".cub"**

#### 2.2.Other items that can be add

:warning: You can't use the letters **1, 0, N, S, E, W** to add new items on the map.  
:warning: You can't use the same letter mutliple time for the same item.  
:warning: A letter can't define a wall and a floor or a ceiling at the same time.  


- Wall type: **\<letter in ascii to insert in the map>** <.xpm file's name for the 4 orientations>
- Wall type: **N_\<letter in ascii to insert in the map>** <.xpm file's name for the north texture>
- Wall type: **S_\<letter in ascii to insert in the map>** <.xpm file's name for the south texture>
- Wall type: **E_\<letter in ascii to insert in the map>** <.xpm file's name for the east texture>
- Wall type: **W_\<letter in ascii to insert in the map>** <.xpm file's name for the west texture>
- Ceiling: **C_\<letter in ascii to insert in the map>** <.xpm file's name for the ceiling>
- Floor: **F_\<letter in ascii to insert in the map>** <.xpm file's name for the floor>

##### **Door**
:warning: The door can't lead outside of the map.  
:warning: The door needs to have one wall on each side.  
:white_check_mark: You can add floor or ceiling texture to the door.

- Door : **D_\<letter in ascii to insert in the map>** <.xpm file's name>
- Lock door : **D_\<letter of the wanted object>_\<letter in ascii to insert in the map>** <.xpm file's name door before the interaction> <.xpm file's name door after the interaction>
- Exit door for the game's end : **T_\<letter in ascii to insert in the map>** <.xpm file's name>
  - :warning: There is only one exit on the map

##### **Object**

:white_check_mark: You can add floor or ceiling texture to the object.

- Object with collision : **OW_\<letter in ascii to insert in the map>** <.xpm file's name>
- Object interactive with collision : **OI_\<letter in ascii to insert in the map>** <.xpm file's name object on the floor> <.xpm file's name object in the hand> <.xpm file's name object before the interaction> <.xpm file's name object after the interaction>
- Object without collision : **OE_\<letter in ascii to insert in the map>** <.xpm file's name>
- Object interactive without collision : **OI_\<letter in ascii to insert in the map>** <.xpm file's name object on the floor> <.xpm file's name object in the hand>
- Receptacle : **R_\<letter of the wanted object>_\<letter in ascii to insert in the map>** <.xpm file's name receptacle before the interaction> <.xpm file's name receptacle after the interaction>

##### **Narrator**

- Narrator on a cell or door : **H_\<letter in ascii to insert in the map>** <.wav or .mp3 file's name> <.txt file's name for subitle>
- Narrator on a receptacle : **HR_\<letter in ascii to insert in the map>** <.wav or .mp3 file's name for empty receptacle> <.txt file's name for subitle for empty receptacle> <.wav or .mp3 file's name when the receptacle is completed> <.txt file's name for subitle when the receptacle is completed>

:warning: In the subtitle file, a new line will indicate the separation between two subtitle's blocks.

##### **Music**

- Music on a cell or door : **M_\<letter in ascii to insert in the map>** <.wav or .mp3 file's name>
- Music on an object : **MO_\<letter in ascii to insert in the map>** <.wav or .mp3 file's name>
- Music on a receptacle : **MR_\<letter in ascii to insert in the map>** <.wav or .mp3 file's name for empty receptacle> <.wav or .mp3 file's name when the receptacle is completed>

##### **Animation**

- Instead of giving a file .xpm you can give a directory, where there is an other directory inside, which contains all the animation frame and a file config.cfg.
- The frames need to be sorted by alphabetical order.
- The config file contains the time between each frames and on a new line the time between each animation in milisecond.

##### **Random Texture**

- Instead of giving a file .xpm you can give a directory, where there is all the random textures or animations you want.
- For the animation inside this directory, you give a directory, which contains all the animation frame and a file config.cfg.

Example :
```
NO ./path_to_the_north_texture.xpm
SO ./path_to_the_south_texture.xpm
WE ./path_to_the_west_texture.xpm
EA ./path_to_the_east_texture.xpm
F ./path_to_the_floor_texture.xpm
C ./path_to_the_ceiling_animation_directory/

T_a ./path_to_the_end_door_texture.xpm
MR_a ./path_to_the_empty_receptacle_music.wav ./path_to_the_full_receptacle_music.mp3

D_i ./path_to_the_door_texture.xpm

D_c_b ./path_to_the_lock_door_texture.xpm ./path_to_the_open_door_texture.xpm
OI_c ./path_to_the_ojbect_on_the_floor_texture.xpm ./path_to_the_object_in_the_hand_texture.xpm
MO_c ./path_to_the_object_music.wav
F_c ./path_to_the_floor_texture.xpm
C_c ./path_to_the_ceiling_texture.xpm

M_d ./path_to_the_cell_music.wav

OI_e ./path_to_the_ojbect_on_the_floor_texture.xpm ./path_to_the_object_in_the_hand_texture.xpm
MO_e ./path_to_the_object_music.wav
R_e_f ./path_to_the_empty_receptacle_texture.xpm ./path_to_the_full_recepacle_texture.xpm
MR_f ./path_to_the_empty_receptacle_music.wav ./path_to_the_full_receptacle_music.mp3

OI_g ./path_to_the_ojbect_on_the_floor_texture.xpm ./path_to_the_object_in_the_hand_texture.xpm ./path_to_the_object_before_interaction_texture.xpm ./path_to_the_object_after_interaction_texture.xpm
H_g ./path_to_the_object_narrator.wav ./path_to_the_subtitle_narrator.txt
R_g_h ./path_to_the_empty_receptacle_texture.xpm ./path_to_the_full_recepacle_texture.xpm
HR_h ./path_to_the_empty_receptacle_narrator.wav ./path_to_the_subtitle_narrator.txt ./path_to_the_full_receptacle_narrator.mp3 ./path_to_the_subtitle_narrator.txt

OW_j ./path_to_the_colliding_object_texture.xpm
OE_k ./path_to_the_non_colliding_object_texture.xpm

l ./path_to_the_texture.xpm
N_l ./path_to_the_north_texture.xpm

F_m ./path_to_the_floor_texture.xpm
C_m ./path_to_the_ceiling_texture.xpm

MAP
11111111111111111llllllllll
1000000i000000000lmmmmmmmml
100000010000j0000immmmkmmml
10000001000000000lmmmmmmmml
11111111111a11i11llllilllll
11111000d000000001000000001
1100b000000000000i000000001
1000100c0000000e01000000001
110110h0000g000f01000000001
11i11111i1111111111111111i1
10000001000000000i000000001
100000010000000001000000001
1000000i0000000001000000001
111111111111111111111111111
```
