package ClaseDeBaza;

import java.util.ArrayList;
import java.util.Vector;

/*
 * Clasa MediumMarket, descrisa in enuntul temei.
 */
public class MediumMarket extends Magazin {
	private ArrayList<String> categorii;
	
	public MediumMarket(String nume, Vector<Factura> facturi) {
		super.nume = nume;
		super.facturi = facturi;
		tip = "MediumMarket";
		categorii = new ArrayList<String>();
	}
	
	/*
	 * Realizeaza o multime a categoriilor produselor
	 * existente in acel magazin.
	 */
	public void makeCategorii() {
		for(Factura i : facturi) {
			for(ProdusComandat j : i.produseComandate) {
				if(categorii.contains(j.getProdus().getCategorie()) == false) {
					categorii.add(j.getProdus().getCategorie());
				}
			}
		}
	}

	@Override
	public double calculScutiriTaxe() {
		double rezultatPartial;

		makeCategorii();
		for(String i : categorii) {
			rezultatPartial = 0;
			for(Factura j : facturi) {
				for(ProdusComandat k : j.produseComandate) {
					if(k.getProdus().getCategorie().equals(i)) {
						rezultatPartial += k.getTaxa() / 100.0 * (k.getProdus().getPret() * k.getCantitate());
					}
				}
			}
			
			if(rezultatPartial > getTotalCuTaxe() / 2) {
				setScutit(true);
				return 0.95 * getTotalCuTaxe();
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
