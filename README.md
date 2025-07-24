## utfdraw  - `basic terminal paint program`


**this is in very early stages and probably broken, unstable, etc.**

![](https://i.imgur.com/Y7Pt4GO.png)

*editor screenshot*


## setup
##### clone the repository
`git clone https://github.com/soporus/utfdraw.git`
##### grab the submodule for termbox2 dependency

`cd termbox2`

`git submodule init`

`git submodule update`

return to main directory

`cd ..`



## Compile

*tested with **clang 19**, though **gcc** should work.*

###### optional:

get latest LLVM/clang:

`wget https://apt.llvm.org/llvm.sh`

`chmod +x llvm.sh`

`sudo ./llvm.sh <version number>`


### Build utfdraw
compile with clang:

`make`

compile with gcc

`make main-gcc`

*or edit the makefile, of course*

run the executable
`./main` or `./main-gcc`



## controls

### draw and move

|action  | key|
|------------- | -------------|
|**movement**  | w a s d  |
|  | h j k l |
|  | arrows |
|  | mouse right |
|**move** & **draw**  | W A S D | 
| | H J K L |
| | mouse left|
|**line to right edge** | END|
|**line to left edge** | HOME|
|**line to top edge** | PG UP|
|**line to bot edge** | PG DOWN|

### brush
| char | key |
|-------|-----|
| **blank** | 0 |
| **░** | 1 |
| **▒** | 2 |
| **▓** | 3 |
| **█** | 4 |
| **▀** | 5 |
| **▔** | 6 |
| **▁** | 7 |
| **▄** | 8 |
| **◼** | 9 |
| **next left** | q *or* Q |
| **next right** | e *or* E |

### color
| action | color | key |
|--------|-------|-----|
| **sub** | *red* | r |
| **sub** | *green* | g |
| **sub** | *blue* | b |
| **add** | *red* | R |
| **add** | *green* | G |
| **add** | *blue* | B |
| **pick** | *pick color under cursor* | y or Y |

<span style="color: #c01070">
Color is displayed in the center of the screen as a 6 digit hex value. A of pair digits represent red, green, and blue respectively.  Use 'r' 'g' 'b' keys in upper or lowercase to adjust each color.</span>

**#**<span style="color: #FF0000">Rr</span><span style="color: #00FF00">Gg</span><span style="color: #0000FF">Bb</span>

### file
| action | key |
|------------|---|
| **open** | ctrl o |
| **save** | ctrl p |

<span style="color: #c01070"> File functions are not fully implemented.  Currently there is a fixed file location, `drawing.utf.`  The program will open and/or write to that file only.  An example is provided in the root directory.</span>
