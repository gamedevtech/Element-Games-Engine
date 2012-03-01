class Communication:
    def __init__(self, db):
        self.db = db
        self.action_mapping = {
            10: 'send',
            11: 'receive'
        }

    def Handle(self, action_index, message):
        return self.action_mapping.get(action_index, None)
