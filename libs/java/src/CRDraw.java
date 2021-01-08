import java.io.IOException;
import java.net.*;

class Color{
    int r;
    int g;
    int b;

    public void setColor(int r, int g, int b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }

    @Override
    public String toString(){
        return "red=\"" + r + "\" green=\""+ g +"\"" + " blue=\""+ b + "\"";
    }
}

class Point {
    private final double x;
    private final double y;

    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    @Override
    public String toString() {
        return String.format("<Point x=\"%f\" y=\"%f\"></Point>", x, y);
    }

    public String toStringIndex(int index) {
        return String.format("x%d=\"%f\" y%d=\"%f\"", index, x, index, y);
    }
}

public class CRDraw {

    public static final String DEFAULT_HOST = "localhost";
    public static final int DEFAULT_PORT = 5000;
    public static final int DEFAULT_TTL = 5000;
    public static final int DEFAULT_FONT_SIZE = 14;

    private InetAddress viewerAddress;
    private int viewerPort;
    private DatagramSocket socket;

    private StringBuilder buffer = new StringBuilder();
    private boolean buffered = true;

    private Color color = new Color();

    private void sendMessage(String msg) throws IOException {
        byte[] buf = ("<Shapes>" + msg + "</Shapes>").getBytes();

        DatagramPacket packet = new DatagramPacket(buf, buf.length, viewerAddress, viewerPort);
        socket.send(packet);
    }

    CRDraw(String host, int port) throws SocketException, UnknownHostException {
        viewerPort = port;
        viewerAddress = InetAddress.getByName(host);
        socket = new DatagramSocket();
    }

    CRDraw(String host) throws UnknownHostException, SocketException {
        viewerPort = DEFAULT_PORT;
        viewerAddress = InetAddress.getByName(host);
        socket = new DatagramSocket();
    }

    CRDraw(int port) throws SocketException {
        viewerPort = port;
        try {
            viewerAddress = InetAddress.getByName(DEFAULT_HOST);
        } catch (UnknownHostException e) {}
        socket = new DatagramSocket();
    }

    CRDraw() throws SocketException {
        viewerPort = DEFAULT_PORT;
        try {
            viewerAddress = InetAddress.getByName(DEFAULT_HOST);
        } catch (UnknownHostException e) {}
        socket = new DatagramSocket();
    }

    void bufferOnOff(boolean on_off){
        buffered = on_off;
    }
    void setColor(int r, int g, int b){
        color.setColor(r,g,b);
    }
    void drawAll() throws IOException {
        if (buffered) {
            sendMessage(buffer.toString());
            buffer.setLength(0);
        }
    }

    void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y) throws IOException {
        drawEllipse(id,diam_vertical, diam_horizontal, x, y, color, DEFAULT_TTL);
    }
    void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y, Color color) throws IOException {
        drawEllipse(id,diam_vertical, diam_horizontal, x, y, color, DEFAULT_TTL);
    }
    void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y, int time_to_live) throws IOException {
        drawEllipse(id,diam_vertical, diam_horizontal, x, y, color, time_to_live);
    }
    void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y, Color color, int time_to_live) throws IOException {
        String msg = "<Ellipse ttl=\""+time_to_live+"\" id=\""+id+"\" "+color.toString()+" Diam_vertical=\""+diam_vertical+"\" Diam_horizontal=\""+diam_horizontal+"\" x=\""+x+"\" y=\""+y+"\"></Ellipse>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    void drawCircle(String id, double diam, double x, double y) throws IOException {
        drawCircle(id,diam,x,y,color,DEFAULT_TTL);
    }
    void drawCircle(String id, double diam, double x, double y, Color color) throws IOException {
        drawCircle(id,diam,x,y,color,DEFAULT_TTL);
    }
    void drawCircle(String id, double diam, double x, double y, int time_to_live) throws IOException {
        drawCircle(id,diam,x,y,color,time_to_live);
    }
    void drawCircle(String id, double diam, double x, double y, Color color, int time_to_live) throws IOException {
        drawEllipse(id,diam,diam,x,y,color,time_to_live);
    }

    void drawText(String id, String text, double x, double y) throws IOException {
        drawText(id, DEFAULT_FONT_SIZE,text, x, y, color, DEFAULT_TTL);
    }
    void drawText(String id, String text, double x, double y, Color color) throws IOException {
        drawText(id, DEFAULT_FONT_SIZE,text, x, y, color, DEFAULT_TTL);
    }
    void drawText(String id, String text, double x, double y, int time_to_live) throws IOException {
        drawText(id, DEFAULT_FONT_SIZE,text, x, y, color, time_to_live);
    }
    void drawText(String id, int font_size, String text, double x, double y) throws IOException {
        drawText(id, font_size, text, x, y, color, DEFAULT_TTL);
    }
    void drawText(String id, int font_size, String text, double x, double y, Color color) throws IOException {
        drawText(id, font_size,text, x, y, color, DEFAULT_TTL);
    }
    void drawText(String id, int font_size, String text, double x, double y, int time_to_live) throws IOException {
        drawText(id, font_size, text, x, y, color, time_to_live);
    }
    void drawText(String id, int font_size, String text, double x, double y, Color color, int time_to_live) throws IOException {
        String msg = "<Text ttl=\""+time_to_live+"\" id=\""+id+"\" "+color.toString()+" text=\""+text+"\" x=\""+x+"\" y=\""+y+"\" font_size=\""+font_size+"\"></Text>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    void drawRectangle(String id, double width, double height, double x, double y) throws IOException {
        drawRectangle(id,width,height,x,y,color,DEFAULT_TTL);
    }
    void drawRectangle(String id, double width, double height, double x, double y, Color color) throws IOException {
        drawRectangle(id,width,height,x,y,color,DEFAULT_TTL);
    }
    void drawRectangle(String id, double width, double height, double x, double y, int time_to_live) throws IOException {
        drawRectangle(id,width,height,x,y,color,time_to_live);
    }
    void drawRectangle(String id, double width, double height, double x, double y, Color color, int time_to_live) throws IOException {
        String msg = "<Rectangle ttl=\""+time_to_live+"\" id=\""+id+"\" "+color.toString()+" Width=\""+width+"\" Height=\""+height+"\" x=\""+x+"\" y=\""+y+"\"></Rectangle>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    void drawSquare(String id, double width, double x, double y) throws IOException {
        drawSquare(id,width,x,y,color,DEFAULT_TTL);
    }
    void drawSquare(String id, double width, double x, double y, Color color) throws IOException {
        drawSquare(id,width,x,y,color,DEFAULT_TTL);
    }
    void drawSquare(String id, double width, double x, double y, int time_to_live) throws IOException {
        drawSquare(id,width,x,y,color,time_to_live);
    }
    void drawSquare(String id, double width, double x, double y, Color color, int time_to_live) throws IOException {
        drawRectangle(id,width,width,x,y,color,time_to_live);
    }

    void drawPolygon(String id, Point[] point_list) throws IOException {
        drawPolygon(id,point_list,color, DEFAULT_TTL);
    }
    void drawPolygon(String id, Point[] point_list, Color color) throws IOException {
        drawPolygon(id,point_list,color, DEFAULT_TTL);
    }
    void drawPolygon(String id, Point[] point_list, int time_to_live) throws IOException {
        drawPolygon(id,point_list,color,time_to_live);
    }
    void drawPolygon(String id, Point[] point_list, Color color, int time_to_live) throws IOException {
        StringBuilder msg_builder = new StringBuilder("<Polygon id=\""+id+"\" ttl=\""+time_to_live+"\" "+color+">");
        for (Point point : point_list) {
            msg_builder.append(point.toString());
        }
        msg_builder.append("</Polygon>");
        if(buffered){
            buffer.append(msg_builder.toString());
        }
        else{
            sendMessage(msg_builder.toString());
        }
    }

    void drawLine(String id, double x0, double y0, double x1, double y1) throws IOException {
        drawLine(id, x0, y0, x1, y1, color, DEFAULT_TTL);
    }
    void drawLine(String id, double x0, double y0, double x1, double y1, Color color) throws IOException {
        drawLine(id, x0, y0, x1, y1, color, DEFAULT_TTL);
    }
    void drawLine(String id, double x0, double y0, double x1, double y1, int time_to_live) throws IOException {
        drawLine(id, x0, y0, x1, y1, color, time_to_live);
    }
    void drawLine(String id, double x0, double y0, double x1, double y1, Color color, int time_to_live) throws IOException {
        String msg = String.format(
                "<Line ttl=\"%d\" id=\"%s\" %s x0=\"%f\" y0=\"%f\" x1=\"%f\" y1=\"%f\"></Line>",
                time_to_live, id, color,
                x0, y0, x1, y1);
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    void drawLine2(String id, Point p0, Point p1) throws IOException {
        drawLine2(id, p0, p1, color, DEFAULT_TTL);
    }
    void drawLine2(String id, Point p0, Point p1, Color color) throws IOException {
        drawLine2(id, p0, p1, color, DEFAULT_TTL);
    }
    void drawLine2(String id, Point p0, Point p1, int time_to_live) throws IOException {
        drawLine2(id, p0, p1, color, time_to_live);
    }
    void drawLine2(String id, Point p0, Point p1, Color color, int time_to_live) throws IOException {
        String msg = String.format("<Line ttl=\"%d\" id=\"%s\" %s %s %s></Line>", time_to_live, id, color, p0.toStringIndex(0), p1.toStringIndex(1));
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }


}