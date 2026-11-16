#!/bin/bash -xe
echo "Creating the installer"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

VERSION=$(grep -oP "(?<=MControlCenter VERSION )[0-9.]*" "$REPO_ROOT/CMakeLists.txt")

DIST_DIR="$REPO_ROOT/MControlCenter-$VERSION-bin"
APP_DIR="$DIST_DIR/app"

rm -rf "$DIST_DIR"
rm -f "$DIST_DIR.tar.gz"

mkdir -p "$DIST_DIR"
mkdir -p "$APP_DIR"

cp "$REPO_ROOT/build/mcontrolcenter" "$APP_DIR"
cp "$REPO_ROOT/build/helper/mcontrolcenter-helper" "$APP_DIR"

strip --strip-unneeded "$APP_DIR/mcontrolcenter"
strip --strip-unneeded "$APP_DIR/mcontrolcenter-helper"

cp "$REPO_ROOT/src/helper/mcontrolcenter-helper.conf" "$REPO_ROOT/src/helper/mcontrolcenter.helper.service" "$APP_DIR"
cp "$REPO_ROOT/resources/mcontrolcenter.desktop" "$REPO_ROOT/resources/mcontrolcenter.svg" "$APP_DIR"

cp "$SCRIPT_DIR/install.sh" "$SCRIPT_DIR/uninstall.sh" "$DIST_DIR"

cd "$REPO_ROOT"
tar -czvf "$DIST_DIR.tar.gz" "MControlCenter-$VERSION-bin"

rm -rf "$DIST_DIR"

echo "Installer created successfully"
