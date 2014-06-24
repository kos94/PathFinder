import components.Vector2d;

import java.awt.*;
import java.util.ArrayList;

/**
 * Created by 350z6_000 on 23.06.2014.
 */
class ProjectionView extends Panel {
    /**
     * Проекция траектрии.
     */
    private ArrayList<Vector2d> projection;
    /**
     * Коэффициенты сжатия проекции. Нужны для корректного изменения размера компонента. Рассчитываются в {@link
     * #addProjection(java.util.ArrayList)}.
     */
    private double kh, kw;
    /**
     * Наименьшие значения x и y проектции. Нужны для корректного отображения траектрии в компонентеа. Рассчитываются в
     * {@link #addProjection(java.util.ArrayList)}.
     */
    private double minh, minw;

    public ProjectionView() {
        super();
        projection = new ArrayList<Vector2d>();
    }

    public void addProjection(ArrayList<Vector2d> projection) {
        this.projection = projection;
        double maxh = projection.get(0).getX();
        minh = projection.get(0).getX();
        double maxw = projection.get(0).getY();
        minw = projection.get(0).getY();
        for (Vector2d aProjection : projection) {
            if (aProjection.getX() > maxh)
                maxh = aProjection.getX();
            if (aProjection.getX() < minh)
                minh = aProjection.getX();
            if (aProjection.getY() > maxw)
                maxw = aProjection.getY();
            if (aProjection.getY() < minw)
                minw = aProjection.getY();
        }
        kh = maxh - minh;
        kw = maxw - minw;
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        g.setColor(Color.black);
        if (projection.isEmpty()) {
            g.drawLine(0, 0, getWidth(), getHeight());
            g.drawLine(getWidth(), 0, 0, getHeight());
        } else {
            double kkh = getWidth() / kh;
            double kkw = getHeight() / kw;
            Vector2d last=null;
            for (Vector2d aProjection : projection) {
                Vector2d pD = aProjection;
                pD = new Vector2d((pD.getX() - minh) * kkh, (pD.getY() - minw) * kkw);
                if(last!=null)
                    g.drawLine((int) last.getX(), (int) last.getY(), (int) pD.getX(), (int) pD.getY());
                last=pD;
            }
        }
        //todo отрисовка нескольких линий
    }
}
