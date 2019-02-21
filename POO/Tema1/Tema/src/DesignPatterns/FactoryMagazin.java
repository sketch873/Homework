package DesignPatterns;

import java.util.Vector;

import ClaseDeBaza.Factura;
import ClaseDeBaza.HyperMarket;
import ClaseDeBaza.Magazin;
import ClaseDeBaza.MediumMarket;
import ClaseDeBaza.MiniMarket;

/*
 * Clasa care implementeaza Factory Pattern.
 */
public class FactoryMagazin{
	
	public static Magazin buildMagazin(String tip, String denumire, Vector<Factura> facturi) {
		if(tip.equals("MiniMarket")) {
			return 	new MiniMarket(denumire, facturi);
		} else if(tip.equals("MediumMarket")) {
			return new MediumMarket(denumire, facturi);
		} else if(tip.equals("HyperMarket")) {
			return new HyperMarket(denumire, facturi);
		}
		return null;
	}

}
