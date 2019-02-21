build: build-antlr


build-antlr:
	java -jar antlr-4.7.2-complete.jar -no-visitor -no-listener Expr.g4
	javac -cp ".:antlr-4.7.2-complete.jar" *.java

run:
	java -cp ".:antlr-4.7.2-complete.jar:binAntlr" AntlrParser

clean:
	rm -rf *.class *.interp *.tokens Expr*java
