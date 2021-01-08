import java.io.IOException;

public class Main {

    public static void main(String[] args) throws IOException {
        CRDraw crDraw = new CRDraw();
        //crDraw.bufferOnOff(false);
        crDraw.setColor(255,0,0);
        crDraw.drawEllipse("1.loc.1",50,20,0,0);
        Color c = new Color();
        c.setColor(0,255,0);
        crDraw.drawCircle("1.loc.2",50,100,0,c);
        crDraw.drawText("1.loc.3","text");
        crDraw.drawRectangle("1.plan.1",50,50,100,200,10000);
        crDraw.drawSquare("1.plan.2",20,200,200);
        Point p1 = new Point(150,200);
        Point p2 = new Point(160,180);
        Point p3 = new Point(170,210);
        Point p4 = new Point(180,200);
        Point[] point_list = new Point[4];
        point_list[0] = p1;
        point_list[1] = p2;
        point_list[2] = p3;
        point_list[3] = p4;
        crDraw.drawPolygon("1.plan.3",point_list);
        crDraw.drawLine("1.plan.4",450,100,450,150);
        crDraw.drawLine2("1.yo.1",p1,p4,15000);
        crDraw.drawAll();
    }
}
