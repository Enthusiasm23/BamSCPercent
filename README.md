# BAM Soft Clipping Percentage

## Description
This program calculates the percentage of reads in a BAM file that contain soft clipping (CIGAR 'S'). It is designed to leverage HTSlib for efficient processing, making it especially beneficial for large BAM files.

## Compilation and Installation
First clone the repository, then set the HTSlib path, then compile the program and give executable permissions:

```bash
git clone <repository_url>
cd BamSCPercent
export HTSLIB_PATH=/path/to/htslib
make
chmod +x bamSCPercent
```

Replace "/path/to/htslib" with the actual path where HTSlib is installed on your system.

## Usage
```shell
bamSCPercent <BAM file> [threads]
```

### Arguments

- `<BAM file>`: Path to the BAM file.
- `[threads]`: Optional. Number of threads to use for processing. The default value is 8.
  - Note: Excessive threads may not improve performance due to I/O limitations.

## Notes

- This program depends on HTSlib. Ensure HTSlib is properly installed and accessible on your system.
- For small BAM files, using 'samtools view' might be faster and more straightforward.

## Alternative using samtools

You can calculate the percentage of soft-clipped reads using samtools and awk with the following command:

```shell
samtools view -@ [threads] <BAM file> | awk '{ total_lines++; if ($6 ~ /S/) lines_with_s++ } END { printf "%.2f\n", lines_with_s / total_lines * 100 }'
```
This command can also be executed using a script named `bamSCPercent.sh`. Save the above command in a file named bamSCPercent.sh, and replace <BAM file> and [threads] with the appropriate parameters. To run the script, use:

```shell
./bamSCPercent.sh <BAM file> [threads]
```

Make sure that your bamSCPercent.sh script has execution permissions. You can grant execution permissions using the command chmod +x bamSCPercent.sh.

Ensure that samtools is in your environment path. If samtools is not found, you can add it to your path using the following command:

```shell
export PATH=/path/to/samtools:$PATH
```
Replace "/path/to/samtools" with the actual path where SamTools is installed on your system.

## Troubleshooting

- If you encounter an error like "error while loading shared libraries: libhts.so.2to3part12: cannot open shared object file: No such file or directory", it indicates that the HTSlib shared library is not found in your library path. To resolve this, you can add the HTSlib path to your `LD_LIBRARY_PATH` environment variable. For example:

```shell
export LD_LIBRARY_PATH=/path/to/htslib(lib):$LD_LIBRARY_PATH
```

Replace "/path/to/htslib/htslib(lib)" with the actual path where HTSlib is installed on your system.