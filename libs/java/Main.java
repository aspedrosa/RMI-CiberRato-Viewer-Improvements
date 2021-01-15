/**
 * @author André Pedrosa, nºmec 85098
 * @author Rafael Maio, nºmec 84909
 */

import java.io.IOException;

/**
 * This file is an example of the CRDraw functions call.
 */

public class Main {

    public static void main(String[] args) throws IOException {
        /* Random shapes draw */
        CRDraw crDraw = new CRDraw();
        //crDraw.bufferOnOff(false);
        crDraw.setColor(255,0,0);
        crDraw.drawEllipse("1.loc.1",1,2,2,2);
        Color c = new Color();
        c.setColor(0,255,0);
        crDraw.drawCircle("1.loc.2",0.8,4,4,c);
        crDraw.drawText("1.loc.3","text", 6, 6);
        crDraw.drawRectangle("1.plan.1",0.5,0.5,8,8,10000);
        crDraw.drawSquare("1.plan.2",1.2,0.9,10);
        Point p1 = new Point(10,10.2);
        Point p2 = new Point(11,10.6);
        Point p3 = new Point(12,11);
        Point p4 = new Point(13,14);
        c.setColor(255,0,0);
        Point[] point_list = new Point[4];
        point_list[0] = p1;
        point_list[1] = p2;
        point_list[2] = p3;
        point_list[3] = p4;
        crDraw.drawPolygon("1.plan.3",point_list, 12000);
        crDraw.drawLine("1.plan.4",20,1,17,8);
        crDraw.drawLine2("2.stuff.1",p1,p4,15000);
        crDraw.drawAll();
    }
}
