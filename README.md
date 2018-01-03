### Lambda8

Lambda8 is a Fantasy computer that uses Lisp as its system language.

It's implemented in C using SDL 2.0.

A demo "game" is included.

This software is alpha state. In particular, loading files from disk is mostly a developing stage functionality.

### The machine
The screen has a resolution of 256x128, with 4 bits of depth, rendered at 768x384 (3x). Colours are chosen from a 16-colour palette.

These are the available primitives in Lisp:

- (pix x y c)

  Renders a pixel at coordinates x,y in color c


- (define-sprite filename)

  Loads a sprite into memory from a file on disk. Returns a number which identifies the texture.


- (spr id x y w h)

  Renders the sprite identified by id at coordinates x,y with a size of w,h


- (line x y x2 y2 c)

  Draws a line from x,y to x2,y2 using color c


- (rect x y x2 y2 c)

  Draws a filled rectangle from x,y to x2,y2 using color c


- (rectb x y x2 y2 c)

  Draws a rectangle from x,y to x2,y2 using color c


- (circ x y r c)

  Draws a filled circle centered on x,y with radius r using color c


- (circb x y r c)

  Draws a circle centered on x,y with radius r using color c


- (cls n)

  Clears the screen using color n


- (define-sfx filename)

  Loads a sound effect into memory from a file on disk. Returns a number which identifies the sfx.


- (sfx id)

  Plays the sound effect identified by id.


- (printxy text x y c)

  Prints the text at x,y in color c


## Core library
Lambda8 packs a core library with the following functions (defined in Lisp):

- (btn n)

  Returns #t if button n is pressed


- (btnp n)

  Returns #t if button n is pressed but wasn't pressed the previous
  frame (prevents repetition)


- (filter p ls)

  Returns a new list with the elements that satisfy predicate p, in
  the same order


- (collideAABB a b)

  Returns #t if the bounding boxes of sprites a and b are overlapping


- (make-sprite filename-or-id x y w h)

  Creates a new sprite object. It can either load a new texture from a
  file, or use the ID of an already loaded texture.
