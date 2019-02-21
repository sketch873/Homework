package ClaseDeBaza;
import java.util.Vector;

/*
 * Clasa Factura, descrisa in enuntul temei.
 */
public class Factura {
	public Vector<ProdusComandat> produseComandate;
	public String denumire;
	
	public Factura() {
		produseComandate = new Vector<ProdusComandat>();
	}
	
	public double getTotalCuTaxe(){
		double result = 0;
		for(ProdusComandat i : produseComandate) {
			result += i.getTaxa()/100 * (i.getCantitate() * i.getProdus().getPret());
		}
		return result;
	}
	
	public double getTotalFaraTaxe() {
		double result = 0;
		for(ProdusComandat i : produseComandate) {
			result += i.getCantitate() * i.getProdus().getPret();
		}
		return result;
	}
	
	public double getTaxe() {
		double result = 0;
		for(ProdusComandat i : produseComandate) {
			result += i.getTaxa();
		}
		
		return result;
	}
	
	public double getTotalTaraCuTaxe(String name) {
		double result = 0;
		for(ProdusComandat i : produseComandate) {
			if(i.getProdus().getTaraOrigine().equals(name)) {
				result += i.getTaxa()/100 * (i.getCantitate() * i.getProdus().getPret());
			}
		}
		
		return result;
	}
	
	public double getTotalTaraFaraTaxe(String name) {
		double result = 0;
		for(ProdusComandat i : produseComandate) {
			if(i.getProdus().getTaraOrigine().equals(name)) {
				result += i.getCantitate() * i.getProdus().getPret();
			}
		}
		
		return result;
	}
	
	public double getTaxeTara(String name) {
		double result = 0;
		for(ProdusComandat i : produseComandate) {
			if(i.getProdus().getTaraOrigine().equals(name)) {
				result += i.getTaxa();
			}
		}
		return result;
	}
	
	public String toString() {
		String result = "";
		for(ProdusComandat i : produseComandate) {
			result += i + "\n";
		}
		return result;
	}
}
