# KWin Horizontal Sliding Notifications (Plasma 6)

![preview](preview.gif)

This KWin effect provides a tactile, spring-y **horizontal slide** for Plasma notifications, replacing the default fade animations. Originally designed for KDE Neon, this version has been updated with a **generic build system** to support various Plasma 6 distributions.

This project file reworked to make a successful generic CMakeLists.txt and SlidingEffects.cpp plugin

##Updated for Plasma 6.6

This effect has been updated for KDE Plasma 6.6.
API: Uses the new KWin::Region signature.
Path: Plugins must now be installed to /usr/lib/qt6/plugins/kwin/effects/.
Sound: Uses pw-play with the Ocean sound theme (.oga).

###Installation for Plasma 6:

```bash
mkdir build && cd build
cmake ..
make
sudo install -m 755 kwin_final_sliding.so /usr/lib/qt6/plugins/kwin/effects/
sudo ln -s /usr/lib/qt6/plugins/kwin/effects/kwin_final_sliding.so /usr/lib/qt6/plugins/kwin/effects/plugins/kwin_final_sliding.so
```

## Key Features

- **Horizontal Only**: Strict lock on the **Y** and **Z** axes to prevent diagonal drifting.
- **Tactile Entrance**: Uses an **OutBack** easing curve for a physical “snap” feel.
- **Stealth Exit**: A manual timer-based exit that “cuts the feed” before KWin unmaps the window, eliminating final-frame stutter.
- **Notification Sound**: Automatically triggers a notification sound using `pw-play`.
- **Generic Compatibility**: Uses CMake discovery to find KWin headers regardless of whether the distro uses `/usr/include/kwin` or `/usr/include/KWin`.

## Prerequisites

To build this plugin, you need the C++ development environment and KWin/KF6 headers. 

### KDE Neon / Ubuntu / Kubuntu
```bash
sudo apt install g++ cmake extra-cmake-modules libkwin-dev \
     libkf6config-dev libkf6configwidgets-dev libkf6coreaddons-dev \
     libkf6windowsystem-dev qt6-base-dev
```

### Fedora
```bash
sudo dnf install gcc-c++ cmake extra-cmake-modules kwin-devel \
     kf6-kconfig-devel kf6-kconfigwidgets-devel kf6-kcoreaddons-devel \
     kf6-kwindowsystem-devel qt6-qtbase-devel
```

### Arch Linux
```bash
sudo pacman -S base-devel cmake extra-cmake-modules kwin \
     kconfig kconfigwidgets kcoreaddons kwindowsystem qt6-base
```

## Notification Sound
The effect is hardcoded to play a sound via PipeWire on window entry:
- **Command**: `/usr/bin/pw-play`
- **Target**: `/usr/share/sounds/oxygen/stereo/message-new-instant.ogg`

**Note**: If the sound file or `pw-play` is missing, the plugin will remain silent and continue to function normally without crashing.

## How to Install

### 1. Build the Plugin
The generic `CMakeLists.txt` will automatically detect your system's library paths.

```bash
mkdir build && cd build
cmake ..
make
```

### 2. Install
This will install the plugin to the appropriate KDE Qt6 plugin directory:

```bash
sudo make install
```

### 3. Activate & Reload
Enable the plugin and tell KWin to reload its effects without restarting your session:

```bash
# Enable the plugin
kwriteconfig6 --file kwinrc --group Plugins --key kwin_final_slidingEnabled true

# Reload KWin effects
qdbus6 org.kde.KWin /Effects org.kde.KWin.Effects.loadDefaultEffects
```

## Troubleshooting
KWin 6 headers are not considered a stable API. If the build fails:
- **Missing Headers**: Ensure `libkwin-dev` (or your distro's equivalent) is installed. The CMake script looks for `effect/effect.h`.
- **Library Mismatch**: The script looks for `libkwin.so.6`. If you are on an older or experimental system, verify your KWin library version in `/usr/lib/`.
- **Logs**: Check `journalctl -f` while reloading effects to see if KWin reports any "Plugin could not be loaded" errors.
