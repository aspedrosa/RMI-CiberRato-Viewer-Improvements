/**
 * @author André Pedrosa, nºmec 85098
 * @author Rafael Maio, nºmec 84909
 */

import java.io.IOException;
/**
 * This file implements the library to draw graphic shapes
 * and text.
 * Should be used by agents.
 * Communicates with the viewer.
 */

import java.net.*;


public class CRDraw {

    /**
     * Default host address.
     */
    public static final String DEFAULT_HOST = "localhost";

    /**
     * Default host port.
     */
    public static final int DEFAULT_PORT = 5000;

    /**
     * Shape default time to live.
     */
    public static final int DEFAULT_TTL = 5000;

    /**
     * Text default font size.
     */
    public static final int DEFAULT_FONT_SIZE = 14;

    /**
     * Viewer host address.
     */
    private InetAddress viewerAddress;

    /**
     * Viewer host port.
     */
    private int viewerPort;

    /**
     * Socket.
     */
    private DatagramSocket socket;

    /**
     * Messages buffer.
     */
    private StringBuilder buffer = new StringBuilder();

    /**
     * Indicates if the messages buffer is being used.
     */
    private boolean buffered = true;

    /**
     * Default color
     */
    private Color color = new Color();

    /**
     * Builds and sends the message via socket to host address and port.
     * @param msg -> message to send.
     * @throws IOException
     */
    private void sendMessage(String msg) throws IOException {
        byte[] buf = ("<Shapes>" + msg + "</Shapes>").getBytes();

        DatagramPacket packet = new DatagramPacket(buf, buf.length, viewerAddress, viewerPort);
        socket.send(packet);
    }

    /**
     * Draw instantiation.
     * @param host -> viewer host address.
     * @param port -> viewer host port.
     * @throws SocketException
     * @throws UnknownHostException
     */
    public CRDraw(String host, int port) throws SocketException, UnknownHostException {
        viewerPort = port;
        viewerAddress = InetAddress.getByName(host);
        socket = new DatagramSocket();
    }

    /**
     * Draw instantiation.
     * @param host -> viewer host address.
     * @throws UnknownHostException
     * @throws SocketException
     */
    public CRDraw(String host) throws UnknownHostException, SocketException {
        viewerPort = DEFAULT_PORT;
        viewerAddress = InetAddress.getByName(host);
        socket = new DatagramSocket();
    }

    /**
     * Draw instantiation
     * @param port -> viewer host port
     * @throws SocketException
     */
    public CRDraw(int port) throws SocketException {
        viewerPort = port;
        try {
            viewerAddress = InetAddress.getByName(DEFAULT_HOST);
        } catch (UnknownHostException e) {}
        socket = new DatagramSocket();
    }

    /**
     * Draw instantiation
     * @throws SocketException
     */
    public CRDraw() throws SocketException {
        viewerPort = DEFAULT_PORT;
        try {
            viewerAddress = InetAddress.getByName(DEFAULT_HOST);
        } catch (UnknownHostException e) {}
        socket = new DatagramSocket();
    }

    /**
     * Turn the buffer on or off
     * @param on_off -> Turns the buffer on (true) or off (false)
     */
    public void bufferOnOff(boolean on_off){
        buffered = on_off;
    }

    /**
     * Set default color.
     * @param r -> Red quantity.
     * @param g -> Green quantity.
     * @param b -> Blue  quantity.
     */
    public void setColor(int r, int g, int b){
        color.setColor(r,g,b);
    }

    /**
     * Send all the messages in the buffer.
     * @throws IOException
     */
    public void drawAll() throws IOException {
        if (buffered) {
            System.out.println(buffer.toString());
            sendMessage(buffer.toString());
            buffer.setLength(0);
        }
    }

    /**
     * Create the message to draw an ellipse.
     * @param id -> Ellipse identifier.
     * @param diam_vertical -> Ellipse vertical diameter.
     * @param diam_horizontal -> Ellipse horizontal diameter.
     * @param x -> x position of the center point of the ellipse.
     * @param y -> y position of the center point of the ellipse.
     * @throws IOException
     */
    public void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y) throws IOException {
        drawEllipse(id,diam_vertical, diam_horizontal, x, y, color, DEFAULT_TTL);
    }

    /**
     * Create the message to draw an ellipse.
     * @param id -> Ellipse identifier.
     * @param diam_vertical -> Ellipse vertical diameter.
     * @param diam_horizontal -> Ellipse horizontal diameter.
     * @param x -> x position of the center point of the ellipse.
     * @param y -> y position of the center point of the ellipse.
     * @param color -> Ellipse color.
     * @throws IOException
     */
    public void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y, Color color) throws IOException {
        drawEllipse(id,diam_vertical, diam_horizontal, x, y, color, DEFAULT_TTL);
    }

    /**
     * Create the message to draw an ellipse.
     * @param id -> Ellipse identifier.
     * @param diam_vertical -> Ellipse vertical diameter.
     * @param diam_horizontal -> Ellipse horizontal diameter.
     * @param x -> x position of the center point of the ellipse.
     * @param y y position of the center point of the ellipse.
     * @param time_to_live -> Ellipse time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y, int time_to_live) throws IOException {
        drawEllipse(id,diam_vertical, diam_horizontal, x, y, color, time_to_live);
    }

    /**
     * Create the message to draw an ellipse.
     * @param id -> Ellipse identifier.
     * @param diam_vertical -> Ellipse vertical diameter.
     * @param diam_horizontal -> Ellipse horizontal diameter.
     * @param x -> x position of the center point of the ellipse.
     * @param y -> y position of the center point of the ellipse.
     * @param color -> Ellipse color.
     * @param time_to_live -> Ellipse time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawEllipse(String id, double diam_vertical, double diam_horizontal, double x, double y, Color color, int time_to_live) throws IOException {
        String msg = "<Ellipse ttl=\""+time_to_live+"\" id=\""+id+"\" "+color.toString()+" Diam_vertical=\""+diam_vertical+"\" Diam_horizontal=\""+diam_horizontal+"\" x=\""+x+"\" y=\""+y+"\"></Ellipse>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    /**
     * Creates the message to draw a circle.
     * @param id -> Circle identifier.
     * @param diam -> Circle diameter.
     * @param x -> x position of the center point of the circle.
     * @param y -> y position of the center point of the circle.
     * @throws IOException
     */
    public void drawCircle(String id, double diam, double x, double y) throws IOException {
        drawCircle(id,diam,x,y,color,DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a circle.
     * @param -> id Circle identifier.
     * @param -> diam Circle diameter.
     * @param -> x x position of the center point of the circle.
     * @param -> y  y position of the center point of the circle.
     * @param -> color Circle color.
     * @throws IOException
     */
    public void drawCircle(String id, double diam, double x, double y, Color color) throws IOException {
        drawCircle(id,diam,x,y,color,DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a circle.
     * @param -> id Circle identifier.
     * @param -> diam Circle diameter.
     * @param -> x x position of the center point of the circle.
     * @param -> y  y position of the center point of the circle.
     * @param -> time_to_live Circle time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawCircle(String id, double diam, double x, double y, int time_to_live) throws IOException {
        drawCircle(id,diam,x,y,color,time_to_live);
    }

    /**
     *  Creates the message to draw a circle.
     * @param -> id Circle identifier.
     * @param -> diam Circle diameter.
     * @param -> x x position of the center point of the circle.
     * @param -> y  y position of the center point of the circle.
     * @param -> color Circle color.
     * @paramv time_to_live Circle time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawCircle(String id, double diam, double x, double y, Color color, int time_to_live) throws IOException {
        drawEllipse(id,diam,diam,x,y,color,time_to_live);
    }

    /**
     * Creates the message to draw text.
     * @param id-> Text identifier.
     * @param text -> Quote to write.
     * @param x -> x position of the center point of the text.
     * @param y -> y position of the center point of the text.
     * @throws IOException
     */
    public void drawText(String id, String text, double x, double y) throws IOException {
        drawText(id, DEFAULT_FONT_SIZE,text, x, y, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw text.
     * @param id -> Text identifier.
     * @param text -> Quote to write.
     * @param x -> x position of the center point of the text.
     * @param y -> y position of the center point of the text.
     * @param color -> Text color.
     * @throws IOException
     */
    public void drawText(String id, String text, double x, double y, Color color) throws IOException {
        drawText(id, DEFAULT_FONT_SIZE,text, x, y, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw text.
     * @param id -> Text identifier.
     * @param text -> Quote to write.
     * @param x -> x position of the center point of the text.
     * @param y -> y position of the center point of the text.
     * @param time_to_live -> Text time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawText(String id, String text, double x, double y, int time_to_live) throws IOException {
        drawText(id, DEFAULT_FONT_SIZE,text, x, y, color, time_to_live);
    }

    /**
     * Creates the message to draw text.
     * @param id -> Text identifier.
     * @param font_size -> Text font size.
     * @param text -> Quote to write.
     * @param x -> x position of the center point of the text.
     * @param y -> y position of the center point of the text.
     * @throws IOException
     */
    public void drawText(String id, int font_size, String text, double x, double y) throws IOException {
        drawText(id, font_size, text, x, y, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw text.
     * @param id -> Text identifier.
     * @param font_size -> Text font size.
     * @param text -> Quote to write.
     * @param x -> x position of the center point of the text.
     * @param y -> y position of the center point of the text.
     * @param color -> Text color.
     * @throws IOException
     */
    public void drawText(String id, int font_size, String text, double x, double y, Color color) throws IOException {
        drawText(id, font_size,text, x, y, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw text.
     * @param id -> Text identifier.
     * @param font_size -> Text font size.
     * @param text -> Quote to write.
     * @param x -> x position of the center point of the text.
     * @param y -> y position of the center point of the text.
     * @param time_to_live -> Text time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawText(String id, int font_size, String text, double x, double y, int time_to_live) throws IOException {
        drawText(id, font_size, text, x, y, color, time_to_live);
    }

    /**
     * Creates the message to draw text.
     * @param id -> Text identifier.
     * @param font_size -> Text font size.
     * @param text -> Quote to write.
     * @param x -> x position of the center point of the text.
     * @param y -> y position of the center point of the text.
     * @param color -> Text color.
     * @param time_to_live -> Text time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawText(String id, int font_size, String text, double x, double y, Color color, int time_to_live) throws IOException {
        String msg = "<Text ttl=\""+time_to_live+"\" id=\""+id+"\" "+color.toString()+" text=\""+text+"\" x=\""+x+"\" y=\""+y+"\" font_size=\""+font_size+"\"></Text>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    /**
     * Creates the message to draw a rectangle.
     * @param id -> Rectangle identifier.
     * @param width -> Rectangle width.
     * @param height -> Rectangle height.
     * @param x -> x position of the center point of the rectangle.
     * @param y -> y position of the center point of the rectangle.
     * @throws IOException
     */
    public void drawRectangle(String id, double width, double height, double x, double y) throws IOException {
        drawRectangle(id,width,height,x,y,color,DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a rectangle.
     * @param id -> Rectangle identifier.
     * @param width -> Rectangle width.
     * @param height -> Rectangle height.
     * @param x -> x position of the center point of the rectangle.
     * @param y -> y position of the center point of the rectangle.
     * @param color -> Rectangle color.
     * @throws IOException
     */
    public void drawRectangle(String id, double width, double height, double x, double y, Color color) throws IOException {
        drawRectangle(id,width,height,x,y,color,DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a rectangle.
     * @param id -> Rectangle identifier.
     * @param width -> Rectangle width.
     * @param height -> Rectangle height.
     * @param x -> x position of the center point of the rectangle.
     * @param y -> y position of the center point of the rectangle.
     * @param time_to_live -> Rectangle time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawRectangle(String id, double width, double height, double x, double y, int time_to_live) throws IOException {
        drawRectangle(id,width,height,x,y,color,time_to_live);
    }

    /**
     * Creates the message to draw a rectangle.
     * @param id -> Rectangle identifier.
     * @param width -> Rectangle width.
     * @param height -> Rectangle height.
     * @param x -> x position of the center point of the rectangle.
     * @param y -> y position of the center point of the rectangle.
     * @param color -> Rectangle color.
     * @param time_to_live -> Rectangle time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawRectangle(String id, double width, double height, double x, double y, Color color, int time_to_live) throws IOException {
        String msg = "<Rectangle ttl=\""+time_to_live+"\" id=\""+id+"\" "+color.toString()+" Width=\""+width+"\" Height=\""+height+"\" x=\""+x+"\" y=\""+y+"\"></Rectangle>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    /**
     * Creates the message to draw a square.
     * @param id -> Square identifier.
     * @param width -> Square width (=height).
     * @param x -> x position of the center point of the square.
     * @param y -> y position of the center point of the square.
     * @throws IOException
     */
    public void drawSquare(String id, double width, double x, double y) throws IOException {
        drawSquare(id,width,x,y,color,DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a square.
     * @param id -> Square identifier.
     * @param width -> Square width (=height).
     * @param x -> x position of the center point of the square.
     * @param y -> y position of the center point of the square.
     * @param color -> Square color.
     * @throws IOException
     */
    public void drawSquare(String id, double width, double x, double y, Color color) throws IOException {
        drawSquare(id,width,x,y,color,DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a square.
     * @param id -> Square identifier.
     * @param width -> Square width (=height).
     * @param x -> x position of the center point of the square.
     * @param y -> y position of the center point of the square.
     * @param time_to_live -> Square time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawSquare(String id, double width, double x, double y, int time_to_live) throws IOException {
        drawSquare(id,width,x,y,color,time_to_live);
    }

    /**
     * Creates the message to draw a square.
     * @param id -> Square identifier.
     * @param width -> Square width (=height).
     * @param x -> x position of the center point of the square.
     * @param y -> y position of the center point of the square.
     * @param color -> Square color.
     * @param time_to_live -> Square time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawSquare(String id, double width, double x, double y, Color color, int time_to_live) throws IOException {
        drawRectangle(id,width,width,x,y,color,time_to_live);
    }

    /**
     * Creates the message to draw a polygon.
     * @param id -> Polygon identifier.
     * @param point_list -> List of polygon points (the order matters).
     * @throws IOException
     */
    public void drawPolygon(String id, Point[] point_list) throws IOException {
        drawPolygon(id,point_list,color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a polygon.
     * @param id -> Polygon identifier.
     * @param point_list -> List of polygon points (the order matters).
     * @param color -> Polygon color.
     * @throws IOException
     */
    public void drawPolygon(String id, Point[] point_list, Color color) throws IOException {
        drawPolygon(id,point_list,color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a polygon.
     * @param id -> Polygon identifier.
     * @param point_list -> List of polygon points (the order matters).
     * @param time_to_live -> Polygon time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawPolygon(String id, Point[] point_list, int time_to_live) throws IOException {
        drawPolygon(id,point_list,color,time_to_live);
    }

    /**
     * Creates the message to draw a polygon.
     * @param id -> Polygon identifier.
     * @param point_list -> List of polygon points (the order matters).
     * @param color -> Polygon color.
     * @param time_to_live -> Polygon time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawPolygon(String id, Point[] point_list, Color color, int time_to_live) throws IOException {
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


    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param x0 -> x position of the first line point.
     * @param y0 -> y position of the second line point.
     * @param x1 -> x position of the first line point.
     * @param y1 -> y position of the second line point.
     * @throws IOException
     */
    public void drawLine(String id, double x0, double y0, double x1, double y1) throws IOException {
        drawLine(id, x0, y0, x1, y1, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param x0 -> x position of the first line point.
     * @param y0 -> y position of the second line point.
     * @param x1 -> x position of the first line point.
     * @param y1 -> y position of the second line point.
     * @param color -> Line color.
     * @throws IOException
     */
    public void drawLine(String id, double x0, double y0, double x1, double y1, Color color) throws IOException {
        drawLine(id, x0, y0, x1, y1, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param x0 -> x position of the first line point.
     * @param y0 -> y position of the second line point.
     * @param x1 -> x position of the first line point.
     * @param y1 -> y position of the second line point.
     * @param time_to_live -> Line time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawLine(String id, double x0, double y0, double x1, double y1, int time_to_live) throws IOException {
        drawLine(id, x0, y0, x1, y1, color, time_to_live);
    }

    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param x0 -> x position of the first line point.
     * @param y0 -> y position of the second line point.
     * @param x1 -> x position of the first line point.
     * @param y1 -> y position of the second line point.
     * @param color -> Line color.
     * @param time_to_live -> Line time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawLine(String id, double x0, double y0, double x1, double y1, Color color, int time_to_live) throws IOException {
        String msg = "<Line ttl=\""+time_to_live+"\" id=\""+id+"\" "+color+" x0=\""+x0+"\" y0=\""+y0+"\" x1=\""+x1+"\" y1=\""+y1+"\"></Line>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }

    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param p0 -> First point.
     * @param p1 -> Second point.
     * @throws IOException
     */
    public void drawLine2(String id, Point p0, Point p1) throws IOException {
        drawLine2(id, p0, p1, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param p0 -> First point.
     * @param p1 -> Second point.
     * @param color -> Line color.
     * @throws IOException
     */
    public void drawLine2(String id, Point p0, Point p1, Color color) throws IOException {
        drawLine2(id, p0, p1, color, DEFAULT_TTL);
    }

    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param p0 -> First point.
     * @param p1 -> Second point.
     * @param time_to_live -> Line time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawLine2(String id, Point p0, Point p1, int time_to_live) throws IOException {
        drawLine2(id, p0, p1, color, time_to_live);
    }

    /**
     * Creates the message to draw a line.
     * @param id -> Line identifier.
     * @param p0 -> First point.
     * @param p1 -> Second point.
     * @param color -> Line color.
     * @param time_to_live -> Line time to live (erases after ttl milliseconds).
     * @throws IOException
     */
    public void drawLine2(String id, Point p0, Point p1, Color color, int time_to_live) throws IOException {
        String msg = "<Line ttl=\""+time_to_live+"\" id=\""+id+"\" "+color+" "+p0.toStringIndex(0)+" "+p1.toStringIndex(1)+"></Line>";
        if(buffered){
            buffer.append(msg);
        }
        else{
            sendMessage(msg);
        }
    }


}
