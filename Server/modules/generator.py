from sector import Sector

class Generator:
    def __init__(self, db):
        self.db = db
	self.ps = 30856802500000000.0 # The size of one parsec in meters

    def GetSector(self, position=(0.0, 0.0, 0.0)):
        sector_coords = (
            int(position[0] / 16384.0),
            int(position[1] / 16384.0),
            int(position[2] / 16384.0)
        )
        sector_result = self.db.FindOne('sectors', {'coords': sector_coords})
        sector_data = None
        new_sector = True
        if sector_result and len(sector_result):
            sector_data = sector_result[0]
            new_sector = False
        sector = Sector(sector_data)
        return sector

