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

## Install Java

<https://www.oracle.com/java/technologies/downloads/>
<https://www.tecmint.com/install-java-on-arch-linux/>

### CLI

```bash
sudo pacman -S jdk-openjdk

```

```bash
tar xvzpf jdk-20_linux-x64_bin.tar.gz
```

### To create a symbolic link

```bash
ln -s /home/user/<directory-app> /opt/java
```

### Create a Path

```sh
JAVA_HOME=/opt/java
CLASSPATH=$JAVA_HOME/lib
PATH=$JAVA_HOME/bin:$PATH
export JAVA_HOME
export CLASSPATH
```

## Install Antlr

```bash
python -m pip install antlr4-tools
```

```bash
antlr
```

### Usage

```bash
antlr4-parse Expr.g4 prog -tree
```

```bash
antlr4-parse Expr.g4 prog -tokens -trace
```

```bash
antlr4-parse Expr.g4 prog -gui
```

```bash
antlr4 Expr.g4 
```

```bash
antlr4 -Dlanguage=Cpp Expr.g4
```
