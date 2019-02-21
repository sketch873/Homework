package InterfataGrafica;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.StringTokenizer;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import ClaseDeBaza.Afisare;
import ClaseDeBaza.Factura;
import ClaseDeBaza.Magazin;
import ClaseDeBaza.Produs;
import Comparatori.CompCategorii;
import Comparatori.CompMagazinTara;
import Comparatori.CompTotalCuTaxe;
import Comparatori.SortareDenumire;
import Comparatori.SortareTara;
import DesignPatterns.Gestiune;
import Parser.FindCategorii;
import Parser.FindFacturi;
import Parser.ParserProdus;

public class PaginaPrincipala extends JFrame {
	//meniu principal
	private JButton goAdaugare;
	private JButton administrareProduse;
	private JButton afisareStatistici;
	private JButton exit;
	
	//meniu statistici
	private JButton returnPrincipal;
	
	//meniu adaugare
	private JButton setProduse;
	private JButton setFacturi;
	private JButton setTaxe;
	private JButton makeOut;
	private JButton back;
	
	//variabile numar apasari
	private boolean clicksDenumire = false;
	private boolean clicksTara = false;
	
	//meniuri
	private JPanel meniuPrincipal;
	private JPanel meniuAdaugare;
	private JPanel meniuStatistici;
	private JPanel meniuLogare;
	
	//edit produse
	private ArrayList<Produs> produse;
	private ArrayList<String> tari;
	private ArrayList<String> liniiProduse;
	
	//deschiderea fisierelor
	private JFileChooser jFileProduse;
	private JFileChooser jFileChooser;
	
/*
 * Setarea frame-ului principal si pornirea interfetei grafice.
 */
	public PaginaPrincipala() {
		jFileChooser = new JFileChooser();
		jFileProduse = new JFileChooser();
		
		setTitle("Gestiune Magazine");
		setSize(300, 500);
		
		meniuStatistici = new JPanel();
		meniuStatistici.setBounds(0, 0, 300, 500);
		meniuStatistici.setLayout(null);
		returnPrincipal = new JButton("Back");
		meniuStatistici.add(returnPrincipal);
		returnPrincipal.setBounds(85, 400, 100, 25);
		returnPrincipal.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
					setContentPane(meniuPrincipal);
			}

		});
		
//realizarea panelurilor afisare ulterior		
		makePrincipalPanel();
		makeAdaugarePanel();
		makeLogarePanel();

//realizarea butoanelor de pe paneluri
		makeActionPrincipal();
		makeActionAdaugare();		
		
//primul panel afisat
		setLayout(null);
		setContentPane(meniuLogare);
		show();

	}
	
/*
 * Realizarea panelului cu statistici.
 */
	private void makeStatisticiPanel() {
		

		JTextArea totalCuTaxe = new JTextArea();
		JTextArea totalCuTari = new JTextArea();
		JTextArea totalCuCategorii = new JTextArea();
		JTextArea totalFactura = new JTextArea();

		//Afisarea se face in Scroll pane pentru a avea spatiu
		JScrollPane p1 = new JScrollPane(totalCuTaxe);
		JScrollPane p2 = new JScrollPane(totalCuTari);
		JScrollPane p3 = new JScrollPane(totalCuCategorii);
		JScrollPane p4 = new JScrollPane(totalFactura);
		
		p1.setBounds(0, 0, 280, 100);
		totalCuTaxe.setEditable(false);
		totalCuTaxe.setBackground(getBackground());
		p2.setBounds(0, 100, 280, 100);
		totalCuTari.setEditable(false);
		totalCuTari.setBackground(getBackground());
		
		p3.setBounds(0, 200, 280, 100);
		totalCuCategorii.setEditable(false);
		totalCuCategorii.setBackground(getBackground());

		p4.setBounds(0, 300, 280, 100);
		totalFactura.setEditable(false);
		totalFactura.setBackground(getBackground());
		
		//prima bulina
		Collections.sort(Gestiune.getInstance().magazine, new CompTotalCuTaxe());
		String result = "Cele mai mari vanzari cu taxe: ";
		result += Gestiune.getInstance().magazine.get(0).nume + "\n";
		result += "Total fara taxe: " + Gestiune.getInstance().magazine.get(0).getTotalFaraTaxe() + "\n";
		result += "Total cu taxe: " + Gestiune.getInstance().magazine.get(0).getTotalCuTaxe() + "\n";
		result += "Total cu taxe scutite: " + Gestiune.getInstance().magazine.get(0).calculScutiriTaxe() + "\n";
		totalCuTaxe.setText(result);
		
		//a doua bulina
		result = "";
		for(String i : tari) {
			Collections.sort(Gestiune.getInstance().magazine, new CompMagazinTara(i));
			result += "Cele mai mari vanzari pentru ";
			result += i + " : " + Gestiune.getInstance().magazine.get(0).nume + "\n";
			result += "Total fara taxe: " + Gestiune.getInstance().magazine.get(0).getTotalFaraTaxe() + "\n";
			result += "Total cu taxe: " + Gestiune.getInstance().magazine.get(0).getTotalCuTaxe() + "\n";
			result += "Total cu taxe scutite: " + Gestiune.getInstance().magazine.get(0).calculScutiriTaxe() + "\n";
		}
		totalCuTari.setText(result);
		
		//a treia bulina
		result = "";
		ArrayList<String> categorii = new FindCategorii(produse).getCategorii();
		for(String i : categorii) {
			Collections.sort(Gestiune.getInstance().magazine, new CompCategorii(i));
			result += "Cele mai mari vanzari pentru ";
			result += i + " : " + Gestiune.getInstance().magazine.get(0).nume + "\n";
			result += "Total fara taxe: " + Gestiune.getInstance().magazine.get(0).getTotalFaraTaxe() + "\n";
			result += "Total cu taxe: " + Gestiune.getInstance().magazine.get(0).getTotalCuTaxe() + "\n";
			result += "Total cu taxe scutite: " + Gestiune.getInstance().magazine.get(0).calculScutiriTaxe() + "\n";
		}
		totalCuCategorii.setText(result);
		
		//a patra bulina
		result = "";
		Factura f = new FindFacturi(Gestiune.getInstance().magazine).getFactura();
		Magazin m = new FindFacturi(Gestiune.getInstance().magazine).getMagazin();
		result += "Factura cu suma totala : " + f.denumire + "\n";
		result += "Magazinul cu factura : " + m.nume + "\n";
		totalFactura.setText(result);
		
		meniuStatistici.add(p1);
		meniuStatistici.add(p2);
		meniuStatistici.add(p3);
		meniuStatistici.add(p4);
		
		
		
	}

/*
 * Realizarea panelului de logare cu user:student parola:student
*/
	private void makeLogarePanel() {
		meniuLogare = new JPanel();
		meniuLogare.setBounds(0, 0, 300, 500);
		meniuLogare.setLayout(null);
		
		JTextField textField = new JTextField();
		textField.setBounds(45, 100, 200, 25);
		JPasswordField passwordField = new JPasswordField();
		passwordField.setBounds(45, 200, 200, 25);
		
		JTextField infoUser = new JTextField("User: (student)");
		JTextField infoParola = new JTextField("Password: (student)");
		infoUser.setEditable(false);
		infoParola.setEditable(false);
		infoUser.setBounds(45, 75, 200, 25);
		infoParola.setBounds(45, 170, 200, 25);
		
		JButton login = new JButton("Login");
		login.setBounds(50, 250, 100, 25);
		login.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if(textField.getText().equals("student") == true && passwordField.getText().equals("student") == true) {
					setContentPane(meniuPrincipal);
				}
			}

		});
		meniuLogare.add(infoParola);
		meniuLogare.add(infoUser);
		meniuLogare.add(login);
		meniuLogare.add(textField);
		meniuLogare.add(passwordField);
		
		add(meniuLogare);
	}
	
/*
 * Realizarea panelului principal
 */
	private void makePrincipalPanel() {
		meniuPrincipal = new JPanel();
		meniuPrincipal.setLayout(null);
		meniuPrincipal.setBounds(0, 0, 300, 500);
		
		goAdaugare = new JButton("Adauga date");
		administrareProduse = new JButton("Administrare");
		afisareStatistici = new JButton("Statistici");
		exit = new JButton("Exit");
		
		administrareProduse.setEnabled(false);
		afisareStatistici.setEnabled(false);

		goAdaugare.setBounds(50, 100, 150, 25);
		administrareProduse.setBounds(50, 150, 150, 25);
		afisareStatistici.setBounds(50, 200, 150, 25);
		exit.setBounds(50, 250, 150, 25);
		
		meniuPrincipal.add(goAdaugare);
		meniuPrincipal.add(administrareProduse);
		meniuPrincipal.add(afisareStatistici);
		meniuPrincipal.add(exit);
		
		add(meniuPrincipal);
		
	}

/*
 * Realizarea meniului de administrare al produselor
 */
	private void makeFrameAdministrare() throws IOException {
		JFrame administrare = new JFrame("Administrare Produse");
		
		JButton afisareProduse = new JButton("Afisare");
		JButton sortareDenumire = new JButton("Sortare denumire");
		JButton sortareTara = new JButton("Sortare tara");
		JButton adaugareProdus = new JButton("Adaugare");
		JButton stergereProdus = new JButton("Stergere");
		JButton editareProdus = new JButton("Editare");
		JTextArea scrieProduse = new JTextArea();
		JScrollPane scrollPane = new JScrollPane(scrieProduse);
		
		administrare.setBounds(300, 0, 500, 500);
		administrare.setLayout(null);
		
		afisareProduse.setBounds(85, 10, 100, 25);
		sortareDenumire.setBounds(30, 45, 200, 25);
		sortareTara.setBounds(30, 80, 200, 25);
		adaugareProdus.setBounds(270, 10, 100, 25);
		stergereProdus.setBounds(270, 45, 100, 25);
		editareProdus.setBounds(270, 80, 100, 25);
		scrollPane.setBounds(10, 120, 460, 330);
		scrieProduse.setEditable(false);
		
		administrare.add(afisareProduse);
		administrare.add(sortareDenumire);
		administrare.add(sortareTara);
		administrare.add(stergereProdus);
		administrare.add(adaugareProdus);
		administrare.add(editareProdus);
		administrare.add(scrollPane);

		//generare setari de baza pentru afisarile ulterioare
		
		makeActionAdministrare(scrieProduse, afisareProduse, sortareDenumire, sortareTara, adaugareProdus, stergereProdus, editareProdus);
		
		administrare.show();
		
	}

/*
 * Setarea butoanelor de pe meniul de administrare
 */
	private void makeActionAdministrare(JTextArea sP, JButton aP, JButton sD, JButton sT, JButton addP, JButton delP, JButton eP) {
		aP.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				String result = "";
				for(Produs i : produse) {
					result += i + "\n";
				}
				sP.setText(result);
			}
		});
		
		sD.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if(clicksDenumire == false) {
					Collections.sort(produse, new SortareDenumire("crescator"));
					String result = "";
					for(Produs i : produse) {
						result += i + "\n";
					}
					sP.setText(result);
					clicksDenumire = true;
				} else {
					Collections.sort(produse, new SortareDenumire("descrescator"));
					String result = "";
					for(Produs i : produse) {
						result += i + "\n";
					}
					sP.setText(result);
					clicksDenumire = false;
				}
			}

		});
		
		sT.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if(clicksTara == false) {
					Collections.sort(produse, new SortareTara("crescator"));
					String result = "";
					for(Produs i : produse) {
						result += i + "\n";
					}
					sP.setText(result);
					clicksTara = true;
				} else {
					Collections.sort(produse, new SortareTara("descrescator"));
					String result = "";
					for(Produs i : produse) {
						result += i + "\n";
					}
					sP.setText(result);
					clicksTara = false;
				}
			}

		});
		
		addP.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				JFrame meniuAdaugareProdus = new JFrame("Adauga produs");
				JButton adauga = new JButton("Adauga");
				JTextField denumire = new JTextField();
				JTextField categorie = new JTextField();
				JTextField taraOrigine = new JTextField();
				JTextField pret = new JTextField();
				JTextField mesaj = new JTextField();
				JTextField eroare = new JTextField();
				
				meniuAdaugareProdus.setLocation(600, 400);
				meniuAdaugareProdus.setSize(500, 200);
				meniuAdaugareProdus.setLayout(null);
				
				eroare.setBounds(5, 5, 475, 25);
				eroare.setText("Adauga un produs nou!");
				eroare.setEditable(false);
				mesaj.setText("Produs                              Categorie                       Tara                                  Pret");
				mesaj.setBounds(5, 30, 475, 25);
				mesaj.setEditable(false);
				denumire.setBounds(5, 55, 125, 25);
				categorie.setBounds(130, 55, 125, 25);
				taraOrigine.setBounds(255, 55, 125, 25);
				pret.setBounds(380, 55, 100, 25);
				adauga.setBounds(175, 90, 100, 25);
				
				meniuAdaugareProdus.add(eroare);
				meniuAdaugareProdus.add(mesaj);
				meniuAdaugareProdus.add(denumire);
				meniuAdaugareProdus.add(categorie);
				meniuAdaugareProdus.add(taraOrigine);
				meniuAdaugareProdus.add(pret);
				meniuAdaugareProdus.add(adauga);
				
				adauga.addActionListener(new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent e) {
						for(Produs i : produse) {
							if(i.getDenumire().equals(denumire.getText()) == true) {
								eroare.setText("Produsul exista deja");
								return;
							}
						}
						//bagam produsul in lista
						Produs p = new Produs();
						p.setCategorie(categorie.getText());
						p.setDenumire(denumire.getText());
						p.setTaraOrigine(taraOrigine.getText());
						p.setPret(Double.parseDouble(pret.getText()));
						produse.add(p);
						
						String result = denumire.getText() + " " + categorie.getText();
						//actualizam produse.txt
						for(String i : tari) {
							if(i.equals(taraOrigine.getText()) == true) {
								result += " " + pret.getText();
							} else {
								result += " 0";
							}
						}
						eroare.setText("Produsul a fost adaugat");
						liniiProduse.add(result);
						try {
							ParserProdus pp = new ParserProdus(jFileProduse.getSelectedFile().toString());
							pp.scrieFisier(liniiProduse);
						} catch (IOException e1) {
							e1.printStackTrace();
						}
						result = "";
						for(Produs i : produse) {
							result += i + "\n";
						}
						sP.setText(result);
					}

				});
				
				
				meniuAdaugareProdus.show();
			}

		});
	
		delP.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				JFrame meniuStergereProdus = new JFrame("Sterge produs");
				JButton sterge = new JButton("Sterge");
				JTextField eroare = new JTextField();
				JTextField info = new JTextField();
				JTextField denumireDeSters = new JTextField();
				
				meniuStergereProdus.setLocation(600, 400);
				meniuStergereProdus.setSize(500, 200);
				meniuStergereProdus.setLayout(null);
				
				eroare.setBounds(0, 20, 500, 25);
				info.setBounds(50, 70, 200, 25);
				denumireDeSters.setBounds(250, 70, 200, 25);
				sterge.setBounds(200, 105, 100, 25);
				
				eroare.setEditable(false);
				info.setEditable(false);
				info.setText("Denumirea produsului");
				
				
				
				meniuStergereProdus.add(eroare);
				meniuStergereProdus.add(info);
				meniuStergereProdus.add(sterge);
				meniuStergereProdus.add(denumireDeSters);
				

				
				sterge.addActionListener(new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent e) {
						//TODO aici e treaba nu sterge din seria de 3 (3 tari) elementul cu IT
						boolean aiciE = true;
						//for e la misto ca probleme, rezolvat TODO
						while(aiciE == true) {
							aiciE = false;
							for(Produs i : produse) {
									if(i.getDenumire().equals(denumireDeSters.getText()) == true) {
										aiciE = true;
									}
							}
							for(int i = 0; i < produse.size(); i++) {
								if(produse.get(i).getDenumire().equals(denumireDeSters.getText()) == true) {
									produse.remove(i);
								}
							}
						
							for(int i = 0; i < liniiProduse.size(); i++) {
								if(liniiProduse.get(i).contains(denumireDeSters.getText()) == true) {
									liniiProduse.remove(i);
								}
							}
						}
										
						try {
							ParserProdus pp = new ParserProdus(jFileProduse.getSelectedFile().toString());
							pp.scrieFisier(liniiProduse);
						} catch (IOException e1) {
							e1.printStackTrace();
						}
						String result = "";
						for(Produs i : produse) {
							result += i + "\n";
						}
						sP.setText(result);
					}

				});
				
				
				
				meniuStergereProdus.show();
			}

		});
		
		eP.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				JFrame meniuEditareProdus = new JFrame("Editeaza produs");
				JTextField info = new JTextField();
				JTextField denumireDeEditat = new JTextField();
				JButton editeaza = new JButton("Editeaza");
				
				info.setBounds(10, 30, 460, 25);
				denumireDeEditat.setBounds(10, 55, 460, 25);
				editeaza.setBounds(200, 80, 100, 25);
				
				meniuEditareProdus.setLocation(600, 400);
				meniuEditareProdus.setSize(500, 200);
				meniuEditareProdus.setLayout(null);
				
				meniuEditareProdus.add(info);
				meniuEditareProdus.add(denumireDeEditat);
				meniuEditareProdus.add(editeaza);

				info.setEditable(false);
				String result = "Introduceti: <denumire> <categorie>";
				for(String i : tari) {
					result += " <" + i + ">";
				}
				info.setText(result);
				
				editeaza.addActionListener(new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent e) {
						StringTokenizer st = new StringTokenizer(denumireDeEditat.getText());
						String denumire = st.nextToken();
						
						for(String i : liniiProduse) {
							if(i.contains(denumire) == true) {
								liniiProduse.set(liniiProduse.indexOf(i), denumireDeEditat.getText());
							}
						}
						
						
						try {
							ParserProdus pp = new ParserProdus(jFileProduse.getSelectedFile().toString());
							pp.scrieFisier(liniiProduse);
							pp.makeArray();
							produse = pp.getProdus();
							
							
						} catch (IOException e1) {
							e1.printStackTrace();
						}
						String result = "";
						for(Produs i : produse) {
							result += i + "\n";
						}
						sP.setText(result);	
						
					}
					
					
				});
				
				meniuEditareProdus.show();
			}
		});
	}
	
/*
 * Realizarea meniului de adaugare al unui produs
 */
	private void makeAdaugarePanel() {
		meniuAdaugare = new JPanel();
		meniuAdaugare.setLayout(null);
		meniuAdaugare.setBounds(0, 0, 300, 500);
		
		//introducere butoane
				setProduse = new JButton("Add produse.txt");
				setFacturi = new JButton("Add facturi.txt");
				setTaxe = new JButton("Add taxe.txt");
				makeOut = new JButton("Make out.txt");
				back = new JButton("Back");
				
				setProduse.setBounds(50, 100, 150, 25);
				setFacturi.setBounds(50, 150, 150, 25);
				setTaxe.setBounds(50, 200, 150, 25);
				makeOut.setBounds(50, 250, 150, 25);
				makeOut.setEnabled(false);
				back.setBounds(50, 300, 150, 25);

				meniuAdaugare.add(setProduse);
				meniuAdaugare.add(setFacturi);
				meniuAdaugare.add(setTaxe);
				meniuAdaugare.add(makeOut);
				meniuAdaugare.add(back);
				
				add(meniuAdaugare);
	}
	
/*
 * Implementarea functionalitatii butoanelor de pe fereastra principala.
 */
	private void makeActionPrincipal() {
		goAdaugare.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				setContentPane(meniuAdaugare);
			}

		});
		
		administrareProduse.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					makeFrameAdministrare();
				} catch (IOException e1) {
					e1.printStackTrace();
				}
			}

		});
		
		afisareStatistici.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				setContentPane(meniuStatistici);
				
			}

		});
		
		exit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}

		});
	}
	
/*
 * Implementarea functionalitatii butoanelor de pe fereastra de adaugare.
 */
	private void makeActionAdaugare() {
		setProduse.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				jFileProduse.showOpenDialog(getParent());
				Gestiune.setProduse(jFileProduse.getSelectedFile().toString());
				setProduse.setEnabled(false);
				if(setTaxe.isEnabled() == false && setProduse.isEnabled() == false && setFacturi.isEnabled() == false) {
					makeOut.setEnabled(true);
				}
			}

		});
		setFacturi.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				jFileChooser.showOpenDialog(getParent());
				Gestiune.setFacturi(jFileChooser.getSelectedFile().toString());
//				System.out.println(jFileChooser.getSelectedFile());
				setFacturi.setEnabled(false);
				if(setTaxe.isEnabled() == false && setProduse.isEnabled() == false && setFacturi.isEnabled() == false) {
					makeOut.setEnabled(true);
				}
			}

		});
		setTaxe.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				jFileChooser.showOpenDialog(getParent());
				Gestiune.setTaxe(jFileChooser.getSelectedFile().toString());
//				System.out.println(jFileChooser.getSelectedFile().toString());
				setTaxe.setEnabled(false);
				
				if(setTaxe.isEnabled() == false && setProduse.isEnabled() == false && setFacturi.isEnabled() == false) {
					makeOut.setEnabled(true);
				}
			}

		});
		
		makeOut.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					jFileChooser.showOpenDialog(getParent());
					Gestiune.makeInit();
					Afisare afisare = new Afisare(jFileChooser.getSelectedFile().toString());
					afisare.makeAfisare();
					ParserProdus pp = new ParserProdus(jFileProduse.getSelectedFile().toString());
					pp.makeArray();
					produse = pp.getProdus();
					tari = pp.getTari();
					liniiProduse = pp.citireFisier();
					makeStatisticiPanel();
					afisareStatistici.setEnabled(true);
					administrareProduse.setEnabled(true);
				} catch (IOException e1) {
					e1.printStackTrace();
				}
			}

		});
		
		back.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				setContentPane(meniuPrincipal);
			}

		});
	}
	
	public static void main(String[] args) throws IOException {
		PaginaPrincipala paginaPrincipala = new PaginaPrincipala();
			
	}

}