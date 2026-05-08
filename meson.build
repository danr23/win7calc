project('win7-calc-gtk4', 'c',
  version: '0.1.0',
  meson_version: '>=0.60')
gtk_dep = dependency('gtk4')
executable('win7calc',
  sources: ['src/main.c', 'src/ui.c', 'src/calc_core.c'],
  dependencies: [gtk_dep],
  install: true)
