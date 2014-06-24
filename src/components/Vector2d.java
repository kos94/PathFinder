package components;

/**
 * Created by 350z6_000 on 23.06.14.
 */
public class Vector2d {
    double x;
    double y;

    public Vector2d() {
        x = y = 0;
    }

    public Vector2d(double X, double Y) {
        x = X;
        y = Y;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    @Override
    public String toString() {
        return "x = " + x + "; y = " + y;
    }
}
