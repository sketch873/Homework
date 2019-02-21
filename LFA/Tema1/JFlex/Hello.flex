import java.util.*;

%%

%class LexIMP
%line
%int
%{
    /* Stiva de instructiuni */
    Stack<Expression> stack = new Stack<Expression>();

    /* Lista de variabile declarate */
    ArrayList<String> variables = new ArrayList<String>();

    /* Pentru a sti cand s-a terminat declararea variabilelor */
    boolean decl = false;

    /* Realizeaza un nod de tip Assign */
    Expression makeAssign() {
        Expression e1 = stack.pop();
        stack.pop();
        Expression e3 = stack.pop();

        return new Assign(e3, e1);
    }

    /* Realizeaza un nod de tip Sequence */
    Expression makeSequence(Sequence s, Expression e) {
        Sequence aux = s;

        while(aux.e2 instanceof Sequence) {
          aux = (Sequence)aux.e2;
        }
        Sequence newOne = new Sequence(aux.e2, e);
        aux.e2 = (Expression) newOne;

        return (Expression) s;
    }

    /* Gaseste ultimul element din arbore pentru suma */
    Expression lastElementSum(Expression e) {
      if(e instanceof AndN) {
        AndN a = (AndN) e;
        if(a.e2 instanceof AndN || a.e2 instanceof Greater)
          return lastElementSum(a.e2);
      }
      if(e instanceof Greater) {
        Greater a = (Greater) e;
        if(a.e2 instanceof AndN || a.e2 instanceof Greater)
          return lastElementSum(a.e2);
      }

      return e;
    }

    /* Face un nod de tip Plus */
    Expression makeSum() {
      Expression e1 = stack.pop();
      stack.pop();
      Expression e3 = stack.pop();

      if(e3 instanceof AndN || e3 instanceof Greater) {
        Expression last = lastElementSum(e3);

        /*  Adaug nodul cat mai jos in arbore  */
        if(last instanceof AndN) {
          AndN a = (AndN) last;
          if(a.e2 instanceof Variable || a.e2 instanceof Number) {
            Plus p = new Plus(a.e2, e1);
            a.e2 = p;
          }
        } else if (last instanceof Greater) {
          Greater a = (Greater) last;
          if(a.e2 instanceof Variable || a.e2 instanceof Number) {
            Plus p = new Plus(a.e2, e1);
            a.e2 = p;
          }
        }
        return e3;
      } else {
        return new Plus(e3, e1);
      }
    }

    /* Gaseste ultimul element din arbore pentru diviziune */
    Expression lastElementDiv(Expression e) {
      if(e instanceof AndN) {
        AndN a = (AndN) e;
        if(a.e2 instanceof AndN || a.e2 instanceof Greater || a.e2 instanceof Plus)
          return lastElementDiv(a.e2);
      }
      if(e instanceof Greater) {
        Greater a = (Greater) e;
        if(a.e2 instanceof AndN || a.e2 instanceof Greater || a.e2 instanceof Plus)
          return lastElementDiv(a.e2);
      }

      if(e instanceof Plus) {
        Plus a = (Plus) e;
        if(a.e2 instanceof AndN || a.e2 instanceof Greater || a.e2 instanceof Plus)
          return lastElementDiv(a.e2);
      }

      return e;
    }

    /* Face un nod de tip Division */
    Expression makeDiv() {
      Expression e1 = stack.pop();
      Div e2 = (Div) stack.pop();
      Expression e3 = stack.pop();

      if(e3 instanceof AndN || e3 instanceof Greater || e3 instanceof Plus) {
        Expression last = lastElementDiv(e3);

        if(last instanceof AndN) {
          AndN a = (AndN) last;
          if(a.e2 instanceof Variable || a.e2 instanceof Number) {
            Division d = new Division(a.e2, e1);
            a.e2 = d;
          }
        } else if (last instanceof Greater) {
          Greater a = (Greater) last;
          if(a.e2 instanceof Variable || a.e2 instanceof Number) {
            Division d = new Division(a.e2, e1);
            a.e2 = d;
          }
        } else if(last instanceof Plus) {
          Plus p3 = (Plus) last;
          Division newOne = new Division(p3.e2, e1, e2.line);
          p3.e2 = newOne;

        }
        return e3;
      }

      return new Division(e3, e1, e2.line);
    }

    /* Gaseste ultimul element din arbore */
    Expression lastElementGt(Expression e) {
      if(e instanceof AndN) {
        AndN a = (AndN) e;
        if(a.e2 instanceof AndN)
          return lastElementGt(a.e2);
      }

      return e;
    }

    /* Realizeaza un nod de tip Greater */
    Expression makeGt() {
      Expression e1 = stack.pop();
      stack.pop();
      Expression e3 = stack.pop();

      if(e3 instanceof AndN) {
        Expression last = lastElementGt(e3);
        AndN a = (AndN) last;
        Greater g = new Greater(a.e2, e1);
        a.e2 = g;
        return e3;
      } else {
        return new Greater(e3, e1);
      }


    }

    /* Realizeaza un nod de tip And */
    Expression makeAnd() {
      Expression e1 = stack.pop();
      stack.pop();
      Expression e3 = stack.pop();

      return new AndN(e3, e1);

    }

    /* Realizeaza un nod de tip Sequence */
    void makeSequence() {
      Expression e1 = stack.pop();
      Expression e2 = stack.pop();
      if(e2 instanceof Assign || e2 instanceof IfN || e2 instanceof While) {
        stack.push(new Sequence(e2, e1));
      } else if (e2 instanceof Sequence) {
        Sequence s = (Sequence) e2;
        Sequence aux = (Sequence) e2;
        while(s.e2 instanceof Sequence) {
          s = (Sequence) s.e2;
        }
        Sequence newOne = new Sequence(s.e2, e1);
        s.e2 = newOne;
        stack.push(aux);
      } else {
        stack.push(e2);
        stack.push(e1);
      }
    }

    /* Realizeaza un nod de tip While */
    void makeWhile() {
      if(stack.size() > 2) {
        Expression e1 = stack.pop();
        Expression e2 = stack.pop();
        if(stack.peek() instanceof Wh && e2 instanceof Bracket && e1 instanceof Block) {
          stack.pop();
          stack.push(new While(e2, e1));
          makeSequence();
        } else {
          stack.push(e2);
          stack.push(e1);
        }
      }
    }

    /* Realizeaza un nod de tip If */
    void makeIf() {
      if(stack.size() > 3) {
        Expression e1 = stack.pop();
        Expression e2 = stack.pop();
        Expression e3 = stack.pop();
        if(stack.peek() instanceof If && e3 instanceof Bracket && e2 instanceof Block && e1 instanceof Block) {
          stack.pop();
          stack.push(new IfN(e3, e2, e1));
          makeSequence();
        } else {
          stack.push(e3);
          stack.push(e2);
          stack.push(e1);
        }
      }
    }

%}

integer = [1-9][0-9]* | 0
string = [a-z]+
boolean = "true" | "false"

open_bracket = "("
close_bracket = ")"
open_block = "{"
close_block = "}"

start = "int"
and = "&&"
not = "!"
sum = "+"
div = "/"
gt = ">"

if = "if"
else = "else"
while = "while"
assign = "="
end_assign = ";"

%%
{start} {decl = true;}

{sum} {stack.push(new Sum());}
{div} {stack.push(new Div(yyline));}
{gt} {stack.push(new Gt());}
{and} {stack.push(new And());}
{if} {stack.push(new If());}
{not} {stack.push(new Not());}
{while} {stack.push(new Wh());}
{boolean} {stack.push(new Bool(yytext()));}
{else} {}

/* Pune pe stiva un flag de egal */
{assign} {stack.push(new Equal());}

{string} {
    if(decl == false) {
      if(!stack.isEmpty() && stack.peek() instanceof Sum) {
        stack.push(new Variable(yytext(), yyline));
        Expression e = makeSum();
        stack.push(e);
      } else if(!stack.isEmpty() && stack.peek() instanceof Div) {
        stack.push(new Variable(yytext(), yyline));
        Expression e = makeDiv();
        stack.push(e);
      } else if(!stack.isEmpty() && stack.peek() instanceof Gt) {
        stack.push(new Variable(yytext(), yyline));
        Expression e = makeGt();
        stack.push(e);
      } else if (!stack.isEmpty() && stack.peek() instanceof And) {
        stack.push(new Variable(yytext(), yyline));
        Expression e = makeAnd();
        stack.push(e);
      } else {
        stack.push(new Variable(yytext(), yyline));
      }
    } else {
        variables.add(yytext());
    }
}
{integer} {
    if(!stack.isEmpty() && stack.peek() instanceof Sum) {
      stack.push(new Number(yytext()));
      Expression e = makeSum();
      stack.push(e);
    } else if(!stack.isEmpty() && stack.peek() instanceof Div) {
      stack.push(new Number(yytext()));
      Expression e = makeDiv();
      stack.push(e);
    } else if(!stack.isEmpty() && stack.peek() instanceof Gt) {
      stack.push(new Number(yytext()));
      Expression e = makeGt();
      stack.push(e);
    } else if(!stack.isEmpty() && stack.peek() instanceof And) {
      stack.push(new Number(yytext()));
      Expression e = makeAnd();
      stack.push(e);
    } else {
      stack.push(new Number(yytext()));
    }

}

/* Cazul parantezelor */
{open_bracket} {stack.push(new Par());}
{close_bracket} {
  System.out.println(stack);
  Expression e = stack.pop();

  /*  Daca sunt mai multe elemente pe stiva atunci le unesc pentru a fi doar unul */
  /*  'Par' e auxiliar pentru a sti unde s-a deschis paranteza */
  while((stack.peek() instanceof Par) == false) {
    if(stack.peek() instanceof Sum) {
      stack.push(e);
      Expression x = makeSum();
      stack.push(x);
    } else if(stack.peek() instanceof Div) {
      stack.push(e);
      Expression x = makeDiv();
      stack.push(x);
    } else if(stack.peek() instanceof Gt) {
      stack.push(e);
      Expression x = makeGt();
      stack.push(x);
    } else if(stack.peek() instanceof And) {
      stack.push(e);
      Expression x = makeAnd();
      stack.push(x);
    }
    e = stack.pop();
  }

  /* Realizez blocul de tip paranteza */
  Bracket b = new Bracket(e);
  stack.pop();

  /* In cazul in care este si un nod de tip Not */
  if(stack.peek() instanceof Not) {
    stack.pop();
    stack.push(new NotB(b));
  } else {
    stack.push(b);
  }
}

/* Cazul unui bloc de instructiuni */
{open_block} {stack.push(new Acol());}
{close_block} {
  //caz de baza, block gol
  Expression e = stack.pop();
  if(e instanceof Acol) {
    stack.push(new Block(new Null()));
  } else {

    /*  Pana gasesc auxiliarul 'Acol' pe stiva */
    while((stack.peek() instanceof Acol) == false) {
      stack.push(e);
      makeSequence();
      e = stack.pop();
    }
    stack.pop();
    Block b = new Block(e);
    stack.push(b);
  }

  /* Daca sunt o conditie si un bloc */
  makeWhile();

  /* Daca sunt o conditie si doua blocuri */
  makeIf();
}

/* Cand se gaseste caracterul ';' */
{end_assign} {
    if(decl == true) {
        decl = false;
    } else {

      /* Cazul in care se poate realiza un nod cu acea asignare */
      if(stack.size() > 2) {
        Expression aux = stack.pop();
        if(!stack.isEmpty() && stack.peek() instanceof Sum) {
          stack.push(aux);
          Expression x = makeSum();
          stack.push(x);
        } else if(!stack.isEmpty() && stack.peek() instanceof Div) {
          stack.push(aux);
          Expression x = makeDiv();
          stack.push(x);
        } else if(!stack.isEmpty() && stack.peek() instanceof Gt) {
          stack.push(aux);
          Expression x = makeGt();
          stack.push(x);
        } else if(!stack.isEmpty() && stack.peek() instanceof And) {
          stack.push(aux);
          Expression x = makeAnd();
          stack.push(x);
        } else {
          stack.push(aux);
        }
    }

      /* Se realizeaza nodul de tip Assign */
      Expression e = makeAssign();
      if(!stack.isEmpty() && stack.peek() instanceof Assign) {
          Expression x = stack.pop();
          stack.push(new Sequence(x, e));
      } else if (!stack.isEmpty() && stack.peek() instanceof Sequence) {
        Expression x = makeSequence((Sequence)stack.pop(), e);
        stack.push(x);
      } else {
        stack.push(e);
      }
    }
}

\n {}
. {}
