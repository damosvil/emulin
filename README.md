# emulin
LIN bus node emulator

The main milestones of this project are:
* Create and upload open hardware schematics and gerber files for a LIN interface.
* Create another (yes OMG!) open source and robust ldf editor.
* Create another open source, fast and robust LIN node emulator / analyzer.

Prerequisites:
* Eclipse CDT Photon - Project's Official IDE
    * Download from: 'https://www.eclipse.org/downloads/packages/release/2018-09/r/eclipse-ide-cc-developers'
* Pkg-config Eclipse plugin
    * Install from Eclipse marketplace
* GTK3 - For user interfaces (https://developer.gnome.org/gtk3/stable/gtk-compiling.html).
    * Install:
         * 'sudo apt install gtk+-3.0'
    * Check the commands:
         * 'pkg-config --cflags gtk+-3.0'
         * 'pkg-config --libs gtk+-3.0'
* Glade - To design the user interfaces
    * Install 'sudo apt install glade'
* Manpages
    * Install 'apt-get install manpages-dev'
    * Install 'apt-get install manpages-posix-dev'


The aproach to meet the goals is to start with a basic emulator (only to emulate a LIN 2.1 compliant slave node), and little by little adding more features.
