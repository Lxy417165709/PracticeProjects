package com.android.accelerometergraph;



// 这是一个笔画管理器
class StrokeManager {
    static final int STROKE_HENG = 0;
    static final int STROKE_SU = 1;
    static final int STROKE_PIE = 2;
    static final int STROKE_NA = 3;
    static final int STROKE_TI = 4;
    static final int STROKE_GOU = 5;
    static final int STROKE_ERROR = -1;


    // 通过笔画标号，获取笔画名
    static String getStrokeName(int stokeMark) {
        switch (stokeMark) {
            case STROKE_HENG:
                return "横";
            case STROKE_SU:
                return "竖";
            case STROKE_PIE:
                return "撇";
            case STROKE_NA:
                return "捺";
            case STROKE_TI:
                return "提";
            case STROKE_GOU:
                return "勾";
        }
        // 表示识别错误
        return "N";
    }

    // 通过笔画标号，获取笔画
    static String getStroke(int stokeMark) {
        switch (stokeMark) {
            case STROKE_HENG:
                return "一";
            case STROKE_SU:
                return "|";
            case STROKE_PIE:
                return "丿";
            case STROKE_NA:
                return "丶";
            case STROKE_TI:
                return "㇀";
            case STROKE_GOU:
                return "亅";
        }
        // 表示识别错误
        return "N";
    }
}
