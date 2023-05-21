
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "ras68k_pilib.h"
#include "pireverb.h"

//
//  eye-catch
//
static uint8_t pireverb_eye_catch[] = PIREVERB_EYE_CATCH;

//
//  ras68k-ext reverb type
//
//   0: no reverb
//   1: room
//   2: studio small
//   3: studio medium
//   4: studio large
//   5: hall
//   6: space echo
//   7: half echo
//
static int16_t g_reverb_type = REVERB_TYPE_NO_REVERB;

//
//  interrupt counters
//
static volatile uint32_t g_interrupt_counter = 0;
static volatile int16_t g_change_enabled = 1;
static volatile int16_t g_quiet_mode = 0;

//
//  text vram preservation buffers
//
static volatile uint8_t g_text_buffer0[ 2 + 2 + 16 * 16 ];
static volatile uint8_t g_text_buffer1[ 2 + 2 + 16 * 16 ];

//
//  timer-D interrupt handler
//
static void __attribute__((interrupt)) __timer_interrupt_handler__(void) {

  g_interrupt_counter++;
  if (g_interrupt_counter >= TIMERD_INTERVAL_COUNT) {
    if (!g_change_enabled) {
      g_change_enabled = 1;
      if (!g_quiet_mode) {
        TCOLOR(1);
        TEXTPUT(0, 496, (struct FNTBUF*)g_text_buffer0);
        TCOLOR(2);
        TEXTPUT(0, 496, (struct FNTBUF*)g_text_buffer1);
      }
    }
    g_interrupt_counter = 0;
  }

  if (g_change_enabled) {
    uint8_t key1 = *((uint8_t*)0x080e);      // IOCS work OPT.2 key
    uint8_t key2 = *((uint8_t*)0x080a);      // IOCS work XF1/XF2 key
    if ((key1 & 0x08) && (key2 & 0x60)) {
      if (key2 & 0x20) {
        // OPT.2 + XF1 ... reverb type change
        g_reverb_type = (g_reverb_type + 1) % NUM_REVERB_TYPES;
        ras68k_pilib_set_reverb_type(g_reverb_type);
      } else {
        // OPT.2 + XF2 ... reverb type display only
      }
      if (!g_quiet_mode) {
        TCOLOR(1);
        TEXTGET(0, 496, (struct FNTBUF*)g_text_buffer0);
        TCOLOR(2);
        TEXTGET(0, 496, (struct FNTBUF*)g_text_buffer1);
        B_PUTMES(6, 0, 31, 15,
                  g_reverb_type == 0 ? REVERB_STR_NO_REVERB :
                  g_reverb_type == 1 ? REVERB_STR_ROOM :
                  g_reverb_type == 2 ? REVERB_STR_STUDIO_SMALL :
                  g_reverb_type == 3 ? REVERB_STR_STUDIO_MEDIUM :
                  g_reverb_type == 4 ? REVERB_STR_STUDIO_LARGE :
                  g_reverb_type == 5 ? REVERB_STR_HALL :
                  g_reverb_type == 6 ? REVERB_STR_SPACE_ECHO :
                  g_reverb_type == 7 ? REVERB_STR_HALF_ECHO : "");
      }
      g_change_enabled = 0;
      g_interrupt_counter = 0;
    }
  }

}

//
//  keep process checker
//
static uint8_t* check_keep_process(const uint8_t* exec_name) {

  uint8_t* psp = (uint8_t*)GETPDB() - 16;

  // find root process
  for (;;) {
    uint32_t parent = B_LPEEK((uint32_t*)(psp + 4));
    if (parent == 0) {
      break;
    }
    psp = (uint8_t*)parent;
  }

  // check memory blocks
  for (;;) {
    if (B_BPEEK((uint8_t*)(psp + 4)) == 0xff) {
      if (stricmp(psp + 196, exec_name) == 0) {   // assuming this keep process is in the user land
        return psp + 16;    // return PDB
      }
    }
    uint32_t child = B_LPEEK((uint32_t*)(psp + 12));
    if (child == 0) {
      break;
    }
    psp = (uint8_t*)child;
  }

  return NULL;
}

//
//  show help message
//
static void show_help_message() {
  printf("usage: pireverb [options]\n");
  printf("options:\n");
  printf("   -r    ... remove running pireverb\n");
  printf("   -t[n] ... reverb type (default:2.studio small)\n");
  printf("   -q    ... quiet mode\n");
  printf("   -h    ... show help message\n");
}

//
//  main
//
int32_t main(int32_t argc, uint8_t* argv[]) {

  // default exit code
  int32_t rc = -1;

  // option parameters
  int16_t remove_process = 0;
  int16_t initial_reverb_type = REVERB_TYPE_STUDIO_SMALL;

  // credit
  printf("PIREVERB - Reverb type controller for ras68k-ext version " PROGRAM_VERSION " by tantan\n");

  // parse command lines
  for (int16_t i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) >= 2) {
      if (argv[i][1] == 'r') {
        remove_process = 1;
      } else if (argv[i][1] == 't') {
        initial_reverb_type = atoi(argv[i]+2);
        if (initial_reverb_type < 0 || initial_reverb_type >= NUM_REVERB_TYPES || strlen(argv[i]) < 3) {
          show_help_message();
          goto exit;
        }
      } else if (argv[i][1] == 'q') {
        g_quiet_mode = 1;   // global variable
      } else if (argv[i][1] == 'h') {
        show_help_message();
        goto exit;
      } else {
        printf("error: unknown option (%s).\n",argv[i]);
        goto exit;
      }
    }
  }

  // self keep check
  uint8_t* pdp = check_keep_process(PROGRAM_NAME);

  // remove pireverb
  if (remove_process) {

    // self keep check
    if (pdp != NULL) {

      // release timer-D interrupt handle
      TIMERDST(0,0,0);

      // release program memory itself
      MFREE((uint32_t)pdp);

      printf("removed " PROGRAM_NAME " successfully.\n");
      rc = 0;

    } else {

      printf(PROGRAM_NAME " is not running.\n");
      rc = 1;

    }

    goto exit;

  }

  if (pdp != NULL) {
    printf("error: " PROGRAM_NAME " is already running.\n");
    rc = 1;
    goto exit;
  }

  // check pilib.x
  if (!ras68k_pilib_keepchk()) {
    printf("error: PILIB.X is not running.\n");
    goto exit;
  }

  // filter on and initial reverb type
  g_reverb_type = initial_reverb_type;
  ras68k_pilib_set_filter_mode(1);
  ras68k_pilib_set_reverb_type(g_reverb_type);

  // set timer-D interrupt handler
  g_interrupt_counter = 0;
  g_change_enabled = 1;
  ((uint32_t*)g_text_buffer0)[0] = (( 16 * 8 ) << 16 ) + 16;
  ((uint32_t*)g_text_buffer1)[0] = (( 16 * 8 ) << 16 ) + 16;
  if (TIMERDST((uint8_t*)(__timer_interrupt_handler__), 7, 200) != 0) {   // 50usec * 200 = 10msec
    printf("error: Timer-D interrupt is being used by other applications. (CONFIG.SYS PROCESS= ?)\n");
    goto exit;
  }

  printf(PROGRAM_NAME " is installed.\n");
  printf("[OPT.2]+[XF1]: change ras68k-ext reverb type\n");
  printf("[OPT.2]+[XF2]: show the current reverb type\n");

  rc = 0;

  // keep self process
  extern uint32_t _HEND;
//  extern unsigned int _PSP;
  KEEPPR(_HEND - _PSP - 0xf0, rc);

exit:
  return rc;
}