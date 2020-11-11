project('cargs', 'c',
  license: 'MIT',
  meson_version: '>= 0.45.1'
)

cargs_inc = include_directories('include')

cargs = library('cargs', 'src/cargs.c',
  install: true,
  include_directories: cargs_inc
)

install_headers('include/cargs.h')

cargs_dep = declare_dependency(include_directories: 'include', link_with: cargs)

if meson.version().version_compare('>= 0.54.0')
  meson.override_dependency('cargs', cargs_dep)
endif
