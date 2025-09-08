class Animal {
    fn speak(): void {
        print("...");
    }
}

class Dog : Animal {
    fn speak(): void {
        print("Woof!");
    }
}

val d: Dog = Dog();
d.speak();
val a: int = 5;
if (a < 10) {
    print("Less than 10");
} else {
    print("Greater or equal to 10");
}

