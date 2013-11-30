import re
from sys import argv
from html.parser import HTMLParser
from crawler import Crawler


class PythonGrabber(HTMLParser):
    RE = re.compile("python", re.IGNORECASE)

    def handle_data(self, data):
        self.pythons += filter(PythonGrabber.RE.search, data.split("."))


    def run(self, data):
        self.pythons = []
        self.feed(data)
        return self.pythons


grabber = PythonGrabber()
pythons = {}
def callback(address, source):
    sentences = grabber.run(source)
    if len(sentences) == 0:
        return
    else:
        pythons[address] = sentences


crawler = Crawler()
for arg in argv[1:]:
    crawler.run(arg, callback)


for (address, sentences) in pythons.items():
    print("\n\n=== %s (%d sentences) ===" % (address, len(sentences)))
    for sentence in sentences:
        print("\"%s\"" % sentence)