import shelve
import re
from xmlrpc.server import SimpleXMLRPCServer


class NabServer:

    def __init__(self, address, port):
        self.db = shelve.open("data.db")
        self.server = SimpleXMLRPCServer((address, port))

        self.server.register_function(self.records, 'records')
        self.server.register_function(self.find, 'find')
        self.server.register_function(self.erase, 'erase')
        self.server.register_function(self.create, 'create')
        self.server.register_function(self.update, 'update')


    def run(self):
        self.server.serve_forever()


    def records(self, numfilter=None):
        try:
            pattern = re.compile(numfilter)
        except:
            # If something is wrong, just match everything.
            pattern = re.compile("")

        result = []
        for name in self.db:
            if pattern.search(self.db[name]['phone']):
                result.append(name)

        print("Records: %s" % str(result))
        return result


    def find(self, eid):
        print("Searching for '%s'" % eid)
        return self.db[eid]


    def erase(self, eid):
        print("Erasing entry '%s'" % eid)
        del self.db[eid]
        return True


    def create(self, entry):
        try:
            name = entry['name']
            record = {
                'phone': entry['phone'],
                'email': entry['email'],
                'address': entry['address'],
                'checkout': entry['checkout']
            }
        except:
            print("Incorect create query.")
            return
        
        print("Creating entry '%s'" % name)
        self.db[name] = record
        self.db.sync()
        return True


    def update(self, eid, entry):
        self.erase(eid)
        self.create(entry)
        return True


server = NabServer("localhost", 8802)
server.run()
