package ClaseDeBaza;

/*
 * Clasa Produs, descrisa in enuntul temei.
 */
public class Produs {
	private String denumire;
	private String categorie;
	private String taraOrigine;
	private Double pret;
	
	public void setDenumire(String d) {
		denumire = d;
	}
	
	public String getDenumire() {
		return denumire;
	}
	
	public void setCategorie(String c) {
		categorie = c;
	}
	
	public String getCategorie() {
		return categorie;
	}
	
	public void setTaraOrigine(String to) {
		taraOrigine = to;
	}
	
	public String getTaraOrigine() {
		return taraOrigine;
	}
	
	public void setPret(Double p) {
		pret = p;
	}
	
	public Double getPret() {
		return pret;
	}
	
	public String toString() {
		return denumire + " " + categorie + " " + taraOrigine + " " + pret;
	}
	
}
