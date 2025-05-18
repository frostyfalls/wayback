wayback_src = files(
  'wayback.c',
  'buffer.c',
  'layer_surface.c',
  'layer_surface_listener.c',
  'output.c',
  'output_listener.c',
  'registry_listener.c',
)
wayback_src += protocols_src
wayback_src += protocols_headers

wayback_deps = [
  tllist_dep,
  pixman_dep,
  wayland_client_dep,
  wayland_protocols_dep,
]

executable(
  'wayback',
  wayback_src,
  include_directories: wayback_inc,
  dependencies: wayback_deps,
  install: true,
)
