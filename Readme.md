This repository is a collection of MOOS apps and other features. It is being developed during my master degree in Electronics Engineering and Computer Science at the Instituto Tecnologico de Aeronautica (ITA) - Brazil.

In my master, I am developing a low cost autonomous surface vehicle. The ASV uses the MOOS-IvP as the navigation software.
This repository is organized as follows:
- src: This folder contains all the source code for the custom MOOS Apps necessary for running the ASV. Some apps has dependancies in the libraries eigen3, sdl2, boost and zeromq.
- missions: This folder contains the several mission files used during the master degree. This folder includes mission files for both the configuration where the boat depends on the GCS and communicates with a RF modem, as well as the files for the independent configuration with the RPi 2, communicating with the GCS with a WiFi modem.
- scripts: this folder contains some scripts to easily deploy missions
- interfaces: this folder contains human interfaces for the MOOS-IvP, such as a whole website backend for data visualization, as well some sketches for frontend apps (web, android and ios)


Any questions please email me so I can help you.
