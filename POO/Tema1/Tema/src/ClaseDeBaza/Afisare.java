package ClaseDeBaza;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import Comparatori.CompFactura;
import Comparatori.CompMagazin;
import DesignPatterns.Gestiune;

/*
 * Clasa pentru afisarea in fisier pe baza
 * fisierului de output ales (in costructor).
 */
public class Afisare {
	private BufferedWriter bw;
	
	public Afisare(String s) throws IOException {
		bw = new BufferedWriter(new FileWriter(s));
	}
	public void makeAfisare() throws IOException {
		
		ArrayList<String> tipuri = new ArrayList<String>();
		tipuri.add("MiniMarket");
		tipuri.add("MediumMarket");
		tipuri.add("HyperMarket");
		Collections.sort(Gestiune.getInstance().magazine, new CompMagazin());
		Collections.sort(Gestiune.getInstance().tari);
		for(String i : tipuri) {
			bw.write(i);
			bw.newLine();
			for(Magazin j : Gestiune.getInstance().magazine) {
				if(j.tip.equals(i)) {
					bw.write(j.nume);
					bw.newLine();
					bw.newLine();
					bw.write("Total " + j.getTotalFaraTaxe() + " " + j.getTotalCuTaxe() + " " + j.calculScutiriTaxe());
					bw.newLine();
					bw.newLine();
					
					bw.write("Tara");
					bw.newLine();
					for(String tara : Gestiune.getInstance().tari) {
						bw.write(tara + " " + j.getTotalTaraFaraTaxe(tara) + " " + j.getTotalTaraCuTaxe(tara) + " " + j.getTotalTaraCuTaxeScutite(tara));
						bw.newLine();
					}
					bw.newLine();
					
					Collections.sort(j.facturi, new CompFactura());
					for(Factura factura : j.facturi) {
						bw.write(factura.denumire);
						bw.newLine();
						bw.newLine();

						bw.write("Total " + factura.getTotalFaraTaxe() + " " + factura.getTotalCuTaxe());
						bw.newLine();
						bw.newLine();
						bw.write("Tara");
						bw.newLine();
						for(String tara : Gestiune.getInstance().tari) {
							bw.write(tara + " " + factura.getTotalTaraFaraTaxe(tara) + " " + factura.getTotalTaraCuTaxe(tara));
							bw.newLine();
						}
					bw.newLine();
					}
				}
			}
		}
		
		bw.close();
	}	

}




