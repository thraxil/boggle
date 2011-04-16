I haven't really coded in C for the last decade. Just Java, Perl, and
Python with excursions into other high-level functional languages. 

Recently, I had the urge to get back to my roots and relearn C. 

For my first little toy project, I'm writing a program that, given a
Boggle board, will solve it, finding all the english words that
appear. 

I've implemented this before in Python about a year ago (I included
that solution too for comparison). 

The C version is clearly still in progress and it's slow going since
I'm re-familiarizing myself with the idioms of developing in that
language. I'm also taking a fundamentally different approach this
time, since I've had a year of background thinking on the most
efficient way to do this. 

Both versions rely on the standard unix 'words' file
(/usr/share/dict/words on most Linux boxen) as a master
dictionary. They also both expect a board to be defined in a textfile
in a very basic manner, eg: 

tntb
gdar
elte
sish

I'm not going to go over the rules of Boggle, since you can google
it. 

The approach I used in Python was to first come up with a reduced
alphabet consisting only of the letters that actually show up on the
board. In the example case above: abdegilnrst. Then I go through each
line of the words file, skipping it if there's an apostrophe, a
capital letter (no proper nouns in Boggle), or any letters in the word
do not appear in the restricted alphabet. The ones that are left are
potentially findable words. Then I do a fairly straightforward search
at each position of the board, going neighbor to neighbor to see if
that word appears. Then at the end, I take all the words that were
found and sort them by length. 

That's a really rough, fairly brute-force approach. I won't
apologize. That's kind of how I like to do things in Python. It
probably took me an hour to write that code and it runs fast enough
that I could use it to cheat and get really high scores on online
Boggle games (start a new game, quickly type the board into a text
file, run the Python script, which took 15 seconds or so, then spend
the next minute or two typing in the words it found, starting with the
longest, highest scoring ones first). Basically a total success as far
as I'm concerned. Python absolute excels for this approach to problem
solving where programmer-time matters a lot and there's a certain
performance threshold of "good enough" that, once you reach, you don't
gain by exceeding. With Python and a couple basic built-in data
structures like lists and hash tables, there are few problems you
can't tackle by just taking a very straightforward approach and adding
a few quick filters to reduce the search-space
drastically. Sophistication is possible, but rarely necessary.


But C is different to me. I feel like since C usually requires more
lines of code and more care to get a program working but offers far
more control and efficiency, it requires a different approach. It's
worth taking the time to get it right to begin with and avoid wasting
cycles and memory. 

I've had a few ideas in my head lately for things that I want to build
that I think will actually form a nice foundation for other
work. Things that would really benefit from being small, fast,
efficient and "right" from the ground up. This is what's got me
interested in getting back into C. 

Back to Boggle. 

The C version takes a completely different approach, that I thought up
recently and seems elegant. It's based on a Prefix Tree, or "Trie"
data structure. See the Wikipedia article:
http://en.wikipedia.org/wiki/Trie

What I do is first go through the words file (again throwing away
words that have apostrophes or capital letters) and building a Trie
out of it. The result is a Trie structure representing every word in
the english language. Checking to see if some word exists is O(n)
where n is the number of letters in the word, and further will always
be very small in this application. Next, we go through each position
on the board as a starting point and then go through all the possible
word paths from that point, checking against the master word Trie. 

Now, the nice thing about using the Trie instead of just a plain hash
table lookup or binary search or something, is that in addition to
being able to tell if a given word exists, we can also find out
whether there are any words that start with a given prefix just as
quickly. That means that as we check every possible path through the
board, we can eliminate subpaths very quickly when the Trie tells us
that there can't possibly be any valid words that start with that
prefix. Eg, starting at the 't' in the upper left corner of the
example board and looking at all the paths that can originate from
there, we can pretty much instantly eliminate them all since there are
no words in the english language that start with 'tn', 'tg', or
'td'. This rapid pruning of the search space pays dividends quickly. 

I'm not done writing the code to actually go through all the paths on
the board, but I'll update here when I am. For now, I'm happy to see
that making the entire Trie of the english language words only takes
82 milliseconds on my computer, so I'm excited to see how well the
rest of it performs. 


