## BYTE CODE 2 [80]
**Tags**: Python bytecode

**Decription**: Our authentication service has a short memory. You need to authenticate one more time! Enter the password in the authentication program at [problem.pyc](problem.pyc) to retrieve the flag.

## Solution
Decompile the bytecode [problem.pyc](problem.pyc) back into Python source code [problem.py](problem.py) with any bytecode decompiler:
```
ameise@STAHLKERN:~/CS2015/re/080_BYTE_CODE_2$ ls
problem.pyc
ameise@STAHLKERN:~/CS2015/re/080_BYTE_CODE_2$ uncompyle6 problem.pyc > problem.py
ameise@STAHLKERN:~/CS2015/re/080_BYTE_CODE_2$ ls
problem.py  problem.pyc
```

We see a byte-by-byte password check:
```Python
user_pass = raw_input('Password > ')
if user_pass[6] == 'a':
    if user_pass[14] == '8':
        if user_pass[3] == '0':
            if user_pass[4] == 'e':
                if user_pass[1] == '4':
                    if user_pass[11] == 'e':
                        if user_pass[5] == '4':
                            if user_pass[8] == '6':
                                if user_pass[15] == 'a':
                                    if user_pass[9] == 'd':
                                        if user_pass[10] == '4':
                                            if user_pass[7] == 'b':
                                                if user_pass[12] == 'a':
                                                    if user_pass[0] == '0':
                                                        if user_pass[2] == '5':
                                                            if user_pass[13] == 'c':
                                                                print 'Success!'
```

We could write a fancy Python script to unscramble the password, but it would be much easier to just do it by hand since we can clearly see all of the indices into `user_pass[]`:
```
ameise@STAHLKERN:~/CS2015/re/080_BYTE_CODE_2$ python problem.pyc
Password > 0450e4ab6d4eac8a
Success!
Your flag: c66c83e50b9863b5
```