- [Basic info](#basic-info)
- [About plugins.dat](#about-pluginsdat)
  - [Windows](#windows)
  - [Linux](#linux)
  - [Mac](#mac)
- [Plugins overview](#plugins-overview)
  - [Access plugins](#access-plugins)
  - [Audio filter plugins](#audio-filter-plugins)
  - [Audio mixer plugins](#audio-mixer-plugins)
  - [Audio output](#audio-output)
  - [Codec plugins](#codec-plugins)
  - [Control plugins](#control-plugins)
  - [D3D11 plugins](#d3d11-plugins)
  - [D3D9 plugins](#d3d9-plugins)
  - [Demux plugins](#demux-plugins)
  - [GUI plugins](#gui-plugins)
  - [Keystore plugins](#keystore-plugins)
  - [Logger plugins](#logger-plugins)
  - [Meta engine plugins](#meta-engine-plugins)
  - [Misc plugins](#misc-plugins)
  - [Packetizer plugins](#packetizer-plugins)
  - [Services discovery plugins](#services-discovery-plugins)
  - [SPU plugins](#spu-plugins)
  - [Stream extractor plugins](#stream-extractor-plugins)
  - [Stream filter plugins](#stream-filter-plugins)
  - [Text renderer plugins](#text-renderer-plugins)
  - [VA-API plugins](#va-api-plugins)
  - [VDPAU plugins](#vdpau-plugins)
  - [Video chroma plugins](#video-chroma-plugins)
  - [Video filter plugins](#video-filter-plugins)
  - [Video output plugins](#video-output-plugins)
  - [Video splitter plugins](#video-splitter-plugins)
  - [Visualization plugins](#visualization-plugins)


## Basic info

**libVLC** is a powerfull cross-platform audio and video library that provides a comprehensive multimedia API. 

All of its shared libraries are stored in build/\<platform_prefix\>/lib. The most important of them:
  - `libvlc` - the main library
  - `libvlccore` - the core part of libVLC

There are also many of **libVLC plugins** which enhance the functionality of the libVLC library by providing codec support, input and output modules, hardware acceleration and many more. All plugins descriptions are presented in [this section](#plugins-overview).

More information about the library can be found in [libVLC documentation](https://wiki.videolan.org/LibVLC/) and [official Gitlab](https://code.videolan.org/videolan/vlc).

## About plugins.dat

Many of plugins are loaded dynamically and to help an application to load these plugins faster during runtime, the `plugins.dat` file is generated..

The `plugins.dat` file can be regenerated if needed by running `vlc-cache-gen` executable from platform-specific build directory as described below.

### Windows

 1. Just run: `/lib/vlc-cache-gen.exe /lib/plugins`

### Linux

 1. Set `LD_LIBRARY_PATH` environment variable: `LD_LIBRARY_PATH=$(pwd)/lib`.
 2. Run: `lib/vlc/vlc-cache-gen lib/vlc/plugins`

### Mac
 
 1. Set `DYLD_LIBRARY_PATH` environment variable: `DYLD_LIBRARY_PATH=$(pwd)/lib`.
 2. Run: `lib/vlc/vlc-cache-gen lib/vlc/plugins`

## Plugins overview

Each plugin is presented as a shared library with following name: `lib<plugin_name>_plugin.(so|dll|dylib)`.

Plugins are divided into several groups according to their purpose and these groups are presented as subdirectories in plugins directory. Each of these subdirectories content are described further.

### Access plugins

This directory contains plugins related to various methods of accessing multimedia content, such as file access, network streaming, or capturing from devices.

| Plugin name | Description |
| --- | --- |
| access_alsa | ALSA audio capture |
| access_concat | Concatenated inputs |
| access_imem | In-memory bit stream input |
| access_jack | JACK audio input |
| access_mms | Microsoft Media Server (MMS) input |
| access_wasapi | Windows Audio Session API input |
| attachment | Input reading an attachment |
| avio | libavformat AVIO access input |
| dshow | DirectShow input |
| dtv | Digital Television and Radio |
| filesystem | File input |
| ftp | FTP input |
| https | HTTPS input module |
| http | HTTP input module |
| idummy | Dummy input plugin, to manage "vlc://" special options |
| imem | Memory input for VLC |
| pulsesrc | PulseAudio input plugin |
| rist | RIST (Reliable Internet Stream Transport) input module |
| rtp | Real-Time Protocol (RTP) packed input |
| satip | SAT>IP Receiver Plugin |
| screen | Screen capture module |
| sdp | Fake input for SDP (Session Description Protocol) scheme |
| shm | Shared memory frame buffer |
| smb | SMB input |
| tcp | TCP input module |
| timecode | Time code subpicture elementary stream generator |
| udp | UDP input module |
| vdr | VDR recordings access plugin |
| xcb_screen | Screen capture (with X11/XCB) |

### Audio filter plugins

Plugins in this directory are responsible for applying various audio filters and effects to audio streams, such as equalization or audio format conversion.

| Plugin name | Description |
| --- | --- |
| audiobargraph_a | Audio part of the BarGraph function |
| audio_format | Audio filter for PCM format conversion |
| chorus_flanger | Basic chorus/flanger/delay audio filter |
| compressor | Dynamic range compressor |
| dolby_surround_decoder | Simple decoder for dolby surround encoded streams |
| equalizer | Equalizer with 10 bands |
| gain | Gain control filter |
| headphone_channel_mixer | Headphone virtual spatialization channel mixer module |
| karaoke | Simple Karaoke filter |
| mad | MPEG audio layer I/II/III decoder |
| mono | Stereo to mono downmixer |
| normvol | Volume normalizer |
| param_eq | Parametric Equalizer |
| remap | Audio channel remapper |
| samplerate | Secret Rabbit Code (a.k.a. libsamplerate) resampler |
| scaletempo_pitch | Pitch Shifter |
| scaletempo | Audio tempo scaler synched with rate |
| simple_channel_mixer | Audio filter for simple channel mixing |
| spatialaudio | Ambisonics renderer and binauralizer |
| spatializer | Audio Spatializer (sound reverberation) |
| speex_resampler | libspeex DSP resampler |
| stereo_widen | Simple stereo widening effect |
| tospdif | Audio filter for A/52/DTS->S/PDIF encapsulation |
| trivial_channel_mixer | Trivial channel mixer plug-in (drops unwanted channels) |
| ugly_resampler | Zero-order hold "ugly" resampler |

### Audio mixer plugins

These plugins deal with mixing and processing audio streams, allowing for tasks like adjusting audio levels or merging multiple audio sources.

| Plugin name | Description |
| --- | --- |
| float_mixer | Single precision audio volume |
| integer_mixer | Integer audio volume |

### Audio output

This directory contains audio output plugins responsible for delivering audio streams to different output devices or audio systems, like speakers or headphones.

| Plugin name | Description |
| --- | --- |
| adummy | Dummy audio output |
| afile | File audio output |
| alsa | ALSA audio output |
| amem | Audio memory output |
| auhal | HAL AudioUnit output (Mac OS X) |
| directsound | DirectX audio output |
| jack | JACK audio output |
| mmdevice | Windows Multimedia Device output |
| pulse | Pulseaudio audio output |
| wasapi | Windows Audio Session API output |
| waveout | WaveOut audio output |

### Codec plugins

Codec plugins handle the encoding and decoding of audio and video data, making it possible to play and manipulate multimedia content in different formats.

| Plugin name | Description |
| --- | --- |
| a52 | ATSC A/52 aka AC-3 audio decoder plugin |
| adpcm | ADPCM audio decoder |
| aes3 | AES3/SMPTE 302M audio decoder |
| aom | AOM video decoder (AV1) |
| araw | Raw/Log Audio decoder |
| aribsub | ARIB subtitles decoder |
| audiotoolboxmidi | AudioToolbox MIDI synthesizer (Mac OS X) |
| avcodec | Video and audio decoder and encoder using libavcodec (FFmpeg) |
| cc | Closed Captions decoder |
| cdg | CDG video decoder |
| crystalhd | Crystal HD hardware video decoder |
| cvdsub | CVD subtitle decoder |
| d3d11va | Direct3D11 Video Acceleration |
| dav1d | Dav1d video decoder (AV1) |
| dca | DTS Coherent Acoustics audio decoder |
| ddummy | Dummy decoder |
| dmo | DirectMedia Object decoder |
| dvbsub | DVB subtitles decoder |
| dxva2 | DirectX Video Acceleration (DXVA) 2.0 |
| faad | AAC audio decoder using libfaad2 |
| flac | Flac audio decoder |
| g711 | G.711 decoder |
| kate | Kate overlay decoder |
| libass | SSA/ASS subtitle decoder using libass |
| libmpeg2 | MPEG I/II video decoder using libmpeg2 |
| lpcm | Linear PCM audio decoder |
| mft | Media Foundation Transform decoder |
| mpg123 | MPEG-1 & 2 audio layer I, II, III + MPEG 2.5 decoder |
| oggspots | OggSpots video decoder |
| opus | Opus audio decoder |
| rawvideo | Pseudo raw video decoder |
| schroedinger | Dirac video decoder using libschroedinger |
| scte18 | SCTE-18 EAS decoder |
| scte27 | SCTE-27 subtitles decoder |
| spdif | S/PDIF pass-through decoder |
| speex | Speex audio decoder |
| spudec | DVD SPU decoder |
| stl | EBU STL subtitles decoder |
| subsdec | Text subtitle decoder |
| substx3g | MP4 tx3g subtitles decoder |
| subsusf | USF subtitles decoder |
| svcdsub | Philips OGT (SVCD subtitle) decoder |
| textst | HDMV TextST subtitles decoder |
| theora | Theora video decoder |
| ttml | TTML subtitles decoder |
| uleaddvaudio | Ulead DV audio decoder |
| vaapi_drm | VA-API video decoder via DRM |
| vaapi | VA-API helpers for the libavcodec decoder |
| videotoolbox | VideoToolbox video decoder (Mac OS X) |
| vorbis | Vorbis audio decoder |
| vpx | WebM video decoder (VP8/VP9) |
| webvtt | WEBVTT subtitles decoder |
| xwd | XWD image decoder |

### Control plugins

Control plugins provide interfaces and mechanisms for local and remote control of the media player, including support for various protocols like HTTP or D-Bus.

| Plugin name | Description |
| --- | --- |
| dummy | Dummy interface |
| gestures | Mouse gestures control interface |
| hotkeys | Hotkeys management interface |
| motion | Laptop built-in motion sensors control interface |
| ntservice | Windows Service interface |
| netsync | Network synchronization between several network clients |
| oldrc | Remote control interface |
| win_hotkeys | Global Hotkeys interface for Windows |
| win_msg | Windows messages interface |
| xcb_hotkeys | Global Hotkeys interface |

### D3D11 plugins

Plugins for video filters using DirectX 11 (Direct3D 11) on Windows.

| Plugin name | Description |
| --- | --- |
| direct3d11_filters | Direct3D11 video adjust and deinterlace filters |

### D3D9 plugins

Plugins for video filters using DirectX 9 (Direct3D 9) on Windows.

| Plugin name | Description |
| --- | --- |
| direct3d9_filters | Direct3D11 adjust and deinterlace filters |

### Demux plugins

Demux plugins are responsible for demultiplexing multimedia containers, separating audio, video, and subtitle streams from a single file or stream source.

| Plugin name | Description |
| --- | --- |
| adaptive | Unified adaptive streaming for DASH/HLS |
| aiff | AIFF (Audio Interchange File Format demuxer) demuxer |
| asf | ASF/WMV demuxer |
| au | AU demuxer |
| avformat | Avformat demuxer |
| avi | AVI demuxer |
| caf | CAF demuxer |
| demuxdump | File dumper |
| demux_cdg | CDG demuxer |
| demux_stl | EBU STL subtitles parser |
| diracsys | Dirac video demuxer |
| directory_demux | Directory import module (import directory content to playlist) |
| es | MPEG-I/II/4 / A52 / DTS / MLP audio |
| flacsys | FLAC demuxer |
| h26x | Raw H264 and HEVC Video demuxer |
| image | Image demuxer |
| mjpeg | M-JPEG camera demuxer |
| mkv | Matroska stream demuxer |
| mp4 | MP4 stream demuxer |
| mpgv | MPEG-I/II video demuxer |
| noseek | Seek prevention demux filter |
| nsc | Windows Media NSC metademux |
| nsv | NullSoft Video demuxer |
| nuv | NuppelVideo (NUV) demuxer |
| ogg | OGG demuxer |
| playlist | Playlist import module |
| ps | MPEG-PS (Program Stream) demuxer |
| pva | PVA demuxer |
| rawaud | Raw audio demuxer |
| rawdv | DV (Digital Video) demuxer |
| rawvid | Raw video demuxer |
| real | Real Media format demuxer |
| smf | SMF demuxer |
| subtitle | Demux for subtitle text files |
| ts | MPEG Transport Stream demuxer |
| tta | TTA demuxer |
| ty | TY Stream audio/video demux |
| vc1 | VC1 video demuxer |
| vobsub | Vobsub subtitles demuxer |
| voc | VOC demuxer |
| wav | WAV demuxer |
| xa | XA demuxer |

### GUI plugins

GUI plugins offer various graphical interfaces and controls for interacting with the media player. Since we don't actually need them, this directory should not exist.

### Keystore plugins

Keystore plugins provide secure storage and retrieval of cryptographic keys, often used for DRM (Digital Rights Management) purposes.

keystore
| Plugin name | Description |
| --- | --- |
| file_keystore | File and crypt keystore (secrets can be stored encrypted in a file, or in a file without any encryption) |
| keychain | Keystore for iOS, Mac OS X and tvOS |
| memory_keystore | Memory keystore (secrets are stored in memory) |

### Logger plugins

These plugins are used for logging and debugging purposes, helping track and diagnose issues within the media player.

| Plugin name | Description |
| --- | --- |
| console_logger | Console logger |
| file_logger | File logger |
| syslog | System logger (syslog) |

### Meta engine plugins

Meta engine plugins handle metadata retrieval and management, allowing for the extraction and display of information about media files.

| Plugin name | Description |
| --- | --- |
| folder | Folder meta data |
| taglib | Taglib tag parser/writer |

### Misc plugins

The "misc" directory may contain miscellaneous plugins that don't fit neatly into other categories, often providing additional functionality or utility.

| Plugin name | Description |
| --- | --- |
| audioscrobbler | Submission of played songs to last.fm |
| export | Playlist export module |
| fingerprinter | Audio fingerprinter module (based on Acoustid) |
| logger | File logging plugin |
| securetransport | TLS support for Mac OS X and iOS |
| stats | Stats decoder/demuxer |
| xdg_screensaver | XDG screen saver inhibition |
| xml | XML Parser (using libxml2) |

### Packetizer plugins

Packetizer plugins are responsible for breaking down and assembling multimedia data into packets suitable for transmission or processing.

| Plugin name | Description |
| --- | --- |
| packetizer_a52 | A/52 audio packetizer |
| packetizer_av1 | AV1 video packetizer |
| packetizer_avparser | Avparser packetizer |
| packetizer_copy | Copy packetizer |
| packetizer_dirac | Dirac packetizer |
| packetizer_dts | DTS audio packetizer |
| packetizer_flac | Flac audio packetizer |
| packetizer_h264 | H.264 video packetizer |
| packetizer_hevc | HEVC/H.265 video packetizer |
| packetizer_mlp | MLP/TrueHD audio packetizer |
| packetizer_mpeg4audio | MPEG4 audio packetizer |
| packetizer_mpeg4video | MPEG4 video packetizer |
| packetizer_mpegaudio | MPEG audio layer I/II/III packetizer |
| packetizer_mpegvideo | MPEG-I/II video packetizer |
| packetizer_vc1 | VC-1 packetizer |

### Services discovery plugins

Services discovery plugins help locate and identify network services, making it easier to find and access media content from various sources.

| Plugin name | Description |
| --- | --- |
| bonjour | mDNS services discovery module based on Bonjour (Mac OS X) |
| mediadirs | Picture/Music/Video user directories as service discoveries (My Pictures, My Music, My Video) |
| podcast | Podcasts |
| pulselist | Audio capture (PulseAudio) |
| sap | SAP interface module |
| windrive | List of disc drives |
| xcb_apps | List of application windows XCB module |

### SPU plugins

SPU (Subpicture Unit) plugins handle the rendering of subtitles and other overlay graphics on video content.

| Plugin name | Description |
| --- | --- |
| audiobargraph_v | Audio Bar Graph Video sub source |
| dynamicoverlay | Dynamic video overlay |
| logo | Logo sub source |
| marq | Marquee display |
| mosaic | Mosaic video sub source |
| rss | RSS and Atom feed display |
| subsdelay | Subtitle delay |

### Stream extractor plugins 

Stream extractor plugins can extract specific streams (e.g., audio or video) from multimedia containers without decoding the entire file. This directory shouldn't exist in our libVLC build.

### Stream filter plugins

These plugins can filter and process multimedia streams in real-time, performing tasks like noise reduction or stream modification.

| Plugin name | Description |
| --- | --- |
| adf | ADF stream filter |
| cache_block | Block stream cache |
| cache_read | Byte stream cache |
| decomp | LZMA, gzip, Burrows-Wheeler decompression module |
| hds | Http Dynamic Streaming (HDS) stream filter |
| inflate | Zlib decompression filter |
| prefetch | Stream prefetch filter |
| record | Internal stream record |
| skiptags | APE/ID3 tags-skipping filter |

### Text renderer plugins

Text renderer plugins are responsible for rendering text and subtitles on the screen during video playback.

| Plugin name | Description |
| --- | --- |
| freetype | Freetype2 font renderer |
| nsspeechsynthesizer | Speech synthesis for Mac OS X systems |
| sapi | Speech synthesis for Windows |
| tdummy | Dummy font renderer |

### VA-API plugins

VA-API (Video Acceleration API) plugins provide hardware acceleration support for video decoding and encoding on systems that support VA-API (only Linux in our case).

| Plugin name | Description |
| --- | --- |
| vaapi_filters | Video Accelerated API filters |

### VDPAU plugins

VDPAU (Video Decode and Presentation API for Unix) plugins offer hardware acceleration for video processing on systems that support VDPAU (only Linux in our case).

vdpau
| Plugin name | Description |
| --- | --- |
| vdpau_adjust | VDPAU adjust video filter |
| vdpau_avcodec | VDPAU video decoder |
| vdpau_chroma | VDPAU surface conversions |
| vdpau_deinterlace | VDPAU deinterlacing filter |
| vdpau_display | VDPAU output |
| vdpau_sharpen | VDPAU sharpen video filter |

### Video chroma plugins

Video chroma plugins handle color space conversion and manipulation of video frames, allowing for optimizations and effects.

| Plugin name | Description |
| --- | --- |
| chain | Video filtering using a chain of video filter modules (as a last resort solution) |
| cvpx | Conversions between CoreVideo buffers (Mac OS X) |
| grey_yuv | Conversions from GREY to I420,YUY2 |
| i420_10_p010 | YUV 10-bits planar to semiplanar 10-bits conversions |
| i420_nv12 | YUV planar to semiplanar conversions |
| i420_rgb_mmx | MMX I420,IYUV,YV12 to RV15,RV16,RV24,RV32 conversions |
| i420_rgb | I420,IYUV,YV12 to RGB2,RV15,RV16,RV24,RV32 conversions |
| i420_rgb_sse2 | SSE2 I420,IYUV,YV12 to RV15,RV16,RV24,RV32 conversions |
| i420_yuy2_mmx | MMX conversions from I420,IYUV,YV12 to YUY2,YUNV,YVYU,UYVY,UYNV,Y422,IUYV |
| i420_yuy2 | Conversions from I420,IYUV,YV12 to YUY2,YUNV,YVYU,UYVY,UYNV,Y422,IUYV,Y211 |
| i420_yuy2_sse2 | SSE2 conversions from I420,IYUV,YV12 to YUY2,YUNV,YVYU,UYVY,UYNV,Y422,IUYV |
| i422_i420 | Conversions from I422,J422 to I420,IYUV,J420,YV12,YUVA |
| i422_yuy2_mmx | MMX conversions from I422 to YUY2,YUNV,YVYU,UYVY,UYNV,Y422,IUYV |
| i422_yuy2 | Conversions from I422 to YUY2,YUNV,YVYU,UYVY,UYNV,Y422,IUYV,Y211 |
| i422_yuy2_sse2 | SSE2 conversions from I422 to YUY2,YUNV,YVYU,UYVY,UYNV,Y422,IUYV |
| rv32 | RV32 conversion filter |
| swscale | Video scaling filter |
| yuvp | YUVP converter |
| yuy2_i420 | Conversions from YUY2,YUNV,YVYU,UYVY,UYNV,Y422 to I420 |
| yuy2_i422 | Conversions from YUY2,YUNV,YVYU,UYVY,UYNV,Y422 to I422 |

### Video filter plugins

Video filter plugins apply various video effects and transformations to video streams, such as deinterlacing or color correction.

| Plugin name | Description |
| --- | --- |
| adjust | Contrast/Hue/Saturation/Brightness video filter |
| alphamask | Alpha mask video filter |
| anaglyph | Convert 3D picture to anaglyph image video filter |
| antiflicker | Antiflicker video filter |
| ball | Ball video filter |
| blendbench | Blending benchmark filter |
| blend | Video pictures blending |
| bluescreen | Bluescreen video filter |
| canvas | Canvas video filter |
| ci_filters | Mac OS X hardware video filters |
| colorthres | Color threshold filter |
| croppadd | Video cropping filter |
| deinterlace | Deinterlacing video filter |
| edgedetection | Edge detection video filter |
| erase | Erase video filter |
| extract | Extract RGB component video filter |
| fps | FPS conversion video filter |
| freeze | Freezing interactive video filter |
| gaussianblur | Gaussian blur video filter |
| gradfun | Gradfun video filter |
| gradient | Gradient video filter |
| grain | Grain video filter |
| hqdn3d | High Quality 3D Denoiser filter |
| invert | Invert video filter |
| magnify | Magnify/Zoom interactive video filter |
| mirror | Mirror video filter |
| motionblur | Motion blur filter |
| motiondetect | Motion detect video filter |
| oldmovie | Old movie effect video filter |
| posterize | Posterize video filter |
| psychedelic | Psychedelic video filter |
| puzzle | Puzzle interactive game video filter |
| ripple | Ripple video filter |
| rotate | Rotate video filter |
| scale | Video scaling filter |
| scene | Scene video filter |
| sepia | Sepia video filter |
| sharpen | Sharpen video filter |
| transform | Video transformation filter |
| vhs | VHS movie effect video filter |
| wave | Wave video filter |

### Video output plugins

These plugins are responsible for delivering video frames to the display or output device.

| Plugin name | Description |
| --- | --- |
| caopengllayer | Core Animation OpenGL Layer (Mac OS X) |
| direct3d11 | Direct3D11 video output |
| direct3d9 | Direct3D9 video output |
| directdraw | DirectX (DirectDraw) video output |
| drawable | Embedded window video |
| egl_x11 | EGL extension for OpenGL |
| fb | GNU/Linux framebuffer video output |
| flaschen | Flaschen-Taschen video output |
| glconv_cvpx | Apple OpenGL CVPX converter (Mac OS X) |
| glconv_vaapi_drm | VA-API OpenGL surface converter for DRM |
| glconv_vaapi_x11 | VA-API OpenGL surface converter for X11 |
| glconv_vdpau | VDPAU OpenGL surface converter |
| glx | GLX extension for OpenGL |
| gl | OpenGL video output |
| vdummy | Dummy video output |
| vmem | Video memory output |
| vout_macosx | Mac OS X OpenGL video output |
| wingdi | Windows GDI video output |
| winhibit | Windows screen saver inhibition |
| xcb_window | X11 embedded window video (XCB) |
| xcb_x11 | X11 video output (XCB) |
| xcb_xv | XVideo output (XCB) |
| yuv | YUV video output |

### Video splitter plugins

Video splitter plugins can split a single video stream into multiple output streams, useful for scenarios like dual display setups.

| Plugin name | Description |
| --- | --- |
| clone | Clone video filter |
| panoramix | Panoramix: wall with overlap video filter |
| wall | Wall video filter |

### Visualization plugins

Visualization plugins generate audio-driven visual effects, often used in media player applications for music visualization.

| Plugin name | Description |
| --- | --- |
| glspectrum | 3D OpenGL spectrum visualization |
| visual | Visualisation system |
