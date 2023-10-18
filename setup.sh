#!/usr/bin/sh

setup(){
    local venvpath="$HOME/Disk/Codes/Projects/Compilers/src/env"

    # Verifica si el directorio del entorno virtual existe
    
	source "${venvpath}/bin/activate"
    

    export CLASSPATH=.:~/.m2/repository/org/antlr/antlr4/4.13.1/antlr4-4.13.1-complete.jar:$CLASSPATH
    alias grun='java org.antlr.v4.gui.TestRig'

	export ANTLR4I=/usr/include/antlr4-runtime
	export ANTLR4L=/usr/lib
}

setup