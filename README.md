zstuff

This is a small unixy C program that splits a big file into smaller files
containing equal numbers of lines. But it does this with gzipped input
and output files, using a large number of child compression processes
to make compression of the split pieces a) possible on-the-fly and
b) really fast.

```
Usage: zsplit -l <lines> <file> <prefix>
```

Example usage:

```bash
$ seq 100 | zsplit -l 10 /dev/stdin x.
$ zcat x.aa.gz
```
