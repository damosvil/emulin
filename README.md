# emulin
LIN bus node emulator

The main milestones of this project are:
* Create and upload open hardware schematics and gerber files for a LIN interface.
* Create another (yes OMG!) open source and robust ldf editor.
* Create another open source, fast and robust LIN node emulator / analyzer.

Prerequisites:
* Eclipse CDT Photon - Project's Official IDE
* Pkg-config Eclipse plugin 
* GTK3 - For user interfaces
* Glade - To design the user interfaces
* en_US locale - Install it: "sudo locale-gen en_US"

The aproach to meet the goals is to start with a basic emulator (only to emulate a LIN 2.1 compliant slave node), and little by little adding more features.
