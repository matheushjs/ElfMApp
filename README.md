ElfMApp
===

ElfMApp is an application designed for evaluating songs on command-line.

The available options are as follows:

```
  -n, --next <integer displacement>   Advances to next song
  -b, --back <integer displacement>   Plays the previous song
  -r, --rate <floating-point rate>    Changes playback rate
  -q, --quit                          Exits the program
  -z, --adv <floating-point seconds>  Forwards current song by some seconds
  -x, --rew <floating-point seconds>  Rewinds current song by some seconds
  -d, --rm                            Deletes current song from drive
  -., --redo                          Repeats last command
  -l, --list                          Lists current playlist
  -s, --select <integer index>        Selects specific song from playlist
  -i, --info                          Shows information about current song
  -h, --help                          Displays this help message
```

Usage
---

- Beginning the program

```
$ ./elfmapp Venture2.mp3  Venture.mp3  Village Dance.mp3  Village Fate.mp3  Village Life.mp3  Voyage.mp3

Playing song 0
Playing

Now Playing:
	Title:     "Venture2"
	Duration:  3m 21s 874ms
```

- Listing songs being evaluated

```
-l

Listing songs
0: Venture2
1: Venture
2: Village Dance
3: Village Fate
4: Village Life
5: Voyage
```

- Advancing to next song

```
-n

Playing song 1

Now Playing:
	Title:     "Venture"
	Duration:  3m 11s 555ms
```

- Changing playback rate

```
-r 1.5

Setting rate to 1.5

Now Playing:
	Title:     "Venture"
	Duration:  3m 11s 555ms
```

- Returning to normal playback rate

```
-r

Setting rate to 1

Now Playing:
	Title:     "Venture"
	Duration:  3m 11s 555ms
```

- Showing info of song being played

```
-i

	Title:    "Venture"
	Position: 0m 24s 796ms / 3m 11s 555ms
```

- Selecting song from the list

```
-s 5

Playing song 5

Now Playing:
	Title:     "Voyage"
	Duration:  3m 0s 741ms
```

#### Default values for commands

| Command | Default Value
|-|-|
  -n, --next | 1
  -b, --back | 1
  -r, --rate | 1.0
  -z, --adv | 5.0
  -x, --rew | 5.0
  -s, --select | None
  
Building
---

QtCreator in a system with support for the multimedia module and for c++11 should be able to build this project.