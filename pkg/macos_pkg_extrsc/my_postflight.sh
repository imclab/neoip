#! /bin/sh
# this is the script 
# - "the postflight script is run after files have been installed.
#    If the script does not return 0, Installer will declare the
#    installion failed. " - from iceberg documentation

# make the symlink to get it started at boot time
# - only for SYSBOOT apps
ln -sf /Applications/neoip-webpeer/pkg_extrsc/org.neoip.webpeer.plist /Library/LaunchDaemons

# TODO what if it is already installed ?
# - i should unload the previous one if it is loaded
# - i should stop the previous one if it is running

# Start the apps
launchctl load /Library/LaunchDaemons/org.neoip.webpeer.plist
launchctl start org.neoip.webpeer

#exit 0




