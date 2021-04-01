## MISSION IMPOSSIBLE 0 [50]
**Summary**: Hard-coded key

**Description**: Agent X, welcome to yet another mission. We've received info that Chimera spies are employing a new protection mechanism: a time bomb. Gaining access to their servers activates the self-destruction mechanism that obliterates all useful information after minutes. The secret file is under `/bomb/flag` (we just gained unprivileged ssh access: `user:agent` / `pass:agent42` at `shell.cyberstakesonline.com:8872`). All our previous agents have failed and you are our only remaining hope. Can you do it in time?

## Solution

SSH into the server and do a reconaissance of `/bomb/`:
```
ameise@STAHLKERN:~$ ssh agent@shell.cyberstakesonline.com -p 8872
agent@shell.cyberstakesonline.com's password: 
agent@9fd33834d6e6[00:09:52]:~$ cd /bomb
agent@9fd33834d6e6[00:09:50]:/bomb$ ls -l
total 12
-rwsr-sr-x 1 secret secret 7556 Sep  6 19:38 bomb
-rw-r----- 1 secret secret   49 Sep  6 19:38 flag
```

Use `objdump` to look at the `main()` function in `bomb`:
```
agent@9fd33834d6e6[00:09:42]:/bomb$ objdump -d -M intel bomb
[ ... ]
080485e9 <main>:
 80485e9:	8d 4c 24 04          	lea    ecx,[esp+0x4]
 80485ed:	83 e4 f0             	and    esp,0xfffffff0
 80485f0:	ff 71 fc             	push   DWORD PTR [ecx-0x4]
 80485f3:	55                   	push   ebp
 80485f4:	89 e5                	mov    ebp,esp
 80485f6:	51                   	push   ecx
 80485f7:	83 ec 14             	sub    esp,0x14
 80485fa:	89 c8                	mov    eax,ecx
 80485fc:	83 38 02             	cmp    DWORD PTR [eax],0x2
 80485ff:	74 07                	je     8048608 <main+0x1f>
 8048601:	b8 01 00 00 00       	mov    eax,0x1
 8048606:	eb 45                	jmp    804864d <main+0x64>
 8048608:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
 804860b:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
 804860e:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
 8048611:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 8048614:	8b 00                	mov    eax,DWORD PTR [eax]
 8048616:	3d 49 74 61 69       	cmp    eax,0x69617449
 804861b:	75 1b                	jne    8048638 <main+0x4f>
 804861d:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 8048620:	83 c0 04             	add    eax,0x4
 8048623:	8b 00                	mov    eax,DWORD PTR [eax]
 8048625:	3d 70 75 27 73       	cmp    eax,0x73277570
 804862a:	75 0c                	jne    8048638 <main+0x4f>
 804862c:	e8 1a ff ff ff       	call   804854b <reveal_flag>
 8048631:	b8 00 00 00 00       	mov    eax,0x0
 8048636:	eb 15                	jmp    804864d <main+0x64>
 8048638:	83 ec 0c             	sub    esp,0xc
 804863b:	68 f9 86 04 08       	push   0x80486f9
 8048640:	e8 cb fd ff ff       	call   8048410 <puts@plt>
 8048645:	83 c4 10             	add    esp,0x10
 8048648:	b8 01 00 00 00       	mov    eax,0x1
 804864d:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
 8048650:	c9                   	leave  
 8048651:	8d 61 fc             	lea    esp,[ecx-0x4]
 8048654:	c3                   	ret    
 8048655:	66 90                	xchg   ax,ax
 8048657:	66 90                	xchg   ax,ax
 8048659:	66 90                	xchg   ax,ax
 804865b:	66 90                	xchg   ax,ax
 804865d:	66 90                	xchg   ax,ax
 804865f:	90                   	nop
[...]
```

In order to call `reveal_flag()`, we must pass two `cmp` checks. The first `cmp` checks bytes 0-3 of `argv[1]`:
```
 804860b:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
 804860e:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
 8048611:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 8048614:	8b 00                	mov    eax,DWORD PTR [eax]
 8048616:	3d 49 74 61 69       	cmp    eax,0x69617449
```
The 4-byte value `0x69617449`, after flipping to account for little-endian storage, matches the following ASCII characters:
```
0x49 ... I
0x74 ... t
0x61 ... a
0x69 ... i
```

The second `cmp` checks bytes 4-7 of `argv[1]`:
```
 804861d:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 8048620:	83 c0 04             	add    eax,0x4
 8048623:	8b 00                	mov    eax,DWORD PTR [eax]
 8048625:	3d 70 75 27 73       	cmp    eax,0x73277570
```
The 4-byte value `0x73277570`, after flipping to account for little-endian storage, matches the following ASCII characters:
```
0x70 ... p
0x75 ... u
0x27 ... '
0x73 ... s
```

Now we have the full key (`Itaipu's`). Make sure to escape the apostrophe on the command line:
```
agent@9fd33834d6e6[00:05:55]:/bomb$ ./bomb Itaipu\'s
Your flag is: 5d15caf818b79ce8b146da6827607fa6_wow_you_are_fast
```