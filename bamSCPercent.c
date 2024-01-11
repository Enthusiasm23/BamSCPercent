#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <htslib/sam.h>


void print_usage(char *program_name) {
    printf("Usage: %s <BAM file> [threads]\n", program_name);
    printf("\nDescription:\n");
    printf("  This program calculates the percentage of reads in a BAM file that contain soft clipping (CIGAR 'S').\n");
    printf("  It leverages HTSlib for efficient processing, especially beneficial for large BAM files.\n\n");
    printf("Arguments:\n");
    printf("  <BAM file>: Path to the BAM file.\n");
    printf("  [threads]: Optional. Number of threads to use for processing (default 8).\n");
    printf("             Note: Excessive threads may not improve performance due to I/O limitations.\n\n");
    printf("Notes:\n");
    printf("  - This program depends on HTSlib. Ensure HTSlib is properly installed and accessible.\n");
    printf("  - For small BAM files, using 'samtools view' might be faster and more straightforward.\n");
    printf("\nAlternative using samtools:\n");
    printf("  You can use the following command with samtools and awk to calculate the percentage of soft-clipped reads:\n");
    printf("  samtools view -@ [threads] <BAM file> | awk '{ total_lines++; if ($6 ~ /S/) lines_with_s++ } END { printf \"%%.2f\\n\", lines_with_s / total_lines * 100 }'\n");
    printf("\nTroubleshooting:\n");
    printf("  - If you encounter an error like \"error while loading shared libraries: libhts.so.2to3part12: cannot open shared object file: No such file or directory\",\n");
    printf("    it indicates that the HTSlib shared library is not found in your library path. To resolve this, add the HTSlib path to your LD_LIBRARY_PATH environment variable.\n");
    printf("    For example:\n");
    printf("    export LD_LIBRARY_PATH=/path/to/htslib:$LD_LIBRARY_PATH\n");
    printf("    Replace \"/path/to/htslib\" with the actual path where HTSlib is installed on your system.\n");
    printf("\nAdditional Resources:\n");
    printf("  - For the latest documentation and version information, please visit the GitHub repository:\n");
    printf("    https://github.com/Enthusiasm23/BamSCPercent.git\n");
}


int main(int argc, char *argv[]) {
    if (argc < 2 || strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
        return 1;
    }

    // 线程数检验
    int thread_count = 8;
    if (argc == 3) {
        char *endptr;
        long val = strtol(argv[2], &endptr, 10);
        if (*endptr != '\0' || val <= 0) {
            fprintf(stderr, "Error: Invalid number of threads '%s'.\n", argv[2]);
            return 1;
        }
        thread_count = val;
    }

    // 打开BAM文件
    samFile *fp = sam_open(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return 1;
    }
    
    // 设置线程数
    if (hts_set_threads(fp, thread_count) < 0) {
        fprintf(stderr, "Error setting thread count\n");
        sam_close(fp);
        return 1;
    }

    // 读取头文件
    bam_hdr_t *hdr = sam_hdr_read(fp);
    if (hdr == NULL) {
        fprintf(stderr, "Error reading header from %s\n", argv[1]);
        sam_close(fp);
        return 1;
    }

    // 初始化对齐数据结构
    bam1_t *aln = bam_init1();
    if (aln == NULL) {
        fprintf(stderr, "Error allocating alignment structure.\n");
        bam_hdr_destroy(hdr);
        sam_close(fp);
        return 1;
    }

    // 读取和处理数据
    int total_lines = 0, lines_with_s = 0;
    while (sam_read1(fp, hdr, aln) >= 0) {
        total_lines++;
        uint32_t *cigar = bam_get_cigar(aln);
        for (int i = 0; i < aln->core.n_cigar; ++i) {
            if (bam_cigar_op(cigar[i]) == BAM_CSOFT_CLIP) {
                lines_with_s++;
                break;
            }
        }
    }

    // 资源释放
    bam_destroy1(aln);
    bam_hdr_destroy(hdr);
    sam_close(fp);

    // 计算和输出结果
    if (total_lines > 0) {
        double ratio = (double)lines_with_s / total_lines * 100;
        printf("%.2f\n", ratio);
    } else {
        fprintf(stderr, "No lines found.\n");
        return 1;
    }

    return 0;
}
