package ClaseDeBaza;

import java.util.ArrayList;
import java.util.Vector;

/*
 * Clasa HyperMarket, descrisa in enuntul temei.
 */
public class HyperMarket extends Magazin {
	private ArrayList<String> tari;

	public HyperMarket(String nume, Vector<Factura> facturi) {
		super.nume = nume;
		super.facturi = facturi;
		tip = "HyperMarket";
		tari = new ArrayList<String>();
	}
	
	public void makeTari() {
		for(Factura i : facturi) {
			for(ProdusComandat j : i.produseComandate) {
				if(tari.contains(j.getProdus().getTaraOrigine()) == false) {
					tari.add(j.getProdus().getTaraOrigine());
				}
			}
		}
	}
	
	

	@Override
	public double calculScutiriTaxe() {
		for(Factura i : facturi) {
			if(i.getTotalCuTaxe() > getTotalCuTaxe() / 10) {
				setScutit(true);
				return 0.99 * getTotalCuTaxe();
			}
		}
		
		return getTotalCuTaxe();
	}
	
	public String toString() {
		String result = "";
		result += super.nume + ":\n";
		for(Factura i : super.facturi) {
			result += "\t" + i + "\n";
		}
		return result;
	}

}