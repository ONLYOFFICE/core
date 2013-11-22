/******************************************************************************* *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2007-2009 Intel Corporation. All Rights Reserved.

File Name: mfxstructures.h

*******************************************************************************/
#ifndef __MFXSTRUCTURES_H__
#define __MFXSTRUCTURES_H__
#include "mfxdefs.h"

#pragma warning(disable: 4201)

#ifdef __cplusplus
extern "C" {
#endif

#define MFX_MAKEFOURCC(A,B,C,D)    ((((int)A))+(((int)B)<<8)+(((int)C)<<16)+(((int)D)<<24))

/* Extended Configuration Header Structure */
typedef struct {
    mfxU32  BufferId;
    mfxU32  BufferSz;
} mfxExtBuffer;

/* Frame Info */
typedef struct {
    mfxU32  reserved[8];

    mfxU32  FourCC;
    mfxU16  Width;
    mfxU16  Height;

    mfxU16  CropX;
    mfxU16  CropY;
    mfxU16  CropW;
    mfxU16  CropH;

    mfxU32  FrameRateExtN;
    mfxU32  FrameRateExtD;
    mfxU16  reserved3;

    mfxU16  AspectRatioW;
    mfxU16  AspectRatioH;

    mfxU16  PicStruct;
    mfxU16  ChromaFormat;
    mfxU16  reserved2;
} mfxFrameInfo;

/* FourCC */
enum {
    MFX_FOURCC_NV12         =MFX_MAKEFOURCC('N','V','1','2'),   /* Native Format */
    MFX_FOURCC_YV12         =MFX_MAKEFOURCC('Y','V','1','2'),
    MFX_FOURCC_YUY2         =MFX_MAKEFOURCC('Y','U','Y','2'),
    MFX_FOURCC_RGB3         =MFX_MAKEFOURCC('R','G','B','3'),   /* RGB24 */
    MFX_FOURCC_RGB4         =MFX_MAKEFOURCC('R','G','B','4')    /* RGB32 */
};

/* PicStruct */
enum {
    MFX_PICSTRUCT_UNKNOWN       =0x00,
    MFX_PICSTRUCT_PROGRESSIVE   =0x01,
    MFX_PICSTRUCT_FIELD_TFF     =0x02,
    MFX_PICSTRUCT_FIELD_BFF     =0x04,

    MFX_PICSTRUCT_FIELD_REPEATED=0x10,  /* first field repeated, pic_struct=5 or 6 in H.264 */
    MFX_PICSTRUCT_FRAME_DOUBLING=0x20,  /* pic_struct=7 in H.264 */
    MFX_PICSTRUCT_FRAME_TRIPLING=0x40   /* pic_struct=8 in H.264 */
};

/* ColorFormat */
enum {
    MFX_CHROMAFORMAT_MONOCHROME =0,
    MFX_CHROMAFORMAT_YUV420     =1,
    MFX_CHROMAFORMAT_YUV422     =2,
    MFX_CHROMAFORMAT_YUV444     =3
};

/* Frame Data Info */
typedef struct {
    mfxU32      reserved[8];

    mfxU64      TimeStamp;
    mfxU32      FrameOrder;
    mfxU16      Locked;
    mfxU16      Pitch;

    /* color planes */
    union {
        mfxU8   *Y;
        mfxU8   *R;
    };
    union {
        mfxU8   *UV;            /* for UV merged formats */
        mfxU8   *VU;            /* for VU merged formats */
        mfxU8   *CbCr;          /* for CbCr merged formats */
        mfxU8   *CrCb;          /* for CrCb merged formats */
        mfxU8   *Cb;
        mfxU8   *U;
        mfxU8   *G;
    };
    union {
        mfxU8   *Cr;
        mfxU8   *V;
        mfxU8   *B;
    };
    mfxU8       *A;
    mfxMemId    MemId;

    /* Additional Flags */
    mfxU16      Corrupted;
    mfxU16      reserved2;
} mfxFrameData;

/* Frame Surface */
typedef struct {
    mfxU32  reserved[4];
    mfxFrameInfo    Info;
    mfxFrameData    Data;
} mfxFrameSurface1;

/* Transcoding Info */
typedef struct {
    mfxU32  reserved[8];

    mfxFrameInfo    FrameInfo;
    mfxU32  CodecId;
    mfxU16  CodecProfile;
    mfxU16  CodecLevel;
    mfxU16  NumThread;

    union {
        /* ENCODE */
        struct {
            mfxU16  TargetUsage;

            mfxU16  GopPicSize;
            mfxU16  GopRefDist;
            mfxU16  GopOptFlag;
            mfxU16  IdrInterval;

            mfxU16  RateControlMethod;
            mfxU16  InitialDelayInKB;
            mfxU16  BufferSizeInKB;
            mfxU16  TargetKbps;
            mfxU16  MaxKbps;

            mfxU16  NumSlice;
            mfxU16  NumRefFrame;
            mfxU16  EncodedOrder;
        };
        struct {    /* DECODE */
            mfxU16  DecodedOrder;
            mfxU16  reserved2[12];
        };
    };
} mfxInfoMFX;

typedef struct {
    mfxU32  reserved[8];
    mfxFrameInfo    In;
    mfxFrameInfo    Out;
} mfxInfoVPP;

typedef struct {
    mfxU32  reserved[4];
    union {
        mfxInfoMFX  mfx;
        mfxInfoVPP  vpp;
    };
    mfxU16  Protected;
    mfxU16  IOPattern;
    mfxExtBuffer** ExtParam;
    mfxU16  NumExtParam;
    mfxU16  reserved2;
} mfxVideoParam;

/* IOPattern */
enum {
    MFX_IOPATTERN_IN_VIDEO_MEMORY   = 0x01,
    MFX_IOPATTERN_IN_SYSTEM_MEMORY  = 0x02,
    MFX_IOPATTERN_OUT_VIDEO_MEMORY  = 0x10,
    MFX_IOPATTERN_OUT_SYSTEM_MEMORY = 0x20
};

/* CodecId */
enum {
    MFX_CODEC_AVC          =MFX_MAKEFOURCC('A','V','C',' '),
    MFX_CODEC_MPEG2       =MFX_MAKEFOURCC('M','P','G','2'),
    MFX_CODEC_VC1         =MFX_MAKEFOURCC('V','C','1',' ')
};

/* CodecProfile, CodecLevel */
enum {
    MFX_PROFILE_UNKNOWN         =0,
    MFX_LEVEL_UNKNOWN           =0,

    /* AVC Profiles & Levels */
    MFX_PROFILE_AVC_BASELINE    =66,
    MFX_PROFILE_AVC_MAIN        =77,
    MFX_PROFILE_AVC_HIGH        =100,

    MFX_LEVEL_AVC_1             =10,
    MFX_LEVEL_AVC_1b            =9,
    MFX_LEVEL_AVC_11            =11,
    MFX_LEVEL_AVC_12            =12,
    MFX_LEVEL_AVC_13            =13,
    MFX_LEVEL_AVC_2             =20,
    MFX_LEVEL_AVC_21            =21,
    MFX_LEVEL_AVC_22            =22,
    MFX_LEVEL_AVC_3             =30,
    MFX_LEVEL_AVC_31            =31,
    MFX_LEVEL_AVC_32            =32,
    MFX_LEVEL_AVC_4             =40,
    MFX_LEVEL_AVC_41            =41,
    MFX_LEVEL_AVC_42            =42,
    MFX_LEVEL_AVC_5             =50,
    MFX_LEVEL_AVC_51            =51,

    /* MPEG-2 Profiles & Levels */
    MFX_PROFILE_MPEG2_SIMPLE    =0x50,
    MFX_PROFILE_MPEG2_MAIN      =0x40,
    MFX_PROFILE_MPEG2_HIGH      =0x10,

    MFX_LEVEL_MPEG2_LOW         =0xA,
    MFX_LEVEL_MPEG2_MAIN        =0x8,
    MFX_LEVEL_MPEG2_HIGH        =0x4,
    MFX_LEVEL_MPEG2_HIGH1440    =0x6,

    /* VC1 Profiles & Levels */
    MFX_PROFILE_VC1_SIMPLE      =(0+1),
    MFX_PROFILE_VC1_MAIN        =(4+1),
    MFX_PROFILE_VC1_ADVANCED    =(12+1),

    /* VC1 levels for simple & main profiles */
    MFX_LEVEL_VC1_LOW           =(0+1),
    MFX_LEVEL_VC1_MEDIAN        =(2+1),
    MFX_LEVEL_VC1_HIGH          =(4+1),

    /* VC1 levels for the advanced profile */
    MFX_LEVEL_VC1_0             =(0x00+1),
    MFX_LEVEL_VC1_1             =(0x01+1),
    MFX_LEVEL_VC1_2             =(0x02+1),
    MFX_LEVEL_VC1_3             =(0x03+1),
    MFX_LEVEL_VC1_4             =(0x04+1)
};

/* GopOptFlag */
enum {
    MFX_GOP_CLOSED          =1,
    MFX_GOP_STRICT          =2
};

/* TargetUsages: 1~7; */
enum {
    MFX_TARGETUSAGE_UNKNOWN         =0,
    MFX_TARGETUSAGE_BEST_QUALITY    =1,
    MFX_TARGETUSAGE_BALANCED        =4,
    MFX_TARGETUSAGE_BEST_SPEED      =7
};

/* RateControlMethod */
enum {
    MFX_RATECONTROL_CBR     =1,
    MFX_RATECONTROL_VBR     =2
};

typedef struct {
    mfxExtBuffer Header;

    mfxU16      reserved1;
    mfxU16      RateDistortionOpt;
    mfxU16      MECostType;
    mfxU16      MESearchType;
    mfxI16Pair  MVSearchWindow;
    mfxU16      EndOfSequence;
    mfxU16      FramePicture;

    union {
        struct {    /* AVC */
            mfxU16      CAVLC;
            mfxU16      reserved2[7];
            mfxU16      RefPicListReordering;
            mfxU16      ResetRefList;
            mfxU16      reserved3[2];

            mfxU16      IntraPredBlockSize;
            mfxU16      InterPredBlockSize;
            mfxU16      MVPrecision;
            mfxU16      MaxDecFrameBuffering;

            mfxU16      AUDelimiter;
            mfxU16      EndOfStream;
            mfxU16      PicTimingSEI;
            mfxU16      VuiNalHrdParameters;
        };
    };
} mfxExtCodingOption;

/* IntraPredBlockSize/InterPredBlockSize */
enum {
    MFX_BLOCKSIZE_UNKNOWN   = 0,
    MFX_BLOCKSIZE_MIN_16X16 = 1, /* 16x16              */
    MFX_BLOCKSIZE_MIN_8X8   = 2, /* 16x16, 8x8         */
    MFX_BLOCKSIZE_MIN_4X4   = 3  /* 16x16, 8x8, 4x4    */
};

/* MVPrecision */
enum {
    MFX_MVPRECISION_UNKNOWN    = 0,
    MFX_MVPRECISION_INTEGER    = (1 << 0),
    MFX_MVPRECISION_HALFPEL    = (1 << 1),
    MFX_MVPRECISION_QUARTERPEL = (1 << 2)
};

/* MECostType */
enum {
    MFX_COSTTYPE_UNKNOWN       = 0,
    MFX_COSTTYPE_SAD           = (1 << 0), /* sum of absolute difference */
    MFX_COSTTYPE_SSD           = (1 << 1), /* sum of squared difference */
    MFX_COSTTYPE_SATD_HADAMARD = (1 << 2)  /* sum of absolute hadamard transformed difference */
};

enum {
    MFX_SEARCHTYPE_UNKNOWN  = 0,
    MFX_SEARCHTYPE_FULL     = (1 << 0),
    MFX_SEARCHTYPE_UMH      = (1 << 1),
    MFX_SEARCHTYPE_LOG      = (1 << 2),
    MFX_SEARCHTYPE_SQUARE   = (1 << 4),
    MFX_SEARCHTYPE_DIAMOND  = (1 << 5)
};

enum {
    MFX_CODINGOPTION_UNKNOWN    =0,
    MFX_CODINGOPTION_ON         =0x10,
    MFX_CODINGOPTION_OFF        =0x20
};

/* Frame Bitstreams */
typedef struct {
    mfxU32      reserved[8];

    mfxU64      TimeStamp;
    mfxU8*      Data;
    mfxU32      DataOffset;
    mfxU32      DataLength;
    mfxU32      MaxLength;

    mfxU16      PicStruct;
    mfxU16      FrameType;
    mfxU16      DataFlag;
    mfxU16      reserved2;
} mfxBitstream;

/* Data Flag */
enum {
    MFX_BITSTREAM_COMPLETE_FRAME    = 0x0001        /* the bitstream contains a complete frame or field pair of data */
};

/* Extended Buffer Ids */
enum {
    MFX_EXTBUFF_CODING_OPTION       =   MFX_MAKEFOURCC('C','D','O','P'),
    MFX_EXTBUFF_CODING_OPTION_SPSPPS=   MFX_MAKEFOURCC('C','O','S','P'),
    MFX_EXTBUFF_VPP_DONOTUSE        =   MFX_MAKEFOURCC('N','U','S','E'),
    MFX_EXTBUFF_VPP_AUXDATA         =   MFX_MAKEFOURCC('A','U','X','D'),
    MFX_EXTBUFF_VPP_DENOISE         =   MFX_MAKEFOURCC('D','N','I','S'),
    MFX_EXTBUFF_VPP_SCENE_ANALYSIS  =   MFX_MAKEFOURCC('S','C','L','Y')
};

/* VPP Conf: Do not use certain algorithms  */
typedef struct {
    mfxExtBuffer    Header;
    mfxU32          NumAlg;
    mfxU32*         AlgList;
} mfxExtVPPDoNotUse;

/* statistics collected for decode, encode and vpp */
typedef struct {
    mfxU32  reserved[16];
    mfxU32  NumFrame;
    mfxU64  NumBit;
    mfxU32  NumCachedFrame;
} mfxEncodeStat;

typedef struct {
    mfxU32  reserved[16];
    mfxU32  NumFrame;
    mfxU32  NumSkippedFrame;
    mfxU32  NumError;
    mfxU32  NumCachedFrame;
} mfxDecodeStat;

typedef struct {
    mfxU32  reserved[16];
    mfxU32  NumFrame;
    mfxU32  NumCachedFrame;
} mfxVPPStat;

typedef struct {
    mfxExtBuffer    Header;
    mfxU32          SpatialComplexity;
    mfxU32          TemporalComplexity;
    mfxU16          SceneChangeRate;
    mfxU16          RepeatedFrame;
} mfxExtVppAuxData;

typedef struct {
    mfxU32      reserved[4];
    mfxU8       *Data;      /* buffer pointer */
    mfxU32      NumBit;     /* number of bits */
    mfxU16      Type;       /* SEI message type in H.264 or user data start_code in MPEG-2 */
    mfxU16      BufSize;    /* payload buffer size */
} mfxPayload;

typedef struct {
    mfxU32  reserved[8];

    mfxU16  FrameType;
    mfxU16  NumExtParam;
    mfxU16  NumPayload;     /* MPEG-2 user data or H.264 SEI message(s) */
    mfxU16  reserved2;

    mfxExtBuffer    **ExtParam;
    mfxPayload      **Payload;      /* for field pair, first field uses even payloads and second field uses odd payloads */
} mfxEncodeCtrl;

/* Buffer Memory Types */
enum {
    /* Buffer types */
    MFX_MEMTYPE_PERSISTENT_MEMORY   =0x0002
};

/* Frame Memory Types */
enum {
    MFX_MEMTYPE_DXVA2_DECODER_TARGET       =0x0010,
    MFX_MEMTYPE_DXVA2_PROCESSOR_TARGET     =0x0020,
    MFX_MEMTYPE_SYSTEM_MEMORY              =0x0040,

    MFX_MEMTYPE_FROM_ENCODE     = 0x0100,
    MFX_MEMTYPE_FROM_DECODE     = 0x0200,
    MFX_MEMTYPE_FROM_VPPIN      = 0x0400,
    MFX_MEMTYPE_FROM_VPPOUT     = 0x0800,

    MFX_MEMTYPE_INTERNAL_FRAME  = 0x0001,
    MFX_MEMTYPE_EXTERNAL_FRAME  = 0x0002
};

typedef struct {
    mfxU32  reserved[4];
    mfxFrameInfo    Info;
    mfxU16  Type;   /* decoder or processor render targets */
    mfxU16  NumFrameMin;
    mfxU16  NumFrameSuggested;
    mfxU16  reserved2;
} mfxFrameAllocRequest;

typedef struct {
    mfxU32      reserved[4];
    mfxMemId    *mids;      /* the array allocated by application */
    mfxU16      NumFrameActual;
    mfxU16      reserved2;
} mfxFrameAllocResponse;

/* FrameType */
enum {
    MFX_FRAMETYPE_I             =0x1,
    MFX_FRAMETYPE_P             =0x2,
    MFX_FRAMETYPE_B             =0x4,
    MFX_FRAMETYPE_S             =0x8,

    MFX_FRAMETYPE_REF           =0x40,
    MFX_FRAMETYPE_IDR           =0x80
};

typedef enum {
    MFX_HANDLE_DIRECT3D_DEVICE_MANAGER9         =1      /* IDirect3DDeviceManager9      */
} mfxHandleType;

typedef enum {
    MFX_SKIPMODE_NOSKIP=0,
    MFX_SKIPMODE_MORE=1,
    MFX_SKIPMODE_LESS=2
} mfxSkipMode;

/* Library initialization and deinitialization */
typedef enum {
    MFX_IMPL_AUTO=0,        /* Auto Selection/In or Not Supported/Out */
    MFX_IMPL_SOFTWARE,      /* Pure Software Implementation */
    MFX_IMPL_HARDWARE,      /* Hardware Accelerated Implementation */
    MFX_IMPL_UNSUPPORTED=0  /* One of the MFXQueryIMPL returns */
} mfxIMPL;

/* Version Info */
typedef union {
    struct {
        mfxU16  Minor;
        mfxU16  Major;
    };
    mfxU32  Version;
} mfxVersion;

typedef struct {
    mfxExtBuffer    Header;
    mfxU8           *SPSBuffer;
    mfxU8           *PPSBuffer;
    mfxU16          SPSBufSize;
    mfxU16          PPSBufSize;
    mfxU16          SPSId;
    mfxU16          PPSId;
} mfxExtCodingOptionSPSPPS;

#ifdef __cplusplus
} // extern "C"
#endif

#endif

