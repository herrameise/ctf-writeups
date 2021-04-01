## NOPCRACKER [120]

## Solution
Used black box PIN script:
```
ameise@STAHLKERN:~/CS2015/re/120_NOPCRACKER$ python -u BlackBox.py 
brute forcing key[0] 0123456789abc
key: c
brute forcing key[1] 0123456789abc
key: cc
brute forcing key[2] 0123456789a
key: cca
brute forcing key[3] 0123456789abc
key: ccac
brute forcing key[4] 0123
key: ccac3
brute forcing key[5] 01234567
key: ccac37
brute forcing key[6] 0123
key: ccac373
brute forcing key[7] 0123456789abcdef
key: ccac373f
brute forcing key[8] 0123456789ab
key: ccac373fb
brute forcing key[9] 0123456789abcd
key: ccac373fbd
brute forcing key[10] 0123456
key: ccac373fbd6
brute forcing key[11] 0123456789ab
key: ccac373fbd6b
brute forcing key[12] 0123456789
key: ccac373fbd6b9
brute forcing key[13] 0123456
key: ccac373fbd6b96
brute forcing key[14] 0123456
key: ccac373fbd6b966
brute forcing key[15] 0
key: ccac373fbd6b9660
brute forcing key[16] 012
key: ccac373fbd6b96602
brute forcing key[17] 01234
key: ccac373fbd6b966024
brute forcing key[18] 0123456789abcde
key: ccac373fbd6b966024e
brute forcing key[19] 0123456789abcd
key: ccac373fbd6b966024ed
brute forcing key[20] 0123456
key: ccac373fbd6b966024ed6
brute forcing key[21] 0
key: ccac373fbd6b966024ed60
brute forcing key[22] 0123456789
key: ccac373fbd6b966024ed609
brute forcing key[23] 0123
key: ccac373fbd6b966024ed6093
brute forcing key[24] 0123456789abcd
key: ccac373fbd6b966024ed6093d
brute forcing key[25] 0123456789abcde
key: ccac373fbd6b966024ed6093de
brute forcing key[26] 0123456789abcd
key: ccac373fbd6b966024ed6093ded
brute forcing key[27] 01234567
key: ccac373fbd6b966024ed6093ded7
brute forcing key[28] 0123456789ab
key: ccac373fbd6b966024ed6093ded7b
brute forcing key[29] 012
key: ccac373fbd6b966024ed6093ded7b2
brute forcing key[30] 0123456789ab
key: ccac373fbd6b966024ed6093ded7b2b
brute forcing key[31] 0123456
key: ccac373fbd6b966024ed6093ded7b2b6
ameise@STAHLKERN:~/CS2015/re/120_NOPCRACKER$ ./nopcracker ccac373fbd6b966024ed6093ded7b2b6
Success! Here is your key:
ccac373fbd6b966024ed6093ded7b2b6
```