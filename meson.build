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
git = find_program('git', required: false, native: true)
if git.found()
  git_commit = run_command([git, 'describe', '--dirty'], check: false)
  if git_commit.returncode() == 0
    version = '"@0@"'.format(git_commit.stdout().strip())
  endif
endif
add_project_arguments('-DWAYBACK_VERSION=@0@'.format(version), language: 'c')

wayland_client = dependency('wayland-client')
wayland_protocols = dependency('wayland-protocols')

subdir('protocols')

wayback_inc = include_directories('include')

wayback_src = files(
  'global_registry.c',
  'layer_surface.c',
  'layer_surface_registry.c',
  'output.c',
  'output_registry.c',
  'shm.c',
  'wayback.c',
  'wayland.c',
)
wayback_src += protocols_src
wayback_src += protocols_headers

wayback_deps = [
  wayland_client,
  wayland_protocols,
]

executable(
  meson.project_name(),
  wayback_src,
  include_directories: [wayback_inc],
  dependencies: wayback_deps,
  install: true,
)
