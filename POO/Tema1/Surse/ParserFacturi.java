package Parser;

import Parser.ParserFacturi;
import ClaseDeBaza.Factura;
import ClaseDeBaza.Magazin;
import ClaseDeBaza.Produs;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.Vector;

import ClaseDeBaza.ProdusComandat;
import DesignPatterns.FactoryMagazin;

/*
 * ParseFacturi e o clasa care se foloseste de ParseProdus.
 * Dupa ce s-a realizat multimea produselor posibile se
 * realizeaza facturile pentru magazinele din facturi.txt.
 */
public class ParserFacturi {
	private String read;
	private Vector<Factura> facturi;
	private ArrayList<Magazin> magazine;
	private BufferedReader br;
	private StringTokenizer stMagazin, stFactura;
	private Factura factura;
	private ProdusComandat pc;
	private ParserProdus pp;
	
	public ParserFacturi(String s1, String s2) throws IOException {
		magazine = new ArrayList<Magazin>();
		pp = new ParserProdus(s2);
		pp.makeArray();
		br = new BufferedReader(new FileReader(s1));
	}
	
	/*
	 * Parseaza facturi.txt realizand o
	 * multime de magazine.
	 */
	public void makeParse() throws IOException {
		read = br.readLine();
		while(read != null) {
			if(read.contains("Magazin")) {
				
				//separ pentru randul respectiv
				stMagazin = new StringTokenizer(read, ":");
				stMagazin.nextToken();
				
				//curatam facturile
				facturi = new Vector<Factura>();
				
				//aici newline
				br.readLine();
				magazine.add(FactoryMagazin.buildMagazin(stMagazin.nextToken(), stMagazin.nextToken(), facturi));
				
			} else if(read.contains("Factura")) {
			
				factura = new Factura();
				factura.denumire = read;
				parseFactura();
				facturi.add(factura);

			}
			read = br.readLine();
		}
	}
	
	/*
	 * Pentru fiecare magazin parseaza cate o factura.
	 */
	private void parseFactura() throws IOException {

		br.readLine();
		read = br.readLine();
		while(read != null &&  read.length() != 0) {
			stFactura = new StringTokenizer(read);
			pc = new ProdusComandat();
			String denumire = stFactura.nextToken();
			
			String tara = stFactura.nextToken();
			for(Produs i : pp.getProdus()) {
				if(i.getDenumire().equals(denumire) == true && i.getTaraOrigine().equals(tara) == true) {
					pc.setProdus(i);
					pc.setCantitate(Integer.parseInt(stFactura.nextToken()));
					pc.setTaxa(1.0);
					break;
				}
			}
			factura.produseComandate.add(pc);
			read = br.readLine();
		}
	}
	
	public String toString() {
		String result = "";
		
		for(Magazin i : magazine) {
			result += i.nume + " ";
			for(Factura j : i.facturi) {
				result += j.denumire + " ";
			}
			result += "\n";
		}
		
		return result;
	}
	
	public ArrayList<Magazin> getMagazine() {
		return magazine;
	}
	
	public ArrayList<Produs> getProduse() {
		return pp.getProdus();
	}
}
