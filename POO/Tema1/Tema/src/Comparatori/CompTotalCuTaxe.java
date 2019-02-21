package Comparatori;
import java.util.Comparator;

import ClaseDeBaza.Magazin;

/*
 * Clasa comparator pentru sortarea magazinelor in functie
 * de totalul cu taxe.
 */
public class CompTotalCuTaxe implements Comparator<Magazin>{

	@Override
	public int compare(Magazin arg0, Magazin arg1) {
		return (int) (arg1.getTotalCuTaxe() - arg0.getTotalCuTaxe());
	}

}
