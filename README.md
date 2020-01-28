# Example Build

```
[chckyn@grace:~/dev/interview-prep]$ nix-shell nix-files/simple.nix --command 'source $setup; return'

[nix-shell:~/dev/interview-prep]$ make
mkdir -p build/./src/
clang-format --verbose -i -style=Google src/simple.c
Formatting src/simple.c
clang -I./src -MMD -MP  -c src/simple.c -o build/./src/simple.c.o
clang ./build/./src/simple.c.o -o build/simple

[nix-shell:~/dev/interview-prep]$ build/simple
Hello, world!
```
