import socket
from typing import Tuple, List


class CRDraw:

    def __init__(self, robName, robId, host="127.0.0.1", port=5000):
        self.robName = robName
        self.robId = robId
        self.host = host
        self.port = port

        self.sock = socket.socket(socket.AF_INET,  # Internet
                                  socket.SOCK_DGRAM)  # UDP

        self.color = (0, 0, 0)

        self.buffered = True
        self.buffer = []

        self.ttl = 5000

    def bufferOnOff(self, on_off):
        self.buffered = on_off

    def _sendMessage(self, msgs):
        if isinstance(msgs, list):
            msg = f"<Shapes>{''.join(msgs)}</Shapes>"
        else:
            msg = f"<Shapes>{msgs}</Shapes>"

        self.sock.sendto(msg.encode(), (self.host, self.port))

    def drawAll(self):
        if self.buffered and self.buffer:
            self._sendMessage(self.buffer)

    def setColor(self, r, g, b):
        self.color = (r, g, b)

    def drawEllipse(self, id, diam_vertical, diam_horizontal, x, y, color: Tuple[int, int, int]=None, ttl: int=None):
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Ellipse ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" Diam_vertical="{diam_vertical}" Diam_horizontal="{diam_horizontal}" x="{x}" y="{y}"></Ellipse>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawText(self, id, text, x, y, font_size=14, color=None,ttl=None):
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Text ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" text="{text}" x="{x}" y="{y}" font_size="{font_size}"></Text>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawCircle(self, id, diam, x, y, color=None, ttl=None):
        self.drawEllipse(id, diam, diam, x, y, color, ttl)

    def drawRectangle(self, id, width, height, x, y, color=None, ttl=None):
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Rectangle ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" Width="{width}" Height="{height}" x="{x}" y="{y}"></Rectangle>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawSquare(self, id, width, x, y, color=None, ttl=None):
        self.drawRectangle(id, width, width, x, y, color, ttl)

    def drawPolygon(self, id, point_list: List[Tuple[int, int]], color=None, ttl=None):
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
        r, g, b = self.color if color is None else color
        ttl = self.ttl if ttl is None else ttl
        msg = f'<Line ttl="{ttl}" id="{id}" red="{r}" green="{g}" blue="{b}" x0="{x0}" y0="{y0}" x1="{x1}" y1="{y1}"></Line>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawLine2(self, id, p0: Tuple[int, int], p1: Tuple[int, int], color=None, ttl=None):
        self.drawLine(id, *p0, *p1, color, ttl)


if __name__ == "__main__":
    crd = CRDraw("ola", 1)
    crd.bufferOnOff(False)
    #points = [(0, 50), (400, 60), (50, 30), (700, 200)]
    #crd.drawPolygon(1, points, (0, 0, 255))
    #crd.drawLine2(2, (700, 200), (850, 250), (255, 0, 0))
    #crd.drawSquare(3, 20, 700, 200, (255, 255, 0))
    #crd.drawRectangle(4, 50, 20, 700, 200, (255, 0, 0))
    crd.setColor(255,0,0);
    crd.drawText("1.4.5","olaaaaaaa", 10, 10, 20);
    crd.setColor(0,255,0);
    crd.drawText("1.4.6","lllllllllll", 100, 100);
    crd.drawAll()
