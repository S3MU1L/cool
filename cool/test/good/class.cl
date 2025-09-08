class Point {
    var x: int = 0;
    var y: int = 0;

    fn move(dx: int, dy: int): void {
        x = x + dx;
        y = y + dy;
    }
}

val p: Point = Point();
p.move(5, 7);
print(p.x);
print(p.y);

