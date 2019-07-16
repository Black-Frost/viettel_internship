import falcon
import logging

from wsgiref import simple_server


logging.getLogger().setLevel(logging.WARNING)


def decrypt(f):
    return ''.join(filter(lambda x: x != chr(0x00), map(lambda x: chr(((x-0x0f) % 0xff) ^ 0x21), f)))


def encrypt(f):
    return list(map(lambda x: (ord(x) ^ 0x21) - 0x0f, [x if i == 0 else '\x00' for i, x in enumerate(f)]))


class Freenki:
    def on_post(self, req, resp):
        code = req.bounded_stream.read(1).decode()
        mac_addr = req.bounded_stream.read(2*6).decode()
        data = ''
        if code != '4':
            data = req.bounded_stream.read()
            data = decrypt(data)

        print("{} {} {}".format(code, mac_addr, len(data)))
        print(data)
        print("===================================\n")

        if code == '0':
            resp.media = "Would this string appear?"
        elif code == '2':
            resp.media = encrypt(u'this is a very looooooooooooooooooooooooooooong payload\r')
        else:
            resp.media = "Anything would be ok"
        resp.status = falcon.HTTP_200

app = falcon.API()
app.add_route('/btob_asiana/udel_confirm.php', Freenki())
print()

if __name__ == "__main__":
    # httpd = simple_server.make_server('127.0.0.1', 80, app)
    # httpd.serve_forever()
    pass
