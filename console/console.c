#include "console.h"
#include "portmap.h"  

#define VGA_BUFFER ((char*)0xb8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define TAB_WIDTH 8

static void update_cursor();
static VGA_Color terminal_font_color = GRAY;
static VGA_Color terminal_background_color = BLACK;
static int cursor_position = 0;

void clear_terminal() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_BUFFER[i * 2] = ' ';
        VGA_BUFFER[i * 2 + 1] = (terminal_background_color << 4) | terminal_font_color;
    }
    cursor_position = 0;
    update_cursor();
}

void set_terminal_font_color(VGA_Color color) {
    terminal_font_color = color;
}

void set_terminal_background_color(VGA_Color color) {
    terminal_background_color = color;
}

static void update_cursor() {
    uint16_t position = cursor_position;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF)); 
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF)); 
}

void print_character_with_color(char character, VGA_Color color) {
    if (character == '\n') {
        cursor_position += VGA_WIDTH - (cursor_position % VGA_WIDTH);
    } else {
        if (cursor_position < VGA_WIDTH * VGA_HEIGHT) {
            VGA_BUFFER[cursor_position * 2] = character;
            VGA_BUFFER[cursor_position * 2 + 1] = (terminal_background_color << 4) | color;
            cursor_position++;
        }
    }
    if (cursor_position >= VGA_WIDTH * VGA_HEIGHT) {
        cursor_position = 0;
    }
    update_cursor();
}

void print_string_with_color(char* str, VGA_Color color) {
    while (*str) {
        print_character_with_color(*str++, color);
    }
}

void print_line_with_color(char* str, VGA_Color color) {
    print_string_with_color(str, color);
    print_character_with_color('\n', color);
}

void print_character(char character) {
    print_character_with_color(character, terminal_font_color);
}

void print_string(char* str) {
    print_string_with_color(str, terminal_font_color);
}

void print_line(char* str) {
    print_line_with_color(str, terminal_font_color);
}

