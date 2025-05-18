project(
  'wayback',
  'c',
  version: '0.1.0',
  license: 'Unlicense',
  default_options: [
    'c_std=c99',
    'warning_level=3',
    'werror=true',
  ],
)

add_project_arguments('-D_POSIX_C_SOURCE=200809L', language: 'c')

version = '"@0@"'.format(meson.project_version())
add_project_arguments('-DWAYBACK_VERSION=@0@'.format(version), language: 'c')

tllist_dep = dependency('tllist')
pixman_dep = dependency('pixman-1')
wayland_client_dep = dependency('wayland-client')
wayland_protocols_dep = dependency('wayland-protocols')

wayback_inc = [
  include_directories('include'),
]

subdir('protocols')
subdir('src')
