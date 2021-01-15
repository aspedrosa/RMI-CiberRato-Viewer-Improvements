 
public class Color{
    /**
     * Red quantity.
     * */
    int r;

    /**
     * Green quantity.
     * */
    int g;

    /**
     * Blue quantity.
     * */
    int b;


    /**
     * Set default color.
     * @param r -> red quantity.
     * @param g -> green quantity.
     * @param b -> blue quantity.
     */
    public void setColor(int r, int g, int b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }

    /**
     * Converts the color to a string in message format.
     * @return Color in message format.
     */
    @Override
    public String toString(){
        return "red=\"" + r + "\" green=\""+ g +"\"" + " blue=\""+ b + "\"";
    }
}
