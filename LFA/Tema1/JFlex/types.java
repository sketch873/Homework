abstract interface Expression {

	String show();
	String interpret();
	void test();
}

class While implements Expression {
	Expression e1;
	Expression e2;

	public While(Expression e1, Expression e2) {
		super();
		this.e1 = e1;
		this.e2 = e2;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<WhileNode> while\n";

		print += e1.show() + e2.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		while(e1.interpret().equals("true"))
			e2.interpret();
		return null;
	}

	@Override
	public void test() {
		e1.test();
		e2.test();

	}

}

class Sequence implements Expression {
	Expression e1;
	Expression e2;

	public Sequence(Expression e1, Expression e2) {
		super();
		this.e1 = e1;
		this.e2 = e2;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<SequenceNode>\n";

		print += e1.show() + e2.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		e1.interpret();
		e2.interpret();
		return null;
	}

	@Override
	public void test() {
		e1.test();
		e2.test();
	}

}

class Main implements Expression {
	Expression e;

	public Main(Expression e) {
		super();
		this.e = e;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<MainNode>\n";

		print += e.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		e.interpret();
		return null;
	}

	@Override
	public void test() {
		e.test();
	}

}

class AndN implements Expression {
	Expression e1;
	Expression e2;

	public AndN(Expression e1, Expression e2) {
		super();
		this.e1 = e1;
		this.e2 = e2;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<AndNode> &&\n";

		print += e1.show() + e2.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		if(e1.interpret().equals("true") && e2.interpret().equals("true"))
			return "true";
		return "false";
	}

	@Override
	public void test() {
		e1.test();
		e2.test();
	}

}

class Greater implements Expression {
	Expression e1;
	Expression e2;

	public Greater(Expression e1, Expression e2) {
		super();
		this.e1 = e1;
		this.e2 = e2;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<GreaterNode> >\n";

		print += e1.show() + e2.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		int v1 = Integer.parseInt(e1.interpret());
		int v2 = Integer.parseInt(e2.interpret());

		if(v1 > v2)
			return "true";

		return "false";
	}

	@Override
	public void test() {
		e1.test();
		e2.test();
	}

}

class Plus implements Expression {
	Expression e1;
	Expression e2;

	public Plus(Expression e1, Expression e2) {
		super();
		this.e1 = e1;
		this.e2 = e2;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<PlusNode> +\n";

		print += e1.show() + e2.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		int result = Integer.parseInt(e1.interpret());
		try {
			result += Integer.parseInt(e2.interpret());
		} catch(NumberFormatException e) {

		}

		return "" + result;
	}

	@Override
	public void test() {
		e1.test();
		e2.test();
	}

}

class Division implements Expression {
	Expression e1;
	Expression e2;
	int line;

	public Division(Expression e1, Expression e2, int line) {
		super();
		this.e1 = e1;
		this.e2 = e2;
		this.line = line;
	}

	public Division(Expression e1, Expression e2) {
		super();
		this.e1 = e1;
		this.e2 = e2;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<DivNode> /\n";

		print += e1.show() + e2.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		int result = Integer.parseInt(e1.interpret());

		try {
			result /= Integer.parseInt(e2.interpret());
		} catch(ArithmeticException e) {
			if(Parser.error.equals("none"))
				Parser.error = "DivideByZero " + (line+1) + "\n";
		}

		return "" + result;
	}

	@Override
	public void test() {
		e1.test();
		e2.test();

	}

}

class Block implements Expression {
	Expression e = new Null();

	public Block(Expression e) {
		super();
		this.e = e;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<BlockNode> {}\n";

		if(e instanceof Null) {
			return build;
		}

		print += e.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		if((e instanceof Null) == false)
			e.interpret();
		return null;
	}

	@Override
	public void test() {
		e.test();
	}

}

class NotB implements Expression {
	Expression e;

	public NotB(Expression e) {
		super();
		this.e = e;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<NotNode> !\n";

		print += e.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		if(e.interpret() == "true")
			return "false";
		return "true";
	}

	@Override
	public void test() {
		e.test();
	}

}

class IfN implements Expression {
	Expression condB;
	Expression thenB;
	Expression elseB;

	public IfN(Expression e1, Expression e2, Expression e3) {
		super();
		this.condB = e1;
		this.thenB = e2;
		this.elseB = e3;
	}
	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<IfNode> if\n";

		print += condB.show() + thenB.show() + elseB.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		if(condB.interpret().equals("true"))
			thenB.interpret();
		else
			elseB.interpret();
		return null;
	}
	@Override
	public void test() {
		condB.test();
		thenB.test();
		elseB.test();
	}

}

class Bracket implements Expression {
	Expression e;

	public Bracket(Expression e) {
		super();
		this.e = e;
	}
	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<BracketNode> ()\n";

		print += e.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		return e.interpret();
	}
	@Override
	public void test() {
		e.test();
	}

}

class Assign implements Expression {
	Expression e1;
	Expression e2;

	public Assign(Expression e1, Expression e2) {
		super();
		this.e1 = e1;
		this.e2 = e2;
	}

	@Override
	public String show() {
		String build = "";
		String print = "";

		build += "<AssignmentNode> =\n";

		print += e1.show() + e2.show();

		build += Parser.addNewline(print);

		return build;
	}

	@Override
	public String interpret() {
		Parser.values.put(((Variable)e1).getVar(), e2.interpret());
		return null;
	}

	@Override
	public void test() {
		e2.test();
	}

}

class Number implements Expression {
	String number;

	public Number(String number) {
		super();
		this.number = number;
	}

	@Override
	public String show() {
		return "<IntNode> " + number + "\n";
	}

	@Override
	public String interpret() {
		return number;
	}

	@Override
	public void test() {
	}

}

class Bool implements Expression {
	String value;

	public Bool(String value) {
		super();
		this.value = value;
	}

	@Override
	public String show() {
		return "<BoolNode> " + value + "\n";
	}

	@Override
	public String interpret() {
		return value;
	}

	@Override
	public void test() {
	}

}

class Variable implements Expression {
	String var;
	int line;

	public Variable(String var, int line) {
		super();
		this.var = var;
		this.line = line;
	}

	@Override
	public String show() {
		return "<VariableNode> " + var + "\n";
	}

	@Override
	public String interpret() {
		if(Parser.values.containsKey(var) == false && Parser.error.equals("none")) {
			Parser.error = "UnassignedVar " + (line+1) + "\n";
			return "1";
		}

		return Parser.values.get(var);
	}

	public String getVar() {

		if(Parser.variables.contains(var) == false && Parser.error.equals("none")) {
			Parser.error = "UnassignedVar " + (line+1) + "\n";
			return var;
		}
		return var;
	}

	@Override
	public void test() {
		if(Parser.variables.contains(var) == false)
			Parser.error = "UnassignedVar " + (line+1) + "\n";
		}
	}
