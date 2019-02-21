package Comparatori;
import java.util.Comparator;
import ClaseDeBaza.Produs;

/*
 * Clasa care ajuta la compararea produselor in functie de
 * tara. Folosit pentru afisarea produselor, task2.
 */
public class SortareTara implements Comparator<Produs>{
	private String tipSortare;
	
	public SortareTara(String s) {
		tipSortare = s;
	}
	
	@Override
	public int compare(Produs o1, Produs o2) {
		if(tipSortare.equals("crescator")) {
			return o1.getTaraOrigine().compareTo(o2.getTaraOrigine());
		} else {
			return o2.getTaraOrigine().compareTo(o1.getTaraOrigine());
		}
	}

}
