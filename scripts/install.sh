#!/bin/bash -e

BIN_PATH='/usr/bin/'
LIB_EXEC_PATH='/usr/libexec/'
SCALABLE_ICONS_PATH='/usr/share/icons/hicolor/scalable/apps/'
SHORTCUTS_PATH='/usr/share/applications/'
DBUS_SYSTEM_PATH='/usr/share/dbus-1/system.d/'
DBUS_SERVICES_PATH='/usr/share/dbus-1/system-services/'

APP_DIR='./app/'

APP_BIN='mcontrolcenter'
SVG_ICON='mcontrolcenter.svg'
SHORTCUT='mcontrolcenter.desktop'
HELPER_BIN='mcontrolcenter-helper'
DBUS_CONF='mcontrolcenter-helper.conf'
DBUS_SERVICE='mcontrolcenter.helper.service'

echo "Installation start"

install -vDm755 $APP_DIR$APP_BIN $BIN_PATH$APP_BIN

rm -fv /home/$SUDO_USER/.local/share/applications/$SHORTCUT
install -vDm644 $APP_DIR$SHORTCUT $SHORTCUTS_PATH$SHORTCUT

install -vDm644 $APP_DIR$SVG_ICON $SCALABLE_ICONS_PATH$SVG_ICON

install -vDm755 $APP_DIR$HELPER_BIN $LIB_EXEC_PATH$HELPER_BIN

install -vDm644 $APP_DIR$DBUS_CONF $DBUS_SYSTEM_PATH$DBUS_CONF

install -vDm644 $APP_DIR$DBUS_SERVICE $DBUS_SERVICES_PATH$DBUS_SERVICE

# Install ALSA mute LED udev rule so it persists across reboots
echo "Installing ALSA mute LED udev rule..."
cat << 'EOF' > /etc/udev/rules.d/99-msi-mute-led.rules
ACTION=="add", SUBSYSTEM=="sound", KERNEL=="controlC[0-9]*", RUN+="/bin/sh -c 'for d in /sys/class/sound/ctl-led/speaker/card*/attach; do [ -w \"$$d\" ] && echo \"Master Playback Switch\" > \"$$d\"; done 2>/dev/null || true'"
EOF
chmod 644 /etc/udev/rules.d/99-msi-mute-led.rules
udevadm control --reload-rules 2>/dev/null || true
udevadm trigger --subsystem-match=sound 2>/dev/null || true

# Apply mute LED attachment immediately to any active sound cards
for d in /sys/class/sound/ctl-led/speaker/card*/attach; do
    if [ -w "$d" ]; then
        echo "Master Playback Switch" > "$d" 2>/dev/null || true
    fi
done

# Restart running helper and app so new binaries take effect immediately
busctl call mcontrolcenter.helper / dmitry_s93.MControlCenter quit 2>/dev/null || true
killall -9 mcontrolcenter-helper 2>/dev/null || true
killall -9 mcontrolcenter 2>/dev/null || true

echo "Installation was successful"

