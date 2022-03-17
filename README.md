# oric_sprite
Draw sprites in HIRES for CC65

I was inspired by the ["Efficient rasterization"](https://osdk.org/index.php?page=articles&ref=ART19) article on OSDK.org, to try and see if I can draw sprites fast using cc65 instead of OSDK.

Previously I attempted to use the cc65 Oric tgi libraries, but those are just calling the ROM routines and, as discussed in the above mentioned article, those are quite slow.

So, I removed the tgi code and just used assembler to go into HIRES mode with `asm("jsr $EC33");`. Later I found a bit of code int the [8bit Unity library](https://github.com/8bit-Dude/8bit-Unity) that initializes HIRES correctly for Oric 1 and Oric Atmos. You can find all the initializations in the `setup()` procedure.

Then I tried to just put characters from the character table to the HIRES screen, but found out that they are laid out differently, because the HIRES byte, although it contains just 6 pixels like the character fonts, they have a set flag on the second bit, so we have to also set this flag on each byte of the character table by adding binary 01000000 (or decimal 64).
For more information of how the HIRES screen memory is organized and how each byte stores information about the draw mode and for six pixels OR color attributes, look [here](https://osdk.org/index.php?page=articles&ref=ART9).

    void draw_char(byte cnum, int haddr) {
        byte i;
        byte offst = cnum<<3;
        for(i=0;i<8;i++) {
            int j = (i<<5)+(i<<3);
            byte* addr = (byte*)haddr+j;
            *addr = 64+chr[offst+i];
        }
    }

`haddr` is the address of the location in HIRES memory (starting at #A000) where the character is going to be drawn. `cnum` is the character number, starting from the first drawable character from the character map. You can see the character map and the address of the Hires screen etc in the last figure showing the memory map [here](https://osdk.org/index.php?page=documentation&subpage=memorymap).

To get the address of the character we want to draw, we have to multiply the character number `cnum` with 8, because each character is made of 8 bytes: `byte offst = cnum<<3;`

Each byte forms a line of six pixels, but to draw the next line exactly under it we have to skip 40 bytes (the number of bytes on a screen line).

The shift `>>` or `<<` operations in the above code are probably an unnecessary optimisation for the multiplications needed (you can find more about this [here](https://www.geeksforgeeks.org/multiplication-two-numbers-shift-operator/)). I think the compiler would do this anyway, but just to be sure we do it because Oric's CPU is very slow at multiplying. LATER EDIT: I managed to replace multiplications with numbers that are not powers of two, replacing them with repeated addition in the for loop. Also checked the generated assembler code and indeed, multiplications with powers of two are replaced with shifts by the compiler.

The sprite drawing procedure is very similar to that for the characters, but we put the flag bit "on" in the sprite itself, so we don't need to set it in the procedure. Also the whole byte array is passed to the procedure, so we don't need to calculate the addres in memory for each line.

The rest of the code is quite easy to understand, I think, so I won't go into further details at the moment.
