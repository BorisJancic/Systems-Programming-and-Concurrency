# Labs for ECE 252, Systems Programming and Concurrency
## Made in partnership with Max Long

## Lab 1: PNG Concatenation and Directory Search
Validate and concatenate PNGs.
DFS through directories to find all valid PNGs.

## Lab 2: Multithreading
Parallel PNG retrieval from the web and decompression.
PNGs are then concatenated.

## Lab 3: Multiprocessing
Multiprocessed PNG retrieval and decompression.
Shared [circular buffer](https://en.wikipedia.org/wiki/Circular_buffer) used to synchronize processes.

## Lab 4: Multithreaded Webcrawler
Multithreaded webcrawler using [monitors](https://en.wikipedia.org/wiki/Monitor_(synchronization)) to synchronize the production and consumption of URLs.

## Lab 5: Asynchronous Non-Blocking I/O Webcrawler
Single-threaded non-blocking webcrawler using the continuous retrieval method (eliminates potential bottleneck when using the batchwise method).
Performs almost as well as multithreading, as HTTP requests are the bottleneck.
