# Lubuntu Update

Copyright (c) 2023-2024 Lubuntu Contributors. Licensed under the GNU General Public License version 2, or (at your option) any later version.

Build dependencies are Qt 5.15 and cmake, runtime dependencies are apt, apt-get, and diff.

To build:

```bash
cd lubuntu-update
mkdir build
cd build
cmake ..
make -j$(nproc)
```

To use, copy the lubuntu-update-backend script to /usr/libexec/lubuntu-update-backend, then compile and run the updater. It is highly recommended that you **do not** install the updater all the way into your system with `sudo make install` or similar.

It is highly recommended that you use a Lubuntu virtual machine for testing and development. Use the latest development release if at all possible, unless you know you need to test an earlier release.

Qt Creator is recommended for editing the code. It is present in Ubuntu's official repos and can be installed using `sudo apt install qtcreator`.

## Missing features

* The Details button is hidden and does nothing. Eventually it should display a list of packages, the old version of them, the new version of them, and a link to their Launchpad page.
* There's no support for release upgrading. This is currently unnecessary as this updater is only going to be shipped in Noble and later, but it will become a big deal in the (potentially near) future.
* There's no support for doing an `apt update` for checking for recent updates. This seems rather important *now*.
* Most of the internal strings aren't translatable...
* ...and the bit of translation support there is, is totally untested and quite possibly not functioning properly. Borrow from what we did with lubuntu-installer-prompt to fix this.
