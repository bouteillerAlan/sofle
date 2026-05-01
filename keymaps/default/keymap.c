// Copyright 2024
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

// Tap dance indices matching the Vial save TD(0)..TD(3)
enum {
    TD_COMM, // tap=,  hold=[  dtap={  dhold=(
    TD_DOT,  // tap=.  hold=]  dtap=}  dhold=)
    TD_SLSH, // tap=/  hold=\  dtap=|
    TD_SPC,  // tap=Spc  hold=MO(1)  dtap/dhold=MO(2)
};

typedef enum { TD_NONE, TD_SINGLE_TAP, TD_SINGLE_HOLD, TD_DOUBLE_TAP, TD_DOUBLE_HOLD } td_state_t;
typedef struct { td_state_t state; } td_tap_t;

static td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1)
        return (state->interrupted || !state->pressed) ? TD_SINGLE_TAP : TD_SINGLE_HOLD;
    if (state->count == 2)
        return (state->interrupted || !state->pressed) ? TD_DOUBLE_TAP : TD_DOUBLE_HOLD;
    return TD_NONE;
}

static td_tap_t td0 = {TD_NONE};
static void td0_finished(tap_dance_state_t *s, void *u) {
    switch ((td0.state = cur_dance(s))) {
        case TD_SINGLE_TAP:  register_code(KC_COMM); break;
        case TD_SINGLE_HOLD: register_code(KC_LBRC); break;
        case TD_DOUBLE_TAP:  add_mods(MOD_BIT(KC_LSFT)); register_code(KC_LBRC); break;
        case TD_DOUBLE_HOLD: add_mods(MOD_BIT(KC_LSFT)); register_code(KC_9);    break;
        default: break;
    }
}
static void td0_reset(tap_dance_state_t *s, void *u) {
    switch (td0.state) {
        case TD_SINGLE_TAP:  unregister_code(KC_COMM); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LBRC); break;
        case TD_DOUBLE_TAP:  unregister_code(KC_LBRC); del_mods(MOD_BIT(KC_LSFT)); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_9);    del_mods(MOD_BIT(KC_LSFT)); break;
        default: break;
    }
    td0.state = TD_NONE;
}

static td_tap_t td1 = {TD_NONE};
static void td1_finished(tap_dance_state_t *s, void *u) {
    switch ((td1.state = cur_dance(s))) {
        case TD_SINGLE_TAP:  register_code(KC_DOT);  break;
        case TD_SINGLE_HOLD: register_code(KC_RBRC); break;
        case TD_DOUBLE_TAP:  add_mods(MOD_BIT(KC_LSFT)); register_code(KC_RBRC); break;
        case TD_DOUBLE_HOLD: add_mods(MOD_BIT(KC_LSFT)); register_code(KC_0);    break;
        default: break;
    }
}
static void td1_reset(tap_dance_state_t *s, void *u) {
    switch (td1.state) {
        case TD_SINGLE_TAP:  unregister_code(KC_DOT);  break;
        case TD_SINGLE_HOLD: unregister_code(KC_RBRC); break;
        case TD_DOUBLE_TAP:  unregister_code(KC_RBRC); del_mods(MOD_BIT(KC_LSFT)); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_0);    del_mods(MOD_BIT(KC_LSFT)); break;
        default: break;
    }
    td1.state = TD_NONE;
}

static td_tap_t td2 = {TD_NONE};
static void td2_finished(tap_dance_state_t *s, void *u) {
    switch ((td2.state = cur_dance(s))) {
        case TD_SINGLE_TAP:  register_code(KC_SLSH); break;
        case TD_SINGLE_HOLD: register_code(KC_BSLS); break;
        case TD_DOUBLE_TAP:  add_mods(MOD_BIT(KC_LSFT)); register_code(KC_BSLS); break;
        default: break;
    }
}
static void td2_reset(tap_dance_state_t *s, void *u) {
    switch (td2.state) {
        case TD_SINGLE_TAP:  unregister_code(KC_SLSH); break;
        case TD_SINGLE_HOLD: unregister_code(KC_BSLS); break;
        case TD_DOUBLE_TAP:  unregister_code(KC_BSLS); del_mods(MOD_BIT(KC_LSFT)); break;
        default: break;
    }
    td2.state = TD_NONE;
}

static td_tap_t td3 = {TD_NONE};
static void td3_finished(tap_dance_state_t *s, void *u) {
    switch ((td3.state = cur_dance(s))) {
        case TD_SINGLE_TAP:               register_code(KC_SPC); break;
        case TD_SINGLE_HOLD:              layer_on(1); break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:              layer_on(2); break;
        default: break;
    }
}
static void td3_reset(tap_dance_state_t *s, void *u) {
    switch (td3.state) {
        case TD_SINGLE_TAP:               unregister_code(KC_SPC); break;
        case TD_SINGLE_HOLD:              layer_off(1); break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:              layer_off(2); break;
        default: break;
    }
    td3.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_COMM] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td0_finished, td0_reset),
    [TD_DOT]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td1_finished, td1_reset),
    [TD_SLSH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td2_finished, td2_reset),
    [TD_SPC]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td3_finished, td3_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// Layer 0 — base
// ,-----------------------------------------.                    ,-----------------------------------------.
// |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | ESC  |
// |------+------+------+------+------+------|                    |------+------+------+------+------+------|
// | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
// |------+------+------+------+------+------|                    |------+------+------+------+------+------|
// |LSft  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
// |------+------+------+------+------+------|  MUTE |    | MUTOG |------+------+------+------+------+------|
// |LCtrl |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |,/[/{|(|./]|}|)| ///\||  =  |
// `-----------------------------------------/       /     \      \-----------------------------------------'
//               | [  | LGUI | LAlt |TD_SPC| Spc |/       \| Ent | MO2 | Bspc | RAlt |  ]  |
//               `----------------------------------'         '-----------------------------------'
[0] = LAYOUT(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,              KC_6,    KC_7,    KC_8,    KC_9,    KC_0,           KC_ESC,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,           KC_MINS,
  KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,        KC_QUOT,
  KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE, MU_TOGG, KC_N,    KC_M,    TD(TD_COMM), TD(TD_DOT), TD(TD_SLSH), KC_EQL,
           KC_LBRC, KC_LGUI, KC_LALT, TD(TD_SPC), KC_SPC,        KC_ENT,  MO(2),   KC_BSPC, KC_RALT, KC_RBRC
),

// Layer 1 — navigation / function
[1] = LAYOUT(
  KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,          KC_F7,   KC_F8,   KC_F9,   KC_F10,     KC_F11,     KC_F12,
  _______,    _______, KC_HOME, KC_UP,   KC_END,  _______,         _______, _______, _______, _______,    _______,    _______,
  _______,    _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGUP,         _______, _______, KC_LBRC, KC_RBRC,    S(KC_BSLS), _______,
  S(KC_LCTL), _______, _______, _______, KC_PGDN, _______, _______,_______, _______, _______, S(KC_LBRC), S(KC_RBRC), KC_BSLS, _______,
              _______, _______, _______, _______, _______,         _______, MO(3),   KC_DEL,  _______,    _______
),

// Layer 2 — symbols
[2] = LAYOUT(
  S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6),           S(KC_7), S(KC_8), S(KC_9), S(KC_0), S(KC_MINS), _______,
  _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______,  _______,   _______,
  _______, _______, _______, _______, _______, _______,            KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT,  _______,   _______,
  _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______,  _______, _______,   _______, _______,
           _______, _______, _______, MO(3),   _______,            _______, _______, KC_DEL,  _______,  _______
),

// Layer 3 — system / RGB / backlight
[3] = LAYOUT(
  QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, BL_TOGG,            BL_UP,   RGB_MOD,  RGB_SAI, RGB_HUI, RGB_VAI, RGB_SPI,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,            BL_DOWN, RGB_RMOD, RGB_SAD, RGB_HUD, RGB_VAD, RGB_SPD,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,            XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAI,   RGB_VAD, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
           _______, _______, _______, _______,  _______,            _______, _______,  _______, _______, _______
),

};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD), ENCODER_CCW_CW(KC_LEFT,  KC_RIGHT) },
    [1] = { ENCODER_CCW_CW(_______,  _______), ENCODER_CCW_CW(_______,  _______) },
    [2] = { ENCODER_CCW_CW(_______,  _______), ENCODER_CCW_CW(_______,  _______) },
    [3] = { ENCODER_CCW_CW(RGB_HUD,  RGB_HUI), ENCODER_CCW_CW(RGB_SAD,  RGB_SAI) },
};
#endif
