#  Copyright (C) 2006 Artifex Software, Inc.
#  All Rights Reserved.
#
#  This software is provided AS-IS with no warranty, either express or
#  implied.
#
#  This software is distributed under license and may not be copied, modified
#  or distributed except as expressly authorized under the terms of that
#  license.  Refer to licensing information at http://www.artifex.com/
#  or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
#  San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
#
# $Id: lwf_jp2.mak 8022 2007-06-05 22:23:38Z giles $

# makefile for Luratech lwf_jp2 library code.
# Users of this makefile must define the following:
#       SHARE_JPX - whether to compile in or link to the library
#       JPXSRCDIR - the library source directory
#
# gs.mak and friends define the following:
#       JPXOBJDIR - the output obj directory
#       JPXGENDIR - generated (.dev) file directory
#	LWF_JPXI_ - include path for the library headers
#       JPXCF_ - cflags for building the library
#
# We define the lwf_jp2.dev target and its dependencies
#
# This partial makefile compiles the lwf_jp2 library for use in
# Ghostscript.

LWF_JP2_MAK=$(GLSRC)lwf_jp2.mak

LWF_JP2_SRC=$(JPXSRCDIR)$(D)library$(D)source$(D)
LWF_JP2_GEN=$(JPXOBJDIR)$(D)
LWF_JP2_OBJ=$(JPXOBJDIR)$(D)

# source files to build from the CSDK source

lwf_jp2_OBJS = \
	$(LWF_JP2_OBJ)jp2_adt_band_array.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_band_buffer.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_block_array.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_cache.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_comp.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_component_array.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_decomp.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_ebcot_decoder.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_external_cache.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_image.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_memory.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_mq_decoder.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_mq_state.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_packet_decoder.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_precinct_array.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_rate.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_rate_list.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_read_bits.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_read_data.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_reader_requirements.$(OBJ)	\
	$(LWF_JP2_OBJ)jp2_adt_resolution_array.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_tile_array.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_adt_tlm_marker_array.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2_adt_write_data.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_buffer.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2c_code_cb.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2c_coder.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2c_codestream.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2c_file_format.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2c_format.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2c_memory.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_code_cb.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_common.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2c_progression.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2c_quant.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2c_wavelet.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2c_wavelet_lifting.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2c_wavelet_lifting_mmx.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2c_weights.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2c_write.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2d_codestream.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2d_decoder.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2d_file_format.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2d_format.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2d_image.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2d_memory.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2d_partial_decoding.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2d_progression.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2d_quant.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2d_scale.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2d_wavelet.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2d_wavelet_lifting.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2d_wavelet_lifting_mmx.$(OBJ)		\
	$(LWF_JP2_OBJ)jp2d_write.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2_icc.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2_license.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2_packet.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2_tag_tree.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2t_codestream.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2t_file_format.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2t_image.$(OBJ)				\
	$(LWF_JP2_OBJ)jp2t_memory.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2t_progression.$(OBJ)			\
	$(LWF_JP2_OBJ)jp2t_transcoder.$(OBJ)

lwf_jp2_HDRS = \
	$(LWF_JP2_SRC)jp2_adt_band_array.h		\
	$(LWF_JP2_SRC)jp2_adt_band_buffer.h		\
	$(LWF_JP2_SRC)jp2_adt_block_array.h		\
	$(LWF_JP2_SRC)jp2_adt_cache.h			\
	$(LWF_JP2_SRC)jp2_adt_comp.h			\
	$(LWF_JP2_SRC)jp2_adt_component_array.h		\
	$(LWF_JP2_SRC)jp2_adt_decomp.h			\
	$(LWF_JP2_SRC)jp2_adt_ebcot_decoder.h		\
	$(LWF_JP2_SRC)jp2_adt_external_cache.h		\
	$(LWF_JP2_SRC)jp2_adt_image.h			\
	$(LWF_JP2_SRC)jp2_adt_memory.h			\
	$(LWF_JP2_SRC)jp2_adt_mq_decoder.h		\
	$(LWF_JP2_SRC)jp2_adt_mq_state.h		\
	$(LWF_JP2_SRC)jp2_adt_packet_decoder.h		\
	$(LWF_JP2_SRC)jp2_adt_precinct_array.h		\
	$(LWF_JP2_SRC)jp2_adt_rate.h			\
	$(LWF_JP2_SRC)jp2_adt_rate_list.h		\
	$(LWF_JP2_SRC)jp2_adt_read_bits.h		\
	$(LWF_JP2_SRC)jp2_adt_read_data.h		\
	$(LWF_JP2_SRC)jp2_adt_reader_requirements.h	\
	$(LWF_JP2_SRC)jp2_adt_resolution_array.h	\
	$(LWF_JP2_SRC)jp2_adt_tile_array.h		\
	$(LWF_JP2_SRC)jp2_adt_tlm_marker_array.h	\
	$(LWF_JP2_SRC)jp2_adt_write_data.h		\
	$(LWF_JP2_SRC)jp2_assembly.h			\
	$(LWF_JP2_SRC)jp2_buffer.h			\
	$(LWF_JP2_SRC)jp2c_coder.h			\
	$(LWF_JP2_SRC)jp2c_codestream.h			\
	$(LWF_JP2_SRC)jp2c_file_format.h		\
	$(LWF_JP2_SRC)jp2c_format.h			\
	$(LWF_JP2_SRC)jp2c_memory.h			\
	$(LWF_JP2_SRC)jp2_code_cb.h			\
	$(LWF_JP2_SRC)jp2_codestream.h			\
	$(LWF_JP2_SRC)jp2_common.h			\
	$(LWF_JP2_SRC)jp2c_progression.h		\
	$(LWF_JP2_SRC)jp2c_quant.h			\
	$(LWF_JP2_SRC)jp2c_wavelet.h			\
	$(LWF_JP2_SRC)jp2c_wavelet_lifting.h		\
	$(LWF_JP2_SRC)jp2c_wavelet_lifting_mmx.h	\
	$(LWF_JP2_SRC)jp2c_weights.h			\
	$(LWF_JP2_SRC)jp2c_write.h			\
	$(LWF_JP2_SRC)jp2d_codestream.h			\
	$(LWF_JP2_SRC)jp2d_decoder.h			\
	$(LWF_JP2_SRC)jp2d_file_format.h		\
	$(LWF_JP2_SRC)jp2d_format.h			\
	$(LWF_JP2_SRC)jp2d_image.h			\
	$(LWF_JP2_SRC)jp2d_memory.h			\
	$(LWF_JP2_SRC)jp2d_partial_decoding.h		\
	$(LWF_JP2_SRC)jp2d_progression.h		\
	$(LWF_JP2_SRC)jp2d_quant.h			\
	$(LWF_JP2_SRC)jp2d_scale.h			\
	$(LWF_JP2_SRC)jp2d_wavelet.h			\
	$(LWF_JP2_SRC)jp2d_wavelet_lifting.h		\
	$(LWF_JP2_SRC)jp2d_wavelet_lifting_mmx.h	\
	$(LWF_JP2_SRC)jp2d_write.h			\
	$(LWF_JP2_SRC)jp2_file_format.h			\
	$(LWF_JP2_SRC)jp2_icc.h				\
	$(LWF_JP2_SRC)jp2_image.h			\
	$(LWF_JP2_SRC)jp2_license.h			\
	$(LWF_JP2_SRC)jp2_mac_carbon.h			\
	$(LWF_JP2_SRC)jp2_packet.h			\
	$(LWF_JP2_SRC)jp2_tag_tree.h			\
	$(LWF_JP2_SRC)jp2t_codestream.h			\
	$(LWF_JP2_SRC)jp2t_file_format.h		\
	$(LWF_JP2_SRC)jp2t_image.h			\
	$(LWF_JP2_SRC)jp2t_memory.h			\
	$(LWF_JP2_SRC)jp2t_progression.h		\
	$(LWF_JP2_SRC)jp2t_transcoder.h			\
	$(LWF_JP2_SRC)lwf_jp2_geo.h			\
	$(LWF_JP2_SRC)lwf_jp2.h

# switch in the selected library .dev
$(LWF_JP2_GEN)lwf_jp2.dev : $(TOP_MAKEFILES) $(LWF_JP2_GEN)lwf_jp2_$(SHARE_JPX).dev
	$(CP_) $(LWF_JP2_GEN)lwf_jp2_$(SHARE_JPX).dev $(LWF_JP2_GEN)lwf_jp2.dev

# external link .dev
$(LWF_JP2_GEN)lwf_jp2_1.dev : $(TOP_MAKEFILES) $(LWF_JP2_MAK) $(ECHOGS_XE)
	$(SETMOD) $(LWF_JP2_GEN)lwf_jp2_1 -lib lwf_jp2

# compile our own .dev
$(LWF_JP2_GEN)lwf_jp2_0.dev : $(TOP_MAKEFILES) $(LWF_JP2_MAK) $(ECHOGS_XE) $(lwf_jp2_OBJS)
	$(SETMOD) $(LWF_JP2_GEN)lwf_jp2_0 $(lwf_jp2_OBJS)

# define our specific compiler
LWF_JP2_CC=$(CC_) $(CFLAGS) $(I_)$(LWF_JPXI_)$(_I) $(JPXCF_)
LWF_JP2_O=$(O_)$(LWF_JP2_OBJ)

LWF_JP2_DEP=$(AK) $(LWF_JP2_MAK)

# explicit rules for building each source file
# for simplicity we have every source file depend on all headers

$(LWF_JP2_OBJ)jp2_adt_band_array.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_band_array.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_band_array.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_band_array.c

$(LWF_JP2_OBJ)jp2_adt_band_buffer.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_band_buffer.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_band_buffer.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_band_buffer.c

$(LWF_JP2_OBJ)jp2_adt_block_array.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_block_array.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_block_array.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_block_array.c

$(LWF_JP2_OBJ)jp2_adt_cache.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_cache.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_cache.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_cache.c

$(LWF_JP2_OBJ)jp2_adt_comp.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_comp.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_comp.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_comp.c

$(LWF_JP2_OBJ)jp2_adt_component_array.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_component_array.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_component_array.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_component_array.c

$(LWF_JP2_OBJ)jp2_adt_decomp.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_decomp.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_decomp.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_decomp.c

$(LWF_JP2_OBJ)jp2_adt_ebcot_decoder.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_ebcot_decoder.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_ebcot_decoder.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_ebcot_decoder.c

$(LWF_JP2_OBJ)jp2_adt_external_cache.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_external_cache.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_external_cache.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_external_cache.c

$(LWF_JP2_OBJ)jp2_adt_image.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_image.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_image.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_image.c

$(LWF_JP2_OBJ)jp2_adt_memory.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_memory.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_memory.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_memory.c

$(LWF_JP2_OBJ)jp2_adt_mq_decoder.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_mq_decoder.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_mq_decoder.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_mq_decoder.c

$(LWF_JP2_OBJ)jp2_adt_mq_state.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_mq_state.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_mq_state.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_mq_state.c

$(LWF_JP2_OBJ)jp2_adt_packet_decoder.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_packet_decoder.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_packet_decoder.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_packet_decoder.c

$(LWF_JP2_OBJ)jp2_adt_precinct_array.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_precinct_array.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_precinct_array.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_precinct_array.c

$(LWF_JP2_OBJ)jp2_adt_rate.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_rate.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_rate.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_rate.c

$(LWF_JP2_OBJ)jp2_adt_rate_list.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_rate_list.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_rate_list.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_rate_list.c

$(LWF_JP2_OBJ)jp2_adt_read_bits.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_read_bits.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_read_bits.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_read_bits.c

$(LWF_JP2_OBJ)jp2_adt_read_data.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_read_data.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_read_data.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_read_data.c

$(LWF_JP2_OBJ)jp2_adt_reader_requirements.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_reader_requirements.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_reader_requirements.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_reader_requirements.c

$(LWF_JP2_OBJ)jp2_adt_resolution_array.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_resolution_array.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_resolution_array.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_resolution_array.c

$(LWF_JP2_OBJ)jp2_adt_tile_array.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_tile_array.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_tile_array.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_tile_array.c

$(LWF_JP2_OBJ)jp2_adt_tlm_marker_array.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_tlm_marker_array.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_tlm_marker_array.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_tlm_marker_array.c

$(LWF_JP2_OBJ)jp2_adt_write_data.$(OBJ) : $(LWF_JP2_SRC)jp2_adt_write_data.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_adt_write_data.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_adt_write_data.c

$(LWF_JP2_OBJ)jp2_buffer.$(OBJ) : $(LWF_JP2_SRC)jp2_buffer.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_buffer.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_buffer.c

$(LWF_JP2_OBJ)jp2c_code_cb.$(OBJ) : $(LWF_JP2_SRC)jp2c_code_cb.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_code_cb.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_code_cb.c

$(LWF_JP2_OBJ)jp2c_coder.$(OBJ) : $(LWF_JP2_SRC)jp2c_coder.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_coder.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_coder.c

$(LWF_JP2_OBJ)jp2c_codestream.$(OBJ) : $(LWF_JP2_SRC)jp2c_codestream.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_codestream.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_codestream.c

$(LWF_JP2_OBJ)jp2c_file_format.$(OBJ) : $(LWF_JP2_SRC)jp2c_file_format.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_file_format.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_file_format.c

$(LWF_JP2_OBJ)jp2c_format.$(OBJ) : $(LWF_JP2_SRC)jp2c_format.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_format.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_format.c

$(LWF_JP2_OBJ)jp2c_memory.$(OBJ) : $(LWF_JP2_SRC)jp2c_memory.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_memory.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_memory.c

$(LWF_JP2_OBJ)jp2_code_cb.$(OBJ) : $(LWF_JP2_SRC)jp2_code_cb.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_code_cb.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_code_cb.c

$(LWF_JP2_OBJ)jp2_common.$(OBJ) : $(LWF_JP2_SRC)jp2_common.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_common.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_common.c

$(LWF_JP2_OBJ)jp2c_progression.$(OBJ) : $(LWF_JP2_SRC)jp2c_progression.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_progression.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_progression.c

$(LWF_JP2_OBJ)jp2c_quant.$(OBJ) : $(LWF_JP2_SRC)jp2c_quant.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_quant.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_quant.c

$(LWF_JP2_OBJ)jp2c_wavelet.$(OBJ) : $(LWF_JP2_SRC)jp2c_wavelet.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_wavelet.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_wavelet.c

$(LWF_JP2_OBJ)jp2c_wavelet_lifting.$(OBJ) : $(LWF_JP2_SRC)jp2c_wavelet_lifting.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_wavelet_lifting.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_wavelet_lifting.c

$(LWF_JP2_OBJ)jp2c_wavelet_lifting_mmx.$(OBJ) : $(LWF_JP2_SRC)jp2c_wavelet_lifting_mmx.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_wavelet_lifting_mmx.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_wavelet_lifting_mmx.c

$(LWF_JP2_OBJ)jp2c_weights.$(OBJ) : $(LWF_JP2_SRC)jp2c_weights.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_weights.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_weights.c

$(LWF_JP2_OBJ)jp2c_write.$(OBJ) : $(LWF_JP2_SRC)jp2c_write.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2c_write.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2c_write.c

$(LWF_JP2_OBJ)jp2d_codestream.$(OBJ) : $(LWF_JP2_SRC)jp2d_codestream.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_codestream.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_codestream.c

$(LWF_JP2_OBJ)jp2d_decoder.$(OBJ) : $(LWF_JP2_SRC)jp2d_decoder.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_decoder.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_decoder.c

$(LWF_JP2_OBJ)jp2_demo.$(OBJ) : $(LWF_JP2_SRC)jp2_demo.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_demo.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_demo.c

$(LWF_JP2_OBJ)jp2d_file_format.$(OBJ) : $(LWF_JP2_SRC)jp2d_file_format.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_file_format.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_file_format.c

$(LWF_JP2_OBJ)jp2d_format.$(OBJ) : $(LWF_JP2_SRC)jp2d_format.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_format.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_format.c

$(LWF_JP2_OBJ)jp2d_image.$(OBJ) : $(LWF_JP2_SRC)jp2d_image.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_image.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_image.c

$(LWF_JP2_OBJ)jp2d_memory.$(OBJ) : $(LWF_JP2_SRC)jp2d_memory.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_memory.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_memory.c

$(LWF_JP2_OBJ)jp2d_partial_decoding.$(OBJ) : $(LWF_JP2_SRC)jp2d_partial_decoding.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_partial_decoding.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_partial_decoding.c

$(LWF_JP2_OBJ)jp2d_progression.$(OBJ) : $(LWF_JP2_SRC)jp2d_progression.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_progression.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_progression.c

$(LWF_JP2_OBJ)jp2d_quant.$(OBJ) : $(LWF_JP2_SRC)jp2d_quant.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_quant.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_quant.c

$(LWF_JP2_OBJ)jp2d_scale.$(OBJ) : $(LWF_JP2_SRC)jp2d_scale.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_scale.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_scale.c

$(LWF_JP2_OBJ)jp2d_wavelet.$(OBJ) : $(LWF_JP2_SRC)jp2d_wavelet.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_wavelet.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_wavelet.c

$(LWF_JP2_OBJ)jp2d_wavelet_lifting.$(OBJ) : $(LWF_JP2_SRC)jp2d_wavelet_lifting.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_wavelet_lifting.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_wavelet_lifting.c

$(LWF_JP2_OBJ)jp2d_wavelet_lifting_mmx.$(OBJ) : $(LWF_JP2_SRC)jp2d_wavelet_lifting_mmx.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_wavelet_lifting_mmx.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_wavelet_lifting_mmx.c

$(LWF_JP2_OBJ)jp2d_write.$(OBJ) : $(LWF_JP2_SRC)jp2d_write.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2d_write.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2d_write.c

$(LWF_JP2_OBJ)jp2_icc.$(OBJ) : $(LWF_JP2_SRC)jp2_icc.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_icc.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_icc.c

$(LWF_JP2_OBJ)jp2_license.$(OBJ) : $(LWF_JP2_SRC)jp2_license.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_license.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_license.c

$(LWF_JP2_OBJ)jp2_packet.$(OBJ) : $(LWF_JP2_SRC)jp2_packet.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_packet.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_packet.c

$(LWF_JP2_OBJ)jp2_tag_tree.$(OBJ) : $(LWF_JP2_SRC)jp2_tag_tree.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2_tag_tree.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2_tag_tree.c

$(LWF_JP2_OBJ)jp2t_codestream.$(OBJ) : $(LWF_JP2_SRC)jp2t_codestream.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2t_codestream.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2t_codestream.c

$(LWF_JP2_OBJ)jp2t_file_format.$(OBJ) : $(LWF_JP2_SRC)jp2t_file_format.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2t_file_format.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2t_file_format.c

$(LWF_JP2_OBJ)jp2t_image.$(OBJ) : $(LWF_JP2_SRC)jp2t_image.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2t_image.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2t_image.c

$(LWF_JP2_OBJ)jp2t_memory.$(OBJ) : $(LWF_JP2_SRC)jp2t_memory.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2t_memory.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2t_memory.c

$(LWF_JP2_OBJ)jp2t_progression.$(OBJ) : $(LWF_JP2_SRC)jp2t_progression.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2t_progression.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2t_progression.c

$(LWF_JP2_OBJ)jp2t_transcoder.$(OBJ) : $(LWF_JP2_SRC)jp2t_transcoder.c $(LWF_JP2_DEP) $(lwf_jp2_HDRS)
	$(LWF_JP2_CC) $(LWF_JP2_O)jp2t_transcoder.$(OBJ) $(C_) $(LWF_JP2_SRC)jp2t_transcoder.c


# end of file
