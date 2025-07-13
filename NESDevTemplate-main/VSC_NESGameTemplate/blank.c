/*	simple template, for cc65, for NES
 *  writing to the screen with rendering disabled
 *	using nesdoug
 *	Doug Fraker 2018
 *  and NESDoug's version of neslib
 *  Modded by Dr Mike Reddy 2024
 */	
 
 
 
#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 
#include "level1.h"

#define BLACK 0x0f
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
// there's some oddities in the palette code, black must be 0x0f, white must be 0x30

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
// all variables should be global for speed
// zeropage global is even faster

const unsigned char palette[] = {
BLACK, DK_GY, LT_GY, WHITE,
BLACK,0x18,0x28,0x38,
BLACK,0x05,0x16,0x27,
BLACK,0x0B,0x1A,0x29
}; 

const unsigned char sprite_palette[] = {
BLACK, DK_GY, LT_GY, WHITE,
BLACK,0x18,0x28,0x38,
BLACK,0x05,0x16,0x27,
BLACK,0x0B,0x1A,0x29
}; 

const char player_sprite[]=
{
	0, 0, 0x02, 1,
	0, 8, 0x03, 1,
	8, 0, 0x12, 1,
	8, 8, 0x13, 1,
	128
};

struct sprite_data {
	unsigned char x;
	unsigned char y;
	unsigned char height;
	unsigned char width;
};

struct sprite_data player_data = {24, 192, 16, 16};

void main (void) {
	
	ppu_off(); // screen off

	pal_bg(palette); //	load the BG palette

	bank_spr(0);

	bank_bg(1);

	pal_spr(sprite_palette);

	vram_adr(NAMETABLE_A);

	vram_write(level1, 1024);

	ppu_on_all(); //	turn on screen
	
	while (1){
		// infinite loop
		
		oam_meta_spr(player_data.x, player_data.y, player_sprite );
		
	}
}
	
	