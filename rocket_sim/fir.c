
#include <stdio.h>

#define TAPS 16
#define TSTEP1 32
#define TSTEP2 48

// short coef[TAPS*2] = {
// #include "coef.inc"
// };

// short input[TSTEP1+TSTEP2] = {
// #include "input.inc"
// };


#include "expected.inc"

// CLOBBER is a compiler barrier
// Some resources on compiler barriers:  
//   https://youtu.be/nXaxk27zwlk  (original source, as far as I can tell)
//   https://stackoverflow.com/questions/37786547/enforcing-statement-order-in-c
//   https://preshing.com/20120625/memory-ordering-at-compile-time/
static void clobber() {
  asm volatile ("" : : : "memory");
}



int main( int argc, char* argv[] )
{

  int n,m,i;
  volatile short *coef=(short *)0x60004000;
  volatile short *input=(short *)0x60002000;
  // Uncomment the next line to avoid memory controller accesses
  // short output[TSTEP1+TSTEP2];
  volatile short *output=(short *)0x60001000;
  short error,total_error;

  volatile long long *dma_st     = (volatile long long*)  0x70000000;
  volatile long long **dma_sr    = (volatile long long**) 0x70000010;
  volatile long long **dma_dr    = (volatile long long**) 0x70000018;
  volatile long long *dma_len    = (volatile long long*)  0x70000020;
  volatile long long *accel_st   = (volatile long long*)  0x70010000;
  volatile long long *accel_ctrl = (volatile long long*)  0x70010008;
  volatile long long *accel_w    = (volatile long long*)  0x70010010;
  volatile long long *accel_x    = (volatile long long*)  0x70010030;
  volatile long long *accel_z    = (volatile long long*)  0x70010050;

  *accel_ctrl=0x01;

  *dma_sr=(volatile long long*)((long)coef & 0x1fffffff);; // memctl coeff address
  *dma_dr=(volatile long long*)((long)accel_w & 0x1fffffff); // fir coeff address
  *dma_len=TAPS*2; // starts coef transfer
  clobber();

  while(*accel_st != 0x01);

  *accel_ctrl=0x02;

  *dma_sr=(volatile long long*)((long)input & 0x1fffffff);
  *dma_dr=(volatile long long*)((long)accel_x & 0x1fffffff);
  *dma_len=TSTEP1*2; // starts X value transfer
  clobber();

  while(*accel_st != 0x02);
  
  *accel_ctrl=0x04;
  
  *dma_sr=(volatile long long*)((long)accel_z & 0x1fffffff);
  *dma_dr=(volatile long long*)((long)output & 0x1fffffff);
  *dma_len=TSTEP1*2; // starts transfer 
  clobber();
  
  while(*accel_st != 0x04);

  *accel_ctrl=0x01;

  *dma_sr=(volatile long long*)((((long)coef&0xffff0000)|0x00004020) & 0x1fffffff);; // memctl coeff address
  *dma_dr=(volatile long long*)((long)accel_w & 0x1fffffff); // fir coeff address
  *dma_len=TAPS*2; // starts coef transfer
  clobber();

  while(*accel_st != 0x01);

  *accel_ctrl=0x03;

  *dma_sr=(volatile long long*)((((long)input&0xffff0000)|0x00002040) & 0x1fffffff);
  *dma_dr=(volatile long long*)((long)accel_x & 0x1fffffff);
  *dma_len=TSTEP2*2; // starts X value transfer
  clobber();

  while(*accel_st != 0x03);
  
  *accel_ctrl=0x05;

  *dma_sr=(volatile long long*)((long)accel_z & 0x1fffffff);
  *dma_dr=(volatile long long*)((((long)output&0xffff0000)|0x00001040) & 0x1fffffff);
  *dma_len=TSTEP2*2; // starts transfer 
  clobber();
  
while(*accel_st != 0x05);

  // for (n=0; n<(TSTEP1+TSTEP2); n++) {
  //   error=expected[n]-output[n];              // Error for this time-step
  //   printf("value of each error: %d is %d\n",n,error);
  //   // total_error+=(error<0)?(-error):(error);  // Absolute value
  //   if(error<0)
  //   error = -error;
  //   total_error += error;
  //   //  printf("cpu main k: %d output: %d expected %d\n",n,output[n],expected[n]);
  // }
  // printf("cpu main FIR total error: %d\n",total_error);
  *accel_ctrl=(volatile long long)0x0f;       // Exit
  return 0;
}
