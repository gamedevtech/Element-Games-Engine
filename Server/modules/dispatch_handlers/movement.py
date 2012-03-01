class Movement:
    def __init__(self, db):
        self.db = db
        self.action_mapping = {
            30: 'send_position'
        }

    def Handle(self, action_index, message):
        return self.action_mapping.get(action_index, None)
