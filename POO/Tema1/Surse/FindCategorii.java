package Parser;
import java.util.ArrayList;

import ClaseDeBaza.Produs;

/*
 * Clasa are rolul de a determina toate categoriile
 * de produse dintr-o multime de produse.
 */
public class FindCategorii {
	private ArrayList<Produs> produse;
	private ArrayList<String> categorii;
	
	public FindCategorii(ArrayList<Produs> p) {
		produse = p;
		categorii = new ArrayList<String>();
		for(Produs i : produse) {
			if(categorii.contains(i.getCategorie()) == false) {
				categorii.add(i.getCategorie());
			}
		}
	}
	
	public ArrayList<String> getCategorii() {
		return categorii;
	}
}
