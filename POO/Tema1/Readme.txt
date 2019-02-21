Martin Andrei-Costin
322CC

	Ierarhia de fisiere se afla in mai multe pachete, in functie de intrebuintarea
	claselor:
			-> ClaseDeBaza : cele descrise in enuntul temei
			-> DesignPatterns : clasele care realizeaza Factory si Singleton Pattern
			-> Parser : clasele care realizeaza parsari (de fisiere sau de multimi)
			-> Comparatori : comparatorii folositi in sortarea elementelor prelucrate si afisate
			-> InterfataGrafica : clasa care realizeaza o interfata grafica

	Metoda parsare:
	* Ordinea parsarii este produse.txt -> facturi.txt -> taxe.txt
	* ParserTaxe are o instanta a ParserFacturi, care are o instanta a ParserProduse.
	* Outputul unui parser este dat ca input pentru urmatorul (alaturi de fisierul aferent),
	astfel ca se realizeaza o multime de produse, dupa care o multime de facturi aranjate pe
	magazine, peste care se adauga un set de taxe.
	
	Interfata grafica:
	OBS: Pentru a facilita de butoanele din interfata trebuie :
 		1. Sa fie adaugate cele trei fisiere pentru a debloca
		scrierea in out.txt.
		2. Scris in out.txt pentru a debloca administrarea 
		produselor si afisarea statisticilor.

	Interfata grafica este una minimala, cu putine butoane aranjate usor de inteles,
	cu o singura adaugare pentru fiecare rulare.