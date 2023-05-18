# FTL Godmode (for Linux)

## Behavior:

Every 5 seconds, does the following (if the player ship exists):
 - Fully upgrades your weapons, shields, engines, and drones systems
 - Sets the previous systems to full health

 - Sets your fuel, missiles, and drone parts to 999
 - Sets your hull to 30
 - Sets your scrap to 9999
 - Sets your max power to 25 (the max)
 - Sets the amount of power in use to 0
 - Sets your weapon system to be using 1 power (0 and it won't shoot)
 - Sets your shields and engines to be using max power (max activation)
 - Sets your number of active shields to your number of enabled shields

## Requirements:
 - GCC
 - A POSIX environment (pthread, dirent, etc.)

## Installation:

 - Run `hook.sh install`. This:
    - Compiles `inject.c` to `inject.so`.
    - Installs a line to load the hack.
        - Replaces a previous installation.

 - Copy the output from the previous step (looks like `modded <something>`).
 - Right-click FTL in Steam.
 - Click `Properties`.
 - Under `General`, paste it into the `LAUNCH OPTIONS` box.

## Disabling/uninstallation:

To disable, remove everything from the `LAUNCH OPTIONS` box.

To uninstall, run `hook.sh uninstall`, then disable the mod.
