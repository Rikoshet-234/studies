import io, string, re


class WordIterator(object):

    def __init__(self, stream):
        self.stream = stream


    def __iter__(self):
        # Move to beginning of a stream (if possible).
        if self.stream.seekable():
            self.stream.seek(0)
        self.lineiter = iter(self.stream)

        self.words = []
        self.current = 0

        return self


    def __next__(self):
        # Fetch a new line if required.
        while len(self.words) <= self.current:
            line = "-\n"
            while len(line) > 1 and line[-2] == '-':
                line = line[:-2] + next(self.lineiter)

            PUNCTUATION = "[%s]" % re.escape(string.punctuation)
            self.words = re.sub(PUNCTUATION, ' ', line).split()
            self.current = 0

        self.current += 1
        return self.words[self.current - 1]


def stats(stream):
    counter = {}
    for word in WordIterator(stream):
        length = len(word)
        if length in counter:
            counter[length] += 1
        else:
            counter[length] = 1

    for (length, words) in sorted(counter.items()):
        print("% 3d => % 5d" % (length, words))


stream = io.StringIO("""
adasda-dsada fd-\nas dfasda
   dfasda
  ab
    dasa-
dfsdfsfs
de gh
    gdfgfd
""")
stats(stream)
