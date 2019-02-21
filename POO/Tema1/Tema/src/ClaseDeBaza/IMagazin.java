package ClaseDeBaza;

/*
 * Interfata IMagazin, descrisa in enuntul temei.
 */
public interface IMagazin {
	double getTotalFaraTaxe();
	double getTotalCuTaxe();
	double getTaxe();
	double getTotalTaraFaraTaxe(String tara);
	double getTotalTaraCuTaxe(String tara);
	double getTaxeTara(String tara);
	double getTotalTaraCuTaxeScutite(String tara);
	double calculScutiriTaxe();
}
