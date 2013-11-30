import re
from html.parser import HTMLParser
from urllib.request import urlopen
from urllib.parse import urljoin


class LinkGrabber(HTMLParser):
    ABSOLUTE = re.compile("https?:\/\/")

    def __init__(self, relative=False):
        super(LinkGrabber, self).__init__()
        self.RELATIVE = relative # Look only for relative links.


    def handle_starttag(self, tag, attrs):
        if tag != 'a':
            return

        for (attr, val) in attrs:
            if attr == 'href':
                if not self.RELATIVE:
                    self.links.append(val)
                elif not LinkGrabber.ABSOLUTE.match(val):
                    self.links.append(val)


    def run(self, data):
        self.links = []
        self.feed(data)
        return self.links


class Crawler(object):

    def __init__(self, depth=2, verbose=True, relative=True):
        self.MAXIMUM_DEPTH = depth
        self.VERBOSE = verbose
        self.RELATIVE = relative


    def __crawl(self, address, depth):
        # Check for recursion limits.
        if depth > self.MAXIMUM_DEPTH or address in self.visited:
            return

        # Skip .pdf files.
        if re.match('.*\.pdf$', address):
            return

        try:
            source = str(urlopen(address).read())
        except:
            return
        
        if self.callback:
            self.callback(address, source)

        links = LinkGrabber(self.RELATIVE).run(source)
    
        if self.VERBOSE:
            indent = 2 * depth * ' ' # For pretty-printing purposes.
            print("%s + %s (%d bytes, %d links found)" % (indent, address, len(source), len(links)))
        
        for link in links:
            self.__crawl(urljoin(address, link), depth + 1)
        

    def run(self, address, callback = None):
        self.visited = set()
        self.callback = callback
        self.__crawl(address, 0)
