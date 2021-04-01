## SYZYGY [100]

## Solution
**NEED FULL WRITEUP**

Use black box PIN script [BlackBox.py](BlackBox.py):
```
ameise@STAHLKERN:~/CS2015/re/100_SYZYGY$ python -u BlackBox.py 
brute forcing key[0] 0123456789a
key: a
brute forcing key[1] 01234567
key: a7
brute forcing key[2] 0123456789ab
key: a7b
brute forcing key[3] 0123456
key: a7b6
brute forcing key[4] 0123456789abcd
key: a7b6d
brute forcing key[5] 0123456789abcde
key: a7b6de
brute forcing key[6] 012345
key: a7b6de5
brute forcing key[7] 01234
key: a7b6de54
brute forcing key[8] 0123456789a
key: a7b6de54a
brute forcing key[9] 01234
key: a7b6de54a4
brute forcing key[10] 0123456
key: a7b6de54a46
brute forcing key[11] 0123456789
key: a7b6de54a469
brute forcing key[12] 0123456789ab
key: a7b6de54a469b
brute forcing key[13] 01234567
key: a7b6de54a469b7
brute forcing key[14] 0123456789abcd
key: a7b6de54a469b7d
brute forcing key[15] 0123456789a
key: a7b6de54a469b7da
brute forcing key[16] 01234
key: a7b6de54a469b7da4
brute forcing key[17] 01
key: a7b6de54a469b7da41
brute forcing key[18] 012
key: a7b6de54a469b7da412
brute forcing key[19] 0123456789abcdef
key: a7b6de54a469b7da412f
brute forcing key[20] 0123
key: a7b6de54a469b7da412f3
brute forcing key[21] 0
key: a7b6de54a469b7da412f30
brute forcing key[22] 0123456789abc
key: a7b6de54a469b7da412f30c
brute forcing key[23] 0123456789a
key: a7b6de54a469b7da412f30ca
brute forcing key[24] 0123456789abc
key: a7b6de54a469b7da412f30cac
brute forcing key[25] 0123456789ab
key: a7b6de54a469b7da412f30cacb
brute forcing key[26] 0123456789abcde
key: a7b6de54a469b7da412f30cacbe
brute forcing key[27] 0
key: a7b6de54a469b7da412f30cacbe0
brute forcing key[28] 0123456789
key: a7b6de54a469b7da412f30cacbe09
brute forcing key[29] 0123
key: a7b6de54a469b7da412f30cacbe093
brute forcing key[30] 0123
key: a7b6de54a469b7da412f30cacbe0933
brute forcing key[31] 0
key: a7b6de54a469b7da412f30cacbe09330
ameise@STAHLKERN:~/CS2015/re/100_SYZYGY$ ./syzygy a7b6de54a469b7da412f30cacbe09330
Success! Here is your key: a7b6de54a469b7da412f30cacbe09330
```