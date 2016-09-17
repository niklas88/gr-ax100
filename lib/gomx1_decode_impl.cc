/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "gomx1_decode_impl.h"

extern "C" {
#include <fec.h>
#include "randomizer.h"
}

namespace gr {
  namespace ax100 {

    gomx1_decode::sptr
    gomx1_decode::make(bool verbose)
    {
      return gnuradio::get_initial_sptr
        (new gomx1_decode_impl(verbose));
    }

    /*
     * The private constructor
     */
    gomx1_decode_impl::gomx1_decode_impl(bool verbose)
      : gr::block("gomx1_decode",
              gr::io_signature::make(0, 0, 0),
	      gr::io_signature::make(0, 0, 0))
    {
      d_verbose = verbose;

      // init FEC
      ccsds_generate_sequence(d_ccsds_sequence, RS_LEN);

      message_port_register_out(pmt::mp("out"));
      message_port_register_in(pmt::mp("in"));
      set_msg_handler(pmt::mp("in"),
		      boost::bind(&gomx1_decode_impl::msg_handler, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    gomx1_decode_impl::~gomx1_decode_impl()
    {
    }

    void
    gomx1_decode_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    }

    int
    gomx1_decode_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      return 0;
    }

    void
    gomx1_decode_impl::msg_handler (pmt::pmt_t pmt_msg) {
      pmt::pmt_t msg = pmt::cdr(pmt_msg);
      uint8_t data[HEADER_LEN + RS_LEN];
      uint8_t data_scratch[RS_LEN];
      uint8_t tmp;
      int rs_res;
      int frame_len;
      size_t offset(0);
      // Lengths of known packets
      static int known_lengths[] = { 0xf8, 0xf6, 0x28 };
      int i;

      memcpy(data, pmt::uniform_vector_elements(msg, offset), sizeof(data));

      ccsds_xor_sequence(data + HEADER_LEN, d_ccsds_sequence, RS_LEN);

      // first try with length in header
      // (Golay decoder not implemented yet)
      frame_len = data[2];

      memcpy(data_scratch, data + HEADER_LEN, frame_len);
      rs_res = decode_rs_8(data_scratch, NULL, 0, RS_LEN - frame_len);

      if (rs_res < 0) {
	// try with known packet sizes
	for (i = 0; i < sizeof(known_lengths)/sizeof(int); i++) {
	  if (d_verbose) {
	    std::printf("RS decode failed with frame length = %d. Trying length = %d.\n",
			frame_len, known_lengths[i]);
	  }
	  frame_len = known_lengths[i];
	  memcpy(data_scratch, data + HEADER_LEN, frame_len);
	  rs_res = decode_rs_8(data_scratch, NULL, 0, RS_LEN - frame_len);
	  if (rs_res >= 0) break;
	}
      }

      // Send via GNUradio message if RS ok
      if (rs_res >= 0) {
	// Swap CSP header
	tmp = data_scratch[0];
	data_scratch[0] = data_scratch[3];
	data_scratch[3] = tmp;
	tmp = data_scratch[1];
	data_scratch[1] = data_scratch[2];
	data_scratch[2] = tmp;
	
	if (d_verbose) {
	  std::printf("RS decode OK. Byte errors: %d.\n", rs_res);
	}

	// Send by GNUradio message
	message_port_pub(pmt::mp("out"),
			 pmt::cons(pmt::PMT_NIL,
				   pmt::init_u8vector(frame_len - 32, data_scratch)));
      }
      else if (d_verbose) {
	std::printf("RS decode failed.\n");
      }
      
      
    }    

  } /* namespace ax100 */
} /* namespace gr */

