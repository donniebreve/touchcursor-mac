# touchcursor-mac
Port of the [touchcursor-linux](https://github.com/donniebreve/touchcursor-linux) project to macOS.

_Icon by Gregor Cresnar_

# Description 
This application remaps the `uiophjklnmy` keys to different movement keys when the spacebar is pressed down, allowing you to keep your hands on the home row.

```
i - up
j - left
k - down
l - right
u - home
o - end
p - backspace
h - page up
n - page down
m - del
y - insert
```

# Installation
In progress, but for now you may download this repository and build with XCode.

# Thanks to  
[Martin Stone, Touch Cursor](https://github.com/martin-stone/touchcursor): wonderful project for cursor movement when coding.


# Installation
- Create automator wrapper app
- add shell script that runs what is in 'setup.sh' script
- i added the executable to my sudo visudo path
- add automator wrapper app as login item

in sudo visudo

charltonprovatas ALL=(ALL) NOPASSWD: /Applications/touchcursor-mac.app/Contents/Resources/setup.sh
charltonprovatas ALL=(ALL) NOPASSWD: /Applications/touchcursor-mac.app/Contents/MacOS/touchcursor-mac

# Some old notes so i don't forget

Checked with the CG Folks and apparently CGEventTap stops working when the system is in secure input mode. This can be enabled by any process and can stay 'stuck', preventing taps from ever coming through until you log out or restart the machine.

This is also confirmed by the author of karabiner elements
https://github.com/pqrs-org/Karabiner-Elements/blob/main/docs/DEVELOPMENT.md#cgeventtapcreate

It's not fully working yet but I'm getting the callbacks now for when you tap a key. I just need to wire up the keycodes with the map I have

cprovatas added 2 commits on Mar 31
@cprovatas
moving to iokit since CGEventTap doesnt work when processes like logi… …
e332bee
@cprovatas
WIP sending events using IOHIDPostEvent
8843090
@cprovatas
 
Owner
Author
cprovatas commented 27 days ago
As I was implementing this, I discovered that there is an SPI called CGSSetSecureEventInput, that appears to toggle secure input mode. If this API actually works, then I wouldn't need to rewrite all of this using the IOKit APIs. I will investigate this first.

@cprovatas
 
Owner
Author
cprovatas commented 14 days ago • 
That doesn't work^. Window server validates the connection id against the app the enabled secure input mode. Any app can't disable it.

@cprovatas
 
Owner
Author
cprovatas commented 14 days ago
Explored using the Services API, which seems plausable. It essentially creates a menu item on every application calls some SPI called _NSSaveServiceActions. The only part thats unclear is if you can create these service actions programmatically

@cprovatas
 
Owner
Author
cprovatas commented 14 days ago
CGSSetSymbolicHotKey might be usable

@cprovatas
 
Owner
Author
cprovatas commented 14 days ago
related: https://avaidyam.github.io/2018/03/16/Building-Better-RegisterEventHotKey.html

@cprovatas
 
Owner
Author
cprovatas commented 8 days ago
Another method is to use HID Service Filters but I couldn't get that to work in Swift, also the debugging experience would be terrible running a plugin against the kernel

@cprovatas
 
Owner
Author
cprovatas commented 7 days ago • 
To detect if a connection is trusted– Skylight checks for presence of /var/db/.AccessibilitySecureTap file and if the process has the com.apple.private.accessibility.secureTap entitlement. I think also the kTCCServiceAccessibility entitlement. I confirmed this locally by using the Hot Key APIs

@cprovatas
 
Owner
Author
cprovatas commented yesterday
CGSSetHotKey doesn't really work with just shift as the modifier key while in secure input mode. I also couldn't figure out how to filter input. Attempting to use a forked project instead

@cprovatas
 
Owner
Author
cprovatas commented now
Using this as main project now – https://github.com/cprovatas/keymapping-2
