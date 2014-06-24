import components.Vector2d;

import java.util.ArrayList;

/**
 * Created by 350z6_000 on 24.06.2014.
 */
public abstract class Recognition {
    /**
     * Проекция траектрии.
     */
    private static ArrayList<Vector2d> pr;

    public static String recognize(ArrayList<Vector2d> projection) {
        pr = projection;
        //todo все сделать=)
        return "-";
    }
}
