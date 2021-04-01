## BYTE CODE [60]

**Summary**: Hard-coded password in Python compiled bytecode.

**Description**: You need to authenticate to proceed! Enter the password into the authentication program at [problem.pyc](problem.pyc) to retrieve the flag.

## Solution

Run `strings` on the bytecode file (`problem.pyc`) to get the password:
```
ameise@STAHLKERN:~/CS2015/re/060_BYTE_CODE$ strings problem.pyc 
0a616dc82afc889c320e4d4248acff3as
Password > s
Success!t
Your flag: %ss
Wrong.N(
passwordt	
raw_inputt	
user_passt
flag(
problem.pyt
<module>
ameise@STAHLKERN:~/CS2015/re/060_BYTE_CODE$ python problem.pyc 
Password > 0a616dc82afc889c320e4d4248acff3a
Success!
Your flag: e9103cb293eefffd
```