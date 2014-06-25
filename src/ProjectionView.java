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
    private ArrayList<ArrayList<Vector2d>> projection;
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
        clearProjection();
    }

    public void clearProjection() {
        projection = new ArrayList<ArrayList<Vector2d>>();
        repaint();
    }

    public void setProjection(ArrayList<Vector2d> projection) {
        clearProjection();
        addProjection(projection);
    }

    public void addProjection(ArrayList<Vector2d> projection) {
        this.projection.add(projection);
        double maxh = projection.get(0).getX();
        minh = projection.get(0).getX();
        double maxw = projection.get(0).getY();
        minw = projection.get(0).getY();
        for (ArrayList<Vector2d> pr : this.projection)
            for (Vector2d aProjection : pr) {
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
        repaint();
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        Graphics2D g2 = (Graphics2D) g;
        g2.setColor(Color.black);
        if (projection.isEmpty()) {
            g2.drawLine(0, 0, getWidth(), getHeight());
            g2.drawLine(getWidth(), 0, 0, getHeight());
        } else {
            g2.setStroke(new BasicStroke(3));
            double kkh = (getWidth() - 1) / kh;
            double kkw = (getHeight() - 1) / kw;
            Vector2d last = null;
            for (ArrayList<Vector2d> aProjections : projection) {
                if ((projection.size() == 3) || (projection.size() == 4)) {
                    g2.drawLine(
                            (int) ((aProjections.get(0).getX() - minh) * kkh),
                            (int) ((aProjections.get(0).getY() - minw) * kkw),
                            (int) ((aProjections.get(aProjections.size() - 1).getX() - minh) * kkh),
                            (int) ((aProjections.get(aProjections.size() - 1).getY() - minw) * kkw));
                } else {
                    for (Vector2d aProjection : aProjections) {
                        Vector2d pD = aProjection;
                        pD = new Vector2d((pD.getX() - minh) * kkh, (pD.getY() - minw) * kkw);
                        if (last != null)
                            g2.drawLine(
                                    (int) last.getX(),
                                    (int) last.getY(),
                                    (int) pD.getX(),
                                    (int) pD.getY());
                        last = pD;
                    }
                }
                g2.setColor(new Color((int) (Math.random() * 150) + 50, (int) (Math.random() * 150) + 50, (int) (Math.random() * 150) + 50));
            }
        }
    }
}
