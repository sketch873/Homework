package DesignPatterns;

import java.io.IOException;
import java.util.ArrayList;
import java.util.TreeMap;

import ClaseDeBaza.Magazin;
import ClaseDeBaza.Produs;
import Parser.ParserFacturi;
import Parser.ParserTaxe;

/*
 * Clasa gestiune, implementata cu Singleton Pattern.
 */
public class Gestiune {
	private static Gestiune gestiune = new Gestiune();
	
	public static ArrayList<Produs> produse;
	public static ArrayList<Magazin> magazine;
	public static TreeMap<String, TreeMap<String, Integer>> dictionar;
	public static ArrayList<String> tari;
	
	private static String addProduse;
	private static String addFacturi;
	private static String addTaxe;
	
	private Gestiune() {
		produse = new ArrayList<Produs>();
		magazine = new ArrayList<Magazin>();
		dictionar = new TreeMap<String, TreeMap<String, Integer>>();
	}
	
	public static void setProduse(String s) {
		addProduse = s;
	}
	
	public static void setFacturi(String s) {
		addFacturi = s;
	}
	
	public static void setTaxe(String s) {
		addTaxe = s;
	}
	
	public static void makeInit() throws IOException {
		ParserTaxe pt = new ParserTaxe(addTaxe, addFacturi, addProduse);
		pt.makeParse();
		magazine = pt.getMagazine();
		tari = pt.getTari();
		
		ParserFacturi pf = new ParserFacturi(addFacturi, addProduse);
		pf.makeParse();
		produse = pf.getProduse();
		
		for(String i : tari) {
			dictionar.put(i, pt.makeTree(i));
		}
	}
	
	public static Gestiune getInstance() {
		return	gestiune;
	}
	
	public String toString() {
		String result = "";
		
		result += dictionar.toString();
		
		return result;
	}
	
}
