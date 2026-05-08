# win7calc
kind of looks like the windows 7's calculator but probably not that similar, feel free to tell me what i should change in the design to make it look more like it
<h1> How to install it</h1>
run <code> git clone https://github.com/danr23/win7calc.git && cd win7calc/win7-calc-gtk4</code>
then to compile it run <code> meson compile -C builddir</code> (you can replace builddir with a name of your build directory)
and then run <code> sudo ln -sf ~/win7calc/win7-calc-gtk4/builddir/win7calc /usr/local/bin/win7calc && sudo cp ~/win7calc/win7-calc-gtk4/desktopstuff/win7calc.desktop /usr/share/applications/</code> to make it appear in SevenStart menu or whatever it is called in kde.
