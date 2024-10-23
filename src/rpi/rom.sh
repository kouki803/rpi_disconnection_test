#!/bin/bash

# ファイルのパス
CONFIG_FILE="/etc/overlayroot.conf"

# 引数チェック
if [ "$1" == "enable" ]; then
    # ROM化を有効にする
    sudo sed -i 's/^overlayroot=.*$/overlayroot="tmpfs"/' $CONFIG_FILE
    echo "ROM mode enabled"
    sudo reboot

elif [ "$1" == "disable" ]; then
    # ROM化を無効にする
    sudo sed -i 's/^overlayroot=.*$/overlayroot=""/' $CONFIG_FILE
    echo "ROM mode disabled"

else
    # help
    echo "使用方法: $0 [enable|disable]"
    exit 1
fi