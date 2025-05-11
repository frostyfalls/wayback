# wayback

## Building

```
meson setup --buildtype=release build/
ninja -C build/
ninja -C build/ install
```

## Goals

- [ ] Support images with Pixman and accompanying filetype libraries
- [ ] Support output events (plugging, unplugging)
    - May already work, needs testing
- [ ] Support user input
- [ ] Support fractional scaling

## Thanks

- [swaybg][swaybg] for understanding `wlr-layer-shell` interface
- [wbg][wbg] for understanding `wlr-layer-shell` interface
- [hello-wayland][hello-wayland] for understanding shared memory and rendering
- [wayland-book][wayland-book] for understanding Wayland architecture
- [labwc][labwc] for Meson project setup with Wayland

## License

Unlicense, see LICENSE for more information.

[swaybg]: https://github.com/swaywm/swaybg
[wbg]: https://codeberg.org/dnkl/wbg
[hello-wayland]: https://github.com/emersion/hello-wayland
[wayland-book]: https://wayland-book.com/introduction.html
[labwc]: https://github.com/labwc/labwc
