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
# $Id: ldf_jb2.mak 8022 2007-06-05 22:23:38Z giles $

# makefile for Luratech ldf_jb2 library code.
# Users of this makefile must define the following:
#       SHARE_JBIG2 - whether to compile in or link to the library
#       JBIG2SRCDIR - the library source directory
#
# gs.mak and friends define the following:
#       JBIG2OBJDIR - the output obj directory
#       JBIG2GENDIR - generated (.dev) file directory
#       LDF_JB2I_ - include path for the library
#       JB2CF_ - cflags for building the library
#
# We define the ldf_jb2.dev target and its dependencies
#
# This partial makefile compiles the ldf_jb2 library for use in
# Ghostscript.

LDF_JB2_MAK=$(GLSRC)ldf_jb2.mak

LDF_JB2_SRC=$(JBIG2SRCDIR)$(D)
LDF_JB2_GEN=$(JBIG2OBJDIR)$(D)
LDF_JB2_OBJ=$(JBIG2OBJDIR)$(D)

# paths to source directories
LDF_JB2_COMMON=$(JBIG2SRCDIR)$(D)source$(D)common$(D)
LDF_JB2_COMPRESS=$(JBIG2SRCDIR)$(D)source$(D)compress$(D)


# source files to build from the CSDK source

ldf_jb2_common_OBJS = \
	$(LDF_JB2_OBJ)jb2_adt_cache.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_context_buffer.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_context_decoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_context_ref_buffer.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_context_ref_decoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_decoder_collective_bitmap.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_decoder_generic_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_decoder_halftone_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_decoder_pattern_dict.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_decoder_symbol_dict.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_decoder_text_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_external_cache.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_file.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_file_extras.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_handle_document.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_huffman_decoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_huffman_table.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_huffman_table_standard.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_huffman_table_symbol.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_huffman_table_user_defined.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_huffman_tree.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_location.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_memory.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_message.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_mmr_decoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_mmr_tables.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_mq_decoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_mq_state.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_pattern_dict.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_pdf_file.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_pdf_stream.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_props_decompress.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_read_bit_buffer.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_read_data.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_render_common.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_render_generic_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_render_halftone_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_render_text_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_array.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_end_of_stripe.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_generic_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_halftone_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_page_info.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_pattern_dict.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_symbol_dict.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_table.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_text_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_segment_types.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_symbol.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_symbol_dict.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_symbol_instance.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_write_bits.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_write_data.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_write_pdf.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_common.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_license_dummy.$(OBJ)

ldf_jb2_compress_OBJS = \
	$(LDF_JB2_OBJ)jb2_adt_component.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_component_class.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_component_group.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_component_match.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_context_encoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_context_ref_encoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_encoder_generic_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_encoder_symbol_dict.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_encoder_text_region.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_handle_compress.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_huffman_encoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_mmr_encoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_mq_encoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_props_compress.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_run_array.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_stack.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_stripe_clean_up.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_stripe_encoder.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_stripe_half_tone.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_stripe_preprocessing.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_stripe_text.$(OBJ)		\
	$(LDF_JB2_OBJ)jb2_adt_symbol_unify.$(OBJ)

ldf_jb2_common_HDRS = \
	$(LDF_JB2_COMMON)jb2_adt_cache.h		\
	$(LDF_JB2_COMMON)jb2_adt_context_buffer.h		\
	$(LDF_JB2_COMMON)jb2_adt_context_decoder.h		\
	$(LDF_JB2_COMMON)jb2_adt_context_ref_buffer.h		\
	$(LDF_JB2_COMMON)jb2_adt_context_ref_decoder.h		\
	$(LDF_JB2_COMMON)jb2_adt_decoder_collective_bitmap.h		\
	$(LDF_JB2_COMMON)jb2_adt_decoder_generic_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_decoder_halftone_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_decoder_pattern_dict.h		\
	$(LDF_JB2_COMMON)jb2_adt_decoder_symbol_dict.h		\
	$(LDF_JB2_COMMON)jb2_adt_decoder_text_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_external_cache.h		\
	$(LDF_JB2_COMMON)jb2_adt_file.h		\
	$(LDF_JB2_COMMON)jb2_adt_file_extras.h		\
	$(LDF_JB2_COMMON)jb2_adt_huffman_decoder.h		\
	$(LDF_JB2_COMMON)jb2_adt_huffman_table.h		\
	$(LDF_JB2_COMMON)jb2_adt_huffman_table_entry.h		\
	$(LDF_JB2_COMMON)jb2_adt_huffman_table_standard.h		\
	$(LDF_JB2_COMMON)jb2_adt_huffman_table_symbol.h		\
	$(LDF_JB2_COMMON)jb2_adt_huffman_table_user_defined.h		\
	$(LDF_JB2_COMMON)jb2_adt_huffman_tree.h		\
	$(LDF_JB2_COMMON)jb2_adt_location.h		\
	$(LDF_JB2_COMMON)jb2_adt_memory.h		\
	$(LDF_JB2_COMMON)jb2_adt_message.h		\
	$(LDF_JB2_COMMON)jb2_adt_mmr_decoder.h		\
	$(LDF_JB2_COMMON)jb2_adt_mmr_tables.h		\
	$(LDF_JB2_COMMON)jb2_adt_mq_decoder.h		\
	$(LDF_JB2_COMMON)jb2_adt_mq_ids.h		\
	$(LDF_JB2_COMMON)jb2_adt_mq_state.h		\
	$(LDF_JB2_COMMON)jb2_adt_pattern_dict.h		\
	$(LDF_JB2_COMMON)jb2_adt_pdf_file.h		\
	$(LDF_JB2_COMMON)jb2_adt_pdf_stream.h		\
	$(LDF_JB2_COMMON)jb2_adt_props_decompress.h		\
	$(LDF_JB2_COMMON)jb2_adt_read_bit_buffer.h		\
	$(LDF_JB2_COMMON)jb2_adt_read_data.h		\
	$(LDF_JB2_COMMON)jb2_adt_render_common.h		\
	$(LDF_JB2_COMMON)jb2_adt_render_generic_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_render_halftone_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_render_text_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_array.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_end_of_file.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_end_of_page.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_end_of_stripe.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_generic_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_halftone_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_page_info.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_pattern_dict.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_symbol_dict.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_table.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_text_region.h		\
	$(LDF_JB2_COMMON)jb2_adt_segment_types.h		\
	$(LDF_JB2_COMMON)jb2_adt_symbol.h		\
	$(LDF_JB2_COMMON)jb2_adt_symbol_array.h		\
	$(LDF_JB2_COMMON)jb2_adt_symbol_dict.h		\
	$(LDF_JB2_COMMON)jb2_adt_symbol_instance.h		\
	$(LDF_JB2_COMMON)jb2_adt_symbol_instance_array.h		\
	$(LDF_JB2_COMMON)jb2_adt_write_bits.h		\
	$(LDF_JB2_COMMON)jb2_adt_write_data.h		\
	$(LDF_JB2_COMMON)jb2_adt_write_pdf.h		\
	$(LDF_JB2_COMMON)jb2_common.h		\
	$(LDF_JB2_COMMON)jb2_defines.h		\
	$(LDF_JB2_COMMON)jb2_license.h

ldf_jb2_compress_HDRS = \
	$(LDF_JB2_COMPRESS)jb2_adt_run_array.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_encoder_text_region.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_context_ref_encoder.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_encoder_generic_region.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_props_compress.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_component_match.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_symbol_unify.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_huffman_encoder.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_stripe_half_tone.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_component.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_stripe_clean_up.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_run.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_stripe_text.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_stack.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_component_group.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_encoder_symbol_dict.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_context_encoder.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_component_array.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_mq_encoder.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_mmr_encoder.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_stripe_preprocessing.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_component_class.h		\
	$(LDF_JB2_COMPRESS)jb2_adt_stripe_encoder.h


ldf_jb2_OBJS=$(ldf_jb2_common_OBJS) $(ldf_jb2_compress_OBJS)
ldf_jb2_HDRS=$(ldf_jb2_common_HDRS) $(ldf_jb2_compress_HDRS)

# switch in the selected library .dev
$(LDF_JB2_GEN)ldf_jb2.dev : $(TOP_MAKEFILES) $(LDF_JB2_MAK) $(LDF_JB2_GEN)ldf_jb2_$(SHARE_JBIG2).dev
	$(CP_) $(LDF_JB2_GEN)ldf_jb2_$(SHARE_JBIG2).dev $(LDF_JB2_GEN)ldf_jb2.dev

# external link .dev
$(LDF_JB2_GEN)ldf_jb2_1.dev : $(TOP_MAKEFILES) $(LDF_JB2_MAK) $(ECHOGS_XE)
	$(SETMOD) $(LDF_JB2_GEN)ldf_jb2_1 -lib ldf_jb2

# compile our own .dev
$(LDF_JB2_GEN)ldf_jb2_0.dev : $(TOP_MAKEFILES) $(LDF_JB2_MAK) $(ECHOGS_XE) $(ldf_jb2_OBJS)
	$(SETMOD) $(LDF_JB2_GEN)ldf_jb2_0 $(ldf_jb2_common_OBJS)
	$(ADDMOD) $(LDF_JB2_GEN)ldf_jb2_0 $(ldf_jb2_compress_OBJS)

# define our specific compiler
LDF_JB2_CC=$(CC_) $(I_)$(LDF_JB2I_) $(II)$(LDF_JB2_COMMON) $(II)$(LDF_JB2_COMPRESS)$(_I) $(JB2CF_)
LDF_JB2_O=$(O_)$(LDF_JB2_OBJ)

LDF_JB2_DEP=$(AK) $(LDF_JB2_MAK)


# explicit rules for building each source file
# for simplicity we have every source file depend on all headers

$(LDF_JB2_OBJ)jb2_adt_cache.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_cache.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_cache.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_cache.c

$(LDF_JB2_OBJ)jb2_adt_context_buffer.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_context_buffer.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_context_buffer.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_context_buffer.c

$(LDF_JB2_OBJ)jb2_adt_context_decoder.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_context_decoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_context_decoder.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_context_decoder.c

$(LDF_JB2_OBJ)jb2_adt_context_ref_buffer.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_context_ref_buffer.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_context_ref_buffer.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_context_ref_buffer.c

$(LDF_JB2_OBJ)jb2_adt_context_ref_decoder.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_context_ref_decoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_context_ref_decoder.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_context_ref_decoder.c

$(LDF_JB2_OBJ)jb2_adt_decoder_collective_bitmap.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_decoder_collective_bitmap.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_decoder_collective_bitmap.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_decoder_collective_bitmap.c

$(LDF_JB2_OBJ)jb2_adt_decoder_generic_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_decoder_generic_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_decoder_generic_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_decoder_generic_region.c

$(LDF_JB2_OBJ)jb2_adt_decoder_halftone_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_decoder_halftone_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_decoder_halftone_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_decoder_halftone_region.c

$(LDF_JB2_OBJ)jb2_adt_decoder_pattern_dict.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_decoder_pattern_dict.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_decoder_pattern_dict.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_decoder_pattern_dict.c

$(LDF_JB2_OBJ)jb2_adt_decoder_symbol_dict.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_decoder_symbol_dict.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_decoder_symbol_dict.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_decoder_symbol_dict.c

$(LDF_JB2_OBJ)jb2_adt_decoder_text_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_decoder_text_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_decoder_text_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_decoder_text_region.c

$(LDF_JB2_OBJ)jb2_adt_external_cache.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_external_cache.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_external_cache.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_external_cache.c

$(LDF_JB2_OBJ)jb2_adt_file.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_file.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_file.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_file.c

$(LDF_JB2_OBJ)jb2_adt_file_extras.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_file_extras.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_file_extras.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_file_extras.c

$(LDF_JB2_OBJ)jb2_adt_handle_document.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_handle_document.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_handle_document.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_handle_document.c

$(LDF_JB2_OBJ)jb2_adt_huffman_decoder.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_huffman_decoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_huffman_decoder.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_huffman_decoder.c

$(LDF_JB2_OBJ)jb2_adt_huffman_table.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_huffman_table.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_huffman_table.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_huffman_table.c

$(LDF_JB2_OBJ)jb2_adt_huffman_table_standard.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_huffman_table_standard.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_huffman_table_standard.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_huffman_table_standard.c

$(LDF_JB2_OBJ)jb2_adt_huffman_table_symbol.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_huffman_table_symbol.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_huffman_table_symbol.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_huffman_table_symbol.c

$(LDF_JB2_OBJ)jb2_adt_huffman_table_user_defined.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_huffman_table_user_defined.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_huffman_table_user_defined.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_huffman_table_user_defined.c

$(LDF_JB2_OBJ)jb2_adt_huffman_tree.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_huffman_tree.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_huffman_tree.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_huffman_tree.c

$(LDF_JB2_OBJ)jb2_adt_location.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_location.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_location.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_location.c

$(LDF_JB2_OBJ)jb2_adt_memory.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_memory.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_memory.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_memory.c

$(LDF_JB2_OBJ)jb2_adt_message.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_message.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_message.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_message.c

$(LDF_JB2_OBJ)jb2_adt_mmr_decoder.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_mmr_decoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_mmr_decoder.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_mmr_decoder.c

$(LDF_JB2_OBJ)jb2_adt_mmr_tables.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_mmr_tables.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_mmr_tables.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_mmr_tables.c

$(LDF_JB2_OBJ)jb2_adt_mq_decoder.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_mq_decoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_mq_decoder.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_mq_decoder.c

$(LDF_JB2_OBJ)jb2_adt_mq_state.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_mq_state.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_mq_state.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_mq_state.c

$(LDF_JB2_OBJ)jb2_adt_pattern_dict.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_pattern_dict.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_pattern_dict.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_pattern_dict.c

$(LDF_JB2_OBJ)jb2_adt_pdf_file.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_pdf_file.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_pdf_file.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_pdf_file.c

$(LDF_JB2_OBJ)jb2_adt_pdf_stream.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_pdf_stream.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_pdf_stream.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_pdf_stream.c

$(LDF_JB2_OBJ)jb2_adt_props_decompress.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_props_decompress.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_props_decompress.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_props_decompress.c

$(LDF_JB2_OBJ)jb2_adt_read_bit_buffer.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_read_bit_buffer.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_read_bit_buffer.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_read_bit_buffer.c

$(LDF_JB2_OBJ)jb2_adt_read_data.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_read_data.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_read_data.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_read_data.c

$(LDF_JB2_OBJ)jb2_adt_render_common.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_render_common.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_render_common.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_render_common.c

$(LDF_JB2_OBJ)jb2_adt_render_generic_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_render_generic_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_render_generic_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_render_generic_region.c

$(LDF_JB2_OBJ)jb2_adt_render_halftone_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_render_halftone_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_render_halftone_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_render_halftone_region.c

$(LDF_JB2_OBJ)jb2_adt_render_text_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_render_text_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_render_text_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_render_text_region.c

$(LDF_JB2_OBJ)jb2_adt_segment.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment.c

$(LDF_JB2_OBJ)jb2_adt_segment_array.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_array.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_array.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_array.c

$(LDF_JB2_OBJ)jb2_adt_segment_end_of_file.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_end_of_file.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_end_of_file.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_end_of_file.c

$(LDF_JB2_OBJ)jb2_adt_segment_end_of_page.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_end_of_page.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_end_of_page.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_end_of_page.c

$(LDF_JB2_OBJ)jb2_adt_segment_end_of_stripe.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_end_of_stripe.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_end_of_stripe.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_end_of_stripe.c

$(LDF_JB2_OBJ)jb2_adt_segment_generic_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_generic_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_generic_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_generic_region.c

$(LDF_JB2_OBJ)jb2_adt_segment_halftone_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_halftone_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_halftone_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_halftone_region.c

$(LDF_JB2_OBJ)jb2_adt_segment_page_info.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_page_info.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_page_info.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_page_info.c

$(LDF_JB2_OBJ)jb2_adt_segment_pattern_dict.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_pattern_dict.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_pattern_dict.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_pattern_dict.c

$(LDF_JB2_OBJ)jb2_adt_segment_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_region.c

$(LDF_JB2_OBJ)jb2_adt_segment_symbol_dict.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_symbol_dict.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_symbol_dict.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_symbol_dict.c

$(LDF_JB2_OBJ)jb2_adt_segment_table.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_table.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_table.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_table.c

$(LDF_JB2_OBJ)jb2_adt_segment_text_region.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_text_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_text_region.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_text_region.c

$(LDF_JB2_OBJ)jb2_adt_segment_types.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_segment_types.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_segment_types.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_segment_types.c

$(LDF_JB2_OBJ)jb2_adt_symbol.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_symbol.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_symbol.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_symbol.c

$(LDF_JB2_OBJ)jb2_adt_symbol_dict.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_symbol_dict.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_symbol_dict.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_symbol_dict.c

$(LDF_JB2_OBJ)jb2_adt_symbol_instance.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_symbol_instance.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_symbol_instance.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_symbol_instance.c

$(LDF_JB2_OBJ)jb2_adt_write_bits.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_write_bits.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_write_bits.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_write_bits.c

$(LDF_JB2_OBJ)jb2_adt_write_data.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_write_data.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_write_data.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_write_data.c

$(LDF_JB2_OBJ)jb2_adt_write_pdf.$(OBJ) : $(LDF_JB2_COMMON)jb2_adt_write_pdf.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_write_pdf.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_adt_write_pdf.c

$(LDF_JB2_OBJ)jb2_common.$(OBJ) : $(LDF_JB2_COMMON)jb2_common.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_common.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_common.c

$(LDF_JB2_OBJ)jb2_license_dummy.$(OBJ) : $(LDF_JB2_COMMON)jb2_license_dummy.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_license_dummy.$(OBJ) $(C_) $(LDF_JB2_COMMON)jb2_license_dummy.c

$(LDF_JB2_OBJ)jb2_adt_component.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_component.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_component.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_component.c

$(LDF_JB2_OBJ)jb2_adt_component_class.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_component_class.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_component_class.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_component_class.c

$(LDF_JB2_OBJ)jb2_adt_component_group.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_component_group.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_component_group.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_component_group.c

$(LDF_JB2_OBJ)jb2_adt_component_match.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_component_match.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_component_match.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_component_match.c

$(LDF_JB2_OBJ)jb2_adt_context_encoder.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_context_encoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_context_encoder.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_context_encoder.c

$(LDF_JB2_OBJ)jb2_adt_context_ref_encoder.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_context_ref_encoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_context_ref_encoder.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_context_ref_encoder.c

$(LDF_JB2_OBJ)jb2_adt_encoder_generic_region.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_encoder_generic_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_encoder_generic_region.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_encoder_generic_region.c

$(LDF_JB2_OBJ)jb2_adt_encoder_symbol_dict.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_encoder_symbol_dict.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_encoder_symbol_dict.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_encoder_symbol_dict.c

$(LDF_JB2_OBJ)jb2_adt_encoder_text_region.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_encoder_text_region.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_encoder_text_region.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_encoder_text_region.c

$(LDF_JB2_OBJ)jb2_adt_handle_compress.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_handle_compress.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_handle_compress.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_handle_compress.c

$(LDF_JB2_OBJ)jb2_adt_huffman_encoder.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_huffman_encoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_huffman_encoder.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_huffman_encoder.c

$(LDF_JB2_OBJ)jb2_adt_mmr_encoder.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_mmr_encoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_mmr_encoder.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_mmr_encoder.c

$(LDF_JB2_OBJ)jb2_adt_mq_encoder.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_mq_encoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_mq_encoder.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_mq_encoder.c

$(LDF_JB2_OBJ)jb2_adt_props_compress.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_props_compress.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_props_compress.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_props_compress.c

$(LDF_JB2_OBJ)jb2_adt_run_array.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_run_array.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_run_array.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_run_array.c

$(LDF_JB2_OBJ)jb2_adt_stack.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_stack.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_stack.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_stack.c

$(LDF_JB2_OBJ)jb2_adt_stripe_clean_up.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_stripe_clean_up.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_stripe_clean_up.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_stripe_clean_up.c

$(LDF_JB2_OBJ)jb2_adt_stripe_encoder.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_stripe_encoder.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_stripe_encoder.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_stripe_encoder.c

$(LDF_JB2_OBJ)jb2_adt_stripe_half_tone.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_stripe_half_tone.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_stripe_half_tone.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_stripe_half_tone.c

$(LDF_JB2_OBJ)jb2_adt_stripe_preprocessing.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_stripe_preprocessing.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_stripe_preprocessing.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_stripe_preprocessing.c

$(LDF_JB2_OBJ)jb2_adt_stripe_text.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_stripe_text.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_stripe_text.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_stripe_text.c

$(LDF_JB2_OBJ)jb2_adt_symbol_unify.$(OBJ) : $(LDF_JB2_COMPRESS)jb2_adt_symbol_unify.c $(LDF_JB2_DEP) $(ldf_jb2_HDRS)
	$(LDF_JB2_CC) $(LDF_JB2_O)jb2_adt_symbol_unify.$(OBJ) $(C_) $(LDF_JB2_COMPRESS)jb2_adt_symbol_unify.c


# end of file
