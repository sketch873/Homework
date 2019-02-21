build: build-flex


build-flex: make-flex ./JFlex/types.java ./JFlex/Parser.java ./JFlex/aux.java
	javac ./JFlex/Parser.java ./JFlex/LexIMP.java ./JFlex/types.java ./JFlex/aux.java -d binFlex

make-flex: ./JFlex/Hello.flex
	jflex JFlex/Hello.flex

run:
	java -classpath binFlex Parser

clean:
	rm -rf binFlex binAntlr JFlex/*IMP*
