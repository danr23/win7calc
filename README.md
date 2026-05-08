# win7calc
kind of looks like the windows 7's calculator.
<h1> How to install it</h1>
run <code> git clone https://github.com/danr23/win7calc.git && cd win7calc/win7-calc-gtk3</code>
then to compile it run <code> meson setup buildir && meson compile -C builddir</code> (you can replace builddir with a name of your build directory)
and then run <code> sudo ln -sf ~/win7calc/win7-calc-gtk3/builddir/win7calc /usr/local/bin/win7calc && sudo cp ~/win7calc/win7-calc-gtk3/desktop/win7calc.desktop /usr/share/applications/</code> to make it appear in the start menu.
