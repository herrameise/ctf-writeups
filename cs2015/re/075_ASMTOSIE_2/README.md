## ASMTOSIE 2 [75]
**Tags**: reversing x86

**Description**: Some customers have been complaining that we need to expand our decompiling capabilities. Are you up to the task? Connect to `shell.cyberstakesonline.com:59592`, you know the drill.

## Solution
After connecting to the server, we are given the following block of x86 instruction to decompile into C code:
```
.LFB0:    
          push      ebp       
          mov       ebp, esp  
          push      esi       
          push      ebx       
          mov       ecx, DWORD PTR [ebp+8]
          mov       eax, DWORD PTR [ebp+8]
          cmp       eax, -668203058
          setnb     al        
          movzx     ebx, al   
          mov       eax, DWORD PTR [ebp+12]
          or        eax, 2    
          mov       esi, eax  
          mov       eax, DWORD PTR [ebp+8]
          cdq       
          idiv      esi       
          mov       edx, eax  
          mov       eax, DWORD PTR [ebp+12]
          add       eax, edx  
          imul      eax, eax, -1039577924
          mov       edx, DWORD PTR [ebp+12]
          or        edx, 2    
          mov       esi, edx  
          mov       edx, 0    
          div       esi       
          add       eax, ebx  
          or        eax, 2    
          mov       ebx, eax  
          mov       eax, ecx  
          mov       edx, 0    
          div       ebx       
          test      eax, eax  
          je        .L2       
          mov       eax, DWORD PTR [ebp+8]
          cmp       eax, -1357278678
          setnb     al        
          movzx     edx, al   
          mov       eax, DWORD PTR [ebp+8]
          sub       edx, eax  
          mov       eax, DWORD PTR [ebp+8]
          add       eax, edx  
          jmp       .L3       
.L2:      
          mov       eax, DWORD PTR [ebp+8]
          mov       edx, DWORD PTR [ebp+8]
          cmp       edx, -837399105
          setnb     dl        
          movzx     ecx, dl   
          mov       edx, DWORD PTR [ebp+8]
          add       edx, ecx  
          mov       ecx, edx  
          or        ecx, 2    
          mov       edx, 0    
          div       ecx       
.L3:      
          pop       ebx       
          pop       esi       
          pop       ebp       
          ret       
```

The C function should have the prototype:
```C
int testb(int, int);
```

We will call the two arguments `A` and `B`, respectively, so that our function prototype becomes:
```C
int testb(int A, int B);
```

In the `cdecl` calling convention used by many 32-bit x86 binaries, arguments are pushed right-to-left onto the stack. If we were to break right after the function prologue finishes (right after `mov ebp, esp`), the stack would look as follows:
```
( lower memory )

[ebp] .......... saved EBP value from the function that called `testb()`
[ebp + 4] ...... return address into the function that called `testb()`
[ebp + 8] ...... A (first argument)
[ebp + 12] ..... B (second argument)

( higher memory )
```

Now we can replace these `ebp` offsets in the disassembly and remove the function prologue and epilogue to make it more readable. We'll also add some comments after each instruction to show higher-level constructs and help us with decompilation later:
```
.LFB0:    
          [... prologue ...]    
+-------- mov       ecx, {A}                 ; ecx = A
|          
|         mov       eax, {A}                 ; eax = A
|         cmp       eax, -668203058          ; A [?] -668203058
|         setnb     al                       ; al = (A >= -668203058) ? 0 : 1
|   +---- movzx     ebx, al                  ; ebx = (A >= -668203058) ? 0 : 1
|   |       
|   |     mov       eax, {B}                 ; eax = B
|   |     or        eax, 2                   ; eax = B|2
|   |     mov       esi, eax                 ; esi = B|2
|   |     mov       eax, {A}                 ; eax = A
|   |     cdq                                ; edx:eax = A
|   |     idiv      esi                      ; eax = A / (B|2)
|   |     mov       edx, eax                 ; edx = A / (B|2)
|   |       
|   |     mov       eax, {B}                 ; eax = B
|   |     add       eax, edx                 ; eax = B + (A / (B|2))
|   |     imul      eax, eax, -1039577924    ; eax = (B + (A / (B|2))) * -1039577924
|   |       
|   |     mov       edx, {B}                 ; edx = B
|   |     or        edx, 2                   ; edx = B|2
|   |     mov       esi, edx                 ; esi = B|2
|   |     mov       edx, 0                   ; edx = 0
|   |     div       esi                      ; eax = (unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2)
|   |      
|   +---> add       eax, ebx                 ; eax = ((unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2)) + ...
|                                            ;       ((A >= -668203058) ? 0 : 1)
|         or        eax, 2                   ; eax = (((unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2)) + ...
|                                            ;       ((A >= -668203058) ? 0 : 1)) | 2
|         mov       ebx, eax                 ; ebx = (((unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2)) + ...
|                                            ;       ((A >= -668203058) ? 0 : 1)) | 2
|
+-------> mov       eax, ecx                 ; eax = A
          mov       edx, 0                   ; edx = 0
          div       ebx                      ; eax = (unsigned int)A /
                                             ;       (
                                             ;         (((unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2)) + ...
                                             ;         ((A >= -668203058) ? 0 : 1)) | 2
                                             ;       )
          test      eax, eax                 ; (see_above) [?] (see_above)
          je        .L2                      ; (see_above) == 0


          mov       eax, {A}                 ; eax = A
          cmp       eax, -1357278678         ; A [?] -1357278678
          setnb     al                       ; al = ((A >= -1357278678) ? 0 : 1)
          movzx     edx, al                  ; edx = ((A >= -1357278678) ? 0 : 1)
          
          mov       eax, {A}                 ; eax = A
          sub       edx, eax                 ; edx = ((A >= -1357278678) ? 0 : 1) - A
          mov       eax, {A}                 ; eax = A
          add       eax, edx                 ; eax = A + (((A >= -1357278678) ? 0 : 1) - A)
          jmp       .L3       
          
.L2:      
          mov       eax, {A}                 ; eax = A
          
          mov       edx, {A}                 ; edx = A
          cmp       edx, -837399105          ; A [?] -837399105
          setnb     dl                       ; dl = ((A >= -837399105) ? 0 : 1)
          movzx     ecx, dl                  ; ecx = ((A >= -837399105) ? 0 : 1)
          
          mov       edx, {A}                 ; edx = A
          add       edx, ecx                 ; edx = A + ((A >= -837399105) ? 0 : 1)
          mov       ecx, edx                 ; ecx = A + ((A >= -837399105) ? 0 : 1)
          or        ecx, 2                   ; ecx = (A + ((A >= -837399105) ? 0 : 1)) | 2
          mov       edx, 0                   ; edx = 0
          div       ecx                      ; eax = (unsigned int)A / ((A + ((A >= -837399105) ? 0 : 1)) | 2)
          
.L3:      
          [... epilogue ...]
          return eax
```

The function has the basic control structure:
```C
int testa(int A, int B) {

  if (some_expression == 0) // .LFB0 block, first part
  {
    return something; // .L2 block
  }
  else
  {
    return something_else; // .LFB0 block, second part (fall-through)
  }
}
```

Filling in with the actual expressions we decompiled above:
```C
int testb(int A, int B) {

  if (
       ((unsigned int)A /
       ((((unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2)) + ((A >= -668203058) ? 0 : 1)) | 2)
     ) == 0)
  {
    return (unsigned int)A / (unsigned int)((A + ((A >= -837399105) ? 0 : 1)) | 2);
  }
  else
  {
    return A + (((A >= -1357278678) ? 0 : 1) - A);
  }
}
```

We can dump the x86 code provided by the server to a file ([testa.asm](testa.asm)) and assemble it using `nasm`. Then we can use `gcc` to link the assembled object file with a driver program ([test.c](test.c)) that contains our decompiled C function `testb()`. See [Makefile](Makefile) to see how this was accomplished. Fuzzing each function with random integers will demonstrate with some certainty that they always return the same value:
```
ameise@STAHLKERN:~/CS2015/re/075_ASMTOSIE_2$ ls
Makefile  testa.asm  test.c
ameise@STAHLKERN:~/CS2015/re/075_ASMTOSIE_2$ make
nasm -f elf32 -o testa.o testa.asm
gcc -m32 -mpreferred-stack-boundary=2 -fno-stack-protector -O0 testa.o test.c -o test
ameise@STAHLKERN:~/CS2015/re/075_ASMTOSIE_2$ ls
Makefile  test  testa.asm  testa.o  test.c
ameise@STAHLKERN:~/CS2015/re/075_ASMTOSIE_2$ ./test 
SUCCESS
SUCCESS
SUCCESS
SUCCESS
[...]
```

Now we remove newlines from our `testb()` function definition and submit it to the server:
```
[Type your C function now]
int testb(int A, int B) {  if (  (    (unsigned int)A    /    (      (        ((unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2))        +        ((A >= -668203058) ? 0 : 1)      ) | 2    )  ) == 0)  {    return (unsigned int)A / (unsigned int)((A + ((A >= -837399105) ? 0 : 1)) | 2);  }  else  {    return A + (((A >= -1357278678) ? 0 : 1) - A);  }}
Congrats! Here is your flag: 345fe6df122a137e7b4005b22dd438fc_youre_a_real_retsam
```