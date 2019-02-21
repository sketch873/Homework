package Parser;


import ClaseDeBaza.Produs;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class ParserProdus {
	private ArrayList<Produs> produse;
	private ArrayList<String> tari;
	private BufferedReader br;
	private String adresa;
	
	public ParserProdus(String s1) throws FileNotFoundException {
		adresa = s1;
		produse = new ArrayList<Produs>();
		tari = new ArrayList<String>();
		br = new BufferedReader(new FileReader(s1));
	}
	
	/*
	 * Metoda care citeste produse.txt
	 * si parseaza inputul pentru clasa Produs.
	 */
	public void makeArray() throws IOException {
		StringTokenizer st;
		String name;
		Produs produs;
		String denumire;
		String categorie;
		name = br.readLine();
		st = new StringTokenizer(name);
		st.nextToken();
		st.nextToken();
		while(st.hasMoreTokens()) {
			tari.add(st.nextToken());
		}
		
		name = br.readLine();
		while(name != null) {
			st = new StringTokenizer(name);
			denumire = st.nextToken();
			categorie = st.nextToken();
			for(String t : tari) {
				produs = new Produs();
				produs.setCategorie(categorie);
				produs.setDenumire(denumire);
				produs.setTaraOrigine(t);
				produs.setPret(Double.parseDouble(st.nextToken()));
				produse.add(produs);
			}
			
			name = br.readLine();
		}
		br.close();
	}
	
	/*
	 * Metoda citeste produse.txt si returneaza
	 * multimea liniilor din fisier.
	 */
	public ArrayList<String> citireFisier() throws IOException {
		ArrayList<String> randuri = new ArrayList<String>();
		br = new BufferedReader(new FileReader(adresa));
		String read;
		read = br.readLine();
		while(read != null) {
			randuri.add(read);
			read = br.readLine();
		}
		br.close();
		return randuri;
	}
	
	/*
	 * Metoda scrie in produse.txt o multime de linii.
	 */
	public void scrieFisier(ArrayList<String> date) throws IOException {
		FileWriter fw = new FileWriter(adresa);
		for(String i : date) {
			fw.write(i);
			fw.write(13);
		}
		fw.close();
	}
	
	public ArrayList<Produs> getProdus() {
		return produse;
	}
	
	public ArrayList<String> getTari() {
		return tari;
	}
	
	public String toString() {
		String result = "";
		for(Produs i : produse) {
			result += i.getCategorie() + i.getDenumire() + i.getTaraOrigine() + i.getPret() + "\n";
		}
		return result;
	}
}
