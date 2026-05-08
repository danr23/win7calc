project('win7-calc-gtk3', 'c',
  version: '0.1.0',
  meson_version: '>=0.60')
gtk_dep = dependency('gtk+-3.0')
m_dep = meson.get_compiler('c').find_library('m')
executable('win7calc',
  sources: ['src/main.c', 'src/ui.c', 'src/calc_core.c'],
  dependencies: [gtk_dep, m_dep],
  install: true)
