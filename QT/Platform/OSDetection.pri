# This file is to add features deemed necessary that don't exist in qmake.conf
# There is no linux setting in qmake.conf

# All supported platforms that require tweaks
PLATFORM_NAME="generic"
unix: PLATFORM_NAME="Linux"
mac: PLATFORM_NAME="Macosx"
win32: PLATFORM_NAME="Windows"

!equals(PLATFORM_NAME, "generic"): include($$PLATFORM_NAME".pri")

