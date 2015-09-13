# Installation #

```
$ phpize
$ ./configure ; make
$ sudo make install
```

# Using #

```
$ php -dextension=pod.so -dpod.dump_path=/home/felipe/opcodes test.php
$ cat /home/felipe/opcodes
Current file: /home/felipe/dev/test.php
+ Opcode ZEND_ASSIGN
- Op type: IS_CV 'a' '0' (type: null refcount: 3 is_ref: 0)
- Op type: IS_CONST '1' (type: integer refcount: 2 is_ref: 1)
- Extended value: 0
- Line: 3

+ Opcode ZEND_ASSIGN_ADD
- Op type: IS_CV '1' (type: integer refcount: 1 is_ref: 0)
- Op type: IS_CONST '2' (type: integer refcount: 2 is_ref: 1)
- Extended value: 0
- Line: 4

+ Opcode ZEND_PRINT
- Op type: IS_CONST 'foo' (type: string refcount: 2 is_ref: 1)
- Op type: IS_UNUSED
- Extended value: 0
- Line: 6

+ Opcode ZEND_FREE
- Op type: IS_TMP_VAR
- Op type: IS_UNUSED
- Extended value: 0
- Line: 6

+ Opcode ZEND_RETURN
- Op type: IS_CONST '1' (type: integer refcount: 2 is_ref: 1)
- Op type: IS_UNUSED
- Extended value: 0
- Line: 7
```