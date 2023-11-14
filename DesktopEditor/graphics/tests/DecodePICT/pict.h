#ifndef PICT_H
#define PICT_H

typedef long long ssize_t;

typedef enum
{
  UndefinedCache,
  DiskCache,
  DistributedCache,
  MapCache,
  MemoryCache,
  PingCache
} CacheType;

typedef enum
{
  UndefinedType,
  BilevelType,
  GrayscaleType,
  GrayscaleAlphaType,
  PaletteType,
  PaletteAlphaType,
  TrueColorType,
  TrueColorAlphaType,
  ColorSeparationType,
  ColorSeparationAlphaType,
  OptimizeType,
  PaletteBilevelAlphaType
} ImageType;

typedef enum
{
  UndefinedClass,
  DirectClass,
  PseudoClass
} ClassType;

typedef enum
{
  UndefinedPixelTrait = 0x000000,
  CopyPixelTrait = 0x000001,
  UpdatePixelTrait = 0x000002,
  BlendPixelTrait = 0x000004
} PixelTrait;

typedef enum
{
  UndefinedPixelChannel = 0,
  RedPixelChannel = 0,
  CyanPixelChannel = 0,
  GrayPixelChannel = 0,
  LPixelChannel = 0,
  LabelPixelChannel = 0,
  YPixelChannel = 0,
  aPixelChannel = 1,
  GreenPixelChannel = 1,
  MagentaPixelChannel = 1,
  CbPixelChannel = 1,
  bPixelChannel = 2,
  BluePixelChannel = 2,
  YellowPixelChannel = 2,
  CrPixelChannel = 2,
  BlackPixelChannel = 3,
  AlphaPixelChannel = 4,
  IndexPixelChannel = 5,
  ReadMaskPixelChannel = 6,
  WriteMaskPixelChannel = 7,
  MetaPixelChannel = 8, /* deprecated */
  CompositeMaskPixelChannel = 9,
  MetaPixelChannels = 10,
  IntensityPixelChannel = 64,  /* ???? */
  CompositePixelChannel = 64,  /* ???? */
  SyncPixelChannel = 65      /* not a real channel */
} PixelChannel;

typedef enum
{
  UndefinedColorspace,
  CMYColorspace,           /* negated linear RGB colorspace */
  CMYKColorspace,          /* CMY with Black separation */
  GRAYColorspace,          /* Single Channel greyscale (non-linear) image */
  HCLColorspace,
  HCLpColorspace,
  HSBColorspace,
  HSIColorspace,
  HSLColorspace,
  HSVColorspace,           /* alias for HSB */
  HWBColorspace,
  LabColorspace,
  LCHColorspace,           /* alias for LCHuv */
  LCHabColorspace,         /* Cylindrical (Polar) Lab */
  LCHuvColorspace,         /* Cylindrical (Polar) Luv */
  LogColorspace,
  LMSColorspace,
  LuvColorspace,
  OHTAColorspace,
  Rec601YCbCrColorspace,
  Rec709YCbCrColorspace,
  RGBColorspace,           /* Linear RGB colorspace */
  scRGBColorspace,         /* ??? */
  sRGBColorspace,          /* Default: non-linear sRGB colorspace */
  TransparentColorspace,
  xyYColorspace,
  XYZColorspace,           /* IEEE Color Reference colorspace */
  YCbCrColorspace,
  YCCColorspace,
  YDbDrColorspace,
  YIQColorspace,
  YPbPrColorspace,
  YUVColorspace,
  LinearGRAYColorspace,     /* Single Channel greyscale (linear) image */
  JzazbzColorspace,
  DisplayP3Colorspace,
  Adobe98Colorspace,
  ProPhotoColorspace,
  OklabColorspace,
  OklchColorspace
} ColorspaceType;

typedef struct _PrimaryInfo
{
  double
    x,
    y,
    z;
} PrimaryInfo;

typedef struct _ChromaticityInfo
{
  PrimaryInfo
    red_primary,
    green_primary,
    blue_primary,
    white_point;
} ChromaticityInfo;

typedef struct _PICTCode
{
  const char
    *name;

  ssize_t
    length;

  const char
    *description;
} PICTCode;

typedef struct
{
  char
    *path;

  unsigned char
    *datum;

  size_t
    length,
    signature;

  char
    *name;
} StringInfo;

typedef struct
{
    short
      top,
      left,
      bottom,
      right;
} PICTrectangle;

typedef struct
{
  PixelChannel
    channel;

  PixelTrait
    traits;

  ssize_t
    offset;
} PixelChannelMap;

typedef struct
{
  short
    version,
    pack_type;

  size_t
    pack_size,
    horizontal_resolution,
    vertical_resolution;

  short
    pixel_type,
    bits_per_pixel,
    component_count,
    component_size;

  size_t
    plane_bytes,
    table,
    reserved;
} PICTPixmap;

typedef struct
{
    size_t
      width,
      height;

    ssize_t
      x,
      y;
} RectangleInfo;

typedef enum
{
  UndefinedException,
  WarningException = 300,
  ResourceLimitWarning = 300,
  TypeWarning = 305,
  OptionWarning = 310,
  DelegateWarning = 315,
  MissingDelegateWarning = 320,
  CorruptImageWarning = 325,
  FileOpenWarning = 330,
  BlobWarning = 335,
  StreamWarning = 340,
  CacheWarning = 345,
  CoderWarning = 350,
  FilterWarning = 352,
  ModuleWarning = 355,
  DrawWarning = 360,
  ImageWarning = 365,
  WandWarning = 370,
  RandomWarning = 375,
  XServerWarning = 380,
  MonitorWarning = 385,
  RegistryWarning = 390,
  ConfigureWarning = 395,
  PolicyWarning = 399,
  ErrorException = 400,
  ResourceLimitError = 400,
  TypeError = 405,
  OptionError = 410,
  DelegateError = 415,
  MissingDelegateError = 420,
  CorruptImageError = 425,
  FileOpenError = 430,
  BlobError = 435,
  StreamError = 440,
  CacheError = 445,
  CoderError = 450,
  FilterError = 452,
  ModuleError = 455,
  DrawError = 460,
  ImageError = 465,
  WandError = 470,
  RandomError = 475,
  XServerError = 480,
  MonitorError = 485,
  RegistryError = 490,
  ConfigureError = 495,
  PolicyError = 499,
  FatalErrorException = 700,
  ResourceLimitFatalError = 700,
  TypeFatalError = 705,
  OptionFatalError = 710,
  DelegateFatalError = 715,
  MissingDelegateFatalError = 720,
  CorruptImageFatalError = 725,
  FileOpenFatalError = 730,
  BlobFatalError = 735,
  StreamFatalError = 740,
  CacheFatalError = 745,
  CoderFatalError = 750,
  FilterFatalError = 752,
  ModuleFatalError = 755,
  DrawFatalError = 760,
  ImageFatalError = 765,
  WandFatalError = 770,
  RandomFatalError = 775,
  XServerFatalError = 780,
  MonitorFatalError = 785,
  RegistryFatalError = 790,
  ConfigureFatalError = 795,
  PolicyFatalError = 799
} ExceptionType;

typedef struct _ElementInfo
{
  void
    *value;

  struct _ElementInfo
    *next;
} ElementInfo;

typedef struct _LinkedListInfo
{
  size_t
    capacity,
    elements;

  ElementInfo
    *head,
    *tail,
    *next;

  size_t
    signature;
}LinkedListInfo;

typedef struct _ExceptionInfo
{
  ExceptionType
    severity;

  int
    error_number;

  char
    *reason,
    *description;

  void
    *exceptions;

  int
    relinquish;

  size_t
    signature;
}ExceptionInfo;

typedef struct _NexusInfo
{
    RectangleInfo
      region;

    size_t
      length;

    unsigned char
      *pixels;

    int
      authentic_pixel_cache;

    size_t
      signature;

    struct _NexusInfo
      *virtual_nexus;
} NexusInfo;

typedef struct _NodeInfo
{
  void
    *key;

  void
    *value;

  struct _NodeInfo
    *left,
    *right;
} NodeInfo;

typedef struct
{
  NodeInfo
    *root;

  int
    (*compare)(const void *,const void *);

  int
    balance;

  void
    *key,
    *next;

  size_t
    nodes;

  size_t
    signature;
} SplayTreeInfo;

typedef struct
{
  ClassType
    storage_class;

  ColorspaceType
    colorspace;

  PixelTrait
    alpha_trait;

  double
    fuzz;

  size_t
    depth,
    count;

  double
    red,
    green,
    blue,
    black,
    alpha,
    index;
} PixelInfo;

typedef struct
{
  int
    temporary,          /* image file to be deleted after read "ephemeral:" */
    adjoin,             /* save images to separate scene files */
    affirm,
    antialias;

  char
    *size,              /* image generation size */
    *extract,           /* crop/resize string on image read */
    *page,
    *scenes;            /* scene numbers that is to be read in */

  size_t
    scene,              /* starting value for image save numbering */
    number_scenes,      /* total number of images in list - for escapes */
    depth;              /* current read/save depth of images */

  size_t
    quality;            /* compression quality */

  char
    *sampling_factor,   /* Chroma subsampling ratio string */
    *server_name,       /* X windows server name - display/animate */
    *font,              /* DUP for draw_info */
    *texture,           /* montage/display background tile */
    *density;           /* DUP for image and draw_info */

  double
    pointsize,
    fuzz;               /* current color fuzz attribute */

  PixelInfo
    alpha_color,        /* deprecated */
    background_color,   /* user set background color */
    border_color,       /* user set border color */
    transparent_color;  /* color for transparent index in color tables */
                        /* NB: fill color is only needed in draw_info! */
                        /* the same for undercolor (for font drawing) */

  int
    dither,             /* dither enable-disable */
    monochrome;         /* read/write pcl,pdf,ps,xps as monochrome image */

  ColorspaceType
    colorspace;

  int
    ping,                    /* fast read image attributes, not image data */
    verbose;                 /* verbose output enable/disable */

  void
    *options;                /* splay tree of global options */

  void
    *profile;

  void
    *client_data;

  void
    *blob;

  size_t
    length;

  char
    magick[4096],    /* image file format (file magick) */
    unique[4096],    /* unique temporary filename - delegates */
    filename[4096];  /* filename when reading/writing image */

  size_t
    signature;

  PixelInfo
    matte_color;        /* matte (frame) color */
} ImageInfo;

typedef struct _GeometryInfo
{
  double
    rho,
    sigma,
    xi,
    psi,
    chi;
} GeometryInfo;

typedef enum
{
  UndefinedChannel = 0x0000,
  RedChannel = 0x0001,
  GrayChannel = 0x0001,
  CyanChannel = 0x0001,
  LChannel = 0x0001,
  GreenChannel = 0x0002,
  MagentaChannel = 0x0002,
  aChannel = 0x0002,
  BlueChannel = 0x0004,
  bChannel = 0x0002,
  YellowChannel = 0x0004,
  BlackChannel = 0x0008,
  AlphaChannel = 0x0010,
  OpacityChannel = 0x0010,
  IndexChannel = 0x0020,             /* Color Index Table? */
  ReadMaskChannel = 0x0040,          /* Pixel is Not Readable? */
  WriteMaskChannel = 0x0080,         /* Pixel is Write Protected? */
  MetaChannel = 0x0100,              /* not used */
  CompositeMaskChannel = 0x0200,     /* SVG mask */
  CompositeChannels = 0x001F,
  AllChannels = 0X7FFFFFF,
  /*
    Special purpose channel types.
    FUTURE: are these needed any more - they are more like hacks
    SyncChannels for example is NOT a real channel but a 'flag'
    It really says -- "User has not defined channels"
    Though it does have extra meaning in the "-auto-level" operator
  */
  TrueAlphaChannel = 0x0100, /* extract actual alpha channel from opacity */
  RGBChannels = 0x0200,      /* set alpha from grayscale mask in RGB */
  GrayChannels = 0x0400,
  SyncChannels = 0x20000,    /* channels modified as a single unit */
  DefaultChannels = AllChannels
} ChannelType;

typedef enum
{
  ReadMode,
  WriteMode,
  IOMode,
  PersistMode
} MapMode;

//typedef struct _CacheInfo
//{
//  ClassType
//    storage_class;

//  ColorspaceType
//    colorspace;

//  PixelTrait
//    alpha_trait;

//  ChannelType
//    channels;

//  size_t
//    columns,
//    rows;

//  size_t
//    metacontent_extent,
//    number_channels;

//  PixelChannelMap
//    channel_map[64];

//  CacheType
//    type;

//  MapMode
//    mode,
//    disk_mode;

//  int
//    mapped;

//  ssize_t
//    offset;

//  size_t
//    length;

////  VirtualPixelMethod
////    virtual_pixel_method;

//  PixelInfo
//    virtual_pixel_color;

//  NexusInfo
//    **nexus_info;

//  unsigned char
//    *pixels;

//  void
//    *metacontent;

//  int
//    file;

//  char
//    filename[4096],
//    cache_filename[4096];

////  CacheMethods
////    methods;

//  void
//    *server_info;

//  int
//    synchronize,
//    debug;

//  ssize_t
//    reference_count;

//  size_t
//    signature;

//  int
//    composite_mask;

//  size_t
//    width_limit,
//    height_limit;
//} CacheInfo;

typedef struct
{
    ClassType
      storage_class;

    ColorspaceType
      colorspace;

    ChromaticityInfo
      chromaticity;

    int
      m_pctVersion;

    size_t
      m_nHeight,
      m_nWidth,
      m_ndepth,
      m_nPixelsSize,
      colors;

    SplayTreeInfo
      *profiles,
      *artifacts;

    double
      fuzz;

    unsigned char
      *ppixels;

    double
      resolutionX,
      resolutionY,
      gamma;

    ImageType
      type;

    PixelInfo
      background_color,
      *colormap;

    PixelTrait
      alpha_trait;

    NexusInfo
      *nexus;

    PixelChannelMap
      *channel_map;

    PixelTrait
      mask_trait;

    int
      taint;

    void
      *cache;

    char
      error[256];
}Image;

int DecodePICT(const char* filepath, Image* image);
void AquireImage(Image* image);

#endif // PICT_H
