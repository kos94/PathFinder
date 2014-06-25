import components.Vector2d;

import java.util.ArrayList;

/**
 * Created by 350z6_000 on 24.06.2014.
 */
public abstract class Recognition {
    /**
     * Проекция траектрии.
     */
    private static ArrayList<Vector2d> PC;
    /**
     * Массив углов относительно оси абсцисс. Генерируется в функции {@link #calcAngles()}.
     */
    private static ArrayList<Double> angles;

    /**
     * Основная функция распознавания проекции.
     *
     * @param projection проекция, которая будет распознаваться.
     * @param pv         компонент, в который будет выводится результат
     *
     * @return Возврящает название фигуры или "-", если фигура не распознана.
     */
    public static String recognize(ArrayList<Vector2d> projection, ProjectionView pv) {
        PC = projection;
        calcAngles();

        if (isCircle()) {
            pv.setProjection(PC);
            return "Circle";
        }
        ArrayList<ArrayList<Vector2d>> lines = isEnd(30, 5);
        pv.clearProjection();
        for (ArrayList<Vector2d> line : lines)
            pv.addProjection(line);
        switch (lines.size()) {
            case 3:
                return "Triangle";
            case 4:
                if (isClock(lines.get(0).get(0),
                        lines.get(1).get(0),
                        lines.get(2).get(0),
                        lines.get(3).get(0)))
                    return "Clock";
                return "Square";
        }
        return "-";
    }

    /**
     * Генерирует массив углов относительно оси абсцисс. <b>Испытательный срок!</b> //todo нужно слить с #isEnd
     *
     * @see components.Vector2d#AngleLineAndAxis(components.Vector2d, components.Vector2d)
     */
    private static void calcAngles() {
        angles = new ArrayList<Double>();
        for (int i = 1; i < PC.size(); i++) {
            angles.add(Vector2d.AngleLineAndAxis(PC.get(i - 1), PC.get(i)));
        }
    }

    /**
     * Фильтр проверяет является ли фигура кругом.
     *
     * @return true - круг. false - не круг.
     */
    private static boolean isCircle() {
        int pNum = angles.size();
        int cut = (int) ((pNum * 0.3) / 2);
        for (int i = cut; i < pNum - cut; ++i) {
            if (Math.abs(angles.get(i) - angles.get(i + 1)) > 20)
                return false;
        }
        return true;
    }


    /**
     * Фильтр проверяет является ли фигура песочными часами. Проверяется наличие перекрестных линий.
     *
     * @param A 1й угол песочных часов.
     * @param B 2й угол песочных часов.
     * @param C 3й угол песочных часов.
     * @param D 4й угол песочных часов.
     *
     * @return true - песочные часы. false - другая фигура.
     */
    private static boolean isClock(Vector2d A, Vector2d B, Vector2d C, Vector2d D) {
        ArrayList<Vector2d> al=new ArrayList<Vector2d>();
        al.add(A);al.add(B);al.add(C);al.add(D);
        for(int i=0;i<2;i++) {
            if (Vector2d.hasIntersection(al.get(0), al.get(1), al.get(2), al.get(3)))
                return true;
            Vector2d temp=al.get(0);
            al.remove(0);
            al.add(temp);
        }
        return false;
    }

    /**
     * Нарезка фигуры на прямые отрезки.
     *
     * @param ang  угол(градус), превышение которого будет считаться поворотом.
     * @param step максимальная длина поворота(количество точек). Если товорот на рисунке больше чем установленная
     *             длина, то он будет считаться как 2 или более поворотов.
     *
     * @return Массив отрезков.
     */
    private static ArrayList<ArrayList<Vector2d>> isEnd(int ang, int step) {
        ArrayList<ArrayList<Vector2d>> A = new ArrayList<ArrayList<Vector2d>>();
        int pNum = PC.size();
        int last = 0;
        for (int i = 0; i < pNum; i++) {
            if ((i < step * 3) || (i + step > pNum - 1))
                continue;
            Vector2d pr = PC.get(i - step * 2);
            Vector2d tec = PC.get(i - step);
            Vector2d tstep = PC.get(i);
            double an1 = Vector2d.AngleLineAndAxis(pr, tec);
            double an2 = Vector2d.AngleLineAndAxis(tec, tstep);
            if (!((an2 + ang > an1) && (an2 - ang < an1))) {
                A.add(new ArrayList<Vector2d>(PC.subList(last, i)));
                last = i - 1;
                i += step * 2;
            }
        }
        A.add(new ArrayList<Vector2d>(PC.subList(last, PC.size())));
        return A;
    }
}
