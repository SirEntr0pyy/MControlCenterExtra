# MControlCenter Extra

**MControlCenter Extra** is an enhanced, highly-optimized fork of [MControlCenter](https://github.com/MControlSuite/MControlCenter) (originally by Dmitry Serov, now maintained by MControlSuite) designed for MSI laptops running GNU/Linux, with specific optimizations for Debian, Ubuntu, Fedora, and Arch Linux.

> **Note:** Tested on the MSI Modern 14 C7M with Debian 13.

<img width="1500" height="1332" alt="MCC3" src="https://github.com/user-attachments/assets/75b5ee18-ed99-4a89-b9fe-1ec0078760e8" />

---

## 🌟 What's New in this Fork?

This fork introduces performance optimizations (zero UI lag), modern MSI hardware fixes (e.g. Modern 14 C7M / MS-14JK), dynamic lighting effects for single-color keyboards, persistent audio mute LED, and full Debian support.

👉 **For the complete list of changes and fixes, see [CHANGES.txt](CHANGES.txt).**

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
- Tested with the MSI Modern 14 C7M with Debian 13 (MS-14JK, AMD Barcelo / Ryzen 7000 series).

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
   git clone https://github.com/SirEntr0pyy/MControlCenterXtra.git
   cd MControlCenterXtra
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

- Upstream project by [MControlSuite](https://github.com/MControlSuite/MControlCenter) (originally by [Dmitry Serov](https://github.com/dmitry-s93)).
- Enhanced and maintained by [SirEntr0pyy](https://github.com/SirEntr0pyy/MControlCenterXtra).
- Distributed under the [GNU General Public License v3 (GPL-3.0)](LICENSE).
