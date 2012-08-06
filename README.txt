Chocozuma's Revenge
Created for TINS 2012 By Evan Wallace, Kieren Wallace, Anna Besley and Luke Besley.

Controls:
Mouse: Control View
WASD: Control dragon
ENTER: Enter cave, the game is saved when you enter the cave, so you can resume later from the main menu.

Compilation:
  Prerequisites:
    C++ compiler
    Allegro5 with the ogg and png codecs installed
    The game was made and tested with Clang (Xcode 4.4 version)
        on OSX 10.7 with Allegro 5.0.7.
    Need 
      liballegro
      liballegro_primitives
      liballegro_main
      liballegro_image
      liballegro_font
      liballegro_audio
      liballegro_acodec

  Detailed Steps:
    - Put (links to) the allegro libraries in `ext/allegro/lib`
    - Put (links to) the allegro headers in ext/allegro/include
    - If you are running OSX, there is an XCode project supplied in
      `ide/mac/xcode` which should correctly compile the game.
    - Otherwise, you are on your own.
    - You need to compile every .cpp file
    - Link them .cpp files together with the given allegro libraries
    - When you run the game, current working directory must contain every
      file from `res/`

Prebuilt binary:
   See `https://github.com/Mankarse/TINS2012Game`.
   I will update it with prebuilt binaries when I get the time.
   Please email me at onlyone@senet.com.au if your platform is not supported.