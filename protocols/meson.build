wayland_protocols_dir = wayland_protocols.get_variable('pkgdatadir')
wayland_scanner = find_program('wayland-scanner')

protocols_src = []
protocols_headers = []

foreach protocol : [
  'wlr-layer-shell-unstable-v1.xml',
  wayland_protocols_dir / 'stable/xdg-shell/xdg-shell.xml',
]
  protocols_src += custom_target(
    protocol.underscorify() + '-private-code',
    output: '@BASENAME@.c',
    input: protocol,
    command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
  )

  protocols_src += custom_target(
    protocol.underscorify() + '-client-header',
    output: '@BASENAME@.h',
    input: protocol,
    command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@']
  )
endforeach
