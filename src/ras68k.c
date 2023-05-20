#include <stdint.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "ras68k.h"

//
//  pilib.x keep check
//
int32_t ras68k_pilib_keepchk() {
  
  uint32_t eye_catch_addr = INTVCG(0x25) - 4;

  uint8_t eye_catch[4];
  for (int16_t i = 0; i < 4; i++) {
    eye_catch[i] = B_BPEEK((uint8_t*)(eye_catch_addr + i));
  }

  return (memcmp(eye_catch, "PEXT", 4) == 0) ? 1 : 0;
}

//
//  init in OPM mode ($0000)
//
int32_t ras68k_pilib_init_opm() {

	register uint32_t reg_d0 asm ("d0") = 0x0000;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    :                   // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//  init in PSG mode ($0001)
//
int32_t ras68k_pilib_init_psg() {

	register uint32_t reg_d0 asm ("d0") = 0x0001;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    :                   // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//  init in MIDI mode ($0010)
//
int32_t ras68k_pilib_init_midi() {

	register uint32_t reg_d0 asm ("d0") = 0x0010;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    :                   // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//  set ADPCM/PCM filter ($0002)
//
int32_t ras68k_pilib_set_pcm_filter(int16_t filter_enabled) {

	register uint32_t reg_d0 asm ("d0") = 0x0002;
	register uint32_t reg_d1 asm ("d1") = filter_enabled;     // 0:off 1:on

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//  set reverb type ($0003)
//
int32_t ras68k_pilib_set_reverb_type(int16_t reverb_type) {

	register uint32_t reg_d0 asm ("d0") = 0x0003;
	register uint32_t reg_d1 asm ("d1") = reverb_type;     // 0 to 7

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//  upload pcm data ($0004 -> $0012)
//
int32_t ras68k_pilib_upload_pcm_data(void* buf, size_t buf_len) {

	register uint32_t reg_d0 asm ("d0") = 0x0004;
	register uint32_t reg_d1 asm ("d1") = buf_len;
  register uint32_t reg_a0 asm ("a0") = (uint32_t)buf;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_a0)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   send register and data ($0005)
//
int32_t ras68k_pilib_send_register_data(uint8_t reg, uint8_t data) {

	register uint32_t reg_d0 asm ("d0") = 0x0005;
	register uint32_t reg_d1 asm ("d1") = reg;
  register uint32_t reg_d2 asm ("d2") = data;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d2)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   send MIDI data in OPM mode ($0006)
//
int32_t ras68k_pilib_send_midi_data(uint8_t data) {

	register uint32_t reg_d0 asm ("d0") = 0x0006;
	register uint32_t reg_d1 asm ("d1") = data;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   stop PCM all channels ($0007)
//
int32_t ras68k_pilib_stop_pcm_all() {

	register uint32_t reg_d0 asm ("d0") = 0x0007;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    :                   // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   stop PCM 1st channel ($0008)
//
int32_t ras68k_pilib_stop_pcm() {

	register uint32_t reg_d0 asm ("d0") = 0x0008;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    :                   // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   play ADPCM ($0009)
//
int32_t ras68k_pilib_play_adpcm(uint32_t mode, void* buf, size_t buf_len) {

	register uint32_t reg_d0 asm ("d0") = 0x0009;
	register uint32_t reg_d1 asm ("d1") = mode;
	register uint32_t reg_d2 asm ("d2") = buf_len;
	register uint32_t reg_a1 asm ("a1") = (uint32_t)buf;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d2),   // input operand
      "r"   (reg_a1)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   play PCM ($000a)
//
int32_t ras68k_pilib_play_pcm(uint16_t channel, uint32_t mode, void* buf, size_t buf_len) {

	register uint32_t reg_d0 asm ("d0") = 0x000a + (channel << 16);
	register uint32_t reg_d1 asm ("d1") = mode;
	register uint32_t reg_d2 asm ("d2") = buf_len;
	register uint32_t reg_a1 asm ("a1") = (uint32_t)buf;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d2),   // input operand
      "r"   (reg_a1)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   play PCM at variable frequency ($000e)
//
int32_t ras68k_pilib_play_pcm_freq(uint16_t channel, uint32_t mode, uint32_t freq, void* buf, size_t buf_len) {

	register uint32_t reg_d0 asm ("d0") = 0x000e + (channel << 16);
	register uint32_t reg_d1 asm ("d1") = mode;
	register uint32_t reg_d2 asm ("d2") = buf_len;
  register uint32_t reg_d3 asm ("d3") = freq;
	register uint32_t reg_a1 asm ("a1") = (uint32_t)buf;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d2),   // input operand
      "r"   (reg_d3),   // input operand
      "r"   (reg_a1)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   set ADPCM mode ($0012)
//
int32_t ras68k_pilib_set_adpcm_mode(uint16_t channel, uint32_t mode) {

	register uint32_t reg_d0 asm ("d0") = 0x000a + (channel << 16);
	register uint32_t reg_d1 asm ("d1") = mode;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}

//
//   set PCM mode and freq ($0013)
//
int32_t ras68k_pilib_set_pcm_mode_freq(uint16_t channel, uint32_t mode, uint32_t freq) {

	register uint32_t reg_d0 asm ("d0") = 0x000a + (channel << 16);
	register uint32_t reg_d1 asm ("d1") = mode;
	register uint32_t reg_d3 asm ("d3") = freq;

  asm volatile (
    "trap #5\n"         // trap #5
    : "+r"  (reg_d0)    // output (&input) operand
    : "r"   (reg_d1),   // input operand
      "r"   (reg_d3)    // input operand
    :                   // clobbered register
  );

  return reg_d0;
}
