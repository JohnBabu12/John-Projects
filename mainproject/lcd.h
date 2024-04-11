#include"types.h"
void Cmd_lcd(u8 cmd);
void Str_lcd(u8 *ptr);
void Cmd_lcd(u8 cmd);
void Write_lcd(u8 dat);
void INIT_lcd(void);
void Char_lcd(u8 dat);
void Itoa_lcd(u32 dat);
void Itoan_lcd(s32 n);
void F32_lcd(f32 n,u8 ndp);
void Bin_lcd(u32 n,u8 nbd);
void Hex_lcd(u32 n);
void Build_CG_ram(u8*,u32 nbytes);
