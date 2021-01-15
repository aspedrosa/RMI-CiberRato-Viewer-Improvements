 
public class Point {

    /**
     * point x position
     */
    private final double x;

    /**
     * point y position
     */
    private final double y;

    /**
     * Point instantiation
     * @param x -> point x position
     * @param y -> point y position
     */
    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    /**
     * Converts the point to a string in polygon message format.
     * @return Point in polygon message format.
     */
    @Override
    public String toString() {
        return "<Point x=\""+x+"\" y=\""+y+"\"></Point>";
    }

    /**
     * Converts the point to a string in line message format.
     * @param index -> point index.
     * @return Converts the point to a string in line message format.
     */
    public String toStringIndex(int index) {
        return "x"+index+"=\""+x+"\" y"+index+"=\""+y+"\"";
    }
}
