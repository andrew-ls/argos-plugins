# Argos Plugins

A collection of plugins for the GNOME Shell extension
[Argos](https://github.com/p-e-w/argos).

## Installation

### Scripts

Most plugins are executable scripts found in "plugins"; the "share" directory
within is required and shared between plugins.

Simply copy the "share" directory and any desired plugins to `~/.config/argos`,
and ensure the plugins are marked with the executable flag.

### Binaries

Other plugins must be compiled. Execute the desired plugin's compilation script
found in "bin", which will create a compiled binary in the "build" directory.
You can then move the binary into `~/.config/argos`.

## Customization

For information on how to customize the position and update interval of plugins,
see [the Argos README](https://github.com/p-e-w/argos#filename-format).

### Scripts

Script plugins can be further customized by editing them: customizable settings
will usually be found at the top of the script.

### Binaries

Compiled programs will have customizable settings defined as shell variables in
".env" files, next to their source files in the "src" directory. Simply edit
these variables as you would a shell script; they will be sourced during
compilation and integrated into the binary.
