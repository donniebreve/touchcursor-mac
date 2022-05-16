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
