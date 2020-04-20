package com.android.accelerometergraph;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;


public class MainActivity extends Activity {


    Recognizer recognizer = Recognizer.getRecognizer(); // 字识别器
    Thread srThread;        // 笔画识别线程
    Button button_begin;    // 开始识别
    Button button_clear;    // 清空笔画
    Button button_cancel;   // 返回上一步
    TextView textView_stokeSeq; // 当前识别序列
    TextView textView_word; // 当前识别的字
    TextView textView_wordSeq;  // 当前识别的字序列
    TextView textView_possibleWordSeq;  // 当前的可能字序列


    // 展示笔画序列
    private void showStrokeSeq() {
        textView_stokeSeq.setText(recognizer.getRealStokeSeq());
    }

    // 展示识别出的字
    private void showWord() {
        textView_word.setText(recognizer.recognize());
    }

    // 展示可能的字序列
    private void showPossibleWordSeq() {
        textView_possibleWordSeq.setText(recognizer.getPossibleWords());
    }

    // 展示，上面三个函数的集合
    private void show(){
        showStrokeSeq();
        showWord();
        showPossibleWordSeq();
    }




    // 初始化该类的所有对象
    private void objInit(){
        setContentView(R.layout.main);
        AccelerometerGraphJNI.init(getAssets());
        button_begin = findViewById(R.id.button);
        button_clear = findViewById(R.id.button2);
        button_cancel = findViewById(R.id.button3);
        textView_stokeSeq = findViewById(R.id.textView);
        textView_word = findViewById(R.id.textView2);
        textView_wordSeq = findViewById(R.id.textView3);
        textView_possibleWordSeq = findViewById(R.id.textView4);
        srThread = new Thread(new StrokeRecognizeThread());

        button_begin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                button_begin.setText("正在识别中...");
                srThread.start();
            }
        });

        button_clear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                recognizer.clear();
                show();
            }
        });
        button_cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                recognizer.cancel();
                show();
            }
        });
        textView_word.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String word = recognizer.recognize();
                if (!word.equals("")){
                    textView_wordSeq.append(word + " ");
                    recognizer.clear();
                    show();
                }
            }
        });
        textView_wordSeq.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                textView_wordSeq.setText("");

            }
        });

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        objInit();
    }

    @Override
    protected void onPause() {
        super.onPause();
        AccelerometerGraphJNI.pause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        AccelerometerGraphJNI.resume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }


    // 笔画识别线程
    private class StrokeRecognizeThread implements Runnable {

        // 下面 2 个参数用于解析书写状态
        int writeInterval = 100;
        int writeSleepTime = 5;

        // 下面 3 个参数用于衡量静止状态
        int stopInterval = 100;
        int stopSleepTime = 5;
        double stopPercent = 0.75;  // 用于衡量静止状态，数值越大，越达禁止状态

        double eps = 1e-2;  // 加速度在 |esp| 范围内，我们就认为是0

        // 求平均值
        private float avg(ArrayList<Double> list) {
            if (list.size() == 0) {
                return 0;
            }
            float total = 0;
            for (int i = 0; i < list.size(); i++) {
                total += list.get(i);
            }
            return total / list.size();
        }

        // 判断加速度是否为0, 加速度在 |esp| 范围内，我们就认为是0
        private boolean isZero(double value) {
            return Math.abs(value) <= eps;
        }

        // 判断是否处于静止状态
        private boolean isStop() {
            // 静止状态是否稳定
            int zeroCount = 0;
            for (int i = 0; i < stopInterval / stopSleepTime; i++) {
                double svx = AccelerometerGraphJNI.get_x();
                double svy = AccelerometerGraphJNI.get_y();
                if (isZero(svx) && isZero(svy)) {
                    zeroCount++;
                }
                try {
                    Thread.sleep(stopSleepTime);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            //System.out.println(zeroCount);
            return zeroCount > (double)stopInterval/stopSleepTime * stopPercent;
        }


        // 通过x,y加速度，判断是什么笔画
        int toStrokeMark(double pvx, double pvy) {
            if (!isZero(pvx) && !isZero(pvy)) {
                if (pvx < 0 && pvy < 0) {
                    return StrokeManager.STROKE_PIE;   // 撇
                } else if (pvx > 0 && pvy < 0) {
                    return StrokeManager.STROKE_NA;   // 捺
                } else if (pvx > 0 && pvy > 0) {
                    return StrokeManager.STROKE_TI;   // 提
                } else {
                    return StrokeManager.STROKE_GOU;   // 勾
                }
            } else if (!isZero(pvx)) {
                return StrokeManager.STROKE_HENG;   // 横
            } else if (!isZero(pvy)) {
                return StrokeManager.STROKE_SU;   // 竖
            } else {
                return StrokeManager.STROKE_ERROR;  // 无法识别
            }
        }

        @Override
        public void run() {
            Boolean isWriting = false;  // 是否处于书写状态
            ArrayList<Double> xArr = new ArrayList<>(); // 用于求取 x加速度 平均值
            ArrayList<Double> yArr = new ArrayList<>(); // 用于求取 y加速度 平均值


            // 这个循环可以监听笔画的生成
            while (true) {
                double vx = AccelerometerGraphJNI.get_x();
                double vy = AccelerometerGraphJNI.get_y();

                // 先判断是否开始书写了
                if (!isZero(vx) || !isZero(vy)) {
                    if (!isWriting) {
                        xArr.clear();
                        yArr.clear();
                        isWriting = true;
                    }

                    // 将x加速度加入均值集
                    if (xArr.size() < writeInterval / writeSleepTime) {
                        xArr.add(vx);
                    }
                    // 将y加速度加入均值集
                    if (yArr.size() < writeInterval / writeSleepTime) {
                        yArr.add(vy);
                    }

                    // 休眠
                    try {
                        Thread.sleep(writeSleepTime);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    continue;
                }

                // 当没加速度时，测试一下是否是用户停止了书写，如果是，则求出笔画，通过当前笔画序列进行描字。
                if (isWriting && isStop()) {
                    isWriting = false;
                    double pvx = avg(xArr); // 获取平均x加速度
                    double pvy = avg(yArr); // 获取平均y加速度

                    // 通过 pvx,pvy 判断是什么笔画
                    int strokeMark = toStrokeMark(pvx, pvy);

                    if (strokeMark != -1){
                        // 等于 -1, 表示识别错误
                        recognizer.put(toStrokeMark(pvx, pvy));
                        show();
                    }
                }
            }
        }
    }


}


