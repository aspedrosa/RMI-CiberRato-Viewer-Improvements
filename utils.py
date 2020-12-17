#!/usr/bin/python3.8

import socket

mandatory_fields = {
    "e": ["red", "green", "blue", "x", "y", "Diam_horizontal", "Diam_vertical"],
    "c": ["red", "green", "blue", "x", "y", "Diam"],
    "r": ["red", "green", "blue", "x", "y", "Width", "Height"],
    "s": ["red", "green", "blue", "x", "y", "Width"],
    "l": ["red", "green", "blue", "x0", "y0", "x1", "y1"],
    "q": ["red", "green", "blue", "text"],
    "p": ["red", "green", "blue"],
}

shapes = {
    "e": "Ellipse",
    "c": "Circle",
    "r": "Rectangle",
    "s": "Square",
    "l": "Line",
    "q": "Quote",
    "p": "Polygon",
}

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    while True:
        try:
            type = input("(E)llipse, (C)ircle, (R)ectangle, (S)quare, (L)ine, (Q)uote, (P)olygon: ").lower()
        except KeyboardInterrupt:
            break

        if type not in mandatory_fields:
            print("Invalid type")
            continue

        message = [f"<Shapes><{shapes[type]} "]
        for attr in mandatory_fields[type]:
            value = input(attr + ": ")
            message.append(f'{attr}="{value}" ')

        if type == "p":
            message.append(f">")
            i = 0
            while True:
                x = input(f"x{i} (Q)uit:").lower()
                if x == "q":
                    break
                y = input(f"y{i} (Q)uit:: ")
                if y == "q":
                    break

                message.append(f'<Point x="{x}" y="{y}"></Point>')
                i += 1
        else:
            message.append(f">")


        message.append(f"</{shapes[type]}></Shapes>")

        print("".join(message))
        sock.sendto(bytes("".join(message), "utf-8"), ("127.0.0.1", 5000))


if __name__ == "__main__":
    main()
