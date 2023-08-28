# Compilers

## Scaner (Lexical Analizer)

```bash
sudo pacman -S base-devel
sudo pacman -S flex
```

```bash
flex <filename>
clang <filename>.yy.c
```

## Parser (Syntax Analizer)

```bash
sudo pacman -S bison
```

```bash
bison <filename>
clang <filename>.tab.c -lm # detect pow -> <math.h>
```
