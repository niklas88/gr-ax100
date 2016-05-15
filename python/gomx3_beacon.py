#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2016 Daniel Estevez <daniel@destevez.net>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

import struct
from datetime import datetime

class beacon_1_0(object):
    def __init__(self, payload):
        assert payload[0] == 0 # check type
        self.timestamp = datetime.utcfromtimestamp(struct.unpack(">i", payload[1:5])[0])
        adsb = struct.unpack(">IffIi", payload[0x74:0x88])
        self.icao24 = adsb[0]
        self.latitude = adsb[1]
        self.longitude = adsb[2]
        self.altitude = adsb[3]
        self.adsb_time = datetime.utcfromtimestamp(adsb[4])

    def __str__(self):
        return ("""Beacon type 1 0:
    Timestamp:\t\t\t{}
    ADS-B receiver data:
        Time:\t\t\t\t{}
        ICAO 24bit address:\t{}
        Latitude:\t\t\t\t{}
        Longitude:\t\t\t{}
        Altitude:\t\t\t\t{}ft""".format(
               self.timestamp,
               self.adsb_time,
               hex(self.icao24),
               self.latitude,
               self.longitude,
               self.altitude))
        
