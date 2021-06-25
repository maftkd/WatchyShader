# WatchyShader
A Watchy subclass with graphics methods for shading and rendering
The source in this repo is meant to be more than just a watch face.
The goal is also to provide developers with functionality to create
their own procedural graphical backgrounds.

![alt text](images/cubeam.png?raw=true)
## AM
![alt text](images/cubepm.png?raw=true)
## PM

## On Dithering:
Since watchy's e-paper display only supports on/off pixels we need
to be a little more creative to achieve gradients, or shades of grey.
Dithering to the rescue. Dithering is how old school video games
could create sunsets with a color palette of 8. The basic principle
is to alternate the pixels so that from a far we see an average.

There are a few different approaches to dithering:
* Image effect - the image is processed in whole from greyscale 
to black/white (i.e. Atkinson algorithm)
* Procedural - using a function to dither on-demand. For example
if(x+y)%intensity==0
* Temporal - flickering pixels on and off so fast, the user cannot see.
* Probability-based - A pixel is drawn using it's "intensity" value
as a step function in a PRNG (like the Lehmer Generator)

At the present, I've found the probability-based to be the easiest to
play with and get going, but in the future I plan to explore other
algorithms.

## Current Features:
* Gradient background
* Clock changes color based on AM / PM
* Perspective transformation and MVP
* Shaded triangle rendering
* Simple animation of model matrix over time

## Upcoming Features:
* 3d model file loading
* 3d clock text
* Alternate dithering algorithms
* Image effects

