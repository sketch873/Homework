package Parser;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.TreeMap;

import ClaseDeBaza.Factura;
import ClaseDeBaza.Magazin;
import ClaseDeBaza.ProdusComandat;

/*
 * Clasa ParsesTaxe se foloseste de ParserFacturi.
 * Dupa ce s-au realizat facturile/magazinele se introduc
 * taxele aferente fecarui produs
 */
public class ParserTaxe {
	private String read;
	private StringTokenizer st;
	private BufferedReader br;
	private ArrayList<String> tari;
	private ArrayList<Integer> taxe;
	private ArrayList<Magazin> magazine;
	private ParserFacturi pf;
	
	public ParserTaxe(String s1, String s2, String s3) throws IOException {
		magazine = new ArrayList<Magazin>();
		taxe = new ArrayList<Integer>();
		tari = new ArrayList<String>();
		br = new BufferedReader(new FileReader(s1));
		pf = new ParserFacturi(s2, s3);
	}
	
	private void initializari() throws IOException {
		pf.makeParse();
		magazine = pf.getMagazine();
	}
	

	/*
	 * Parseaza fiecare linie adaugand pentru
	 * fiecare produs taxa (ProdusComandat.taxa).
	 * 
	 */
	public void makeParse() throws IOException {
		initializari();
		
		st = new StringTokenizer(br.readLine());
		st.nextToken();
		while(st.hasMoreTokens()) {
			tari.add(st.nextToken());
		}
			
		read = br.readLine();
		while(read != null) {
			st = new StringTokenizer(read);
			String categorie = st.nextToken();
			
			for(int i = 0; i < tari.size(); i++) {
				double val = Double.parseDouble(st.nextToken());
				for(Magazin j : magazine) {
					for(Factura k : j.facturi) {
						for(ProdusComandat l : k.produseComandate) {
							if(l.getProdus().getCategorie().equals(categorie) == true && 
							l.getProdus().getTaraOrigine().equals(tari.get(i)) == true) {
								l.setTaxa(100.0 + val);
							}
						}
					}
				}
			}
			
			
			read = br.readLine();
		}
	}
	
	public ArrayList<String> getTari() {
		return tari;
	}
	
	/*
	 * Realizeaza dictionarul (categorie, procent)
	 * pentru fiecare tara.
	 */
	public TreeMap<String, Integer> makeTree(String tara) {
		TreeMap<String, Integer> result = new TreeMap<String, Integer>();
		for(Magazin i : magazine) {
			for(Factura j : i.facturi) {
				for(ProdusComandat k : j.produseComandate) {
					if(k.getProdus().getTaraOrigine().equals(tara) && result.containsKey(k.getProdus().getCategorie()) == false) {
						result.put(k.getProdus().getCategorie(), (int) (k.getTaxa() - 100));
					}
				}
			}
		}
		
		
		return result;
	}
	
	public ArrayList<Magazin> getMagazine() {
		return magazine;
	}
	
}
