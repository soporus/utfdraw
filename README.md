![](https://i.imgur.com/Y7Pt4GO.png)

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
`wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh <version number>`

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
|**movement**  | w a s d, h j k l, arrows|
|**move** & **draw**  | W A S D, H J K L, left mouse|
|**line to right edge** | END|
|**line to left edge** | HOME|
|**line to top edge** | PG UP|
|**line to bot edge** | PG DOWN|

### brush
| block | key |
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
| **next left** | q or Q |
| **next right** | e or E |

### color
| action | color | key |
|--------|-------|-----|
| **sub** | *red* | r |
| **sub** | *green* | g |
| **sub** | *blue* | b |
| **add** | *red* | R |
| **add** | *green* | G |
| **add** | *blue* | B |

### file
| action | key |
|------------|---|
| **open** | o |
| **save** | p |
