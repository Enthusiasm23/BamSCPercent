#!/bin/bash

# 检查是否提供了 BAM 文件作为参数
if [ "$#" -ne 1 ]; then
    exit 1
fi

# 赋值 BAM 文件路径
bam_file=$1
threads=$2

# 使用单个 awk 命令来计算
result=$(samtools view -@ $threads $bam_file | awk '{ total_lines++; if ($6 ~ /S/) lines_with_s++ } END { print total_lines, lines_with_s }')

# 读取总行数和包含 'S' 的行数
total_lines=$(echo $result | cut -d' ' -f1)
lines_with_s=$(echo $result | cut -d' ' -f2)

# 计算比例（保留2位小数）
if [ "$total_lines" -ne 0 ]; then
    # 乘法优先于除法来避免整数除法的问题
    ratio=$(echo "scale=2; $lines_with_s * 100 / $total_lines" | bc)
    echo "$ratio"
else
    exit 1
fi
