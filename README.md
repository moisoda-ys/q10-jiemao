# Q10 + XIAO BLE OLED dongle

This repository builds a ZMK split configuration for a ZitaoTech Q10 and the
Seeed XIAO BLE OLED dongle based on `zmk-dongle-display`.

The dongle is the split central: connect it to a host over USB. The Q10
connects to the dongle over Bluetooth and keeps the Q10 keymap.

The 128x64 SSD1306 OLED uses the XIAO BLE I2C bus. Its status screen shows only
the active Q10 layer, USB or Bluetooth output mode, and the Q10 battery level.

## Firmware artifacts

GitHub Actions builds four UF2 files:

- `xiao_ble_q10_dongle_dongle_display-zmk.uf2` - flash this to the OLED dongle.
- `zitaotech_q10-zmk.uf2` - flash this to the Q10.
- The two `settings_reset` artifacts erase saved Bluetooth/settings data.

Flash each device by holding its existing bootloader button until the UF2
drive appears, then copying the matching file to that drive.

## First pairing

Flash the settings-reset UF2 to both devices once, then flash the normal UF2
files. Connect the dongle by USB and turn on the Q10. If it does not connect,
clear Bluetooth profiles on the Q10 and pair it again using the Q10's existing
Bluetooth-selection key.

The OLED and status screen are based on
[`moisoda-ys/zmk-dongle-display`](https://github.com/moisoda-ys/zmk-dongle-display).
