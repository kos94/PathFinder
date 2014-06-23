import components.*;

import java.util.ArrayList;

/**
 * Класс, который содержит функции
 * для расчетов траектории на основе показаний акселерометра.
 * Created by 350z6_000 on 23.06.14.
 */
abstract class PathCalculator {

    /**
     * Все показаний акселерометра.
     */
    private ArrayList<Vector4d> records;
    /**
     * Траектория
     */
    private ArrayList<Vector3d> points;
    /**
     * Калибровочное ускорение.
     * Вычисляется в {@link #calibration(int)}.
     */
    private Vector4d initialRecord;

    /**
     * Функция запуска расчета траектрии на основе показаний акселерометра.
     *
     * @param acc значения акселерометра
     *
     * @return Возвращает траекторию в виде массивая 3d-точек.
     *
     * @throws Exception Количество кадров калибровки не может быть больше, чем общее количество кадров.
     */
    public static ArrayList<Vector2d> calculateTrajectory(ArrayList<Vector4d> acc) throws Exception {
        PathCalculator pc = new PathCalculator() {
        };
        pc.records = acc;
        pc.calibration(3);
        pc.points = new ArrayList<Vector3d>();
        pc.calcData();
        ArrayList<Vector2d> p = new ArrayList<Vector2d>();
        for (int i = 0; i < pc.points.size(); i++)
            p.add(pc.points.get(i));
        return p;
    }

    /**
     * Функция для калибровки аккселерометра.
     * Рассчитывается среднее ускорение.
     *
     * @param countCalibrationFrames количество кадров калибровки.
     *
     * @throws Exception Количество кадров калибровки не может быть больше, чем общее количество кадров.
     */
    private void calibration(int countCalibrationFrames) throws Exception {
        initialRecord = records.get(0);
        if (countCalibrationFrames >= records.size())
            throw new Exception("Данных с акселерометра недостаточно " +
                    "для определения траектории или сильно много калибровочных кадров.");
        for (int i = 0; i < countCalibrationFrames; i++) {
            initialRecord.add(records.get(i));
        }
        initialRecord.divide(countCalibrationFrames);
        //System.out.print(initialRecord.toString());
    }

    /**
     * Функция для расчета траектории
     */
    private void calcData() {
        Vector3d point;
        Vector4d cur;
        LinearMovement wx = new LinearMovement();
        LinearMovement wy = new LinearMovement();
        LinearMovement wz = new LinearMovement();
        double t, ti, dt;
        int recNum = records.size();
        t = initialRecord.getT();
        for (int i = 0; i < recNum; i++) {
            cur = records.get(i);
            ti = cur.getT();
            cur.subtract(initialRecord);//Отнимаем калибровочное ускорение
           /* cur.deleteNoise( LOW_BORDER_VALUE );*/
            //todo-Noise filter
            dt = (ti - t);
            wx.calcNextCoord(cur.getX(), dt);
            wy.calcNextCoord(cur.getY(), dt);
            wz.calcNextCoord(cur.getZ(), dt);
            point = new Vector3d(wx.getS(), wy.getS(), wz.getS());
            points.add(point);
            t = ti;
        }
    }

    /**
     * Функция поворота для преобразования траектории в 2d-рисунок
     */
    private void rotate() {
        int pNum = records.size();
        Vector3d lastP = points.get(pNum - 1);
        double anB = Math.atan(lastP.getX() / lastP.getY());
        double arg = lastP.getZ() / (Math.cos(anB) * lastP.getY() + Math.sin(anB) * lastP.getX());
        double anA = Math.atan(arg);
        Matrix3x3 m = new Matrix3x3(0, -anA + Math.PI / 2, anB + Math.PI / 2);
        for (int i = 0; i < pNum; i++)
            points.get(i).rotate(m);
    }
}
