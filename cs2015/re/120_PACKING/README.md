## PACKING [120]

## Solution
**NEED FULL WRITEUP**

Use black box PIN script [BlackBox.py](BlackBox.py):
```
ameise@STAHLKERN:~/CS2015/re/120_PACKING$ python -u BlackBox.py 
brute forcing key[0] 012
key: 2
brute forcing key[1] 012
key: 22
brute forcing key[2] 0123456789a
key: 22a
brute forcing key[3] 01234
key: 22a4
brute forcing key[4] 0123456789a
key: 22a4a
brute forcing key[5] 0123456789ab
key: 22a4ab
brute forcing key[6] 0123456
key: 22a4ab6
brute forcing key[7] 0123456789abcdef
key: 22a4ab6f
brute forcing key[8] 012345
key: 22a4ab6f5
brute forcing key[9] 0
key: 22a4ab6f50
brute forcing key[10] 0
key: 22a4ab6f500
brute forcing key[11] 0123
key: 22a4ab6f5003
brute forcing key[12] 012345
key: 22a4ab6f50035
brute forcing key[13] 0123456789abcde
key: 22a4ab6f50035e
brute forcing key[14] 01234567
key: 22a4ab6f50035e7
brute forcing key[15] 0123456789a
key: 22a4ab6f50035e7a
brute forcing key[16] 01234
key: 22a4ab6f50035e7a4
brute forcing key[17] 0123456789abcdef
key: 22a4ab6f50035e7a4f
brute forcing key[18] 012345
key: 22a4ab6f50035e7a4f5
brute forcing key[19] 0123456789abcd
key: 22a4ab6f50035e7a4f5d
brute forcing key[20] 0123456789abcd
key: 22a4ab6f50035e7a4f5dd
brute forcing key[21] 01234567
key: 22a4ab6f50035e7a4f5dd7
brute forcing key[22] 01
key: 22a4ab6f50035e7a4f5dd71
brute forcing key[23] 0123456789ab
key: 22a4ab6f50035e7a4f5dd71b
brute forcing key[24] 012
key: 22a4ab6f50035e7a4f5dd71b2
brute forcing key[25] 0123456789abcdef
key: 22a4ab6f50035e7a4f5dd71b2f
brute forcing key[26] 012
key: 22a4ab6f50035e7a4f5dd71b2f2
brute forcing key[27] 0123456789abcde
key: 22a4ab6f50035e7a4f5dd71b2f2e
brute forcing key[28] 0123456789abc
key: 22a4ab6f50035e7a4f5dd71b2f2ec
brute forcing key[29] 0123
key: 22a4ab6f50035e7a4f5dd71b2f2ec3
brute forcing key[30] 01
key: 22a4ab6f50035e7a4f5dd71b2f2ec31
brute forcing key[31] 0123
key: 22a4ab6f50035e7a4f5dd71b2f2ec313
ameise@STAHLKERN:~/CS2015/re/120_PACKING$ ./packing 22a4ab6f50035e7a4f5dd71b2f2ec313
Success! Here is your key:
22a4ab6f50035e7a4f5dd71b2f2ec313
```