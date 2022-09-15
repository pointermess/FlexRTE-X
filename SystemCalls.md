# System Calls

## Console

The console system call can be called by using the interrupt on `0x40`.

```asm
int 0x40
```

### Print new line (EAX = 0)

Prints a new line.

```asm
  mov eax, 0              ; print new line
  int 0x40                ; console system call
```

### Print character (EAX = 1)

Prints a character in the console.

|Parameter|Description|
|---|---|
|EBX|ASCII value of character to print|


```asm
  mov eax, 1              ; print char
  mov ebx, 43             ; ASCII "+"
  int 0x40                ; console system call
```

### Print new line until zero-byte (EAX = 3)

Prints text until zero-byte including a new line.

|Parameter|Description|
|---|---|
|EBX|Start address of text to print|

```asm
  mov eax, 3              ; print line until 0 
  mov ebx, .str           ; mem-address of 'str' variable
  int 0x40                ; console system call
```

