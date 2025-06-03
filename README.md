ZenType - Typing Speed Trainer
==============================

Description
-----------
ZenType is a lightweight typing speed and accuracy training game built with C++, SFML, and cURL.
It displays random words fetched from the internet, and the player must type them as fast and
accurately as possible.

Press [SPACE] to start or pause the test. Press [F11] to toggle fullscreen.

Features
--------
- Dynamic word list loaded from GitHub on runtime
- Word filtering (removes words shorter than 5 letters)
- Random word shuffling for unique experience every time
- Real-time stats:
  - Accuracy (%)
  - Typing speed (characters per second)
  - Typed words count
- SFML graphical interface with scalable fonts and window resizing
- Font loaded directly from memory (Arial.ttf)
- Fullscreen toggle with F11
- Pause and resume with SPACE key

Requirements
------------
- Windows OS
- Internet connection (for word list download)
- C++17 compatible compiler
- Dependencies:
  - SFML (Graphics, Window, System)
  - libcurl
  - arial.h (header containing embedded Arial.ttf as binary array)

Controls
--------
- SPACE: Start / Pause the timer
- F11: Toggle fullscreen mode
- Any key: Type the letters of the current word
- Close Button: Exit the game

Technical Details
-----------------
- Words are fetched from:
  https://raw.githubusercontent.com/SigmaGit1eR/ZenType-Words/refs/heads/main/ZenType_Words.txt
- Words are parsed by newline
- All words <= 4 characters are removed
- Word order is randomized using std::shuffle
- Font is loaded using `font.loadFromMemory()` from an embedded binary
- Performance and stats calculated using `sf::Clock` and character counters

File Structure
--------------
- main.cpp: Main application logic
- arial.h: Header with binary font data (arial_ttf, arial_ttf_len)
- README.txt: This file

Compilation
-----------
Make sure to link against the following SFML and cURL libraries:
- sfml-graphics
- sfml-window
- sfml-system
- libcurl

Example g++ command:
g++ main.cpp -o ZenType -lsfml-graphics -lsfml-window -lsfml-system -lcurl

Legal & License
---------------
This is a hobby/educational project.

Ensure the usage of the embedded Arial font complies with licensing terms. You can replace the font with your own if needed.

Author
------
Created by: SigmaGit1er
Language: C++17
Frameworks: SFML 2.6, cURL
