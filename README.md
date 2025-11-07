<h1>Chew</h1>
Introducing Chew: a C-based compiler translating a small, Python-like language to ARM64 assembly.

<h2>Running Chew</h2>

To run Chew, create a .ch file and run it in main.c with
```
gcc *.c
```

followed by

```
./a.out
```

in your terminal of choice.

Chew currently emits assembly instructions but can be edited to save to file.

<h2>Example code</h2>

Example .ch code
```
if num > 4 then blah = "notblah".
```

Output (ARM64 assembly)
```
.section .text
.data
.globl _num
_num:
        .quad 0
.globl _blah
_blah:
        .quad 0
.globl _notblah
_notblah:
        .asciz "notblah"

.globl _start

_start:
_.L0:

        ldr x0, =_blah
        ldr x1, =_notblah
        str x1, [x0]

cmp x0, 4
bgt _.L0
```
