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

        msg = '<Robot Id="' + str(robId) + '" Name="' + robName + '" />'

        self.sock.sendto(msg.encode(), (host, port))

        self.color = (0, 0, 0)

        self.buffered = True
        self.buffer = []

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

    def drawEllipse(self, diam_vertical, diam_horizontal, x, y, color: Tuple[int, int, int]=None):
        r, g, b = self.color if color is None else color
        msg = f'<Ellipse red="{r}" green="{g}" blue="{b}" Diam_vertical="{diam_vertical}" Diam_horizontal="{diam_horizontal}" x="{x}" y="{y}"></Ellipse>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawText(self, text, color=None):
        r, g, b = self.color if color is None else color
        msg = f'<Text red="{r}" green="{g}" blue="{b}" text="{text}"></Text>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawCircle(self, diam, x, y, color=None):
        self.drawEllipse(diam, diam, x, y, color)

    def drawRectangle(self, width, height, x, y, color=None):
        r, g, b = self.color if color is None else color
        msg = f'<Rectangle red="{r}" green="{g}" blue="{b}" Width="{width}" Height="{height}" x="{x}" y="{y}"></Rectangle>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawSquare(self, width, x, y, color=None):
        self.drawRectangle(width, width, x, y, color)

    def drawPolygon(self, point_list: List[Tuple[int, int]], color=None):
        r, g, b = self.color if color is None else color
        msg = [f'<Polygon red="{r}" green="{g}" blue="{b}">']
        for point in point_list:
            msg.append(f'<Point x="{point[0]}" y="{point[1]}"></Point>')
        msg.append(f'</Polygon>')
        msg = ''.join(msg)
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawLine(self, x0, y0, x1, y1, color=None):
        r, g, b = self.color if color is None else color
        msg = f'<Line red="{r}" green="{g}" blue="{b}" x0="{x0}" y0="{y0}" x1="{x1}" y1="{y1}"></Line>'
        if self.buffered:
            self.buffer.append(msg)
        else:
            self._sendMessage(msg)

    def drawLine2(self, p0: Tuple[int, int], p1: Tuple[int, int], color=None):
        self.drawLine(*p0, *p1, color)


if __name__ == "__main__":
    crd = CRDraw("ola", 1)
    crd.bufferOnOff(False)
    points = [(0, 50), (400, 60), (50, 30), (700, 200)]
    crd.drawPolygon(points, (0,0,255))
    crd.drawLine2((700, 200), (850, 250), (255, 0, 0))
    crd.drawSquare(20, 700, 200, (255, 255, 0))
    crd.drawRectangle(50, 20, 700, 200, (255, 0, 0))
    crd.drawAll()
