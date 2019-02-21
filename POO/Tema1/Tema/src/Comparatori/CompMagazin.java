package Comparatori;

import java.util.Comparator;
import ClaseDeBaza.Magazin;

/*
 * Clasa comparator al totalului fara taxe pentru un magazin.
 * (Pentru toate tarile la un loc)
 */
public class CompMagazin implements Comparator<Magazin> {

	@Override
	public int compare(Magazin o1, Magazin o2) {
		return (int) (o1.getTotalFaraTaxe() - o2.getTotalFaraTaxe());
	}
	
}