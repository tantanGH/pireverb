#ifndef __H_S44BGP__

#include "kmd.h"

#define PROGRAM_NAME     "S44BGP.X"
#define PROGRAM_VERSION  "0.4.0 (2023/03/23)"

#define EYE_CATCH "Bgp#44pM"
#define EYE_CATCH_LEN (8)

#define MAX_MUSIC (32)
#define MAX_PATH_LEN (256)
#define MAX_DISP_LEN (66)

#define FREAD_BUFFER_LEN (44100 * 4)
#define YM2608_DECODE_BUFFER_BYTES (44100 * 4 * 2)

#define PCM8PP_CHANNEL (1)

#define TIMERD_INTERVAL_MSEC  (10)
#define TIMERD_INTERVAL_COUNT (16)

#define OPM_INTERVAL_MSEC  (64)
#define OPM_INTERVAL_COUNT (10)

#define SJIS_ONPU "\x81\xf4"

typedef struct {
  uint8_t eye_catch[ EYE_CATCH_LEN ];
  int16_t* buffer;
  uint32_t buffer_bytes;
  int16_t volume;
  uint32_t total_time_msec;
  uint8_t file_name[ 256 ];
  KMD_HANDLE kmd;
} PCM_MUSIC;

#endif