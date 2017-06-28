# University of Alberta UAV Imaging #

This repository contains (or will contain) source code and relevant content for the UAV Imaging platform as part of UAV research developed at the University of Alberta

## LICENSE INFORMATION ##

Copyright (C) 2017  University of Alberta

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
The full text is available here: [http://www.gnu.org/licenses/gpl-2.0.html]

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

## Runtime Dependencies ##

The following packages are required to run the scripts:

- v4l-utils
- gstreamer-1.0
- jq
- inotify

On a Debian / Ubuntu system, the following command line instructions could be used to install all required packages:

```
# v4l-utils
sudo apt-get install v4l-utils 

# gstreamer-1.0
sudo apt-get install gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly

# jq
sudo apt-get install jq

# inotify
sudo apt-get install inotify-tools
```

## Repository Structure ##
The structure of the repository is as follows.

***camera***

* Contains the code to interface with individual cameras.

***spec3***

* Contains the code to interface with the 3-cameras multispectral system.

***tests***

* Contains testing code under development.

***utilities***

* Contains a series of utility scripts.