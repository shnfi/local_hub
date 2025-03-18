#!/bin/bash

distro=$(grep '^ID=' /etc/os-release | cut -d= -f2 | tr -d '"')

ubuntu_installation()
{
    sudo apt update
    sudo apt install -y qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools qt6-qmake qt6-declarative-dev qt6-multimedia-dev qt6-webengine-dev qt6-svg-dev qt6-connectivity-dev qt6-quick3d-dev build-essential cmake ninja-build
}

fedora_installation()
{
    sudo dnf install -y qt6-qtbase-devel qt6-qttools-devel qt6-qtsvg-devel qt6-qtwebengine-devel qt6-qtmultimedia-devel qt6-qtconnectivity-devel qt6-qtquick3d-devel cmake gcc-c++ make ninja-build
}

arch_installation()
{
    sudo pacman -Syu --needed qt6-base qt6-tools qt6-declarative qt6-multimedia qt6-webengine qt6-svg qt6-connectivity qt6-quick3d cmake gcc make ninja
}

if [[ $distro == "fedora" || $distro == "rhel" ]]; then
	fedora_installation
elif [[ $distro == "debian" || $distro == "ubuntu" || $distro == "linuxmint" ]]; then
	ubuntu_installation
elif [[ $distro == "arch" ]]; then
	arch_installation
else
	echo "Please install lsb_release module for distro identification!"
fi
