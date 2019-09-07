# Chaos Fractal Generator

This is a fractal generator which can generate many different patterns given different patterns. The idea behind this is that the fractals are generated randomly and dynamically using Chaos Game (https://en.wikipedia.org/wiki/Chaos_game). (Also some of the parameters I used are referenced from http://math.bu.edu/DYSYS/chaos-game/chaos-game.html).

The concept is that starting from a random point on the grid and randomly choosing a corner of an n-sided shape, we move towards it by a factor (e.g. 1/2 = move halfway to the corner) and plot a point at the result. Then we repeat the selection of a corner and plot a new point and so on. Eventually, after many iterations we get a plot of a fractal pattern based on the "attractor" based on the parameters given.

Of course we could vary the different parameters to result in widely different fractals. One of the main concepts is the use of "restrictions" to control the next pixel's position and as a result the fractal as a whole.

More details about the parameters and examples are below.

## Parameters

**Usage:**
```./chaos [-c corners] [-f magfactor] [-s scale] [-i iterations] [-r a[,b,...,n]] [-a] [(-j delimitedset | -n delimitedset) [-x]] [-h]```

### Options: 

```-c corners``` Number of corners (i.e 3 = triangle)

```-f magfactor``` Sets magnification factor (i.e. Distance to move towards corner to get next point, 1/2 = move half the distance to corner)

```-s scale``` Amount to scale final image (i.e 2 = image is 2x size)

```-i iterations``` Number of pixels to generate image

```-g``` Draw lines between corners in red

```-r a[,b,...,n]``` Set angle of rotation of corners. Each corner rotation can be set individually by seperating with ',' Must be equal to number of corners in this case

```-a``` Set restriction that the same corner cannot be selected twice in a row

```-j delimitedset``` Restricts corners if within specified distance (both clockwise and anticlockwise) to previously selected corners

Parameter is in the format ```a,b:c:d,e,f,g``` with ```','``` and ```':'``` as delimiters where:

a and b are the restrictions for the previously selected corner ```c``` is the restricton for the 2nd previously selected corner ```d,e,f``` and ```g``` are the restrictions for the 3rd previously selected corner

Each restriction is delimited by ```','``` More restrictions on earlier corners are added by delimiting with ```':'``` as appropriate To set no restriction for a corner use ```'-'```

**Note:** Mutually exclusive with ```-n``` paramter below

**Example:** ```-j 1,2:-:0:1```

Means the next corner selected cannot be 1 or 2 distance away from the 1st previous corner AND No restrictions on the 2nd previous corner AND No repeat of 3rd previous corner AND 1 distance away from the 4th previous corner


```-n delimitedset``` Similar as ```-j``` parameter but only applies to clockwise distance only (not mirrored)

**Note:** Mutually exclusive with ```-j``` parameter above

**Example:** ```-n 1,4:2```

Means the next corner selected cannot be 1 or 4 distance away clockwise of the 1st previous corner AND 2 distance away clockwise of the 2nd previous corner

```-x``` Used with ```-j``` or ```-n``` parameters to change the AND conditions mentioned above to XOR (exclusive or) Must be used with ```-j``` or ```-n```

```-h``` Open this help menu

## Examples 

(Sierpinski Triangle) ```./chaos -g```

(Inverse of Sierpinski Triangle) ```./chaos -r 180 -g```

```./chaos -r 180,0,0```

(Moth) ```./chaos -c 4 -n 0:2 -x -r 180,0,0,0 -f 0.55```

(Sierpinski Pentagon) ```./chaos -c 5 -f 3/8 -g```

```./chaos -c 5 -j -:2```

```./chaos -c 6 -j -:2```

(Star of Pentagons) ```./chaos -c 5 -j 1 -g```

```./chaos -c 4 -j 2```

(Dragon Curve) ```./chaos -c 4 -n 2 -r 45```

```./chaos -c 6 -j 1:2 -x```

```./chaos -c 6 -j 1```

```./chaos -c 6 -j 3 -a```

(Whirlpool) ```./chaos -c 7 -n 1:0,3:4```

```./chaos -c 7 -n 1,2:0,2:6```

(7-pointed Star) ```./chaos -c 7 -j 1,2```

```./chaos -c 7 -j 1,3:1```

```./chaos -c 7 -a -n 2:1,3 -r 25```

(Pentagon Star) ```./chaos -c 5 -n 1,4:1,4 -x -i 500000```

```./chaos -c 9 -j 1,2:1:1:1:1```

```./chaos -c 7 -j 1,2:1,2 -x -i 700000```

```./chaos -c 8 -j 1,2:2:1```

```./chaos -c 8 -j 1:1:2```

```./chaos -c 6 -n 2:1:1 -r 15```
