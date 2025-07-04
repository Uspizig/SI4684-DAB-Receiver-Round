[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/PE5PVB/TEF-Nextion-Multiband#contributing)
[![License](https://img.shields.io/badge/license%20-%20GNU_GPLv3-GPLv3?color=blue)](https://github.com/PE5PVB/TEF-Nextion-Multiband/blob/main/LICENSE)

# Note:
The version in the repository is an ongoing development. It could and will contain bugs. To make sure you use the latest fully tested firmware, check the releases!
RIGHT NOW only KiCad files and even these not finished.

# SI4684 DAB receiver
Advanced DAB Tuner software for Skyworks SI4684 tuner with ESP32 board and a round color LCD or AMOLED\
This is a fork of the famous PE5PVB SI4684 Project, unfinished and right now only a brainstroming.
Check a more developed project on the origin source
More information: https://www.pe5pvb.nl/

# Building instructions
On Youtube PE5PVB published a video how to build your own radio.

In English:
https://www.youtube.com/watch?v=C_xd0h_HTuU

In Dutch:
https://www.youtube.com/watch?v=wV3G2J327qg

# Libraries
These are the libraries used for this project:
- https://github.com/Bodmer/TFT_eSPI
- https://github.com/uzi18/TFT_eSPI
- https://github.com/Bodmer/JPEGDecoder
- https://github.com/bitbank2/PNGdec


Use these settings in the TFT_eSPI library:
```
#define ILI9341_DRIVER
#define TFT_CS          5
#define TFT_DC          4
#define TFT_RST         17
#define SPI_FREQUENCY   50000000
#define SMOOTH_FONT
```
# Buttons
A brief instruction for the buttons:
- Top encoder: Choose frequency or memory channel
- Bottom encoder: Choose service or set headphones volume
- Top button: Short press: Service information, Long press: Stand-by mode.
- Middle button: Short press: Set mode, Long press: Open menu.
- Lower button: Toggle Slideshow view.
  
## Contributing
I'm open for a new ideas in our project. Feel free to share your thoughts in [Discussions](https://github.com/PE5PVB/SI4684-DAB-Receiver/discussions).\
You can also contribute your own code using [Pull Requests](https://github.com/PE5PVB/SI4684-DAB-Receiver/pulls). We will review it and merge into main branch.

You are using my software and you found a difficulty? Please create new [issue](https://github.com/PE5PVB/SI4684-DAB-Receiver/issues) and describe your problem.

I also would like to invite you to join our Discord community where we share our ideas and help each other with some issues.\
[<img alt="Join the TEF6686 Discord community!" src="https://i.imgur.com/lI9Tuxf.png" height="120">](https://discord.gg/ZAVNdS74mC)  

Special thanks to all [contributors](https://github.com/PE5PVB/TEF-Nextion-Multiband/graphs/contributors). You are awesome! ❤️
## License
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. 

## If you like this software
<a href="https://www.buymeacoffee.com/pe5pvb"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=☕&slug=pe5pvb&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>
