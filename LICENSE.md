LICENSING
=========

The licensing of this library is a bit of a pain, as it draws from
so many other open source projects. As code gets merged, cleaned up,
refactored, and then expanded upon, it can be hard to determine who
wrote which line. Attempting to maintain every single copyright header
was bulky and of questionable value.

So, along with the acknowledgements in the README.md file, I've attempted
to list here the multiple sources that this code was pulled from, the authors
or contributors, and the license that was chosen by them.

I've chosen to license my contributions to this library under the LGPL 2.1.
After reading http://www.gnu.org/licenses/why-not-lgpl.html and considering
the argument against using the LGPL, I'm using it for three reasons:

1. Compatibility with the LGPL 2.1 license used for Arduino's core libraries
2. Plenty of free and non-free OLED libraries already exist, meaning that it
    does not "give free software any particular advantage"
3. Section 7 explicitly details how to properly credit source that is combined
    with other source in a library

* [SSD1306-display-driver-for-Arduino](https://github.com/Hyvok/SSD1306-display-driver-for-Arduino) by Kalle Hyvönen (Hyvok)
  * Based on http://www.dgkelectronics.com/using-a-neat-little-oled-display-with-an-arduino/
  and http://www.dgkelectronics.com/library-for-controlling-ssd1306-based-oled-displays/
  * No license given, and no copyright listed.

* [SSD1306NoBuffer](https://github.com/pocketmoon/SSD1306NoBuffer) by Rob (pocketmoon)
  * No license given, and no copyright listed.
