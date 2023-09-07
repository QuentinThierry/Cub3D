ADD : 
- [X] texture for the floor and ceiling
- [X] random for similar sprites for walls
- [X] organise texture
- [X] objects
- [ ] sounds (ambiance music)
- [ ] exit + enigmes
- [ ] random events (lightning, planks cracks, little girl)
- [ ] menu settings
- [ ] max view distance

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
- open doors and instantly turn around would not open the door for player
- protect text draw outside of the screen
- texture floor and ceiling beug

Minimap : 
- if screen is too small, do not draw minimap
- leaks

Door :
<!-- - texture door only print the half two time -->
<!-- - opti door open -->
<!-- - door parsing -->
<!-- - doors on side of map -->
