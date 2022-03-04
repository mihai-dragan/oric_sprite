#include <time.h>
#include <conio.h>
#include <string.h>
#include <atmos.h>

#define byte unsigned char

void setup() {
    byte* addr;
    byte value = *(char*)0xC800;
    if(value)
            asm("jsr $EC33");       // HIRES in Atmos (ROM 1.1)
    else
            asm("jsr $E9BB");       // HIRES in Oric-1 (ROM 1.0)
    memset((char*)0xBF68, 0, 120);  // Clear lower text area
    addr = (byte*)0x24E;        //
    *addr = 5;                  // set keyb delay at #24E
    addr = (byte*)0x26A;        //
    *addr = 11;                 // disable keyb click with bitflag at #26A
}

byte* chr = (byte*)0x9900;  // pointer to ASCII characters
byte* achr = (byte*)0x9d00; // pointer to alternative characters

byte alien_w1[8] = { 82,76,94,109,127,76,82,82 };
byte alien_w2[8] = { 82,76,94,109,127,76,82,97 };

void draw_spr(byte spr[8], int haddr) {
    byte i;
    for(i=0;i<8;i++) {
        int j = (i<<5)+(i<<3);
        byte* addr = (byte*)haddr+j;
        *addr = spr[i];
    }
}

void draw_char(byte cnum, int haddr) {
    byte i;
    byte offst = cnum<<3;
    for(i=0;i<8;i++) {
        int j = (i<<5)+(i<<3);
        byte* addr = (byte*)haddr+j;
        *addr = 64+chr[offst+i];
    }
}

void draw_achar(byte cnum, int haddr) {
    byte i;
    byte offst = cnum<<3;
    for(i=0;i<8;i++) {
        int j = (i<<5)+(i<<3);
        byte* addr = (byte*)haddr+j;
        *addr = 64+achr[offst+i];
    }
}

void main() {
    byte j,i = 0;
    byte toggle = 0;
    clock_t goal;
    byte c;
    setup();
    while(1) {
        draw_achar(j,0xa000 + 20);
        draw_char(j,0xa000+22);
        if(j<128) j++;
        else j=0;
        if(toggle==0) {
            toggle=1;
            for(i=0;i<10;i++) {
                draw_spr(alien_w1,0xa000+i*2);
            }
        } else {
            toggle=0;
            for(i=0;i<10;i++) {
                draw_spr(alien_w2,0xa000+i*2);
            }
        }
        
        goal = clock () + CLOCKS_PER_SEC/20;
        while ((long) (goal - clock ()) > 0) ;
        
        if(kbhit()>0) {
            c = cgetc();
            if(c==CH_ESC) return;
            /*if(c==CH_CURS_UP) move_up();
            if(c==CH_CURS_DOWN) move_down();
            if(c==CH_CURS_LEFT) move_left();
            if(c==CH_CURS_RIGHT) move_right();*/
        }
    }
}
