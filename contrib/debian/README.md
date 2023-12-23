
Debian
====================
This directory contains files used to package vishaid/vishai-qt
for Debian-based Linux systems. If you compile vishaid/vishai-qt yourself, there are some useful files here.

## vishai: URI support ##


vishai-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install vishai-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your vishai-qt binary to `/usr/bin`
and the `../../share/pixmaps/vishai128.png` to `/usr/share/pixmaps`

vishai-qt.protocol (KDE)

