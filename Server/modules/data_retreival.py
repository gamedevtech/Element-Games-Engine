from generator import Generator
from dispatch_handlers.communication import Communication
from dispatch_handlers.movement import Movement

class DataRetreival:
    def __init__(self, db):
        self.db = db
        self.generator = Generator(self.db)
        self.request_index = {
            0: 'ping',
            1: 'message',
            2: 'login'
        }

    def ActionDispatcher(self, integer_index, message):
        if integer_index < 10:
            return None

        mod = None
        if integer_index >= 10 and integer_index < 30:
            mod = Communication(self.db)
        if integer_index >= 30 and integer_index < 50:
            mod = Movement(self.db)

        return mod.Handle(integer_index, message)

    def HandleRequest(self, request_data):
        request_int = int(request_data[0:4])
        message = request_data[4:]

        dispatcher_result = self.ActionDispatcher(request_int, message)

        if not dispatcher_result:
            request_type = self.request_index.get(request_int, None)
            if request_type:
                if request_type == 'ping':
                    return 'pong'
                elif request_type == 'message':
                    return 'Mesage Received: %s' % message
                elif request_type == 'login':
                    creds = message.split(':')
                    username = creds[0]
                    password = creds[1]
                    row = self.db.FindOne('users', {'username': username, 'password': password})
                    if row:
                        return "success"
                    return "failure"
            return None
        return dispatcher_result
