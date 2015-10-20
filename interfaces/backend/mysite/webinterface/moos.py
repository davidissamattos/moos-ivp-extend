import os
import time
import json

class moosjson:
    def __init__(self,filename):
        self.community = "No name"
        self.moostime = "No time"
        try:
            file = os.path.join(os.path.dirname(os.path.realpath(__file__)), '../json/'+filename)
            self.jsonObject = json.loads(open(file).read())
            self.variables = {}
            self.getData()
            self.boolRead = True
        except:
            self.boolRead = False
        
        
    def getData(self):
        self.community = self.jsonObject['Community']
        self.moostime = self.jsonObject['MOOSTime']
        for v in self.jsonObject['Variables']:
            self.variables[v]=self.jsonObject['Variables'][v]
        