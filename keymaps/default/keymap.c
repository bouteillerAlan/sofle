// Copyright 2024
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "transactions.h"

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
           KC_LBRC, KC_LGUI, KC_LALT, MO(1), KC_SPC,        KC_ENT,  MO(2),   KC_BSPC, KC_RALT, KC_RBRC
),

// Layer 1 — navigation / function
[1] = LAYOUT(
  KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,          KC_F7,   KC_F8,   KC_F9,   KC_F10,     KC_F11,     KC_F12,
  _______,    KC_HOME, KC_UP, KC_END,   _______,  _______,         _______, _______, _______, _______,    _______,    _______,
  _______,    KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGUP, _______,         _______, _______, KC_LBRC, KC_RBRC,    S(KC_BSLS), _______,
  S(KC_LCTL), _______, _______, KC_PGDN, _______, _______, _______,_______, _______, _______, S(KC_LBRC), S(KC_RBRC), KC_BSLS, _______,
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

#ifdef OLED_ENABLE

#define WPM_MAX     120
#define GRID_ROWS   15
#define GRID_COLS   5
#define PAD_COLS    (GRID_COLS - 1)
#define CHAR_TTL_MS 1500

static uint32_t slp_tm;

// ---------- Padding grid (slave-side render state) ----------
static char     pad_char[GRID_ROWS][PAD_COLS];
static uint32_t pad_time[GRID_ROWS][PAD_COLS];
static bool     pad_inited = false;

static void pad_grid_init(void) {
    for (uint8_t r = 0; r < GRID_ROWS; r++) {
        for (uint8_t c = 0; c < PAD_COLS; c++) {
            pad_char[r][c] = '0';
            pad_time[r][c] = 0;
        }
    }
    pad_inited = true;
}

static void pad_grid_feed(char c) {
    if (!pad_inited) pad_grid_init();
    uint8_t r   = (uint8_t)(rand() % GRID_ROWS);
    uint8_t col = (uint8_t)(rand() % PAD_COLS);
    pad_char[r][col] = c;
    pad_time[r][col] = timer_read32();
}

static void pad_grid_decay(void) {
    if (!pad_inited) pad_grid_init();
    for (uint8_t r = 0; r < GRID_ROWS; r++) {
        for (uint8_t c = 0; c < PAD_COLS; c++) {
            if (pad_char[r][c] != '0' && timer_elapsed32(pad_time[r][c]) > CHAR_TTL_MS) {
                pad_char[r][c] = '0';
            }
        }
    }
}

static void render_oled(void) {
    uint8_t wpm = get_current_wpm();
    uint8_t wc  = wpm > WPM_MAX ? WPM_MAX : wpm;

    char buf[4];
    snprintf(buf, sizeof(buf), "%u", wpm);
    uint8_t len = (uint8_t)strlen(buf);
    if (len > GRID_COLS) len = GRID_COLS;

    uint16_t fill = (uint16_t)wc * GRID_ROWS / WPM_MAX;

    pad_grid_decay();

    // Row 0: numeric value
    oled_set_cursor(0, 0);
    for (uint8_t c = 0; c < GRID_COLS; c++) {
        oled_write_char(c < len ? buf[c] : ' ', false);
    }

    // Rows 1..GRID_ROWS: bar column + padding grid (with typed chars)
    for (uint8_t r = 0; r < GRID_ROWS; r++) {
        oled_set_cursor(0, r + 1);
        char bar_char = (r >= GRID_ROWS - fill) ? '#' : '_';
        oled_write_char(bar_char, false);
        for (uint8_t c = 0; c < PAD_COLS; c++) {
            oled_write_char(pad_char[r][c], false);
        }
    }
}

bool oled_task_user(void) {
    if (!is_keyboard_master()) {
        if (get_current_wpm() != 0) {
            oled_on();
            slp_tm = timer_read32();
        } else if (timer_elapsed32(slp_tm) > 60000) {
            oled_off();
        }
        render_oled();
        return false;
    }
    return true;
}

#endif // OLED_ENABLE

// ============================================================
// Split RPC: sync typed characters from master -> slave
// ============================================================

typedef struct {
    char     c;
    uint32_t seq;
} typed_char_sync_t;

// ---- Master side state ----
#define KEY_RING_SIZE 8

static char     key_ring[KEY_RING_SIZE];
static uint32_t key_ring_seq[KEY_RING_SIZE];
static uint8_t  key_ring_head = 0;
static uint32_t global_seq    = 0;

static uint32_t last_sent_seq = 0;

// Push a newly typed char into master's ring buffer
static void key_ring_push(char c) {
    global_seq++;
    key_ring[key_ring_head]     = c;
    key_ring_seq[key_ring_head] = global_seq;
    key_ring_head = (key_ring_head + 1) % KEY_RING_SIZE;
}

// ---- Slave side state ----
static uint32_t last_recv_seq = 0;

// Slave-side handler: called when master sends new data
static void typed_char_sync_slave_handler(uint8_t in_buflen, const void* in_data,
                                           uint8_t out_buflen, void* out_data) {
    const typed_char_sync_t* payload = (const typed_char_sync_t*)in_data;

    if (payload->seq != last_recv_seq && payload->c != 0) {
        last_recv_seq = payload->seq;
#ifdef OLED_ENABLE
        pad_grid_feed(payload->c);
#endif
    }
}

// Master-side: called periodically to push newest char to slave
static void typed_char_sync_master_task(void) {
    if (!is_keyboard_master()) return;
    if (global_seq == last_sent_seq) return; // nothing new

    // Find most recent entry in ring buffer
    uint8_t idx = (key_ring_head + KEY_RING_SIZE - 1) % KEY_RING_SIZE;
    typed_char_sync_t payload = {
        .c   = key_ring[idx],
        .seq = key_ring_seq[idx],
    };

    if (transaction_rpc_send(RPC_ID_TYPED_CHAR, sizeof(payload), &payload)) {
        last_sent_seq = payload.seq;
    }
}

// Register the RPC handler (slave side registers, master side calls send)
void keyboard_post_init_user(void) {
    transaction_register_rpc(RPC_ID_TYPED_CHAR, typed_char_sync_slave_handler);
}

// Called every matrix scan on both halves; only does work on master
void housekeeping_task_user(void) {
    typed_char_sync_master_task();
}

// ============================================================
// Key processing: capture typed chars on master
// ============================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        char c = 0;

        switch (keycode) {
            case KC_A ... KC_Z:
                c = 'a' + (keycode - KC_A);
                break;
            case KC_SPC:
                c = ' ';
                break;
            case KC_DOT:
                c = '.';
                break;
            case KC_COMM:
                c = ',';
                break;
            default:
                c = 0;
        }

        if (c) {
            key_ring_push(c);
        }
    }
    return true;
}

