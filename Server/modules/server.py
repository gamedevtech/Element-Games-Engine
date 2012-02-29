import SocketServer
from data_retreival import DataRetreival

class TCPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        db = self.server.db
        self.data = self.request.recv(4096).strip()

        print "{} wrote:".format(self.client_address[0])
        print self.data

        output = self.server.data.HandleRequest(self.data)
        self.request.sendall(output)

class UDPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        db = self.server.db
        data = self.request[0].strip()
        socket = self.request[1]

        print "{} wrote:".format(self.client_address[0])
        print data

        output = self.server.data.HandleRequest(data)
        socket.sendto(output, self.client_address)

class Server:
    def __init__(self, db, server_type = 'tcp'):
        if server_type == 'tcp':
            self.host = 'localhost'
            self.port = 31534
            self.server = SocketServer.TCPServer((self.host, self.port), TCPHandler)
        else:
            self.host = 'localhost'
            self.port = 31535
            self.server = SocketServer.UDPServer((self.host, self.port), UDPHandler)
        self.db = db

    def Serve(self):
        self.server.db = self.db
        self.server.data = DataRetreival(self.db)
        self.server.serve_forever()
