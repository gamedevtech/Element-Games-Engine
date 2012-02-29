from generator import Generator

class DataRetreival:
    def __init__(self, db):
        self.db = db
        self.generator = Generator(self.db)
        self.request_index = {
            '0000': 'ping',
            '0001': 'send_message',
            '0002': 'get_surroundings'
        }

    def HandleRequest(self, request_data):
        request_type = self.request_index.get(request_data[0:4], None)
        message = request_data[4:]
        if request_type:
            if request_type == 'ping':
                return 'pong'
            elif request_type == 'send_message':
                return 'Mesage Received: %s' % message
            elif request_type == 'get_surroundings':
                position = message.split(':')
                position[0] = float(position[0])
                position[1] = float(position[1])
                position[2] = float(position[2])
                sector = self.generator.GetSector(position)
                return "Got Sector: %f, %f, %f" % (position[0], position[1], position[2])
        else:
            return None
