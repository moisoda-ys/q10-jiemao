# Q10 + nRF52840 OLED dongle

This repository builds a ZMK split configuration for a ZitaoTech Q10 and the
nRF52840/nice!nano-compatible OLED dongle based on `zmk-dongle-display`.

The dongle is the split central: connect it to a host over USB. The Q10
connects to the dongle over Bluetooth and keeps the Q10 keymap.

The 1.3-inch SH1106 OLED uses the pro-micro I2C bus with the required
two-segment offset. Its status screen shows only the active Q10 layer, USB or
Bluetooth output mode, and the Q10 battery level.

## Firmware artifacts

GitHub Actions builds firmware for both dongle variants:

- `nice_nano_v2_q10_dongle_dongle_display-zmk.uf2` - flash this to the OLED dongle.
- `q10_st7789_dongle-zmk.uf2` - flash this to the original nRF52840/ST7789 dongle.
- `zitaotech_q10-zmk.uf2` - flash this to the Q10.
- Matching `settings_reset` artifacts erase saved Bluetooth/settings data.

Flash each device by holding its existing bootloader button until the UF2
drive appears, then copying the matching file to that drive.

Both dongles are BLE split centrals: connect one dongle by USB, then connect
the Q10 to that dongle over Bluetooth. Only flash one dongle firmware at a
time.

## First pairing

Flash the settings-reset UF2 to both devices once, then flash the normal UF2
files. Connect the dongle by USB and turn on the Q10. If it does not connect,
clear Bluetooth profiles on the Q10 and pair it again using the Q10's existing
Bluetooth-selection key.

The OLED and status screen are based on
[`moisoda-ys/zmk-dongle-display`](https://github.com/moisoda-ys/zmk-dongle-display).
