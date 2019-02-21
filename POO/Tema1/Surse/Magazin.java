package ClaseDeBaza;

import java.util.ArrayList;
import java.util.Vector;

/*
 * Clasa abstracta Magazin, descrisa in enuntul temei.
 */
public abstract class Magazin implements IMagazin{
	public String nume;
	public Vector<Factura> facturi;
	public ArrayList<String> tari;
	public String tip;
	private boolean scutit;
	
	/*
	 * realizeaza o multime de tari; toate
	 * tarile de care apartin produsele din
	 * acel magazin.
	 */
	public void makeTari() {
		for(Factura i : facturi) {
			for(ProdusComandat j : i.produseComandate) {
				if(tari.contains(j.getProdus().getTaraOrigine()) == false) {
					tari.add(j.getProdus().getTaraOrigine());
				}
			}
		}
	}
	
	public double getTotalFaraTaxe() {
		double result = 0;
		for(Factura i : facturi) {
			result += i.getTotalFaraTaxe();
		}
		return result;
	}
	
	public double getTotalCuTaxe() {
		double result = 0;
		for(Factura i : facturi) {
			result += i.getTotalCuTaxe();
		}
		return result;
	}
	
	public double getTaxe() {
		double result = 0;
		for(Factura i : facturi) {
			result += i.getTaxe();
		}
		return result;
		
	}
	public double getTotalTaraFaraTaxe(String tara) {
		double result = 0;
		for(Factura i : facturi) {
			result += i.getTotalTaraFaraTaxe(tara);
		}
		return result;
	}
	
	public double getTotalTaraCuTaxe(String tara) {
		double result = 0;
		for(Factura i : facturi) {
			result += i.getTotalTaraCuTaxe(tara);
		}
		return result;
	}
	
	public double getTaxeTara(String tara) {
		double result = 0;
		for(Factura i : facturi) {
			result += i.getTaxeTara(tara);
		}
		return result;
	}
	
	/*
	 * Metoda care seteaza, in urma calculelor pentru
	 * fiecare tip de magazin, daca este scutit de
	 * un procent din taxe.
	 */
	public void setScutit(boolean val) {
		scutit = val;
	}
	
	public double getTotalTaraCuTaxeScutite(String tara) {
		if(tip.equals("MiniMarket") && scutit == true) {
			return getTotalTaraCuTaxe(tara) * 0.9;
		} else if(tip.equals("MediumMarket") && scutit == true) {
			return getTotalTaraCuTaxe(tara) * 0.95;
		} else if(tip.equals("HyperMarket") && scutit == true) {
			return getTotalTaraCuTaxe(tara) * 0.99;	
		}
		
		return getTotalTaraCuTaxe(tara);
	}
}
