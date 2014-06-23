import components.Vector2d;
import components.Vector4d;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;

/**
 * Класс, который запускает главное окно с выбором файла для распознавания.
 * Created by Berlizov on 21.06.14.
 */
class MainWindow extends JFrame implements ActionListener {
    /**
     * Кнопка запуска диалога выбора файла.
     */
    private final JButton buttonBrowse;
    /**
     * Путь для открытия файла.
     */
    private final JTextField textPath;
    /**
     * Выводит текст ошибок или тип фигуры.
     */
    private final JLabel labelType;
    /**
     * Диалог выбора файла.
     */
    private final JFileChooser fc;
    /**
     * Кнопка запуска обработки.
     */
    private JButton buttonStart;


    private MainWindow() {
        super("MagicStick");
        setMinimumSize(new Dimension(300, 300));
        setLocationRelativeTo(null);
        setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        fc = new JFileChooser();
        /**Интерфейс программы*/
        c.weightx = 1.0;
        c.weighty = 0.0;
        c.gridx = 0;
        c.gridy = 0;
        c.fill = GridBagConstraints.BOTH;
        textPath = new JTextField();
        textPath.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                checkNonFree();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                checkNonFree();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                checkNonFree();
            }

            public void checkNonFree() {
                if (textPath.getText().length() > 0)
                    buttonStart.setEnabled(true);
                else
                    buttonStart.setEnabled(false);
            }
        });
        add(textPath, c);
        c.weightx = 0.0;
        c.weighty = 0.0;
        c.gridx = 1;
        c.gridy = 0;
        buttonBrowse = new JButton("Browse");
        buttonBrowse.addActionListener(this);
        add(buttonBrowse, c);
        c.weightx = c.weighty = 1.0;
        c.gridx = 0;
        c.gridy = 1;
        c.gridwidth = 2;
        buttonStart = new JButton("Start");
        buttonStart.addActionListener(this);
        buttonStart.setEnabled(false);
        add(buttonStart, c);
        c.gridx = 0;
        c.gridy = 2;
        labelType = new JLabel();
        add(labelType, c);
        setVisible(true);
    }

    public static void main(String[] args) {
        new MainWindow();
    }

    /**
     * Обработчик нажитий на кнопки.
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource().equals(buttonStart))/*Запуск главной функции для обработки*/
            labelType.setText(start());
        else if (e.getSource().equals(buttonBrowse))/*Откритие файлового диалога при нажатии на кнопку Browse*/ {
            if (fc.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
                textPath.setText(fc.getSelectedFile().getPath());
            }
        }
    }

    /**
     * Функция которая запускает распознавание фигуры на основе данных из файла,
     * по адрессу из textPath.
     *
     * @return Возвращает название фигуры или текст ошибки.
     *
     * @see PathCalculator, #readFromFile(String)
     */
    String start() {
        try {
            ArrayList<Vector4d> t = readFromFile(textPath.getText());
            ArrayList<Vector2d> tr = PathCalculator.calculateTrajectory(t);
            for (Vector2d aTr : tr) System.out.print(aTr + "\n");
            //todo-add recognition
            return t.size() + "";
        } catch (Exception e) {
            return e.getMessage();
        }
    }

    /**
     * Чтение данных из файла и сохранение в массив 4хмерных векторов
     *
     * @param filePath путь к файлу для чтения
     *
     * @return Возвращает массив 4хмерных векторов, если функция отработала успешно.
     *
     * @throws Exception файл содержит не верное количество данных.
     */
    private ArrayList<Vector4d> readFromFile(String filePath) throws Exception {
        File file = new File(filePath);
        ArrayList<Vector4d> al = new ArrayList<Vector4d>();
        double[] x = new double[4];
        BufferedReader br = new BufferedReader(new FileReader(file));
        String line;

            /*Считование данных из файла с проверкой количества данных в файле */
        while ((line = br.readLine()) != null) {
            int count = 0;//Счетчик пробелов
            int j = 0;//укозатель на последний пробел
            for (int i = 0; i < line.length(); i++)
                if ((line.charAt(i) == ' ') || (i == line.length() - 1)) {
                    if (count > 3)//В файле должно былть 4 параметра: x, y, z и время замера
                        throw new Exception("Wrong");
                    x[count] = Double.parseDouble(line.substring(j, i));
                    j = i;
                    count++;
                }
            al.add(new Vector4d(x[0], x[1], x[2], x[3]));
        }
        return al;
    }

}