package com.android.accelerometergraph;

// 一个 用于存放字、编辑距离 的数据结构。
class WordUnit implements Comparable<WordUnit>
{
    private String word;
    private int distance;

    WordUnit(String word, int distance) {
        this.word = word;
        this.distance = distance;
    }

    String getWord(){
        return word;
    }

    public int compareTo(WordUnit a) {
        return this.distance-a.distance;  //按x升序排序
    }
}