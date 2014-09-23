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
- multiple similar "inc" are merged into an "add". (`+++` becomes `add [ptr], 3`, etc.)

Possible optimisations:
- I developed it under windows, on linux libc can be striped out to use interruptions.
- remove useless code, i.e. `+-` or `<>`.
- probably others.
