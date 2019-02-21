grammar Expr;

/* Tot fisierul */
prog:   line (seq | assign | whilee);


/* Declarare */
line:   'int' spaces string (';' | ', ' string)*;

/* Toate cazurile de sequence */
seq:    assign seq
    |   iff seq
    |   whilee seq
    |   assign assign
    |   assign iff
    |   assign whilee
    |   iff assign
    |   iff iff
    |   iff whilee
    |   whilee assign
    |   whilee iff
    |   whilee whilee
    ;



cond:   '(' bExpr ')'
    |   not
    ;

bracket:    '(' aExpr ')'
    ;

block:  '{' '}'
    |   '{' assign '}'
    |   '{' iff '}'
    |   '{' whilee '}'
    |   '{' seq '}'
    ;

iff: 'if' cond block 'else' block;
whilee: 'while' cond block;

assign: string '=' aExpr ';'
    ;

aExpr:   aExpr div
aExpr
    |   aExpr sum
aExpr
    |   num
    |   string
    |   bracket
    ;

bExpr:  bExpr and bExpr
    |   bool
    |   aExpr greater aExpr
    |   not
    |   cond
    ;


WS: [ \t\r\n]+ -> skip;

and:  '&&';
greater:  '>';
sum:  '+';
div:  '/';
not:  '!' cond;

num:    DIGIT(DIGIT | '0')*
    |   '0'
    ;


bool:   'true'
    |   'false'
    ;

string: LETTER+;
spaces: SPACE*;


SPACE:      ' ';
DIGIT:      [1-9];
LETTER:     [a-z];
