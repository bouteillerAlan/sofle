# QMK Setup with Vial for Sofle rev1

## 1. Clone vial-qmk
```
git clone https://github.com/vial-kb/vial-qmk.git ~/Documents/vial-qmk
cd ~/Documents/vial-qmk
```

## 2. Initialize submodules (important, otherwise "lufa/makefile" error)
```
qmk git-submodule
# or: git submodule update --init --recursive
```

## 3. Configure QMK CLI to use this repo
```
qmk config user.qmk_home="$HOME/Documents/vial-qmk"
```

## 4. Verify config
```
qmk config
# should show: user.qmk_home=/home/xxx/Documents/vial-qmk
```

## 5. Compile
```
qmk compile -kb sofle/rev1 -km default
```

## 6. Flash
```
qmk flash -kb sofle/rev1 -km default -bl dfu
```
or
```
cd ~/qmk_firmware
dfu-programmer atmega32u4 erase --force
dfu-programmer atmega32u4 flash sofle_rev1_default.hex
dfu-programmer atmega32u4 reset
```

---

```markdown
# Vial on Linux — WebHID Access Fix

## Problem
`NotAllowedError: Failed to open the device` when connecting on vial.rocks.

## Cause
Missing udev permissions for HID access on Linux.

## Fix

- [ ] Find VID/PID with `lsusb` (e.g. `fc32:0287`)
- [ ] Create rule file:
  ```
  sudo nano /etc/udev/rules.d/59-vial.rules
  ```
- [ ] Add rules (replace VID/PID):
  ```
  SUBSYSTEM=="usb", ATTRS{idVendor}=="fc32", ATTRS{idProduct}=="0287", MODE="0666", GROUP="users", TAG+="uaccess"
  KERNEL=="hidraw*", ATTRS{idVendor}=="fc32", ATTRS{idProduct}=="0287", MODE="0666", GROUP="users", TAG+="uaccess"
  ```
- [ ] Reload udev:
  ```
  sudo udevadm control --reload-rules
  sudo udevadm trigger
  ```
- [ ] Unplug/replug keyboard
- [ ] Verify: `ls -l /dev/hidraw*` → should show `666` perms
- [ ] Retry connection on vial.rocks

## Note
File must be named `59-*.rules` (not `99-*`) due to systemd udev ordering change.
```

---

---



# Sofle Keyboard

![SofleKeyboard version 1](https://i.imgur.com/S5GTKth.jpeg)

Sofle is 6×4+5 keys column-staggered split keyboard. Based on Lily58, Corne and Helix keyboards.

More details about the keyboard and build guides can be found here: [Sofle Keyboard Build Log and Guide](https://josefadamcik.github.io/SofleKeyboard)

* Keyboard Maintainer: [Josef Adamcik](https://josef-adamcik.cz) [Twitter:@josefadamcik](https://twitter.com/josefadamcik)  
* Hardware Supported: SofleKeyboard PCB, ProMicro  
* Hardware Availability: [PCB & Case Data](https://github.com/josefadamcik/SofleKeyboard)

## Firmware Revisions
- `sofle/rev1` is used for v1, v2, and RGB PCBs (**NOT** RGB PCBs purchased from [Keyhive](https://keyhive.xyz))
- `sofle/keyhive` is used for PCBs purchased from [Keyhive](https://keyhive.xyz/shop/sofle)
- [`keyboards/sofle_choc`](../sofle_choc/) is used for Choc PCBs

Make example for this keyboard (after setting up your build environment):

    make sofle/rev1:default
    make sofle/keyhive:default

Flashing example for this keyboard:

    make sofle/rev1:default:flash
    make sofle/keyhive:default:flash

Press reset button on he keyboard when asked.

Disconnect the first half, connect the second one and repeat the process.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix
* **Physical reset button**: Briefly press the button near the TRRS connector. Quickly double-tap if you are using Pro Micro.
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
