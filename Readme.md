My moos-ivp-extend repository for an ASV
========================================

This repository is a collection of MOOS apps and other features. It is being developed during my master degree in Electronics Engineering and Computer Science at the Instituto Tecnologico de Aeronautica (ITA) - Brazil.

In my master, I am developing a low-cost autonomous surface vehicle (ASV). The ASV uses the [MOOS-IvP](<www.moos-ivp.org>) as the navigation software. 

This repository is organized as follows:

-   src: This folder contains all the source code for the custom MOOS Apps necessary for running the ASV. Some apps has dependancies in the libraries eigen3, sdl2, boost and zeromq.

-   missions: This folder contains the several mission files used during the master degree. This folder includes mission files for both the configuration where the boat depends on the GCS and communicates with a RF modem, as well as the files for the independent configuration with the RPi 2, communicating with the GCS with a WiFi modem.

-   scripts: this folder contains some scripts and sketch of scripts to easily deploy missions.

-   interfaces: this folder contains human interfaces for the MOOS-IvP, such as a whole website for data visualization written in Django with Bootstrap 3. This is easily modified if you know basic concepts of Django and HTML.

-   The folders lib, data and other files are the default of the moos-ivp. Please visit the website, see the class notes labs to see how to extend the moos-ivp to your project.

Some links for my dissertation and publications, which contains a detailed explanation of everything!!!
- http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=7490541&tag=1
- The MSc Dissertation is in the root tree of this repo

Any questions please [email](<mailto:issamattos.david@gmail.com>) me so I can help you.
