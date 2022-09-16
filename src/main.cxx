#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace System
{
    namespace VGA
    {
        static const size_t VGAWidth = 80;
        static const size_t VGAHeight = 25;

        enum VGAColor
        {
            VGA_COLOR_BLACK = 0,
            VGA_COLOR_BLUE = 1,
            VGA_COLOR_GREEN = 2,
            VGA_COLOR_CYAN = 3,
            VGA_COLOR_RED = 4,
            VGA_COLOR_MAGENTA = 5,
            VGA_COLOR_BROWN = 6,
            VGA_COLOR_LIGHT_GREY = 7,
            VGA_COLOR_DARK_GREY = 8,
            VGA_COLOR_LIGHT_BLUE = 9,
            VGA_COLOR_LIGHT_GREEN = 10,
            VGA_COLOR_LIGHT_CYAN = 11,
            VGA_COLOR_LIGHT_RED = 12,
            VGA_COLOR_LIGHT_MAGENTA = 13,
            VGA_COLOR_LIGHT_BROWN = 14,
            VGA_COLOR_WHITE = 15,
        };

        static inline uint8_t VGAEntryColor(enum VGAColor fg, enum VGAColor bg)
        {
            return fg | bg << 4;
        }

        static inline uint16_t VGAEntry(unsigned char uc, uint8_t color)
        {
            return (uint16_t)uc | (uint16_t)color << 8;
        }
    }

    namespace Tools
    {
        namespace String
        {
            size_t strlen(const char *str)
            {
                size_t len = 0;
                while (str[len])
                    len++;
                return len;
            }
        }

    }

    namespace Terminal
    {
        size_t TermRow;
        size_t TermCol;
        uint8_t TermColor;
        uint16_t *TermBuffer;

        void TermInit(void)
        {
            TermRow = 0;
            TermCol = 0;
            TermColor = VGA::VGAEntryColor(VGA::VGA_COLOR_LIGHT_GREY, VGA::VGA_COLOR_BLACK);
            TermBuffer = (uint16_t *)0xB8000;

            for (size_t y = 0; y < VGA::VGAHeight; y++)
            {
                for (size_t x = 0; x < VGA::VGAWidth; x++)
                {
                    const size_t index = y * VGA::VGAWidth + x;
                    TermBuffer[index] = VGA::VGAEntry(' ', TermColor);
                }
            }
        }

        void TermSetColor(uint8_t color)
        {
            TermColor = color;
        }

        void TermPutEntryAt(char c, uint8_t color, size_t x, size_t y)
        {
            const size_t index = y * VGA::VGAWidth + x;
            TermBuffer[index] = VGA::VGAEntry(c, color);
        }

        void Clear()
        {
            for (size_t y = 0; y < VGA::VGAHeight; y++)
            {
                for (size_t x = 0; x < VGA::VGAWidth; x++)
                {
                    const size_t index = y * VGA::VGAWidth + x;
                    TermBuffer[index] = VGA::VGAEntry(' ', TermColor);
                }
            }

            TermRow = 0;
            TermCol = 0;
        }

        void CarriageReturn(void)
        {
            TermCol = 0;
        }

        void Newline(void)
        {
            CarriageReturn();
            if (++TermRow == VGA::VGAHeight)
                Clear();
        }

        void TermPutChar(char c)
        {
            switch (c)
            {
            case '\n':
                Newline();
                break;
            case '\r':
                CarriageReturn();
                break;
            default:
                TermPutEntryAt(c, TermColor, TermCol, TermRow);
                if (++TermCol == VGA::VGAWidth)
                {
                    CarriageReturn();
                    Newline();
                }
            }
        }

        void TermWrite(const char *data)
        {
            for (size_t i = 0; i < Tools::String::strlen(data); i++)
                TermPutChar(data[i]);
        }
    }

}

extern "C" void vienna(void)
{
    System::Terminal::TermInit();
    System::Terminal::TermWrite("i love men\nand children\ncarriagereturn\r123\n");
    System::Terminal::TermWrite(pallihiki);
}