wayland_protocols_dir = wayland_protocols.get_variable('pkgdatadir')
wayland_scanner = find_program('wayland-scanner')

wayland_scanner_code = generator(
  wayland_scanner,
  output: '@BASENAME@-protocol.c',
  arguments: ['private-code', '@INPUT@', '@OUTPUT@'],
)
wayland_scanner_client = generator(
  wayland_scanner,
  output: '@BASENAME@-client-protocol.h',
  arguments: ['client-header', '@INPUT@', '@OUTPUT@'],
)

protocols_src = []
foreach p : [
  wayland_protocols_dir / 'stable/xdg-shell/xdg-shell.xml',
]
  protocols_src += wayland_scanner_code.process(p)
  protocols_src += wayland_scanner_client.process(p)
endforeach
