SECTION .data
  global location                   ; this was "prev_loc"
  global value                      ; this was "prev_val"
  global fake_eip                   ; this is normally passed as the third argument to sig_handler()
  global offset                     ; this is added to memory r/w dereferences to simulate the real binary

location:
  DD 0x00000000

value:
  DD 0x00000000

fake_eip:
  DD 0x00000000

offset:
  DD 0x00000000




SECTION .text
  global sighandler


; extern void sighandler();         ; how the C driver program sees this routine
sighandler:

  push   ebp
  mov    ebp, esp
  
  mov    edi, DWORD [offset]        ; <== SET CODE OFFSET
  
  mov    eax, DWORD [location]
  test   eax, eax
  je     check_eip

block1:  
  mov    eax, DWORD [location]
  mov    edx, DWORD [location]
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl
  
  mov    eax, DWORD [location]
  add    eax, 0x1
  mov    edx, DWORD [location]
  add    edx, 0x1
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl
  
  mov    eax, DWORD [location]
  add    eax, 0x2
  mov    edx, DWORD [location]
  add    edx, 0x2
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl
  
  mov    eax, DWORD [location]
  add    eax, 0x3
  mov    edx, DWORD [location]
  add    edx, 0x3
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl
  
  mov    DWORD [location], 0x0

check_eip:
  mov    eax, DWORD [fake_eip]
  and    eax, 0xff000000
  cmp    eax, 0x8000000
  jne    the_end

block2:
  mov    eax, DWORD [fake_eip]
  mov    DWORD [location], eax
  
  mov    eax, DWORD [location]
  mov    edx, eax
  mov    eax, DWORD [location]
  shr    eax, 0x8
  xor    edx, eax
  mov    eax, DWORD [location]
  shr    eax, 0x10
  xor    edx, eax
  mov    eax, DWORD [location]
  shr    eax, 0x18
  xor    eax, edx
  mov    BYTE [value], al
  
  mov    eax, DWORD [location]
  mov    edx, DWORD [location]
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl
  
  mov    eax, DWORD [location]
  add    eax, 0x1
  mov    edx, DWORD [location]
  add    edx, 0x1
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl
  
  mov    eax, DWORD [location]
  add    eax, 0x2
  mov    edx, DWORD [location]
  add    edx, 0x2
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl
  
  mov    eax, DWORD [location]
  add    eax, 0x3
  mov    edx, DWORD [location]
  add    edx, 0x3
  add    edx, edi                   ; <== ADJUST CODE OFFSET
  movzx  ecx, BYTE [edx]
  movzx  edx, BYTE [value]
  xor    edx, ecx
  add    eax, edi                   ; <== ADJUST CODE OFFSET
  mov    BYTE [eax], dl

the_end:
  pop    ebp
  ret
