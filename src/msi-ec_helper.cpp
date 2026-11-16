/* Copyright (C) 2022  Jérôme Lécuyer, Dmitry Serov
 *
 * This file is part of MControlCenter.
 *
 * MControlCenter is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * MControlCenter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MControlCenter. If not, see <https://www.gnu.org/licenses/>.
 */

#include "msi-ec_helper.h"
#include "helper/service.h"
#include "mainwindow.h"

#include <QDBusReply>
#include <QFile>

static inline QString readSysfs(const QString &path) {
    if (QFile file(path); file.open(QIODevice::ReadOnly)) {
        return QString::fromUtf8(file.readAll()).trimmed();
    }
    return QString();
}

static inline bool existsSysfs(const QString &path) {
    return QFile::exists(path);
}

const QString msi_ec_webcam = "/sys/devices/platform/msi-ec/webcam";
const QString msi_ec_webcam_block = "/sys/devices/platform/msi-ec/webcam_block";
const QString msi_ec_fn_key = "/sys/devices/platform/msi-ec/fn_key";
const QString msi_ec_win_key = "/sys/devices/platform/msi-ec/win_key";
const QString msi_ec_cooler_boost = "/sys/devices/platform/msi-ec/cooler_boost";
const QString msi_ec_shift_mode = "/sys/devices/platform/msi-ec/shift_mode";
const QString msi_ec_available_shift_modes = "/sys/devices/platform/msi-ec/available_shift_modes";
const QString msi_ec_super_battery = "/sys/devices/platform/msi-ec/super_battery";
const QString msi_ec_fan_mode = "/sys/devices/platform/msi-ec/fan_mode";
const QString msi_ec_available_fan_modes = "/sys/devices/platform/msi-ec/available_fan_modes";
const QString msi_ec_fw_version = "/sys/devices/platform/msi-ec/fw_version";
const QString msi_ec_fw_release_date = "/sys/devices/platform/msi-ec/fw_release_date";
const QString msi_ec_cpu_realtime_temperature = "/sys/devices/platform/msi-ec/cpu/realtime_temperature";
const QString msi_ec_cpu_realtime_fan_speed = "/sys/devices/platform/msi-ec/cpu/realtime_fan_speed";
const QString msi_ec_cpu_basic_fan_speed = "/sys/devices/platform/msi-ec/cpu/basic_fan_speed";
const QString msi_ec_gpu_realtime_temperature = "/sys/devices/platform/msi-ec/gpu/realtime_temperature";
const QString msi_ec_gpu_realtime_fan_speed = "/sys/devices/platform/msi-ec/gpu/realtime_fan_speed";
const QString msi_ec_bat1_start_threshold = "/sys/class/power_supply/BAT1/charge_control_start_threshold";
const QString msi_ec_bat1_end_threshold = "/sys/class/power_supply/BAT1/charge_control_end_threshold";
const QString msi_ec_bat1_capacity = "/sys/class/power_supply/BAT1/capacity";
const QString msi_ec_bat1_status = "/sys/class/power_supply/BAT1/status";
const QString msi_ec_kbd_backlight_brightness = "/sys/class/leds/msiacpi::kbd_backlight/brightness";

MsiEcHelper::MsiEcHelper() {
    if (!QDBusConnection::systemBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus system bus");
        return;
    }
    iface = new QDBusInterface(SERVICE_NAME, "/msi_ec", INTERFACE_NAME_MSI_EC, QDBusConnection::systemBus());
}

bool MsiEcHelper::isMsiEcModuleLoaded() {
    if (existsSysfs(msi_ec_fw_version))
        return true;
    return getValue<bool>("isMsiEcModuleLoaded", false);
}

//////////////// webcam ////////////////

bool MsiEcHelper::hasWebcam() const {
    if (existsSysfs(msi_ec_webcam))
        return true;
    return getValue<bool>("hasWebcam", false);
}

bool MsiEcHelper::getWebcam() const {
    QString val = readSysfs(msi_ec_webcam);
    if (!val.isEmpty())
        return val == "on";
    return getValue<bool>("getWebcam", false);
}

void MsiEcHelper::setWebcam(bool enabled) const {
    setValue<bool>("setWebcam", enabled);
}

//////////////// webcam_block ////////////////

bool MsiEcHelper::hasWebcamBlock() const {
    if (existsSysfs(msi_ec_webcam_block))
        return true;
    return getValue<bool>("hasWebcamBlock", false);
}

bool MsiEcHelper::getWebcamBlock() const {
    QString val = readSysfs(msi_ec_webcam_block);
    if (!val.isEmpty())
        return val == "on";
    return getValue<bool>("getWebcamBlock", false);
}

void MsiEcHelper::setWebcamBlock(bool enabled) const {
    setValue<bool>("setWebcamBlock", enabled);
}

//////////////// fn_win_swap ////////////////

bool MsiEcHelper::hasFnWinSwap() const {
    if (existsSysfs(msi_ec_fn_key))
        return true;
    return getValue<bool>("hasFnWinSwap", false);
}

bool MsiEcHelper::getFnWinSwap() const {
    QString val = readSysfs(msi_ec_fn_key);
    if (!val.isEmpty())
        return val == "right";
    return getValue<bool>("getFnWinSwap", false);
}

void MsiEcHelper::setFnWinSwap(bool swap) const {
    setValue<bool>("setFnWinSwap", swap);
}

//////////////// cooler_boost ////////////////

bool MsiEcHelper::hasCoolerBoost() const {
    if (existsSysfs(msi_ec_cooler_boost))
        return true;
    return getValue<bool>("hasCoolerBoost", false);
}

bool MsiEcHelper::getCoolerBoost() const {
    QString val = readSysfs(msi_ec_cooler_boost);
    if (!val.isEmpty())
        return val == "on";
    return getValue<bool>("getCoolerBoost", false);
}

void MsiEcHelper::setCoolerBoost(bool enable) const {
    setValue<bool>("setCoolerBoost", enable);
}

//////////////// shift_mode ////////////////
bool MsiEcHelper::hasShiftMode() const {
    if (existsSysfs(msi_ec_shift_mode))
        return true;
    return getValue<bool>("hasShiftMode", false);
}

QList<shift_mode> MsiEcHelper::getAvailableShiftModes() const {
    QString val = readSysfs(msi_ec_available_shift_modes);
    if (val.isEmpty())
        val = getValue<QString>("getAvailableShiftModes", "");
    QList<QString> modesStr = val.split("\n");
    QList<shift_mode> shiftModes;
    for (QString mode : modesStr) {
        if(mode == "eco")
            shiftModes.append(shift_mode::eco_mode);
        else if (mode == "comfort")
            shiftModes.append(shift_mode::comfort_mode);
        else if (mode == "sport")
            shiftModes.append(shift_mode::sport_mode);
        else if (mode == "turbo")
            shiftModes.append(shift_mode::turbo_mode);
    }
    return shiftModes;
}

shift_mode MsiEcHelper::getShiftMode() const {
    QString mode = readSysfs(msi_ec_shift_mode);
    if (mode.isEmpty())
        mode = getValue<QString>("getShiftMode", "");
    if(mode == "eco")
        return shift_mode::eco_mode;
    else if (mode == "comfort")
        return shift_mode::comfort_mode;
    else if (mode == "sport")
        return shift_mode::sport_mode;
    else if (mode == "turbo")
        return shift_mode::turbo_mode;
    else
        return shift_mode::unknown_mode;
}

void MsiEcHelper::setShiftMode(shift_mode mode) const {
    QString modeStr;
    switch(mode) {
        case shift_mode::eco_mode:
            modeStr = "eco";
            break;
        case shift_mode::comfort_mode:
            modeStr = "comfort";
            break;
        case shift_mode::sport_mode:
            modeStr = "sport";
            break;
        case shift_mode::turbo_mode:
            modeStr = "turbo";
            break;
        case shift_mode::unknown_mode:
            return;
        }
    setValue<QString>("setShiftMode", modeStr);
}

//////////////// super_battery ////////////////

bool MsiEcHelper::hasSuperBattery() const {
    if (existsSysfs(msi_ec_super_battery))
        return true;
    return getValue<bool>("hasSuperBattery", false);
}

bool MsiEcHelper::getSuperBattery() const {
    QString val = readSysfs(msi_ec_super_battery);
    if (!val.isEmpty())
        return val == "on";
    return getValue<bool>("getSuperBattery", false);
}

void MsiEcHelper::setSuperBattery(bool enable) const {
    setValue<bool>("setSuperBattery", enable);
}

//////////////// fan_mode ////////////////

bool MsiEcHelper::hasFanMode() const {
    if (existsSysfs(msi_ec_fan_mode))
        return true;
    return getValue<bool>("hasFanMode", false);
}

QList<fan_mode> MsiEcHelper::getAvailableFanModes() const {
    QString val = readSysfs(msi_ec_available_fan_modes);
    if (val.isEmpty())
        val = getValue<QString>("getAvailableFanModes", "");
    QList<QString> modesStr = val.split("\n");
    QList<fan_mode> fanModes;
    for (QString mode : modesStr) {
        if(mode == "auto")
            fanModes.append(fan_mode::auto_fan_mode);
        else if (mode == "silent")
            fanModes.append(fan_mode::silent_fan_mode);
        else if (mode == "basic")
            fanModes.append(fan_mode::basic_fan_mode);
        else if (mode == "advanced")
            fanModes.append(fan_mode::advanced_fan_mode);
    }
    return fanModes;
}

fan_mode MsiEcHelper::getFanMode() const {
    QString mode = readSysfs(msi_ec_fan_mode);
    if (mode.isEmpty())
        mode = getValue<QString>("getFanMode", "");
    if (mode == "auto")
        return fan_mode::auto_fan_mode;
    else if (mode == "silent")
        return fan_mode::silent_fan_mode;
    else if (mode == "basic")
        return fan_mode::basic_fan_mode;
    else if (mode == "advanced")
        return fan_mode::advanced_fan_mode;
    else
        return fan_mode::unknown_fan_mode;
}

void MsiEcHelper::setFanMode(fan_mode mode) const {
    QString modeStr;
    switch (mode) {
        case fan_mode::auto_fan_mode:
            modeStr = "auto";
            break;
        case fan_mode::silent_fan_mode:
            modeStr = "silent";
            break;
        case fan_mode::basic_fan_mode:
            modeStr = "basic";
            break;
        case fan_mode::advanced_fan_mode:
            modeStr = "advanced";
            break;
        default:
            return;
    }
    setValue<QString>("setFanMode", modeStr);
}

//////////////// fw_version ////////////////

QString MsiEcHelper::getFWVersion() const {
    QString val = readSysfs(msi_ec_fw_version);
    if (!val.isEmpty())
        return val;
    return getValue<QString>("getFWVersion", "");
}

//////////////// fw_release_date ////////////////

QString MsiEcHelper::getFWReleaseDate() const {
    QString val = readSysfs(msi_ec_fw_release_date);
    if (!val.isEmpty())
        return val;
    return getValue<QString>("getFWReleaseDate", "");
}

//////////////// CPU ////////////////

// cpu/realtime_temperature 0-100 (celsius scale)
bool MsiEcHelper::hasCPURealtimeTemperature() const {
    if (existsSysfs(msi_ec_cpu_realtime_temperature))
        return true;
    return getValue<bool>("hasCPURealtimeTemperature", false);
}

int MsiEcHelper::getCPURealtimeTemperature() const {
    QString val = readSysfs(msi_ec_cpu_realtime_temperature);
    if (!val.isEmpty())
        return val.toInt();
    return getValue<int>("getCPURealtimeTemperature", -1);
}

// cpu/realtime_fan_speed 0-100 (percent)
bool MsiEcHelper::hasCPURealtimeFanSpeed() const {
    if (existsSysfs(msi_ec_cpu_realtime_fan_speed))
        return true;
    return getValue<bool>("hasCPURealtimeFanSpeed", false);
}

int MsiEcHelper::getCPURealtimeFanSpeed() const {
    QString val = readSysfs(msi_ec_cpu_realtime_fan_speed);
    if (!val.isEmpty())
        return val.toInt();
    return getValue<int>("getCPURealtimeFanSpeed", -1);
}

// cpu/basic_fan_speed 0-100 (percent)
bool MsiEcHelper::hasCPUBasicFanSpeed() const {
    if (existsSysfs(msi_ec_cpu_basic_fan_speed))
        return true;
    return getValue<bool>("hasCPUBasicFanSpeed", false);
}

int MsiEcHelper::getCPUBasicFanSpeed() const {
    QString val = readSysfs(msi_ec_cpu_basic_fan_speed);
    if (!val.isEmpty())
        return val.toInt();
    return getValue<int>("getCPUBasicFanSpeed", -1);
}

void MsiEcHelper::setCPUBasicFanSpeed(int value) const {
    setValue<int>("setCPUBasicFanSpeed", value);
}

//////////////// GPU ////////////////

// gpu/realtime_temperature 0-100 (celsius scale)
bool MsiEcHelper::hasGPURealtimeTemperature() const {
    if (existsSysfs(msi_ec_gpu_realtime_temperature))
        return true;
    return getValue<bool>("hasGPURealtimeTemperature", false);
}

std::optional<int> MsiEcHelper::getGPURealtimeTemperature() const {
    QString val = readSysfs(msi_ec_gpu_realtime_temperature);
    if (!val.isEmpty())
        return val.toInt();
    return getOptionalValue<int>("getGPURealtimeTemperature");
}

// gpu/realtime_fan_speed 0-100 (percent)
bool MsiEcHelper::hasGPURealtimeFanSpeed() const {
    if (existsSysfs(msi_ec_gpu_realtime_fan_speed))
        return true;
    return getValue<bool>("hasGPURealtimeFanSpeed", false);
}

std::optional<int> MsiEcHelper::getGPURealtimeFanSpeed() const {
    QString val = readSysfs(msi_ec_gpu_realtime_fan_speed);
    if (!val.isEmpty())
        return val.toInt();
    return getOptionalValue<int>("getGPURealtimeFanSpeed");
}

//////////////// Charge control ////////////////

// BAT1/charge_control_start_threshold 0-100 (percent)
bool MsiEcHelper::hasBatteryStartThreshold() const {
    if (existsSysfs(msi_ec_bat1_start_threshold))
        return true;
    return getValue<bool>("hasBatteryStartThreshold", false);
}

int MsiEcHelper::getBatteryStartThreshold() const {
    QString val = readSysfs(msi_ec_bat1_start_threshold);
    if (!val.isEmpty())
        return val.toInt();
    return getValue<int>("getBatteryStartThreshold", -1);
}

void MsiEcHelper::setBatteryStartThreshold(int value) const {
    setValue<int>("setBatteryStartThreshold", value);
}

// BAT1/charge_control_end_threshold 0-100 (percent)
bool MsiEcHelper::hasBatteryEndThreshold() const {
    if (existsSysfs(msi_ec_bat1_end_threshold))
        return true;
    return getValue<bool>("hasBatteryEndThreshold", false);
}

int MsiEcHelper::getBatteryEndThreshold() const {
    QString val = readSysfs(msi_ec_bat1_end_threshold);
    if (!val.isEmpty())
        return val.toInt();
    return getValue<int>("getBatteryEndThreshold", -1);
}

void MsiEcHelper::setBatteryEndThreshold(int value) const {
    setValue<int>("setBatteryEndThreshold", value);
}

// BAT1/capacity 0-100 (percent)
bool MsiEcHelper::hasBatteryCapacity() const {
    if (existsSysfs(msi_ec_bat1_capacity))
        return true;
    return getValue<bool>("hasBatteryCapacity", false);
}

int MsiEcHelper::getBatteryCapacity() const {
    QString val = readSysfs(msi_ec_bat1_capacity);
    if (!val.isEmpty())
        return val.toInt();
    return getValue<int>("getBatteryCapacity", -1);
}

// BAT1/status
bool MsiEcHelper::hasBatteryStatus() const {
    if (existsSysfs(msi_ec_bat1_status))
        return true;
    return getValue<bool>("hasBatteryStatus", false);
}

QString MsiEcHelper::getBatteryStatus() const {
    QString val = readSysfs(msi_ec_bat1_status);
    if (!val.isEmpty())
        return val;
    return getValue<QString>("getBatteryStatus", "");
}

//////////////// Keyboard Backlight ////////////////

// kbd_backlight/brightness 0-3
bool MsiEcHelper::hasKeyboardBacklightBrightness() const {
    if (existsSysfs(msi_ec_kbd_backlight_brightness))
        return true;
    return getValue<bool>("hasKeyboardBacklightBrightness", false);
}

int MsiEcHelper::getKeyboardBacklightBrightness() const {
    QString val = readSysfs(msi_ec_kbd_backlight_brightness);
    if (!val.isEmpty())
        return val.toInt();
    return getValue<int>("getKeyboardBacklightBrightness", -1);
}

void MsiEcHelper::setKeyboardBacklightBrightness(int value) const {
    setValue<int>("setKeyboardBacklightBrightness", value);
}

////////////////////////////////

void MsiEcHelper::printError(QDBusError const &error) const {
    if (error.isValid())
        fprintf(stderr, "Call failed: %s\n", qPrintable(error.message()));
}

template <typename T>
inline std::optional<T> MsiEcHelper::getOptionalValue(QString method) const {
    if (QDBusReply<T> reply = iface->call(method); reply.isValid())
        return reply.value();
    printError(iface->lastError());
    return std::nullopt;
}

template <typename T>
inline T MsiEcHelper::getValue(QString method, T defaultValue) const {
    return getOptionalValue<T>(method).value_or(defaultValue);
}

template <typename T>
void MsiEcHelper::setValue(QString method, T value) const {
    iface->asyncCall(method, value);
}
