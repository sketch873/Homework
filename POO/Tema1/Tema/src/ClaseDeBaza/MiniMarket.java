package ClaseDeBaza;

import java.util.ArrayList;
import java.util.Vector;

/*
 * Clasa MiniMarket, descrisa in enuntul temei.
 */
public class MiniMarket extends Magazin{
	private ArrayList<String> tari;

	public MiniMarket(String nume, Vector<Factura> facturi) {
		super.nume = nume;
		super.facturi = facturi;
		tip = "MiniMarket";
		tari = new ArrayList<String>();
	}
	
	/*
	 * Realizeaza o multime a tarilor produselor
	 * dintr-un anumit magazin.
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
	
	
	@Override
	public double calculScutiriTaxe() {
		makeTari();
		for(String i : tari) {
			if(getTotalCuTaxe() / 2 < getTotalTaraCuTaxe(i)) {
				setScutit(true);
				return getTotalCuTaxe() * 0.9;
			}
		}
		
		return getTotalCuTaxe();
	}
	
	public String toString() {
		String result = "";
		result += super.nume + ":\n";
		for(Factura i : super.facturi) {
			result += i + "\n";
		}
		return result;
	}
	
}
