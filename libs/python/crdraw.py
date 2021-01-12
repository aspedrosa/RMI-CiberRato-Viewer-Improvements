import socket
from typing import Tuple, List


class CRDraw:

    def __init__(self, host="127.0.0.1", port=5000):
        self.host = host  # Host address
        self.port = port  # Host port

        self.sock = socket.socket(socket.AF_INET,  # Internet
                                  socket.SOCK_DGRAM)  # UDP

        self.color = (0, 0, 0)  # Default color

        self.buffered = True  # Indicates if the messages buffer is being used
        self.buffer = []  # Messages buffer

        self.ttl = 5000  # Shape time to live default (5 seconds)

    def bufferOnOff(self, on_off):
        """
        Turn the buffer on or off.
        :param on_off: Turns the buffer on (true) or off (false).
        """
        self.buffered = on_off

    def _sendMessage(self, msgs):
        """
        Sends the message via socket to host address and port.
        :param msgs: message.
        """
        if isinstance(msgs, list):
            msg = f"<Shapes>{''.join(msgs)}</Shapes>"
        else:
            msg = f"<Shapes>{msgs}</Shapes>"

        self.sock.sendto(msg.encode(), (self.host, self.port))

    def drawAll(self):
        """
        Send all the messages in the buffer.
        """
        if self.buffered and self.buffer:
            self._sendMessage(self.buffer)

    def setColor(self, r, g, b):
        """
        Set default color.
        :param r: Red quantity.
        :param g: Green quantity.
        :param b: Blue quantity.
        """
        self.color = (r, g, b)

    def drawEllipse(self, id, diam_vertical, diam_horizontal, x, y, color: Tuple[int, int, int]=None, ttl: int=None):
        """
        Create the message to draw an ellipse.
        :param id: Ellipse identifier.
        :param diam_vertical: Ellipse vertical diameter.
        :param diam_horizontal: Ellipse horizontal diameter.
        :param x: x position of the center point of the ellipse.
        :param y: y position of the center point of the ellipse.
        :param color: Ellipse color.
        :param ttl: Ellipse time to live (erases after ttl milliseconds).
        """
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Ellipse ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" Diam_vertical="{diam_vertical}" Diam_horizontal="{diam_horizontal}" x="{x}" y="{y}"></Ellipse>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawText(self, id, text, x, y, font_size=14, color=None,ttl=None):
        """
        Creates the message to draw text.
        :param id: Text identifier.
        :param text: Quote to write.
        :param x: x position of the center point of the text.
        :param y: y position of the center point of the text.
        :param font_size: Text font size.
        :param color: Text color.
        :param ttl: Text time to live (erases after ttl milliseconds).
        """
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Text ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" text="{text}" x="{x}" y="{y}" font_size="{font_size}"></Text>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawCircle(self, id, diam, x, y, color=None, ttl=None):
        """
        Creates the message to draw a circle.
        :param id: Circle identifier.
        :param diam: Circle diameter.
        :param x: x position of the center point of the circle.
        :param y: y position of the center point of the circle.
        :param color: Circle color.
        :param ttl: Circle time to live (erases after ttl milliseconds).
        """
        self.drawEllipse(id, diam, diam, x, y, color, ttl)

    def drawRectangle(self, id, width, height, x, y, color=None, ttl=None):
        """
        Creates the message to draw a rectangle.
        :param id: Rectangle identifier.
        :param width: Rectangle width.
        :param height: Rectangle height.
        :param x: x position of the center point of the rectangle.
        :param y: y position of the center point of the rectangle.
        :param color: Rectangle color.
        :param ttl: Rectangle time to live (erases after ttl milliseconds).
        """
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Rectangle ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" Width="{width}" Height="{height}" x="{x}" y="{y}"></Rectangle>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawSquare(self, id, width, x, y, color=None, ttl=None):
        """
        Creates the message to draw a square.
        :param id: Square identifier.
        :param width: Square width (=height).
        :param x: x position of the center point of the square.
        :param y: y position of the center point of the square.
        :param color: Square color.
        :param ttl: Square time to live (erases after ttl milliseconds).
        """
        self.drawRectangle(id, width, width, x, y, color, ttl)

    def drawPolygon(self, id, point_list: List[Tuple[int, int]], color=None, ttl=None):
        """
        Creates the message to draw a polygon.
        :param id: Polygon identifier.
        :param point_list: List of polygon points (the order matters).
        :param color: Polygon color.
        :param ttl: Polygon time to live (erases after ttl milliseconds).
        """
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = [f'<Polygon ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}">']
        for point in point_list:
            msg.append(f'<Point x="{point[0]}" y="{point[1]}"></Point>')
        msg.append(f'</Polygon>')
        msg = ''.join(msg)
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawLine(self, id, x0, y0, x1, y1, color=None, ttl=None):
        """
        Creates the message to draw a line.
        :param id: Line identifier.
        :param x0: x position of the first line point.
        :param y0: y position of the second line point.
        :param x1: x position of the first line point.
        :param y1: y position of the second line point.
        :param color: Line color.
        :param ttl: Line time to live (erases after ttl milliseconds).
        """
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Line ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" x0="{x0}" y0="{y0}" x1="{x1}" y1="{y1}"></Line>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawLine2(self, id, p0: Tuple[int, int], p1: Tuple[int, int], color=None, ttl=None):
        """
        Creates the message to draw a line.
        :param id: Line identifier.
        :param p0: First point.
        :param p1: Second point.
        :param color: Line color.
        :param ttl: Line time to live (erases after ttl milliseconds).
        """
        self.drawLine(id, *p0, *p1, color, ttl)


if __name__ == "__main__":
    # Draw random shapes
    crd = CRDraw()
    crd.bufferOnOff(True)
    crd.setColor(0, 0, 255);
    points = [(0, 5), (4, 6), (5, 3), (7, 2)]
    crd.drawPolygon("polygon", points, (0, 0, 255))
    crd.drawLine2("line", (7, 2), (8.5, 2.5), (255, 0, 0))
    crd.drawSquare("square", 0.2, 7, 2, (255, 255, 0))
    crd.drawRectangle("rectangle", 5, 2, 7, 2, (255, 0, 0))
    crd.drawCircle("circle", 1, 3, 3)
    crd.drawEllipse("ellipse", 0.5, 1, 4, 4)
    crd.setColor(255, 0, 0);
    crd.drawText("text.1","olaaaaaaa", 10, 10, 20);
    crd.setColor(0, 255, 0);
    crd.drawText("text.2","lllllllllll", 100, 100);
    crd.drawAll()
