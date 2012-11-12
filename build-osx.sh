#!/bin/sh

qmake
make
macdeployqt CirclePackings.app
ACTUAL_SIZE=`du -sm "CirclePackings.app" | awk '{print $1}'` 
DISK_IMAGE_SIZE=$(expr $ACTUAL_SIZE + 20)
VOLUME_NAME="CirclePackings"
DMG_TEMP_NAME="CirclePackings.tmp"
VOLUME_ICON_FILE="images/icon-osx.icns"

rm -f ${DMG_TEMP_NAME}.dmg
hdiutil create -srcfolder "CirclePackings.app" -volname "${VOLUME_NAME}" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${DISK_IMAGE_SIZE}m "${DMG_TEMP_NAME}"
MOUNT_DIR="/Volumes/${VOLUME_NAME}"
echo "Mount directory: $MOUNT_DIR"
DEV_NAME=$(hdiutil attach -readwrite -noverify -noautoopen "${DMG_TEMP_NAME}.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')

echo "making link to Applications dir"
ln -s /Applications "$MOUNT_DIR/Applications"

echo "Copying volume icon file '$VOLUME_ICON_FILE'..."
cp "$VOLUME_ICON_FILE" "$MOUNT_DIR/.VolumeIcon.icns"
SetFile -c icnC "$MOUNT_DIR/.VolumeIcon.icns"

# make sure it's not world writeable
echo "Fixing permissions..."
chmod -Rf go-w "${MOUNT_DIR}" &> /dev/null || true
echo "Done fixing permissions."

# unmount
echo "Unmounting disk image..."
hdiutil detach "${DEV_NAME}"

echo "Compressing disk image..."
hdiutil convert "${DMG_TEMP_NAME}.dmg" -format UDZO -imagekey zlib-level=9 -o "CirclePackings.dmg"
rm -f "${DMG_TEMP_NAME}.dmg"
