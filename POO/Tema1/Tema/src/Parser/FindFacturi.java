package Parser;

import java.util.ArrayList;

import ClaseDeBaza.Factura;
import ClaseDeBaza.Magazin;

/*
 * Gaseste factura cu totalul fara taxe cel mai mare.
 */
public class FindFacturi {
	private ArrayList<Magazin> magazine;
	private Factura factura;
	private Magazin magazin;
	
	public FindFacturi(ArrayList<Magazin> m) {
		magazine = m;
		factura = magazine.get(0).facturi.get(0);
		magazin = magazine.get(0);
		
		for(Magazin i : magazine) {
			for(Factura j : i.facturi) {
				if(j.getTotalFaraTaxe() > factura.getTotalFaraTaxe()) {
					factura = j;
					magazin = i;
				}
			}
		}
	}
	
	public Factura getFactura() {
		return factura;
	}
	
	public Magazin getMagazin() {
		return magazin;
	}
}
