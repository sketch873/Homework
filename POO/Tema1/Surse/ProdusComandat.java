package ClaseDeBaza;

/*
 * Clasa ProdusComandat, descrisa in enuntul temei.
 */
public class ProdusComandat {
	private Produs produs;
	private Double taxa;
	private int cantitate;
	
	public void setProdus(Produs p) {
		produs = p;
	}
	
	public Produs getProdus() {
		return produs;
	}
	
	public void setTaxa(Double t) {
		taxa = t;
	}
	
	public Double getTaxa() {
		return taxa;
	}
	
	public void setCantitate(int c) {
		cantitate = c;
	}
	
	public int getCantitate() {
		return cantitate;
	}
	
	public String toString() {
		String result = "";
		result += produs + " " + cantitate + "  Taxa: " + taxa;
		return result;
	}
}
