build: build-flex

build-flex: make-flex types.java Parser.java aux.java
	javac Parser.java LexIMP.java types.java aux.java

make-flex: Hello.flex
	jflex Hello.flex

run:
	java Parser

clean:
	rm -rf *.class *IMP*
