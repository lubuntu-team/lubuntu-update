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

To use, copy the lubuntu-update-backend script to /usr/bin/lubuntu-update-backend, then compile and run the updater. It is highly recommended that you **do not** install the updater all the way into your system with `sudo make install` or similar.

It is highly recommended that you use a Lubuntu virtual machine for testing and development. Use the latest development release if at all possible, unless you know you need to test an earlier release.

Qt Creator is recommended for editing the code. It is present in Ubuntu's official repos and can be installed using `sudo apt install qtcreator`.
