# gr-ax100

GNUradio decoder for the AX100 transceiver from GomSpace.
This transceiver is used in the satellite GOMX-3.

You can use this sample file from GOMX-3
https://drive.google.com/open?id=0B2pPGQkeEAfdd1VCa2d0UWFXR1k
to test the decoder examples/ax100_rx_fm_demod.grc
It is a short audio file which contains a few 19k2 telemetry packets from GOMX-3
(already FM demodulated)

This fork includes:

* Beacon parser for GOMX-3 beacons of type 1 0 (which include ADS-B data)
* ADS-B data output to KML. This block will only output <Placemark> tags. You need to
  include a proper header and footer to obtain a valid KML file.
  See http://destevez.net/wp-content/uploads/2016/05/gomx3.kml for an example.