package components;

/**
 * Created by 350z6_000 on 23.06.14.
 */
public class Vector4d extends Vector3d {
    private final int t;

    public Vector4d() {
        super();
        t = 0;
    }

    public Vector4d(double X, double Y, double Z, int T) {
        super(X, Y, Z);
        t = T;
    }

    public int getT() {
        return t;
    }

    @Override
    public String toString() {
        return super.toString() + "; t = " + t;
    }
}
