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
		antlr4 -no-listener -visitor -o <folderName> Expr*.g4
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

4. How to use with C++

	### Configure Antlr Runtime

	- Dowload antlr4-runtime 
  
    	1. Using CLI (Arch Linux)
   
			```bash
			sudo pacman -S antlr4-runtime
			```

		2. Manually (Others / Debian)
		
			- Download `antlr4-runtime` via Curl

				```bash
				curl https://www.antlr.org/download/antlr4-cpp-runtime-4.13.1-source.zip -o antlr4-runtime.zip  
				```

			- Now install the necesary dependencies and libraries

				```bash
				sudo apt install cmake
				sudo apt install uuid-dev
				sudo apt install pkg-config 
				```

			- Now we compile and get the libraries from antlr4-runtime

				```bash
				mkdir build && mkdir run && cd build
				cmake ..
				DESTDIR=../run make install
				```

			- Now copy the ANTLR 4 include files to `/usr/local/include` and the ANTLR 4 libraries to `/usr/local/lib`

				```bash
				cd ../run/usr/local/include
				ln -s ~/Apps/antlr4-cpp/run/usr/local/include/antlr4-runtime /usr/local/include
				cd ../lib
				sudo cp * /usr/local/lib
				sudo ldconfig
				```
			- Now `antlr4-runtime` is installed

	### How to use
	- Use with Clang++ (Working)
		1. First create the files using `antlr4` and save them in directory `libs`
			```bash
			antlr4 -no-listener -visitor -o libs -Dlanguage=Cpp *.g4
			```

		2. And now compile with the next command
			```bash
			clang++ -std=c++17 -I/usr/local/include/antlr4-runtime -Ilibs libs/*.cpp -lantlr4-runtime *cpp
			```
			> Use `-std=c++17` in case not detect std libraries

			> Use `-I/usr/local/include/antlr4-runtime` if `antlr4-runtime` was installed manually

			> Use `-I/usr/include/antlr4-runtime` if `antlr4-runtime` was installed with package manager

			> Use `-I$ANTLR4I -L$ANTLR4L` if update the file `setup.sh`, exporting the paths of antlr4
			```sh
			export ANTLR4I=/usr/local/include/antlr4-runtime
			export ANTLR4L=/usr/local/lib/antlr4-runtime
			```

	- Use with Cmake file (Not Working Yet)

        1. Create the file `CMakeLists.txt` with the next configurations
    		```txt
    		cmake_minimum_required(VERSION 3.20)
    		project(antlr)
    		add_executable(antlr main.cpp)
    		target_link_libraries(antlr path/to/antlr4-runtime)
    		```

        2. Configuration for Cmake (create folder `build`)
           ```bash
           mkdir build
           cd build
           mkdir release
           mkdir debug
           ```

        3. Now create two subfolders `release` and `debug`
           ```bash
           mkdir release
           mkdir debug
           ```

        4. Now from each of both subfolders, execute the nex command
           ```bash
           cmake -DCMAKE_BUILD_TYPE=Debug ../.. # For debug
           cmake ../.. # For release
           ```

        5. Now execute the program with `make`
5. How to use LLVM-15
    ### Install llvm

	- Using CLI in Arch
		```bash
		sudo pacman llvm
		```

	- Using CLI in Debian
		```bash
		sudo apt-get install llvm-15 llvm-15-dev llvm-15-doc llvm-15-linker-tools llvm-15-runtime llvm-15-tools
		```

	### Using llvm

    - Generate Code to Dot Language
		- Generate Intermediate code with clang.
			```bash
			clang -S -emit-llvm file.c  
			```
		- Use opt for optimization
			```bash
			opt --dot-cfg file.ll -enable-new-pm=0
			```
		- Use dot
			```bash
			dot -Tpng .prefix_sum.dot -o ps.png   
			```
		- Generate the output with llc (Middle End)
			```bash
			llc file.ll -march=arm -o file.arm  
			```
			
	- Compiling a llvm project
    	- For Arch
			```bash
			clang++ -lLLVM <file>.cpp
			```
			
    	- For Debian
			```bash
			clang++ <file>.cpp $(llvm-config-15 --cxxflags) -lLLVM-15
			```
