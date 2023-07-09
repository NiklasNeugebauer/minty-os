# minty-os
A fresh multi-functional Watchy OS

This is a Watchy firmware that is mainly meant for myself and thus not necessarily optimized for ease of use or general use.
Nonetheless, feel free to use this as you wish and leave an issue if you have ideas for improvements.
A lot of it makes heavy use of the original Watchy code by SQFMI, but the general code structure and some details are altered.

## Design Principles

### Modularity
One of my main goals was support for multiple watch faces.
Right now, there is a WatchFace class interface, which do what their names suggest:
draw - draws the image of the watch face
handleInput - handles a button combination input

Upon wakeup the base system calls these functions of a single watch face.
Multiple watch face support is achieved through a tree of WatchFace child classes.
At its base lives the AppSwitcher, which handles long presses of the UP and DOWN buttons by switching faces.
Other inputs are relayed to the current face.
Its draw function simply calls the draw function of the current face.

New faces are added simply by adding them to the AppSwitcher.
Something like a menu could similarly be implemented as another face with child faces.

### Complex inputs
Upon any button press the InputHandler waits for the input to finish.
Upon termination it creates an action which assigns one of (UNPRESSED | SHORT_PRESS | LONG_PRESS) to all buttons.
The handler is terminated by finishing a long press or by letting go of all buttons.

### Functionality as Services
A Service has three main functions:
It keeps a state and can be called to perform an action or request information.
For example, the WiFi Service keeps track of known networks and can be requested to connect and disconnect to WiFi.
Services are meant to be higher level than a simple hardware abstraction and can group together similar functionality.
For example, the TimeService can both provide the current time from the RTC and perform updates via NTP.

New Services should be added into the ServiceManager.
It initializes its services once at boot time and updates them regularly.
