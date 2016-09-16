/* -*- c++ -*- */

#define AX100_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ax100_swig_doc.i"

%{
#include "ax100/frame_decoder_cc.h"
#include "ax100/gomx3_rs_decode.h"
#include "ax100/gomx1_decode.h"
%}


%include "ax100/frame_decoder_cc.h"
GR_SWIG_BLOCK_MAGIC2(ax100, frame_decoder_cc);
%include "ax100/gomx3_rs_decode.h"
GR_SWIG_BLOCK_MAGIC2(ax100, gomx3_rs_decode);
%include "ax100/gomx1_decode.h"
GR_SWIG_BLOCK_MAGIC2(ax100, gomx1_decode);
