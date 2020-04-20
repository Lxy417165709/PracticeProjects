package com.android.accelerometergraph;


import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;




// 这是一个识别器
class Recognizer {
    private String strokeSeq = "";                              // 当前笔画序列
    private HashMap<String, String> wordSet = new HashMap<String, String>(); // 字库
    private static Recognizer recognizer = new Recognizer();    // 单例对象
    private WordUnit[] WordUnits;                               // 存放预测字、及对应的编辑距离

    // 单例模式
    static Recognizer getRecognizer() {
        return recognizer;
    }

    // 计算 s、p 的最短编辑距离
    private int EditDistance(String s, String p) {
        int[][] dis = new int[s.length() + 1][p.length() + 1];
        for (int i = 1; i <= s.length(); i++) {
            dis[i][0] = i;
        }
        for (int t = 1; t <= p.length(); t++) {
            dis[0][t] = t;
        }
        for (int i = 1; i <= s.length(); i++) {
            for (int t = 1; t <= p.length(); t++) {
                if (s.charAt(i - 1) == p.charAt(t - 1)) {
                    dis[i][t] = dis[i - 1][t - 1];
                } else {
                    dis[i][t] = Math.min(dis[i][t - 1] + 1, dis[i - 1][t] + 1);
                    dis[i][t] = Math.min(dis[i][t], dis[i - 1][t - 1] + 1);
                }
            }
        }
        return dis[s.length()][p.length()];
    }


    // 预测输入，取前3个可能的输入
    // 可以使用堆进行优化
    String getPossibleWords() {

        // 先将当前笔画序列、与字体库的笔画序列进行比较，获取彼此的最短编辑距离
        // 将编辑距离加入 WordUnits
        int i = 0;
        for (String key : wordSet.keySet()) {
            WordUnits[i++] = new WordUnit(wordSet.get(key),EditDistance(strokeSeq, key));
        }
        Arrays.sort(WordUnits, 0, WordUnits.length);

        // 默认取3个，这里可以修改
        StringBuilder ans = new StringBuilder("[");
        int end = Math.min(wordSet.size(),3);
        for (i = 0; i < end; i++) {
            ans.append(WordUnits[i].getWord());
            if (i != end-1) {
                ans.append("，");
            }
        }
        ans.append("]");
        return ans.toString();
    }



    private Recognizer() {

        // 识别库
        wordSet.put("0", "一");
        wordSet.put("00", "二");
        wordSet.put("01", "十");
        wordSet.put("001", "干");
        wordSet.put("010", "土、士");
        wordSet.put("000", "三");
        wordSet.put("30001", "主");
        wordSet.put("0001", "王");
        wordSet.put("00013", "玉");
        wordSet.put("0123", "木");
        wordSet.put("02150", "子");
        wordSet.put("012302150", "李");
        wordSet.put("33220202150", "学");
        wordSet.put("1133210102105", "悦");
        wordSet.put("1010", "口");
        wordSet.put("10100", "日、曰");
        wordSet.put("101010", "由");
        wordSet.put("101001", "由");
        wordSet.put("101000", "目");
        wordSet.put("2105", "儿");
        wordSet.put("23", "八");


        WordUnits = new WordUnit[wordSet.size()];
    }

    // 返回笔画序列
   String getRealStokeSeq() {
        StringBuilder ans = new StringBuilder("[");
        for (int i = 0; i < strokeSeq.length(); i++) {
            ans.append(StrokeManager.getStroke(Integer.parseInt(String.valueOf(strokeSeq.charAt(i)))));
            if (i != strokeSeq.length() - 1) {
                ans.append("，");
            }
        }
        ans.append("]");
        return ans.toString();
    }


    // 将笔画加入笔画序列
    void put(int stokeMark) {
        strokeSeq = strokeSeq + stokeMark;
    }

    // 通过笔画序列，获得汉字
    String recognize() {
        return wordSet.get(strokeSeq);
    }

    // 清空笔画序列
    void clear() {
        strokeSeq = "";
    }

    // 返回上次的笔画序列
    void cancel() {
        if (strokeSeq.length() == 0) {
            return;
        }
        strokeSeq = strokeSeq.substring(0, strokeSeq.length() - 1);
    }

}
