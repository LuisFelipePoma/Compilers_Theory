# Compilers
---
## Scaner (Lexical Analizer)
### FLEX

1. Installation
	```bash
	sudo pacman -S base-devel
	sudo pacman -S flex
	```
 
2. Usage
	```bash
	flex <filename>
	clang <filename>.yy.c
	```
 
---
## Parser (Syntax Analizer)
### BISON
1. Install
	```bash
	sudo pacman -S bison
	```

3. Usage
	```bash
	bison <filename>
	```
	```bash
	clang <filename>.tab.c -lm # detect pow -> <math.h>
	```

### ANTLR
#### Install Java

- Install via CLI

  1. [ArchLinux](https://www.tecmint.com/install-java-on-arch-linux/)
   
	* Now we are going to install the latest version of JRE
		```bash
		sudo pacman -S jre-openjdk
		```

	* Now we are going to install the latest version of JDK
		```bash
		sudo pacman -S jdk-openjdk
		```
   2. Others
		```bash
		sudo apt install openjdk-XX-jdk
		```
		>Install the latest available version for your distro

- Install Manually
  
  1. [Download from Oracle](https://www.oracle.com/java/technologies/downloads/)
     	```bash
		curl -O https://download.oracle.com/java/20/latest/jdk-20_linux-x64_bin.deb
		```
  2. Install JDK
		```bash
		sudo dpkg -i jdk-20_linux-x64_bin.deb
		```
		> If you have errors, install the missing dependencies with the comand below and install again
		```bash
		sudo apt install -y <lib-name>
		```
		
  3. `OPTIONAL` Create a symbolic link 
		```bash
		ln -s /home/user/<directory-app> /opt/java
		```
  
  4. `OPTIONAL` Create a Path
	  ```bash
		JAVA_HOME=/opt/java
		CLASSPATH=$JAVA_HOME/lib
		PATH=$JAVA_HOME/bin:$PATH
		export JAVA_HOME
		export CLASSPATH
	  ```

#### Install Antlr
1. Installation
   * Create a virtual enviroment
		```bash
		python -m venv /path/to/new/virtual/environment
		```
   
   * Install Antlr using pip
		```bash
		python -m pip install antlr4-tools
		```
   
   * Verify installation
		```bash
		antlr
		```
   
2. Usage
	- Show the parser with a **tree** in terminal
		```bash
		antlr4-parse Expr.g4 prog -tree
		```

	- Show the parser with **tokens** in terminal
		```bash
		antlr4-parse Expr.g4 prog -tokens -trace
		```
  
	- Show the parser with a **GUI** in a new window
		```bash
		antlr4-parse Expr.g4 prog -gui
		```
  
3. Compilation
   - Using `antlr4` as a command, we generate the next files:
		```bash
		~ antlr4 Expr.g4 
		~ ls Expr*.java

		ExprBaseListener.java  	 		ExprLexer.java    
		ExprListener.java        		1ExprParser.java
		```
   
		> We can also generate the C++ code.

		```bash
		~ antlr4 -Dlanguage=Cpp Expr.g4
		~ ls Expr*.cpp Expr*.h

		ExprBaseListener.cpp  ExprLexer.cpp         ExprListener.cpp      ExprParser.cpp
		ExprBaseListener.h    ExprLexer.h           ExprListener.h        ExprParser.h
		```

   - Now to compile with two files **ExprLexer** and **ExprParser**



     * How to be `setup.sh`

		```sh
		#!/usr/bin/sh

		setup(){
			local venvpath="$HOME/path/to/env"

			source "${venvpath}/bin/activate"
			
			export CLASSPATH=.:~/.m2/repository/org/antlr/antlr4/4.13.1/antlr4-4.13.1-complete.jar:$CLASSPATH
			alias grun='java org.antlr.v4.gui.TestRig'
		}

		setup
		```
	* Now we compile boths files.
		```bash
		antlr4 Expr*.g4
		```
	* With `javac` we compile the java codes generated.
		```bash
		javac *.java
		```
	* Execute the script
		```bash
		./setup.sh
		```

     * Finally run Antlr with `grun`
		```bash
		grun <GrammarName> <RuleStart> -<param>
		```

