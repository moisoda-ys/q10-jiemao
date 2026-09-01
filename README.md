# Q10 + nRF52840 OLED dongle

This repository builds a ZMK split configuration for a ZitaoTech Q10 and the
nRF52840/nice!nano-compatible OLED dongle based on `zmk-dongle-display`.

The dongle is the split central: connect it to a host over USB. The Q10
connects to the dongle over Bluetooth and keeps the Q10 keymap.

The 1.3-inch SH1106 OLED uses the pro-micro I2C bus with the required
two-segment offset. Its status screen shows only the active Q10 layer, USB or
Bluetooth output mode, and the Q10 battery level.

## Firmware artifacts

GitHub Actions builds two UF2 files:

- `zitaotech_q10-zmk.uf2` - standalone Q10 Bluetooth keyboard firmware.
- `zitaotech_q10_settings_reset-zmk.uf2` - clear saved Q10 Bluetooth/settings
  data before pairing directly with a computer.

Flash each file by holding the Q10 bootloader button until the UF2 drive
appears, then copying the matching file to that drive.

## First pairing

Flash the settings-reset UF2 to both devices once, then flash the normal UF2
files. Connect the dongle by USB and turn on the Q10. If it does not connect,
clear Bluetooth profiles on the Q10 and pair it again using the Q10's existing
Bluetooth-selection key.

The OLED and status screen are based on
[`moisoda-ys/zmk-dongle-display`](https://github.com/moisoda-ys/zmk-dongle-display).
