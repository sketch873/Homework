import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;

import java.io.*;

public class AntlrParser {
    ParseTree tree;
    String prog = "ExprParser$ProgContext";
    String seq = "ExprParser$SeqContext";
    String assign = "ExprParser$AssignContext";

    String aExpr = "ExprParser$AExprContext";
    String bExpr = "ExprParser$BExprContext";

    String iff = "ExprParser$IffContext";
    String whilee = "ExprParser$WhileeContext";


    String num = "ExprParser$NumContext";
    String bool = "ExprParser$BoolContext";
    String string = "ExprParser$StringContext";

    String and = "ExprParser$AndContext";
    String greater = "ExprParser$GreaterContext";
    String div = "ExprParser$DivContext";
    String sum = "ExprParser$SumContext";
    String not = "ExprParser$NotContext";



    String cond = "ExprParser$CondContext";
    String bracket = "ExprParser$BracketContext";
    String block = "ExprParser$BlockContext";

    public AntlrParser(ParseTree tree) {
        this.tree = tree;
    }

    void make(ParseTree parseTree, String print, FileWriter fw) throws IOException{
        /*  AFISARE  */
        if(parseTree.getClass().getName().equals(prog)){
            fw.write(print + "<MainNode>\n");
            make(parseTree.getChild(1), print+"\t", fw);

        } else if(parseTree.getClass().getName().equals(seq)){
            fw.write(print + "<SequenceNode>\n");
            make(parseTree.getChild(0), print+"\t", fw);
            make(parseTree.getChild(1), print+"\t", fw);

        } else if(parseTree.getClass().getName().equals(assign)){
            fw.write(print + "<AssignmentNode> =\n");
            make(parseTree.getChild(0), print + "\t", fw);
            make(parseTree.getChild(2), print + "\t", fw);

        } else if(parseTree.getClass().getName().equals(aExpr)) {
            if(parseTree.getChildCount() == 3) {
                make(parseTree.getChild(1), print + "", fw);
                make(parseTree.getChild(0), print + "\t", fw);
                make(parseTree.getChild(2), print + "\t", fw);
            } else {
                make(parseTree.getChild(0), print + "", fw);
            }

        } else if(parseTree.getClass().getName().equals(bracket)) {
            fw.write(print + "<BracketNode> ()\n");
            make(parseTree.getChild(1), print + "\t", fw);

        } else if(parseTree.getClass().getName().equals(bExpr)) {
            if(parseTree.getChildCount() == 1) {
                make(parseTree.getChild(0), print + "", fw);
            } else {
                make(parseTree.getChild(1), print + "", fw);
                make(parseTree.getChild(0), print + "\t", fw);
                make(parseTree.getChild(2), print + "\t", fw);
            }
        } else if(parseTree.getClass().getName().equals(iff)) {
            fw.write(print + "<IfNode> if\n");
            make(parseTree.getChild(1), print + "\t", fw);
            make(parseTree.getChild(2), print + "\t", fw);
            make(parseTree.getChild(4), print + "\t", fw);
        } else if(parseTree.getClass().getName().equals(cond)) {
            fw.write(print + "<BracketNode> ()\n");
            if(parseTree.getChildCount() == 1) {
                make(parseTree.getChild(0), print + "\t", fw);
            } else {
                make(parseTree.getChild(1), print + "\t", fw);
            }
        } else if(parseTree.getClass().getName().equals(whilee)) {
            fw.write(print + "<WhileNode> while\n");

            make(parseTree.getChild(1), print + "\t", fw);
            make(parseTree.getChild(2), print + "\t", fw);
        } else if(parseTree.getClass().getName().equals(block)) {
            fw.write(print + "<BlockNode> {}\n");
            if(parseTree.getChildCount() == 3) {
                make(parseTree.getChild(1), print + "\t", fw);
            }

        } else if(parseTree.getClass().getName().equals(bool)) {
            fw.write(print + "<BoolNode> " + parseTree.getText() + "\n");
        } else if(parseTree.getClass().getName().equals(string)){
            fw.write(print + "<VariableNode> " + parseTree.getText() + "\n");
        } else if(parseTree.getClass().getName().equals(num)) {
            fw.write(print + "<IntNode> " + parseTree.getText() + "\n");
        } else if(parseTree.getClass().getName().equals(sum)){
            fw.write(print + "<PlusNode> +\n");
        } else if(parseTree.getClass().getName().equals(div)) {
            fw.write(print + "<DivNode> /\n");
        } else if(parseTree.getClass().getName().equals(greater)) {
            fw.write(print + "<GreaterNode> >\n");
        } else if(parseTree.getClass().getName().equals(and)) {
            fw.write(print + "<AndNode> &&\n");
        } else if(parseTree.getClass().getName().equals(not)) {
            fw.write(print + "<NotNode> !\n");
            make(parseTree.getChild(1), print + "\t", fw);
        }

    }

    public static void main(String []args) throws IOException {

        CharStream charStream = CharStreams.fromFileName("input");
        ExprLexer exprLexer = new ExprLexer(charStream);
        CommonTokenStream commonTokenStream = new CommonTokenStream(exprLexer);
        ExprParser exprParser = new ExprParser(commonTokenStream);

        ParseTree parseTree = exprParser.prog();
        FileWriter fw = new FileWriter("arbore");

        AntlrParser ap = new AntlrParser(parseTree);
        ap.make(parseTree, "", fw);

        fw.close();

    }
}
