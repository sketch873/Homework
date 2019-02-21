package Comparatori;
import java.util.Comparator;

import ClaseDeBaza.Produs;

/*
 * Clasa care ajuta la compararea produselor in functie de
 * denumire. Folosit pentru afisarea produselor, task2.
 */
public class SortareDenumire implements Comparator<Produs>{
	private String tipSortare;
	
	public SortareDenumire(String s) {
		tipSortare = s;
	}
	@Override
	public int compare(Produs o1, Produs o2) {
		if(tipSortare.equals("crescator")) {
			return o1.getDenumire().compareTo(o2.getDenumire());
		} else {
			return o2.getDenumire().compareTo(o1.getDenumire());
		}
	}

}
