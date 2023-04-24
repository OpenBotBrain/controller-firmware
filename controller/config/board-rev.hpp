#pragma once

#include <stdint.h>

enum BoardRevision
{                           // ID:0   ID:1     REV
    BOARD_REV_A = 0x00,     //  0      0       0x00

    BOARD_REV_SIZE
};

struct BoardSpecificConfig
{
    const char* board_name;
};

void board_rev_init();
const BoardSpecificConfig* board_rev_get_specific_configuration();
uint8_t board_rev_get_revision();

