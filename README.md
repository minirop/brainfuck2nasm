brainfuck2nasm
==============

Take a brainfuck file and turn it into full ASM (nasm syntax).

Basically it does those:
- `+` becomes `inc [ptr]`.
- `-` becomes `dec [ptr]`.
- `>` becomes `inc ptr`.
- `<` becomes `dec ptr`.
- `[` and `]` become a label, a comparison and a jump to the other's label.
- `.` and `,` become respectively a call to `putchar` and `putchar`.

Some small optimizations are made:
- multiple similar "inc" are merged into an "add".
- I developed it under windows, libc could be striped out to use interruptions.
- probably others.