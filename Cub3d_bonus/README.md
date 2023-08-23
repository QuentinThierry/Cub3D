ADD : 
- [ ] texture for the floor and ceiling
- [ ] random for similar sprites for walls
- [ ] organise texture
- [ ] objects not walls
- [ ] ennemy
- [ ] sounds (ambiance music)
- [ ] objects
- [ ] exit + enigmes
- [ ] random events (lightning, planks cracks, little girl)
- [ ] menu settings

Obligation :
- [x] minimap
- [x] animation (windows + lightning)
- [x] collision with wall
- [ ] door close and open
- [x] mouv with mouse

FIX :
<!-- - parsing, when search len of map need to suppr whitespace at the end -->
- line with only white space return error ?
<!-- - Need to read directory for animation -->
- Check if multiple definition of a texture incompatible
<!-- - stop parsing if not valid caracter on the map -->
- Segfault when no default sprites for symbol in map
- makefile
- object parsing
- security on exiting the map is broken

Minimap : 
- if screen is too small, do not draw minimap

Door :
<!-- - texture door -->
<!-- - opti door open -->
- door parsing
- doors on side of map