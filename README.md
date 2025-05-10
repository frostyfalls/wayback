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

## License

Unlicense, see LICENSE for more information.

[swaybg]: https://github.com/swaywm/swaybg
[labwc]: https://github.com/labwc/labwc
[hello-wayland]: https://github.com/emersion/hello-wayland
[wayland-book]: https://wayland-book.com/introduction.html
