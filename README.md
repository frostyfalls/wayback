# wayback

## Building

```
meson setup --buildtype=release build/
ninja -C build/
ninja -C build/ install
```

## Thanks

- [swaybg][swaybg] for Wayland client code structure help
- [labwc][labwc] for project structure (mainly Meson setup) ideas
- [hello-wayland][hello-wayland] for rendering code
- [wayland-book][wayland-book] for general Wayland architecture help

## Goals

- [ ] Support user-provided input
- [ ] Support images (perhaps with Cairo or per-filetype libraries + Pixman)
- [ ] Support fractional scaling
- [ ] Support reloading the current buffer (change image or color)
- [ ] Support multiple outputs and reload on change
    - Might already work, needs testing

## License

Unlicense, see LICENSE for more information.

[swaybg]: https://github.com/swaywm/swaybg
[labwc]: https://github.com/labwc/labwc
[hello-wayland]: https://github.com/emersion/hello-wayland
[wayland-book]: https://wayland-book.com/introduction.html
