global testa
          
testa:    
.LFB0:    
          push      ebp       
          mov       ebp, esp  
          push      esi       
          push      ebx       
          mov       ecx, DWORD [ebp+8]
          mov       eax, DWORD [ebp+8]
          cmp       eax, -668203058
          setnb     al        
          movzx     ebx, al   
          mov       eax, DWORD [ebp+12]
          or        eax, 2    
          mov       esi, eax  
          mov       eax, DWORD [ebp+8]
          cdq       
          idiv      esi       
          mov       edx, eax  
          mov       eax, DWORD [ebp+12]
          add       eax, edx  
          imul      eax, eax, -1039577924
          mov       edx, DWORD [ebp+12]
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
          mov       eax, DWORD [ebp+8]
          cmp       eax, -1357278678
          setnb     al        
          movzx     edx, al   
          mov       eax, DWORD [ebp+8]
          sub       edx, eax  
          mov       eax, DWORD [ebp+8]
          add       eax, edx  
          jmp       .L3       
.L2:      
          mov       eax, DWORD [ebp+8]
          mov       edx, DWORD [ebp+8]
          cmp       edx, -837399105
          setnb     dl        
          movzx     ecx, dl   
          mov       edx, DWORD [ebp+8]
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
          
          
