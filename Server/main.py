import argparse
from modules.database import Database
from modules.server import Server

parser = argparse.ArgumentParser(description='''Server for Element-Games Engine''')
parser.add_argument('--type', default='tcp')
args = parser.parse_args()

db = Database()

server = Server(db, args.type)
server.Serve()
