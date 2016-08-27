# gr-ax100

GNUradio decoder for the AX100 transceiver from GomSpace.
This transceiver is used in the satellite GOMX-3.

You can find a complete receiver for GOMX-3 that uses this software in
[gr-satellites](https://github.com/daniestevez/gr-satellites).

You need to install Phil Karn's KA9Q libfec.
This [libfec fork](https://github.com/daniestevez/libfec) builds correctly on
modern systems.

You also need to install the following OOT modules:
  * [gr-synctags](https://github.com/daniestevez/gr-synctags)
  * [gr-csp](https://github.com/daniestevez/gr-csp)
  * [gr-kiss](https://github.com/daniestevez/gr-kiss)

To test the decoder you can open `examples/gomx-3.grc` with `gnuradio-companion`.

This fork includes:

  * Beacon parser for GOMX-3 beacons of type 1 0 (which include ADS-B data)
  * ADS-B data output to KML. This block will only output <Placemark> tags. You need to
    include a proper header and footer to obtain a valid KML file.
    See http://destevez.net/wp-content/uploads/2016/05/gomx3.kml for an example.

