# libatemkontroll

C++ API to control Blackmagic ATEM video switchers

Currently the API will handle the very basic functions that we need to control
an ATEM as a master control switcher.

However, these functions have been implemented with a reasonable robustness and
I have strived for generality, so that the library can be used for all use
cases.

## Examples

```
#include <atem.hpp>

// Set ME 0's PGM to color bars
int main() {
    ATEM atem;
    atem.connect('13.37.13.37');
    atem.MixEffects.set_program(1000);
}
```

## Utilities

* `atemctl` - a simple command-line utility to set program/preview or enact
  transitions
* `atemhack` - A utility developed in collaboration with AGS to interact with
  their [DVS8000 hack project](https://gitlab.ags.etc.tu-bs.de/eggert/dvs8000).

## Requirements

```
apt-get install nlohmann-json-dev
```

## Todo

* Media file upload (Stills, bumpers, etc)
* Support for more commands
* API documentation
