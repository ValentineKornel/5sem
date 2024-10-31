package com.example.web5d.model;

public class CounterResponseModel {
    int sx;
    int sy;

    public CounterResponseModel(int sx, int sy) {
        this.sx = sx;
        this.sy = sy;
    }

    public CounterResponseModel(){}

    public int getSx() {
        return sx;
    }

    public void setSx(int sx) {
        this.sx = sx;
    }

    public int getSy() {
        return sy;
    }

    public void setSy(int sy) {
        this.sy = sy;
    }
}
