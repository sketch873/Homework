package Comparatori;

import java.util.Comparator;

import ClaseDeBaza.Factura;

/*
 * Clasa comparator pentru totalul cu taxe al facturilor.
 */
public class CompFactura implements Comparator<Factura> {

	@Override
	public int compare(Factura o1, Factura o2) {
		return (int) (o1.getTotalCuTaxe() - o2.getTotalCuTaxe());
	}
	
}