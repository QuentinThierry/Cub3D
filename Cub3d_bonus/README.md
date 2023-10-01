ADD : 
- [X] texture for the floor and ceiling
- [X] random for similar sprites for walls
- [X] organise texture
- [X] objects
- [X] max view distance
- [X] sounds (ambiance music)
- [X] subtitle narrator
- [X] exit + enigmes
- [X] menu settings

Obligation :
- [x] minimap
- [x] animation (windows + lightning)
- [x] collision with wall
- [x] door close and open
- [x] mouv with mouse

FIX :
<!-- - parsing, when search len of map need to suppr whitespace at the end -->
<!-- - Need to read directory for animation -->
<!-- - stop parsing if not valid caracter on the map -->
<!-- - Check if multiple definition of a texture incompatible -->
<!-- - Segfault when no default sprites for symbol in map -->
<!-- - makefile -->
<!-- - security on exiting the map is broken -->
<!-- - object parsing -->
<!-- - floor is by default where player spawns -->
<!-- - quit if WIN_X or WIN_Y is <= 0 -->
<!-- - check texture obligatoire (NO, SO, WE, EA) -->
<!-- - collision with hit box -->
<!-- - increase wall height -->
<!-- - texture floor and ceiling beug -->
<!-- - animation for object -->
<!-- - open doors and instantly turn around would not open the door for player -->
<!-- - collision vibration when hit object -->
<!-- - si la duree des animation est de 0ms ?? -->
<!-- - on est dans le mur quand on ferme une porte de trop pres -->
<!-- - draw hand when window is too small segfault -->
- protect text draw outside of the screen
- file structure for bonus and mandatory
- white space at the end of a texture printf error because file not found

Minimap : 
<!-- - if screen is too small, do not draw minimap -->
<!-- - leaks -->

Menu:
- when key is already used, several click on already used key would be opaque
- exit button ugly
- LEAKS


Door :
<!-- - show when a door is open -->
<!-- - open with clic of the mouse -->
<!-- - texture door only print the half two time -->
<!-- - opti door open -->
<!-- - door parsing -->
<!-- - doors on side of map -->
- time for door usefull ?? for animation or door open when do I need to update it ???
- fix door against the wall parsing

Objet interactif :
<!-- - take objet on walk -->
<!-- -- take objet with click -->
<!-- -- change image printf if object wall to after -->
<!-- - draw image hand -->
<!-- - drop objet ->draw objet alone on the floor ->become object none wall -->
<!-- - check receptacle if correct change image for receptacle completed -->
<!-- - increase door exit loading bar -->
<!-- - bug for calcutating the distance to take on click or drop the object -->
<!-- - propect only one exit door -->
<!-- - fix can't take object on our case -->
<!-- - no random texture for the exit at the beginning and no animation -->
<!-- - if 0 receptacle door is lock PROBLEM -->

Exit door :
<!-- - desactive the mouv of the player -->
<!-- - stop the player -->
<!-- - mouv the player behind to door on the center of the cell with view angle on the door -->
<!-- - anim the door open with light -->
<!-- - walk under the door -->
<!-- - the end screen -->
- beug door stay open against the wall

Sound :
<!-- - play sound when walk on an object with already something in the hand -->
<!-- - ground lose sound if drop object on a case -->
<!-- - take an object while a narrator is played would skip the next narrator -->
<!-- - same for unlock door ... -->
<!-- - need check sound if receptacle or no to get the next texture -->
<!-- - music during the exit door ?? -->


