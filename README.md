WARNING: Both of these are only lightly tested! Consider them beta.

zsplit
======

This is a small unixy C program that splits a big file into smaller files
containing equal numbers of lines. But it does this with gzipped input
and output files, using a large number of child compression processes
to make compression of the split pieces a) possible on-the-fly and
b) really fast.

```
Usage: zsplit -l <lines> <file> <prefix>
```

Example zsplit usage:

```bash
$ seq 100 | ./zsplit x. 10
$ zcat x.000002.gz
```

zshard
======

Shards a columnar file into compressed child files such that lines with the indicated colNum end up in the same shard (via hashing)

