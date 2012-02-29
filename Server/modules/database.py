import pymongo
import datetime

class Database:
    def __init__(self):
        self.connection = pymongo.Connection("localhost", 27017)
        self.db = self.connection['element-games']

    def GetCollection(self, collection_name):
        return self.db[collection_name]

    def Insert(self, collection_name, data):
        return self.db[collection_name].insert(data)

    def Find(self, collection_name, criteria):
        return self.db[collection_name].find(criteria)

    def FindOne(self, collection_name, criteria):
        return self.db[collection_name].find_one(criteria)
