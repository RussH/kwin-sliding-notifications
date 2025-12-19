# KWin Horizontal Sliding Notifications (Plasma 6)

![preview](preview.gif)

This KWin effect provides a tactile, spring-y **horizontal slide** for Plasma notifications, replacing the default fade animations. It is specifically designed for **KDE Neon**, **KWin 6 (Qt6)**.

This was only built for me on KDE Neon, so I'm not committing to any specific support especially for other Distro's - KDE Neon is fairly picky so I guess it's unlikely to work cleanly elsewhere, but it's a working example for you.

Currently working fine on Plasma 6.2.. we'll see how long it lasts!

## Key Features

- **Horizontal Only**: Strict lock on the **Y** and **Z** axes to prevent diagonal drifting (version 10 looked very odd!)
- **Tactile Entrance**: Uses an **OutBack** easing curve for a physical “snap” feel.
- **Stealth Exit**: A manual timer-based exit that “cuts the feed” **300 ms** before KWin unmaps the window, eliminating the final-frame stutter.
- **Type Restricted**: Specifically targets **Window Type 11 (Notifications)** to avoid interfering with other `plasmashell` elements like panels or tooltips.

## Prerequisites

You will need the following development packages installed on your system (KDE Neon names shown):

```bash
sudo apt install g++ cmake extra-cmake-modules kwin-dev \
  libkf6config-dev libkf6configwidgets-dev libkf6coreaddons-dev qt6-base-dev
```

## How to Install

### 1. Build the Plugin

Navigate to your source folder and run:

```bash
mkdir build && cd build
cmake ..
make
```

### 2. Manual Installation

Because KWin locks the library while it's running, use `install` rather than `cp` to prevent a compositor crash:

```bash
sudo install -m 755 kwin_final_sliding.so \
  /usr/lib/x86_64-linux-gnu/qt6/plugins/kwin/effects/plugins/
```

### 3. Activate

Apply the changes without restarting your session:

```bash
dbus-send --session --dest=org.kde.KWin --type=method_call \
  /KWin org.kde.KWin.reconfigure
```

Or:

```bash
kwin_wayland --replace
```

## Troubleshooting for Future Updates

KWin 6 headers are not considered a stable API. If this breaks after a major Plasma update:

- Check if the `drawWindow` signature in `/usr/include/kwin/effect/animationeffect.h` has changed.
- Verify the `EffectWindow` methods (like `isNotification()`) haven't been renamed.
- Check system logs for loading errors (e.g. “Symbol not found”):

- This is my specific CMakeLists.txt below, I've tried to make the one here more generic so it works for everyone. If you are on KDE Neon you may want to use this instead  

```bash
cmake_minimum_required(VERSION 3.16)
project(kwin_final_sliding)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(Qt6 REQUIRED COMPONENTS Core Gui)
find_package(KF6 REQUIRED COMPONENTS CoreAddons WindowSystem Config WidgetsAddons)

set(CMAKE_AUTOMOC_MOC_OPTIONS "-I${CMAKE_CURRENT_SOURCE_DIR}")

# The target name must match across all three lines below
add_library(kwin_final_sliding MODULE SlidingEffect.cpp)

target_include_directories(kwin_final_sliding SYSTEM PRIVATE /usr/include/kwin)

target_link_libraries(kwin_final_sliding
    /usr/lib/x86_64-linux-gnu/libkwin.so.6
    Qt6::Core
    KF6::CoreAddons
    KF6::WindowSystem
    KF6::ConfigCore
    KF6::WidgetsAddons
)

install(TARGETS kwin_final_sliding DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins)
```
