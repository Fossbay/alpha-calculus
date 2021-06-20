# Alpha Calculus

## A tiny embeddable interpreter

### Usage

alpha function template:

```lisp
a{opcode}({args separated by ','})
```

In Alpha Calculus data can be used by writing '#xx' or '$xx' (where 'xx' is an hexadecimal byte): the first means 'value xx', the second mean 'value in address xx', arguments can also be alpha functions and the value will be their return

the opcodes are listed below:

* 0x00 = **store(addr, val)**:  stores ***val*** in address ***addr***
* 0x01 = **exit(exit_code)**:   exits with ***exit_code*** as ***exit code***
* 0x02 = **putint(...)**:       prints all the values passed as argument
* 0x03 = **putchar(...)**:      prints all the values passed as argument as characters
* 0x04 = **add(...)**:          returns the sum of the arguments
* 0x05 = **sub(x, ...)**:       returns the subtraction of the arguments
* 0x06 = **mul(...)**:          returns the multiplication of the arguments
* 0x07 = **div(x, ...)**:       returns the division of the arguments
* 0x08 = **loop(...)**:         loops a set of operations, equivalent to a **while(true)**
* 0x09 = **break()**:           breaks a loop
