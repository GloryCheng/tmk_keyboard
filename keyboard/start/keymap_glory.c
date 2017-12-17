#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
KEYMAP( \
    ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  NO,  F9,  F10, F11, F12,       PSCR,SLCK,          NO,  NO,  VOLU,VOLD, \
   	GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,      INS, HOME,PGUP,     NLCK,PSLS,PAST,PMNS, \
   	TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,ENT,       DEL, END, PGDN,     P7,  P8,  P9,  PPLS, \
  	CAPS,LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,BSLS,           UP,            P4,  P5,  P6,  PPLS, \
   	LSFT,LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,RSFT,           LEFT,DOWN,RGHT,     P1,  P2,  P3,  PENT, \
    LALT,SPC,                                                                                       P0,  PDOT \
)
};
const action_t PROGMEM fn_actions[] = {
};
