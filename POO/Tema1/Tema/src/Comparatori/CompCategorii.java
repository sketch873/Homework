package Comparatori;

import java.util.Comparator;

import ClaseDeBaza.Factura;
import ClaseDeBaza.Magazin;
import ClaseDeBaza.ProdusComandat;

/*
 * Clasa este un comparator pentru magazine in functie de
 * suma tuturor produselor dintr-o categorie (ultima bulina
 * din task2)
 */
public class CompCategorii implements Comparator<Magazin>{
	private String categorie;
	
	public CompCategorii(String c) {
		categorie = c;
	}
	
	@Override
	public int compare(Magazin arg0, Magazin arg1) {
		double mag0 = 0;
		double mag1 = 0;
		
		for(Factura i : arg0.facturi) {
			for(ProdusComandat j : i.produseComandate) {
				if(j.getProdus().getCategorie().equals(categorie)) {
					mag0 += j.getProdus().getPret() * j.getTaxa() / 100;
				}
			}
		}
		
		for(Factura i : arg1.facturi) {
			for(ProdusComandat j : i.produseComandate) {
				if(j.getProdus().getCategorie().equals(categorie)) {
					mag1 += j.getProdus().getPret() * j.getTaxa() / 100;
				}
			}
		}
		
		return (int) (mag1 - mag0);
	}

}
