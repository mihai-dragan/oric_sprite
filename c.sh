cc65  -Ors --codesize 500 -T -g -t atmos sprite.c
ca65 sprite.s
ld65 -o sprite -t atmos -m sprite.map sprite.o atmos.lib
