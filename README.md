# MControlCenter Extra

**MControlCenter Extra** is an enhanced, highly-optimized fork of [MControlCenter](https://github.com/dmitry-s93/MControlCenter) (originally by Dmitry Serov) designed for MSI laptops running GNU/Linux, with specific optimizations for Debian, Ubuntu, Fedora, and Arch Linux.

<img width="1500" height="1332" alt="MCC3" src="https://github.com/user-attachments/assets/75b5ee18-ed99-4a89-b9fe-1ec0078760e8" />

---

## 🌟 What's New in this Fork?

This fork introduces critical performance improvements, bug fixes for modern MSI hardware (such as Modern 14 C7M / AMD Barcelo APUs / MS-14JK), dynamic lighting effects for single-color keyboards, and full Debian support:

### 💡 Dynamic Keyboard Backlight Effects
- **Animation Modes:** Added dynamic lighting modes for single-color (white) backlit keyboards:
  - **Static:** Solid brightness levels (Off, Low, Medium, High).
  - **Breathing:** Smooth, continuous fade-in and fade-out pulse effect.
  - **Heartbeat:** Double-pulse rhythm effect.
  - **Blink:** Periodic flashing effect.
- **Sleep & Resume Aware:** Lighting effects automatically resume after waking from system suspend/sleep.

### ⚡ Zero-Lag, Fluid UI (Asynchronous Architecture)
- Upstream MControlCenter suffered from severe UI freezing (1–5 seconds per tick) caused by synchronous 256-byte EC reads (`/sys/kernel/debug/ec/ec0/io`) on every timer update.
- **Sub-millisecond Sensor Reads:** CPU and GPU temperatures are now read directly from `msi-ec` sysfs in under 0.001 ms without D-Bus overhead.
- **Asynchronous Tachometer Reads:** Fan speed queries now use targeted 30 ms `pread()` calls inside a dedicated background worker, eliminating all micro-stutters and keeping the Qt GUI completely responsive.

### 🐧 Full Debian 12 & 13 (Trixie) Support
- Upstream marked Debian as unsupported due to kernel `ec_sys` behavior.
- This fork enables runtime `write_support=1` activation for `ec_sys` and works out of the box on modern Debian kernels.

### 🔧 Fix for 8888 RPM Fan Speed Bug
- Modern MSI laptops (such as `MS-14JK` / Modern 14 C7M and `1551` series) map fan tachometer counters to EC register `0xCD`.
- Resolved an issue where the app mistakenly read power register `0xC9` (value ~54), calculating $480000 / 54 \approx 8888\text{ RPM}$.
- The app now automatically detects the firmware model and locks to the correct register.

### 🔇 Persistent Hardware Mute LED (ALSA ctl-led)
- Fixed an issue where the keyboard mute LED stopped working after a system restart or sleep.
- Includes automatic ALSA `Master Playback Switch` attachment through the root helper service, on application launch, on wake from sleep, and via persistent udev rules (`/etc/udev/rules.d/99-msi-mute-led.rules`).

### 🚀 Smart Power Profiles & Advanced Fan Curves
- **Automatic Fallback:** Seamlessly maps performance profiles to `sport_mode` when `turbo_mode` is unsupported by the laptop's EC.
- **Sleep Watcher:** Reapplies custom advanced fan curves and power settings automatically upon waking from system sleep.
- **Distinct Application Identity:** Labeled as **MControlCenter Extra** in application menus to avoid collisions with upstream installs.

---

## Features

- **Sensors:** Realtime CPU and GPU temperature display and fan tachometer readings.
- **Power Modes:**
  - High Performance (with automatic fallback to Sport mode if Turbo is absent)
  - Balanced
  - Silent
  - Super Battery
- **Automation:** Automatic mode switching based on AC charger status or system power profile.
- **Battery Care:** Configurable maximum battery charge thresholds (e.g. 60%, 80%, 100%).
- **Advanced Fan Curves:** Custom multi-point temperature-to-speed curves with sleep reapplication.
- **Keyboard Lighting:** Brightness level selection, timeout mode, and dynamic animation effects.
- **Peripherals:** Toggle USB Power Share, Fn/Super key swap, Cooler Boost, and Webcam lock.

---

## Supported Devices

Device support relies on the Linux kernel driver [**msi-ec**](https://github.com/BeardOverflow/msi-ec) and the `ec_sys` module.

- [**List of tested devices supported by msi-ec**](https://github.com/BeardOverflow/msi-ec/discussions/277)
- Tested and confirmed working on: **MSI Modern 14 C7M (MS-14JK)**, AMD Barcelo / Ryzen 7000 series.

---

## Installation

### Prerequisites

1. **Kernel Driver (`msi-ec`):**
   Verify `msi-ec` is loaded:
   ```bash
   cat /sys/devices/platform/msi-ec/shift_mode
   ```
   If it returns `No such file or directory`, install [msi-ec](https://github.com/BeardOverflow/msi-ec#installation) first.

2. **EC Sysfs Driver (`ec_sys`):**
   Required for temperature and fan controls.
   - **Debian / Ubuntu:** Comes with the kernel. The helper automatically sets `write_support=1`.
   - **Fedora:** Requires [acpi_ec](https://github.com/saidsay-so/acpi_ec.git).
   - **Arch Linux:** Built into kernel or available via DKMS.

3. **Qt6 Libraries:**
   - Debian / Ubuntu / Mint: `libqt6widgets6 libqt6dbus6`
   - Fedora: `qt6-qtbase-gui`
   - Arch Linux: `qt6-base`

---

### Quick Installation (Pre-built Package)

1. Clone or download this repository:
   ```bash
   git clone https://github.com/SirEntr0pyy/MControlCenterExtra.git
   cd MControlCenterExtra
   ```
2. Unpack the installer bundle:
   ```bash
   tar -xvf MControlCenter-0.5.2-bin.tar.gz
   cd MControlCenter-0.5.2-bin
   ```
3. Run the installer:
   ```bash
   sudo ./install.sh
   ```
4. To uninstall:
   ```bash
   sudo ./uninstall.sh
   ```

---

### Building From Source

If you want to compile MControlCenter Extra yourself:

#### 1. Install Build Dependencies
- **Debian / Ubuntu / Mint:**
  ```bash
  sudo apt install build-essential cmake qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libqt6svg6-dev
  ```
- **Fedora:**
  ```bash
  sudo dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qttools-devel
  ```
- **Arch Linux:**
  ```bash
  sudo pacman -S base-devel cmake qt6-base qt6-tools
  ```

#### 2. Build and Package
Run the build and packaging scripts from the project root:
```bash
./scripts/build.sh
./scripts/create_installer.sh
```

#### 3. Install
```bash
tar -xvf MControlCenter-0.5.2-bin.tar.gz
cd MControlCenter-0.5.2-bin
sudo ./install.sh
```

---

## Launch on Session Startup (Autostart)

To have MControlCenter Extra start minimized in the background on login:

```bash
cp /usr/share/applications/mcontrolcenter.desktop ~/.config/autostart/mcontrolcenter.desktop
```

---

## Extra Tweaks

### Dedicated GPUs (NVIDIA / AMD)
- **NVIDIA:** For dynamic power limits to adapt to performance profiles, install the proprietary NVIDIA driver and enable `nvidia-powerd`:
  ```bash
  sudo systemctl enable --now nvidia-powerd
  ```
- **AMD:** Laptops with AMD Advantage dGPUs manage SmartShift and power limits natively out of the box.

### RGB Keyboards
Laptops with per-key or multi-zone RGB keyboards interface over USB. For full RGB customizations, use [OpenRGB](https://openrgb.org/). Single-color backlit keyboards (e.g., white backlight) are controlled natively by this application.

---

## Credits & License

- Original project by [Dmitry Serov](https://github.com/dmitry-s93/MControlCenter).
- Enhanced and maintained by [SirEntr0pyy](https://github.com/SirEntr0pyy/MControlCenterExtra).
- Distributed under the [GNU General Public License v3 (GPL-3.0)](LICENSE).
