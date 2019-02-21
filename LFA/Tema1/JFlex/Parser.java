import java.io.*;
import java.util.*;

public class Parser {

	public static ArrayList<String> variables;
	public static Hashtable<String, String> values = new Hashtable<String, String> ();
	public static String error = "none";

	public static String addNewline(String print) {
		Scanner scanner = new Scanner(print);
		String build = "";
		while (scanner.hasNextLine()) {
			String line = scanner.nextLine();
			build += "\t" + line + "\n";
		}
		scanner.close();
		return build;
	}

	public static void main (String[] args) throws IOException {
		LexIMP l = new LexIMP(new FileReader("input"));

		l.yylex();

		Expression e = new Main(l.stack.peek());
		variables = l.variables;

		FileWriter tw = new FileWriter("arbore");
		String print = e.show();
		tw.write(print);
		tw.close();

		FileWriter vw = new FileWriter("output");
		print = "";

		//Interpretez rezultatul
		e.interpret();

		// Daca nu contine erori atunci il testez
		if(error.equals("none"))
			e.test();

		// Daca tot nu are erori afisez variabilele
		if(error.equals("none")) {
			for(String var : variables) {
				print += var + "=" + values.get(var) + "\n";
			}
			vw.write(print);
		} else {
			vw.write(error);
		}
		vw.close();

	}
}
