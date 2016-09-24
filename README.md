# gr-ax100

GNUradio decoder for transceivers from GomSpace.

This was originally a fork of some code from GomSpace for the NanoCom AX100
transceiver, which is used in GOMX-3. Since then, support for the NanoCom U482C
transceiver used in GOMX-1 and other satellites has been also added.

You can find a complete receivers for GOMX-3, GOMX-1, AISAT and ATEHNOXAT-1
using this software in [gr-satellites](https://github.com/daniestevez/gr-satellites).

You need to install Phil Karn's KA9Q libfec.
This [libfec fork](https://github.com/daniestevez/libfec) builds correctly on
modern systems.

You also need to install the following OOT modules:
  * [gr-synctags](https://github.com/daniestevez/gr-synctags)
  * [gr-csp](https://github.com/daniestevez/gr-csp)
  * [gr-kiss](https://github.com/daniestevez/gr-kiss)

To test the decoders you can open the files in the `examples/` folder with
`gnuradio-companion`.

This fork includes the following (not in the original code from GomSpace):

  * Beacon parser for GOMX-3 beacons of type 1 0 (which include ADS-B data).
  * ADS-B data output to KML. This block will only output <Placemark> tags. You need to
    include a proper header and footer to obtain a valid KML file.
    See http://destevez.net/wp-content/uploads/2016/05/gomx3.kml for an example.
  * Decoder for NanCom U482C.
  * Beacon parser for GOMX-1 beacons of type A (which include ADS-B data). I
    have the format specifications for beacons of type B, but I don't have a
    recording of a beacon of type B to test the parser.
