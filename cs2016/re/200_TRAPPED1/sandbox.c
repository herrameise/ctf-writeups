#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <capstone/capstone.h>



#define CODESIZE 728      // size of code blob to deobfuscate
#define MAX_INSNS 200      // maximum number of 




// global variables
// accessed by signum()
extern void sighandler(void);
extern uint32_t location;     // "prev_loc" in the binary
extern uint32_t value;        // "prev_val" in the binary
extern uint32_t fake_eip;     // simulated EIP state
extern uint32_t offset;       // EIP at start of code blob (0x080484e5) minus &code[0]
uint8_t code[CODESIZE];       // obfuscated code blob (carved from binary)
uint8_t dec[CODESIZE];        // deobfuscated code blob



// debugging/convenience functions
void PrintBytes(uint8_t* bytes, uint32_t len);
void nop(uint32_t offset, uint32_t len);



int main() {
  
  // initialize Capstone
  csh handle;                             // handle to Capstone disassembler
  cs_insn *insn;                          // pointer to instructions disassembled by cs_disasm()
  size_t count;                           // number of instructions disassembled by cs_disasm()
  uint32_t size;                          // instruction size
  char mnemonic[16];                      // instruction
  if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK) {
    fprintf(stderr, "[!] cs_open() failed\n");
    return 1;
  }
  
  // set up code sandbox
  uint32_t eipstart = 0x080484E5;         // real address of first byte in code[] sandbox
  offset = (uint32_t)code - 0x080484E5;   // offset between native binary and code[] sandbox
  uint32_t index = 0x2;                   // index into code sandbox
  fake_eip = eipstart + index;            // current simulated EIP state
  uint8_t* bytes = &code[index];          // pointer to code sandbox

  FILE* fp = fopen("code.dat","r");
  fread(code, 1, CODESIZE, fp);
  fclose(fp);
  memcpy(dec, code, CODESIZE);
  
  // disassemble
  printf("================================================================================\n");
  printf("ADDRESS     INDEX  SIZE  INSN\tOPERANDS\n");
  printf("================================================================================\n");
  int ctr;
  for (ctr = 0; ctr < MAX_INSNS; ctr++) {
  
    sighandler();
  
    count = cs_disasm(handle, bytes, 16, fake_eip, 0, &insn);
  
    if (count > 0) {
      size = insn[0].size;
      strcpy(mnemonic, insn[0].mnemonic);
      index = fake_eip - eipstart;
      printf("0x%08x  [%03x]  [%02x]  %s\t%s\n", (uint32_t)insn[0].address, index, size, mnemonic, insn[0].op_str);
      memcpy(&dec[index], &code[index], size);
      cs_free(insn, count);
    } else {
      fprintf(stderr, "[!] disassembly failed\n");
      break;
    }
    
    if (fake_eip == 0x080484ea) { // initial jump after SIGTRAPs start
      fake_eip = 0x8048511;
      index = fake_eip - eipstart;
      bytes = &code[index];
      printf("[ ... ]\n");
    }
    else if (fake_eip == 0x080486FF) {
      break;
    }
    else {
      fake_eip += size;
      index += size;
      bytes = &code[index];
    }
  }

  cs_close(&handle);
  
  fp = fopen("trapped1","r");
  uint8_t* binary = (uint8_t*)malloc(6872);
  fread(binary, 1, 6872, fp);
  fclose(fp);
  
  fp = fopen("patched", "w");
  memcpy(&binary[1253], dec, 728);
  fwrite(binary, 1, 6872, fp);
  fclose(fp);
  
  return 0;
}



void nop(uint32_t offset, uint32_t len) {
  
  uint32_t ctr;
  for (ctr = 0; ctr < len; ctr++) {
    dec[offset + ctr] = '\x90';
  }
}



void PrintBytes(uint8_t* bytes, uint32_t len) {

  uint32_t ctr;
  for (ctr = 0; ctr < len; ctr++) {
    if ((ctr % 8) == 0) printf("0x%08x <main+%u>:\t", 0x080484E5 + ctr, 99 + ctr);
    printf("0x%02x    ", bytes[ctr]);
    if (((ctr+1) % 8) == 0) printf("\n");
  }
  printf("\n\n");
}
