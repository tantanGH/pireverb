#ifndef __H_PIREVERB__
#define __H_PIREVERB__

#define PROGRAM_NAME     "PIREVERB.X"
#define PROGRAM_VERSION  "0.1.0 (2023/05/21)"

#define PIREVERB_EYE_CATCH "Pi@#RvBB"
#define PIREVERB_EYE_CATCH_LEN (8)

#define TIMERD_INTERVAL_MSEC  (10)
#define TIMERD_INTERVAL_COUNT (50)

#define NUM_REVERB_TYPES (8)

#define REVERB_TYPE_NO_REVERB     (0)
#define REVERB_TYPE_ROOM          (1)
#define REVERB_TYPE_STUDIO_SMALL  (2)
#define REVERB_TYPE_STUDIO_MEDIUM (3)
#define REVERB_TYPE_STUDIO_LARGE  (4)
#define REVERB_TYPE_HALL          (5)
#define REVERB_TYPE_SPACE_ECHO    (6)
#define REVERB_TYPE_HALF_ECHO     (7)

//                                 012345678901234
#define REVERB_STR_NO_REVERB      "0.NO REVERB    "
#define REVERB_STR_ROOM           "1.ROOM         "
#define REVERB_STR_STUDIO_SMALL   "2.STUDIO SMALL "
#define REVERB_STR_STUDIO_MEDIUM  "3.STUDIO MEDIUM"
#define REVERB_STR_STUDIO_LARGE   "4.STUDIO LARGE "
#define REVERB_STR_HALL           "5.HALL         "
#define REVERB_STR_SPACE_ECHO     "6.SPACE ECHO   "
#define REVERB_STR_HALF_ECHO      "7.HALF ECHO    "

#endif