/*
    Copyright 2010-2012 Stefan Fröbe, <frobiac /at/ gmail [d0t] com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <avr/pgmspace.h>
#include "hhstdio.h"
#include "keymap.h"

/**
 *  Mappings of row and columns of the switch matrix to the selected layout matrix.
 *  Currently, the two halves of the keyboard are stacked so that
 *  rows 0-3 correspond to the left half and
 *  rows 4-7 to the right hand side.
 *
 *  All indices are zero-based.
 */
static keycode getKeyStruct(uint8_t row, uint8_t col, uint8_t layer);

uint8_t getModifier(uint8_t row, uint8_t col, uint8_t layer)
{
    return getKeyStruct(row, col, layer).mods;
}

uint8_t getKeyCode(uint8_t row, uint8_t col, uint8_t layer)
{
    return getKeyStruct(row, col, layer).hid;
}

uint8_t getKeyChar(uint8_t row, uint8_t col, uint8_t layer)
{
    uint8_t ch = getKeyStruct(row, col, layer).ch;
    if(/*ch > 127 ||*/ ch < 33)
        return ' '; // '_' (uint8_t)('a');
    return ch;
}

EEMEM Layout   alternateLayoutNr = QWERTZ_DE;
EEMEM MacOrPC  altMacOrPC        = PC;
EEMEM GeoArea  alternateGeoArea  = DE;

keycode getKeyStruct(uint8_t row, uint8_t col, uint8_t layer)
{
    if( g_alternateLayer && layer==0)
        layer=5;
    keycode kc;
    if(PINKYDROP) {
        if( (row<4 && row > 0 && col==1) || (row > 4 && col == 4))
            memcpy_P(&kc, &KeyMatrix[layer][row-1][col], sizeof(kc));
    } else {
        memcpy_P(&kc, &KeyMatrix[layer][row][col], sizeof(kc));
    }

    //printf("\n%d %d %d=%c", kc.hid, kc.mods, kc.ch, kc.ch);
    return kc;
}

void printLayout(uint8_t l)
{
    uint8_t ch;
    printf("\n---");
    for(uint8_t r=0; r<ROWS/2-1; ++r) {
        printf("\n");
        for(uint8_t c=0; c<COLS; ++c) {
            ch=getKeyChar(r,c,l);
            if(ch==92)
                printf("\\\\");
            else
                printf("%c", ch );
        }
        printf("  ");
        for(uint8_t c=0; c<COLS; ++c)
            printf("%c", getKeyChar(r+ROWS/2,c,l) );
    }
}


