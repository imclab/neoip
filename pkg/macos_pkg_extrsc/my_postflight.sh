#! /bin/sh
# this is the script 
# - "the postflight script is run after files have been installed.
#    If the script does not return 0, Installer will declare the
#    installion failed. " - from iceberg documentation

# make the symlink to get it started at boot time
# - only for SYSBOOT apps
ln -sf /Applications/neoip-webpack/org.neoip.webpack.plist /Library/LaunchDaemons


# Start the apps
launchctl load /Library/LaunchDaemons/org.neoip.webpack.plist
launchctl start org.neoip.webpack

#exit 0




