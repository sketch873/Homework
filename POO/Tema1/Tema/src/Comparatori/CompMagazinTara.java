package Comparatori;

import java.util.Comparator;

import ClaseDeBaza.Magazin;

/*
 * Clasa comparator in functie de totalul fara taxa
 * pentru o tara al uneimagazinelor.
 */
public class CompMagazinTara implements Comparator<Magazin>{
	private String tara;
	public CompMagazinTara(String t) {
		tara = t;
	}
	@Override
	public int compare(Magazin arg0, Magazin arg1) {
		return (int) (arg1.getTotalTaraCuTaxe(tara) - arg0.getTotalTaraCuTaxe(tara));
	}

}
