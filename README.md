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

## Config file format:
There's only one field here:

* nextDoReleaseUpgradeNotify=123456789 - Value is number of seconds since the UNIX epoch. Used to determine when to offer the user an upgrade.

## Missing features

* Double-clicking on a package doesn't show detailed information for it yet.
* There's no support for release upgrading. This is currently unnecessary as this updater is only going to be shipped in Noble and later, but it will become a big deal in the (potentially near) future.
* The translation support is mostly untested, though it should work.

## Translations

Run the `gen_ts.sh` script after making any code modifications to ensure that the translations files are up-to-date for translators to work on.

To add a new language to be translated:

* Open the `gen_ts.sh` script and add the locale code for the new language to the `langList` array.
* Run the script after doing this - a new template .ts file will be generated under `src/translations/`.
* Next, add the new template file to the `TS_FILES` list in `CMakeLists.txt` - it will be named `src/translations/lubuntu-update_locale_CODE.ts`, where `locale_CODE` is the locale code of the added language.
* Finally, add a line in the src/translations.qrc resource file to include the new translation file. The line should look like `<file alias="locale_CODE">lubuntu-update_locale_CODE.qm</file>`, where `locale_CODE` is the locale code of the added language. This line should go inside the `<qresource>` tag.

For instance, if I were to add Chinese to the list of languages that could be translated into, I would do this:

    vim gen_ts.sh
    # add this code to the langList array:
    #    'zh_CN'
    ./gen_ts.sh
    vim CMakeLists.txt
    # add this line to the TS_FILES list:
    #    src/translations/lubuntu-update_zh_CN.ts
    vim src/translations.qrc
    # add this line to the list of file resources:
    #    <file alias="zh_CN">lubuntu-update_zh_CN.qm</file>

The program will now pick up the added language at build time. Any translations added to the newly created .ts file will be shown to program users who select the new language.
