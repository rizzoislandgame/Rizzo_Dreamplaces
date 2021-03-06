
#ifndef PALLETE_H
#define PALLETE_H

extern unsigned int palette_complete[256];
extern unsigned int palette_font[256];
extern unsigned int palette_alpha[256];
extern unsigned int palette_nocolormap[256];
extern unsigned int palette_nocolormapnofullbrights[256];
extern unsigned int palette_nofullbrights[256];
extern unsigned int palette_onlyfullbrights[256];
extern unsigned int palette_pantsaswhite[256];
extern unsigned int palette_shirtaswhite[256];
extern unsigned int palette_transparent[256];

// used by hardware gamma functions in vid_* files
void BuildGammaTable8(float prescale, float gamma, float scale, float base, unsigned char *out, int rampsize);
void BuildGammaTable16(float prescale, float gamma, float scale, float base, unsigned short *out, int rampsize);

void Palette_Init(void);

#endif

