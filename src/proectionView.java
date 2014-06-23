import components.Vector2d;

import java.awt.*;
import java.util.ArrayList;

/**
 * Created by 350z6_000 on 23.06.2014.
 */
class proectionView extends Panel {
    private ArrayList<Vector2d> proection;

    public void setProection(ArrayList<Vector2d> proection) {
        this.proection = proection;
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        // DrawRect
    }
}
