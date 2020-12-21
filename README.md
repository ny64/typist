# typist

**typist** is a simple, terminal **typi**ng **s**peed **t**est using only the
standard c libraries.

![typist v0.1 GIF](resources/demo.gif)

## Installation
There already is a binary file you can use.
To compile it yourself you need a C compiler like <code>gcc</code>. Your
installed version will most likely work.

```
git clone https://github.com/ny64/typist.git
cd typist
gcc src/*.c -o typist
```

Now you can already check if typist is working! Try it out by typing <code>
./typist most-used-words.txt</code> into your terminal.

If you want to install typist for other users or use it anywhere on your system,
move the binary file to one of the folders in your $PATH outside of your home
directory. To view these folders type <code>echo $PATH</code> into your
terminal. Then move the binary file into one of the folders. For this example I
will put it into <code>/usr/bin</code>.

```
sudo mv ./typist /usr/bin
```

## Usage

To start a typing test you need to provide a text file you want to write off.

```
typist file.txt
```

The test starts after the program receives the first input. It checks the time
after each input and charts your result after 60 seconds.

There are options you can use to change the behavior of typist, like changing
the time you have till the test ends.
In this example the time is set to 10 minutes.

```
typist --time 600 file.txt
```

To see all available options type <code>typist --help</code> or just
<code>typist</code>.

### Keybindings

| Binding | Function |
|---------|----------|
| <kbd>Ctrl</kbd> + <kbd>q</kbd>; <kbd>Ctrl</kbd> + <kbd>c</kbd> | Exit |
| <kbd>Ctrl</kbd> + <kbd>r</kbd> | Restart typing test |


### Tips

Doing the same test over and over again isn't good practice. I have provided
a text file and a small python script to randomize the most used words in the
English language. You can use this script on any text file, though it will
remove punctuation marks and reformats the text.

```
python3 ./utils/randomize.py ./most-used-words.txt
```

---

### [Issues / Bugs](https://github.com/ny64/typist/issues)

You can also contact me via email if you have any further questions.

---

### License
[GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
© [Peter Breitzler](https://github.com/ny64)
