# Cool Language Specification

**Cool** is a minimal, statically-typed programming language inspired by TinyLang, designed for learning and building
interpreters/compilers. It supports variables, arithmetic, control flow, functions, classes with inheritance, and
first-class functions. The language is intended to be compiled to bytecode and executed on a custom VM.

---

## Features

- **Variables** with explicit mutability: `val` (immutable) and `var` (mutable)
- **Static types**: `int`, `float`, `bool`, `string`, `void`, and user-defined classes
- Arithmetic and boolean expressions
- Control flow: `if`, `else`, `while`, `for`
- Functions and first-class functions, including closures
- Classes and single inheritance with explicitly declared fields and methods

---

## Lexical Structure

### Identifiers

- Consist of letters (`a-z`, `A-Z`), digits (`0-9`), and underscores (`_`)
- Must not start with a digit

### Literals

- **Number literals**: sequences of digits, optionally with decimals (e.g., `42`, `3.14`)
- **String literals**: characters enclosed in double quotes (e.g., `"hello"`)
- **Boolean literals**: `true`, `false`
- **Nil literal**: `nil`

### Keywords

```
true  false  class  print   super  this  while
val   var    fn     return  if     else  for 
and   or     nil    int     float  bool  string void
```

### Operators

```
+  -  *  /  ==  !=  <  <=  >  >=  &&  ||  =
```

### Comments

* Single line: `// comment`

---

## Grammar

(in EBNF)

```
program ::= declaration* EOF

declaration ::= classDecl
| funDecl
| varDecl
| statement

classDecl ::= "class" IDENT ( "extends" IDENT )? "{" classMember* "}"

classMember ::= fieldDecl | funDecl

fieldDecl ::= ( "val" | "var" ) IDENT ":" type ";"

funDecl ::= "fn" function

varDecl ::= ( "val" | "var" ) IDENT ":" type ("=" expression)? ";"

statement ::= exprStmt
| forStmt
| ifStmt
| printStmt
| returnStmt
| whileStmt
| block

exprStmt ::= expression ";"

forStmt ::= "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement

ifStmt ::= "if" "(" expression ")" statement ( "else" statement )?

printStmt ::= "print" expression ";"

returnStmt ::= "return" expression? ";"

whileStmt ::= "while" "(" expression ")" statement

block ::= "{" declaration* "}"

expression ::= assignment

assignment ::= ( call "." )? IDENT "=" assignment
| logic_or

logic_or ::= logic_and ( "or" logic_and )*

logic_and ::= equality ( "and" equality )*

equality ::= comparison ( ( "!=" | "==" ) comparison )*

comparison ::= term ( ( ">" | ">=" | "<" | "<=" ) term )*

term ::= factor ( ( "-" | "+" ) factor )*

factor ::= power ( ( "/" | "" | "%" ) power )

power ::= unary ( "**" unary )*

unary ::= ( "!" | "-" ) unary | call

call ::= primary ( "(" arguments? ")" | "." IDENT )*

primary ::= "true"
| "false"
| "nil"
| "this"
| NUMBER
| STRING
| IDENT
| "(" expression ")"
| "super" "." IDENT
| "fn" "(" parameters? ")" block

function ::= IDENT "(" parameters? ")" ( ":" type )? block

parameters ::= IDENT ":" type ( "," IDENT ":" type )*

arguments ::= expression ( "," expression )*

type ::= "int" | "float" | "bool" | "string" | "void" | IDENT
```

---

## Semantics

### Variables

- Declared with `val` (immutable) or `var` (mutable)
- Example:

```cool
val x: int = 10;     // cannot reassign
var y: int = x + 5;  // can reassign
y = y + 1;
```

### Functions

- Declared with `fn`, can have parameters and return types
- Support recursion, closures and first-class functions
- Example:

```cool
fn add(a: int, b: int): int {
    return a + b;
}
val result: int = add(2, 3);
```

### Control Flow

- `if`, `else`, `while`, and `for` statements
- Example:

```cool
if (x > 0) {
    print "Positive";
} else {
    print "Non-positive";
}
while (x < 10) {
    x = x + 1;
}
for (var i: int = 0; i < 5; i = i + 1) {
    print i;
}
```

### Classes and Inheritance

- Declared with `class`, support single inheritance
- Fields can be `val` or `var`
- Methods must have explicit return types
- Example:

```
class Person {
    val name: string;
    var age: int;

    fn init(name: string, age: int): void {
        this.name = name;
        this.age = age;
    }

    fn greet(): void {
        print "Hello, my name is " + this.name + " and I am " + this.age + " years old.";
    }
}

class Employee extends Person {
    val position: string;

    fn init(name: string, age: int, position: string): void {
        super.init(name, age);
        this.position = position;
    }

    fn work(): void {
        print "I am working as a " + this.position + ".";
    }
}

val john: Person = Person("John", 30);
john.greet();
```