rm *.map
rm *.s
rm *.o

#cc65  -Ors --codesize 500 -T -g -t atmos sprite.c
cc65 -Cl -Ois --codesize 500 -t atmos sprite.c
ca65 sprite.s
ld65 -o sprite.tap -t atmos -m sprite.map sprite.o atmos.lib
