# KWin Horizontal Sliding Notifications (Plasma 6)

# KWin Horizontal Sliding Notifications (Plasma 6)

<p align="center">
  <video src="https://github.com/RussH/kwin-sliding-notifications/blob/main/preview.webm" width="600" controls autoplay loop muted>
    Your browser does not support the video tag.
  </video>
</p>
This KWin effect provides a tactile, spring-y horizontal slide for Plasma notifications, replacing the default diagonal or fade animations. It is specifically designed for KWin 6 (Qt6) and includes a "Stealth Exit" logic to prevent the common flickering/popping issues seen in custom KWin animations.

## Key Features
Horizontal Only: Strict lock on the Y and Z axes to prevent diagonal drifting.

Tactile Entrance: Uses an OutBack easing curve for a physical "snap" feel.

Stealth Exit: A manual timer-based exit that "cuts the feed" 300ms before KWin unmaps the window, eliminating the final-frame stutter.

Type Restricted: Specifically targets Window Type 11 (Notifications) to avoid interfering with other plasmashell elements like panels or tooltips.

## Prerequisites
You will need the following development packages installed on your system (Ubuntu/KDE Neon names shown):

Bash

sudo apt install g++ cmake extra-cmake-modules kwin-dev libkf6config-dev libkf6configwidgets-dev libkf6coreaddons-dev qt6-base-dev
## How to Install
### 1. Build the Plugin
Navigate to your source folder and run:

Bash

mkdir build && cd build
cmake ..
make
### 2. Manual Installation
Because KWin locks the library while it's running, use install rather than cp to prevent a compositor crash:

Bash

sudo install -m 755 kwin_final_sliding.so /usr/lib/x86_64-linux-gnu/qt6/plugins/kwin/effects/plugins/
### 3. Activate
Apply the changes without restarting your session:

Bash

dbus-send --session --dest=org.kde.KWin --type=method_call /KWin org.kde.KWin.reconfigure
or kwin_wayland --replace

## Troubleshooting for Future Updates
KWin 6 headers are not considered a stable API. If this breaks after a major Plasma update:

Check if the drawWindow signature in /usr/include/kwin/effect/animationeffect.h has changed.

Verify the EffectWindow methods (like isNotification()) haven't been renamed.

Check the system logs (journalctl -f | grep kwin) for any loading errors related to "Symbol not found."
