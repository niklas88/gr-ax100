#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: TW-1 test decoder
# Author: Daniel Estevez
# Description: Flowgraph to test the TW-1 decoder
# Generated: Sun Sep 25 12:06:10 2016
##################################################

from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import ax100
import numpy
import synctags


class tw_1(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "TW-1 test decoder")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 48000
        self.samp_per_sym = samp_per_sym = 10
        self.gain_mu = gain_mu = 0.175*3

        ##################################################
        # Blocks
        ##################################################
        self.synctags_fixedlen_tagger_0 = synctags.fixedlen_tagger("syncword", "packet_len", 256*8, numpy.byte)
        self.low_pass_filter_0 = filter.fir_filter_fff(1, firdes.low_pass(
        	1, samp_rate, 2400, 2000, firdes.WIN_HAMMING, 6.76))
        self.digital_descrambler_bb_0 = digital.descrambler_bb(0x21, 0x00, 16)
        self.digital_correlate_access_code_tag_bb_0 = digital.correlate_access_code_tag_bb("10010011000010110101000111011110", 4, "syncword")
        self.digital_clock_recovery_mm_xx_0 = digital.clock_recovery_mm_ff(samp_per_sym*(1+0.0), 0.25*gain_mu*gain_mu, 0.5, gain_mu, 0.005)
        self.digital_binary_slicer_fb_0 = digital.binary_slicer_fb()
        self.blocks_wavfile_source_0 = blocks.wavfile_source("/tmp/tw-1c.wav", False)
        self.blocks_unpacked_to_packed_xx_0 = blocks.unpacked_to_packed_bb(1, gr.GR_MSB_FIRST)
        self.blocks_tagged_stream_to_pdu_0 = blocks.tagged_stream_to_pdu(blocks.byte_t, "packet_len")
        self.blocks_tagged_stream_multiply_length_0 = blocks.tagged_stream_multiply_length(gr.sizeof_char*1, "packet_len", 1/8.0)
        self.blocks_message_debug_1 = blocks.message_debug()
        self.ax100_gomx3_rs_decode_0 = ax100.gomx3_rs_decode(True)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.ax100_gomx3_rs_decode_0, 'out'), (self.blocks_message_debug_1, 'print_pdu'))    
        self.msg_connect((self.blocks_tagged_stream_to_pdu_0, 'pdus'), (self.ax100_gomx3_rs_decode_0, 'in'))    
        self.connect((self.blocks_tagged_stream_multiply_length_0, 0), (self.blocks_tagged_stream_to_pdu_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.blocks_tagged_stream_multiply_length_0, 0))    
        self.connect((self.blocks_wavfile_source_0, 0), (self.low_pass_filter_0, 0))    
        self.connect((self.digital_binary_slicer_fb_0, 0), (self.digital_descrambler_bb_0, 0))    
        self.connect((self.digital_clock_recovery_mm_xx_0, 0), (self.digital_binary_slicer_fb_0, 0))    
        self.connect((self.digital_correlate_access_code_tag_bb_0, 0), (self.synctags_fixedlen_tagger_0, 0))    
        self.connect((self.digital_descrambler_bb_0, 0), (self.digital_correlate_access_code_tag_bb_0, 0))    
        self.connect((self.low_pass_filter_0, 0), (self.digital_clock_recovery_mm_xx_0, 0))    
        self.connect((self.synctags_fixedlen_tagger_0, 0), (self.blocks_unpacked_to_packed_xx_0, 0))    

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.low_pass_filter_0.set_taps(firdes.low_pass(1, self.samp_rate, 2400, 2000, firdes.WIN_HAMMING, 6.76))

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym
        self.digital_clock_recovery_mm_xx_0.set_omega(self.samp_per_sym*(1+0.0))

    def get_gain_mu(self):
        return self.gain_mu

    def set_gain_mu(self, gain_mu):
        self.gain_mu = gain_mu
        self.digital_clock_recovery_mm_xx_0.set_gain_omega(0.25*self.gain_mu*self.gain_mu)
        self.digital_clock_recovery_mm_xx_0.set_gain_mu(self.gain_mu)


def main(top_block_cls=tw_1, options=None):

    tb = top_block_cls()
    tb.start()
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main()
